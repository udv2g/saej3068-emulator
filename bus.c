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

char * schedule_strings[] = { "LI0_LIN_NULL_SCHEDULE", "LI0_GOTO_SLEEP_SCHEDULE", "LI0_Ver", "LI0_Init", "LI0_Op", "LI0_Op_S1" };

FILE * log_file;

void clean_up()
{
  fclose(log_file);
  exit(0);
}

int main(int argc, char **argv)  {
  int sleep_time = 100;
  long int loop_number = 0;
  long int evToSe_miss = -1;
  long int seToEv_miss = -1;

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
  time_t time_int = time(NULL);
  strftime(log_name, MAX_LOG_NAME, "LIN-CP_emu_Log_%Y-%m-%d_%H-%M-%S.txt", localtime(&time_int));

  if ((log_file = fopen(log_name,"w")) == NULL){
    printf("Error opening Log file");
    exit(1);
  }

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
    fprintf(log_file, "=========================================================\n");
    fprintf(log_file, "Schedule: %s, Transfer: %ld\n", schedule_strings[*schedule_picker_p], loop_number);
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
      case LI0_Op_S1:
        current_schedule_p = schedule_op_slash1;
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
          break;
          }
      }
      if(source != NULL && destination != NULL) {
        for(int j = 0; j < 8; j++)  {
          destination[i].data[j] = source[i].data[j];
        }
        destination[i].flag = true;    //Set the received flag
        source[i].flag = true;         //Set the transmitted flag
        print_specific_frame(log_file, i, source[i].data);
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