#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "msleep.h"
#include "lincp-emu.h"
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <time.h>
#include <signal.h>

#define MAX_LOG_NAME 50
#define MAX_TIME_STRING 50

char * schedule_strings[] = { "LI0_LIN_NULL_SCHEDULE", "LI0_GOTO_SLEEP_SCHEDULE", "LI0_Ver", "LI0_Init", "LI0_Op", "LI0_Op3", "LI0_Op252" };

FILE * log_file;
FILE * asc_file;

void clean_up()
{
#ifdef GENERATE_TEXT_LOG
  fclose(log_file);
#endif

#ifdef GENERATE_ASC_LOG
  fprintf(asc_file, "End TriggerBlock\n");
  fclose(asc_file);
#endif
  exit(0);
}

int main(int argc, char **argv)  {
  int sleep_time = 100;
  long int loop_number = 0;
  long int evToSe_miss = -1;
  long int seToEv_miss = -1;
  double ref_time = 0;

  if (argc > 1) {
    int temp;
    if (sscanf(argv[1], "%d", &temp))  {
      sleep_time = temp;
    }
  }
  if (argc == 4) {
    int temp;
    if (sscanf(argv[2], "%d", &temp))  {
      evToSe_miss = temp;
    } else exit(1);
    if (sscanf(argv[3], "%d", &temp))  {
      seToEv_miss = temp;
    } else exit(1);
  }

  char log_name[MAX_LOG_NAME];
  char asc_name[MAX_LOG_NAME];
  char time_string[MAX_TIME_STRING];
  time_t time_int = time(NULL);
  strftime(log_name, MAX_LOG_NAME, "LIN-CP_emu_Log_%Y-%m-%d_%H-%M-%S.txt", localtime(&time_int));
  strftime(asc_name, MAX_LOG_NAME, "LIN-CP_emu_Log_%Y-%m-%d_%H-%M-%S.asc", localtime(&time_int));   //~~lazy~~
  strftime(time_string, MAX_TIME_STRING, "%a %b %-e %I:%M:%S.000 %P %Y", localtime(&time_int));	//%e includes a leading space vector does not use. '-' removes in GCC.  We don't have easy access to sub-second so just use .000

#ifdef GENERATE_TEXT_LOG
  if ((log_file = fopen(log_name,"w")) == NULL){
    printf("Error opening Text Log file");
    exit(1);
  }
#endif

#ifdef GENERATE_ASC_LOG
  if ((asc_file = fopen(asc_name,"w")) == NULL){
    printf("Error opening .asc Log file");
    exit(1);
  }

  fprintf(asc_file, "date %s\n",time_string);
	fprintf(asc_file, "base hex  timestamps absolute\ninternal events logged\n// version 11.0.0\n");
	fprintf(asc_file, "Begin TriggerBlock %s\n",time_string);
	fprintf(asc_file, "%11.6f Start of measurement\n", ref_time);

	ref_time += 0.0028;
	fprintf(asc_file, "%11.6f Li SleepModeEvent 0 starting up in wake mode\n", ref_time);
#endif

  signal(SIGINT, clean_up);   //free log file on ^c

  //printf("%d, %d\n", evToSe_miss, seToEv_miss);

  generic_frame_t *se_frames, *ev_frames;

  int se_frame_memory_location = shm_open(SE_FRAME_MEMORY_IDENTIFIER, O_RDWR, 0777);

  if (se_frame_memory_location < 0) {
      /* handle error */
      printf("se_shm_open failed!\n");
      exit(1);
  } else {

      se_frames = (generic_frame_t *)mmap(NULL, NUMBER_OF_FRAMES*sizeof(generic_frame_t), PROT_READ | PROT_WRITE, MAP_SHARED, se_frame_memory_location, 0);

      if (!se_frames || se_frames == MAP_FAILED)  {
          /* handle error */ ;
          printf("se mmap failed!\n");
          exit(1);
      }
  }
  schedule_picker_t *schedule_picker_p;

  int schedule_picker_memory_location = shm_open(SE_SCHEDULE_MEMORY_IDENTIFIER, O_RDWR, 0777);

  if (schedule_picker_memory_location < 0) {
      /* handle error */
      printf("shm_open failed!\n");
      exit(1);
  } else {

      schedule_picker_p = (schedule_picker_t *)mmap(NULL, sizeof(schedule_picker_t), PROT_READ | PROT_WRITE, MAP_SHARED, schedule_picker_memory_location, 0);

      if (!schedule_picker_p || schedule_picker_p == MAP_FAILED)  {
          /* handle error */ ;
          printf("mmap failed!\n");
          exit(1);
      }
  }

  int ev_frame_memory_location = shm_open(EV_FRAME_MEMORY_IDENTIFIER, O_RDWR, 0777);

  if (ev_frame_memory_location < 0) {
      /* handle error */
      printf("ev_shm_open failed!\n");
      exit(1);
  } else {

      ev_frames = (generic_frame_t *)mmap(NULL, NUMBER_OF_FRAMES*sizeof(generic_frame_t), PROT_READ | PROT_WRITE, MAP_SHARED, ev_frame_memory_location, 0);

      if (!ev_frames || ev_frames == MAP_FAILED)  {
          /* handle error */ ;
          printf("ev mmap failed!\n");
          exit(1);
      }
  }

  schedule_action_t *current_schedule_p;

  while(1)  {
#ifdef GENERATE_TEXT_LOG
    fprintf(log_file, "=========================================================\n");
    fprintf(log_file, "Schedule: %s, Transfer: %ld\n", schedule_strings[*schedule_picker_p], loop_number);
#endif
    switch(*schedule_picker_p)  {
      case LI0_LIN_NULL_SCHEDULE:
      case LI0_GOTO_SLEEP_SCHEDULE:
        goto loop_end;
      case LI0_Ver:
        current_schedule_p = schedule_ver;
        break;
      case LI0_Init:
        current_schedule_p = schedule_init;
        break;
      case LI0_Op:
        current_schedule_p = schedule_op_base;
        break;
      case LI0_Op3:
        current_schedule_p = schedule_op_slash1;
        break;
      case LI0_Op252:
        current_schedule_p = schedule_op_slash2;
        break;
    }

    for(int i = 0; i< NUMBER_OF_FRAMES; i++)  {
      generic_frame_t *source, *destination;
      switch(current_schedule_p[i])  {
        case DoNothing:
          source = NULL;
          destination = NULL;
          break;
        case CopySeToEv:
          if(loop_number == seToEv_miss)  {
            source = NULL;
            destination = NULL;
          } else  {
            source = se_frames;
            destination = ev_frames;
          }
          break;
        case CopyEvToSe:
          if(loop_number == evToSe_miss)  {
            source = NULL;
            destination = NULL;
          } else  {
            source = ev_frames;
            destination = se_frames;
          }
          break;
      }
      if(source != NULL && destination != NULL) {
        for(int j = 0; j < 8; j++)  {
          destination[i].data[j] = source[i].data[j];
        }
        destination[i].flag = true;    //Set the received flag
        source[i].flag = true;         //Set the transmitted flag
#ifdef GENERATE_TEXT_LOG
        print_specific_frame(log_file, i, source[i].data);
#endif

#ifdef GENERATE_ASC_LOG
        print_LIN_record(asc_file, ref_time += 0.011, i, &(source[i]));
#endif
      }
    }

loop_end:
    msleep(sleep_time);
    printf("Schedule: %s\n", schedule_strings[*schedule_picker_p]);
    loop_number++;
  }

  fclose(log_file);
  return 0;
}