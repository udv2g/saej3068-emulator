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
#include <signal.h>
#include "lin-cp.h"
#include <errno.h>
#include "info_code_handler.h"
#include "protocol_version_handler.h"

generic_frame_t *all_frames;
schedule_picker_t *schedule_picker_p;

CREATE_FRAME_POINTERS();

void clean_up()
{
  shm_unlink(FRAME_MEMORY_IDENTIFIER);
#ifdef SE_CONFIG
  shm_unlink(SE_SCHEDULE_MEMORY_IDENTIFIER);
#endif
  exit(0);
}

void sig_user_1() {   //for asynchronous tests
  printf("SIGUSR1 Received!\n");
  //print_all_info_codes(A);
  //printf("code %X status is %x\n", 0x25, check_remote_code(0x25));
  printf("pver %X status is %x\n", 0x0, check_rcvd_pvers(0x0));
  printf("pver %X status is %x\n", 252, check_rcvd_pvers(252));
  printf("pver %X status is %x\n", 75, check_rcvd_pvers(75));
}

int main()  {
  int frame_memory_location = shm_open(FRAME_MEMORY_IDENTIFIER, O_RDWR | O_CREAT, 0777);

  if (frame_memory_location < 0) {
      /* handle error */
      printf("shm_open failed!\n");
      exit(1);
  } else {

      ftruncate(frame_memory_location, NUMBER_OF_FRAMES*sizeof(generic_frame_t));
      all_frames = (generic_frame_t *)mmap(NULL, NUMBER_OF_FRAMES*sizeof(generic_frame_t), PROT_READ | PROT_WRITE, MAP_SHARED, frame_memory_location, 0);

      if (!all_frames || all_frames == MAP_FAILED)  {
          /* handle error */ ;
          printf("mmap failed!\n");
          exit(1);
      }
  }

#ifdef SE_CONFIG
  int schedule_picker_memory_location = shm_open(SE_SCHEDULE_MEMORY_IDENTIFIER, O_RDWR | O_CREAT, 0777);
  printf("%d\n", errno);

  if (schedule_picker_memory_location < 0) {
      /* handle error */
      printf("shm_open failed!\n");
      exit(1);
  } else {

      ftruncate(schedule_picker_memory_location, sizeof(schedule_picker_t));
      schedule_picker_p = (schedule_picker_t *)mmap(NULL, sizeof(schedule_picker_t), PROT_READ | PROT_WRITE, MAP_SHARED, schedule_picker_memory_location, 0);

      if (!schedule_picker_p || schedule_picker_p == MAP_FAILED)  {
          /* handle error */ ;
          printf("mmap failed!\n");
          exit(1);
      }
  }
#endif

  signal(SIGINT, clean_up);     //free memory on ^c
  signal(SIGUSR1, sig_user_1);  //asynchronous tests

  ASSIGN_ALL_FRAMES();

  init_all_frames(all_frames);

  InitializeLINCP();
  DetermineLINCPState(A, startLIN);

  //print_all_frames(all_frames);
  //verify_type_sizes();
  //init_all_frames(all_frames);
  //print_all_frames(all_frames);
  //SeVersionList_p->SeSelectedVersion = 2;
  //SeVersionList_p->SeVersionPageNumber = 0;
  //SeVersionList_p->SeStatusVer = 0;
  //print_all_frames(all_frames);

  while(1)  {
    DetermineLINCPState(A, continueOnLIN);
    //print_all_frames(all_frames);
    //printf("=============================================\n");
    msleep(20);
  }

  shm_unlink(FRAME_MEMORY_IDENTIFIER);
#ifdef SE_CONFIG
  shm_unlink(SE_SCHEDULE_MEMORY_IDENTIFIER);
#endif

  return 0;
}