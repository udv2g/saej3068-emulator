#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "lincp-emu.h"

schedule_action_t schedule_ver[NUMBER_OF_FRAMES] = {
  CopySeToEv,     //SeVersionList
  CopyEvToSe,     //EvVersionList
  DoNothing,      //SeStatus
  DoNothing,      //EvStatus
  DoNothing,      //EvPresentCurrentList
  DoNothing,      //SeNomVoltages
  DoNothing,      //SeMaxCurrentList
  DoNothing,      //EvMaxVoltageList
  DoNothing,      //EvMinVoltageList
  DoNothing,      //EvMaxMinCurrents
  DoNothing,      //CaProperties
  CopySeToEv,     //SeInfoList
  CopyEvToSe,     //EvInfoList
  DoNothing,      //StErrorList
  DoNothing,      //EvErrorList
  DoNothing,      //SeID
  DoNothing,      //EvID
};

schedule_action_t schedule_init[NUMBER_OF_FRAMES] = {
  DoNothing,      //SeVersionList
  DoNothing,      //EvVersionList
  CopySeToEv,     //SeStatus
  CopyEvToSe,     //EvStatus
  DoNothing,      //EvPresentCurrentList
  CopySeToEv,     //SeNomVoltages
  CopySeToEv,     //SeMaxCurrentList
  CopyEvToSe,     //EvMaxVoltageList
  CopyEvToSe,     //EvMinVoltageList
  CopyEvToSe,     //EvMaxMinCurrents
  DoNothing,      //CaProperties
  CopySeToEv,     //SeInfoList
  CopyEvToSe,     //EvInfoList
  DoNothing,      //StErrorList
  DoNothing,      //EvErrorList
  DoNothing,      //SeID
  DoNothing,      //EvID
};

schedule_action_t schedule_op_base[NUMBER_OF_FRAMES] = {
  DoNothing,      //SeVersionList
  DoNothing,      //EvVersionList
  CopySeToEv,     //SeStatus
  CopyEvToSe,     //EvStatus
  CopyEvToSe,     //EvPresentCurrentList
  DoNothing,      //SeNomVoltages
  DoNothing,      //SeMaxCurrentList
  DoNothing,      //EvMaxVoltageList
  DoNothing,      //EvMinVoltageList
  DoNothing,      //EvMaxMinCurrents
  DoNothing,      //CaProperties
  CopySeToEv,     //SeInfoList
  CopyEvToSe,     //EvInfoList
  DoNothing,      //StErrorList
  DoNothing,      //EvErrorList
  DoNothing,      //SeID
  DoNothing,      //EvID
};

schedule_action_t schedule_op_slash1[NUMBER_OF_FRAMES] = {
  DoNothing,      //SeVersionList
  DoNothing,      //EvVersionList
  CopySeToEv,     //SeStatus
  CopyEvToSe,     //EvStatus
  CopyEvToSe,     //EvPresentCurrentList
  DoNothing,      //SeNomVoltages
  DoNothing,      //SeMaxCurrentList
  DoNothing,      //EvMaxVoltageList
  DoNothing,      //EvMinVoltageList
  DoNothing,      //EvMaxMinCurrents
  DoNothing,      //CaProperties
  CopySeToEv,     //SeInfoList
  CopyEvToSe,     //EvInfoList
  DoNothing,      //StErrorList
  DoNothing,      //EvErrorList
  CopySeToEv,     //SeID
  CopyEvToSe,     //EvID
};

void verify_type_sizes()  {
  printf("%s, %ld\n", FRAME_SIZE(generic_frame_t));
  printf("%s, %ld\n", FRAME_SIZE(SeVersionList_t));
  printf("%s, %ld\n", FRAME_SIZE(EvVersionList_t));
  printf("%s, %ld\n", FRAME_SIZE(SeStatus_t));
  printf("%s, %ld\n", FRAME_SIZE(EvStatus_t));
  printf("%s, %ld\n", FRAME_SIZE(EvPresentCurrentList_t));
  printf("%s, %ld\n", FRAME_SIZE(SeNomVoltages_t));
  printf("%s, %ld\n", FRAME_SIZE(SeMaxCurrentList_t));
  printf("%s, %ld\n", FRAME_SIZE(EvMaxVoltageList_t));
  printf("%s, %ld\n", FRAME_SIZE(EvMinVoltageList_t));
  printf("%s, %ld\n", FRAME_SIZE(EvMaxMinCurrents_t));
  printf("%s, %ld\n", FRAME_SIZE(SeInfoList_t));
  printf("%s, %ld\n", FRAME_SIZE(EvInfoList_t));
  printf("%s, %ld\n", FRAME_SIZE(SeID_t));
  printf("%s, %ld\n", FRAME_SIZE(EvID_t));
}


void print_generic_frame(uint8_t frame_number, generic_frame_t * frame)  {
  printf("Frame:%d, %x, %x, %x, %x, %x, %x, %x, %x%c\n", frame_number, frame->data[0], frame->data[1], frame->data[2], frame->data[3], frame->data[4], frame->data[5], frame->data[6], frame->data[7], frame->flag? '*': ' ');
}

void print_specific_frame(FILE * stream, uint8_t frame_number, void * frame) {
  CREATE_FRAME_POINTERS();
  char * statusVerInit[] = { "Incomplete", "Complete", "Error", "Not_Available" };
  char * statusOp[] = { "Deny_V", "Permit_V", "Error", "Not_Available" };
  char * awake[] = { "Sleep", "Awake" };
  char * error[] = { "No_Error", "Error" };

#define PRINT_FRAME_VARIABLE(frame_name, variable_name, format) fprintf(stream,  "%42s: %" #format "\n", #frame_name "->" #variable_name, (frame_name##_p)->variable_name )
#define PRINT_FRAME_ENUM(frame_name, variable_name, strings) fprintf(stream, "%42s: %s\n", #frame_name "->" #variable_name, strings[(frame_name##_p)->variable_name] )

  switch(frame_number) {
    case 0:
      SeVersionList_p = frame;
      PRINT_FRAME_VARIABLE(SeVersionList, SeSelectedVersion, d);
      PRINT_FRAME_ENUM(    SeVersionList, SeStatusOp, statusOp);
      PRINT_FRAME_ENUM(    SeVersionList, SeStatusInit, statusVerInit);
      PRINT_FRAME_ENUM(    SeVersionList, SeStatusVer, statusVerInit);
      PRINT_FRAME_VARIABLE(SeVersionList, SeVersionPageNumber, d);
      PRINT_FRAME_VARIABLE(SeVersionList, SeSupportedVersion1, d);
      PRINT_FRAME_VARIABLE(SeVersionList, SeSupportedVersion2, d);
      PRINT_FRAME_VARIABLE(SeVersionList, SeSupportedVersion3, d);
      PRINT_FRAME_VARIABLE(SeVersionList, SeSupportedVersion4, d);
      PRINT_FRAME_VARIABLE(SeVersionList, SeSupportedVersion5, d);
      break;
    case 1:
      EvVersionList_p = frame;
      PRINT_FRAME_VARIABLE(EvVersionList, EvSelectedVersion, d);
      PRINT_FRAME_ENUM(    EvVersionList, EvAwake, awake);
      PRINT_FRAME_ENUM(    EvVersionList, EvStatusOp, statusOp);
      PRINT_FRAME_ENUM(    EvVersionList, EvStatusInit, statusVerInit);
      PRINT_FRAME_ENUM(    EvVersionList, EvStatusVer, statusVerInit);
      PRINT_FRAME_ENUM(    EvVersionList, EvResponseError, error);
      PRINT_FRAME_VARIABLE(EvVersionList, EvVersionPageNumber, d);
      PRINT_FRAME_VARIABLE(EvVersionList, EvSupportedVersion1, d);
      PRINT_FRAME_VARIABLE(EvVersionList, EvSupportedVersion2, d);
      PRINT_FRAME_VARIABLE(EvVersionList, EvSupportedVersion3, d);
      PRINT_FRAME_VARIABLE(EvVersionList, EvSupportedVersion4, d);
      PRINT_FRAME_VARIABLE(EvVersionList, EvSupportedVersion5, d);
      break;
    case 2:
      SeStatus_p = frame;
      PRINT_FRAME_VARIABLE(SeStatus, SeSelectedVersion, d);
      PRINT_FRAME_ENUM(    SeStatus, SeStatusOp, statusOp);
      PRINT_FRAME_ENUM(    SeStatus, SeStatusInit, statusVerInit);
      PRINT_FRAME_ENUM(    SeStatus, SeStatusVer, statusVerInit);
      PRINT_FRAME_VARIABLE(SeStatus, SeAvailableCurrentL1, d);
      PRINT_FRAME_VARIABLE(SeStatus, SeAvailableCurrentL2, d);
      PRINT_FRAME_VARIABLE(SeStatus, SeAvailableCurrentL3, d);
      PRINT_FRAME_VARIABLE(SeStatus, SeAvailableCurrentN, d);
      break;
    case 3:
      EvStatus_p = frame;
      PRINT_FRAME_VARIABLE(EvStatus, EvSelectedVersion, d);
      PRINT_FRAME_ENUM(    EvStatus, EvAwake, awake);
      PRINT_FRAME_ENUM(    EvStatus, EvStatusOp, statusOp);
      PRINT_FRAME_ENUM(    EvStatus, EvStatusInit, statusVerInit);
      PRINT_FRAME_ENUM(    EvStatus, EvStatusVer, statusVerInit);
      PRINT_FRAME_ENUM(    EvStatus, EvResponseError, error);
      PRINT_FRAME_VARIABLE(EvStatus, EvRequestedCurrentL1, d);
      PRINT_FRAME_VARIABLE(EvStatus, EvRequestedCurrentL2, d);
      PRINT_FRAME_VARIABLE(EvStatus, EvRequestedCurrentL3, d);
      PRINT_FRAME_VARIABLE(EvStatus, EvRequestedCurrentN, d);
      break;
    case 4:
      EvPresentCurrentList_p = frame;
      PRINT_FRAME_VARIABLE(EvPresentCurrentList, EvSelectedVersion, d);
      PRINT_FRAME_VARIABLE(EvPresentCurrentList, EvPresentCurrentL1, d);
      PRINT_FRAME_VARIABLE(EvPresentCurrentList, EvPresentCurrentL2, d);
      PRINT_FRAME_VARIABLE(EvPresentCurrentList, EvPresentCurrentL3, d);
      PRINT_FRAME_VARIABLE(EvPresentCurrentList, EvPresentCurrentN, d);
      break;
    case 5:
      SeNomVoltages_p = frame;
      PRINT_FRAME_VARIABLE(SeNomVoltages, SeSelectedVersion, d);
      PRINT_FRAME_VARIABLE(SeNomVoltages, SeNomVoltageL1N, d);
      PRINT_FRAME_VARIABLE(SeNomVoltages, SeNomVoltageLL, d);
      PRINT_FRAME_VARIABLE(SeNomVoltages, SeFrequency, d);
      break;
    case 6:
      SeMaxCurrentList_p = frame;
      PRINT_FRAME_VARIABLE(SeMaxCurrentList, SeSelectedVersion, d);
      PRINT_FRAME_VARIABLE(SeMaxCurrentList, SeMaxCurrentL1, d);
      PRINT_FRAME_VARIABLE(SeMaxCurrentList, SeMaxCurrentL2, d);
      PRINT_FRAME_VARIABLE(SeMaxCurrentList, SeMaxCurrentL3, d);
      PRINT_FRAME_VARIABLE(SeMaxCurrentList, SeMaxCurrentN, d);
      break;
    case 7:
      EvMaxVoltageList_p = frame;
      PRINT_FRAME_VARIABLE(EvMaxVoltageList, EvSelectedVersion, d);
      PRINT_FRAME_VARIABLE(EvMaxVoltageList, EvMaxVoltageL1N, d);
      PRINT_FRAME_VARIABLE(EvMaxVoltageList, EvMaxVoltageLL, d);
      PRINT_FRAME_VARIABLE(EvMaxVoltageList, EvFrequencies, d);
      break;
    case 8:
      EvMinVoltageList_p = frame;
      PRINT_FRAME_VARIABLE(EvMinVoltageList, EvSelectedVersion, d);
      PRINT_FRAME_VARIABLE(EvMinVoltageList, EvMinVoltageL1N, d);
      PRINT_FRAME_VARIABLE(EvMinVoltageList, EvMinVoltageLL, d);
      break;
    case 9:
      EvMaxMinCurrents_p = frame;
      PRINT_FRAME_VARIABLE(EvMaxMinCurrents, EvSelectedVersion, d);
      PRINT_FRAME_VARIABLE(EvMaxMinCurrents, EvMaxCurrentL1, d);
      PRINT_FRAME_VARIABLE(EvMaxMinCurrents, EvMaxCurrentL2, d);
      PRINT_FRAME_VARIABLE(EvMaxMinCurrents, EvMaxCurrentL3, d);
      PRINT_FRAME_VARIABLE(EvMaxMinCurrents, EvMaxCurrentN, d);
      PRINT_FRAME_VARIABLE(EvMaxMinCurrents, EvMinCurrentL1, d);
      PRINT_FRAME_VARIABLE(EvMaxMinCurrents, EvMinCurrentL2, d);
      PRINT_FRAME_VARIABLE(EvMaxMinCurrents, EvMinCurrentL3, d);
      break;
    case 11:
      SeInfoList_p = frame;
      PRINT_FRAME_VARIABLE(SeInfoList, SeSelectedVersion, d);
      PRINT_FRAME_VARIABLE(SeInfoList, SeInfoPageNumber, d);
      PRINT_FRAME_VARIABLE(SeInfoList, SeInfoEntry1, 0X);
      PRINT_FRAME_VARIABLE(SeInfoList, SeInfoEntry2, 0X);
      PRINT_FRAME_VARIABLE(SeInfoList, SeInfoEntry3, 0X);
      PRINT_FRAME_VARIABLE(SeInfoList, SeInfoEntry4, 0X);
      PRINT_FRAME_VARIABLE(SeInfoList, SeInfoEntry5, 0X);
      PRINT_FRAME_VARIABLE(SeInfoList, SeInfoEntry6, 0X);
      break;
    case 12:
      EvInfoList_p = frame;
      PRINT_FRAME_VARIABLE(EvInfoList, EvSelectedVersion, d);
      PRINT_FRAME_VARIABLE(EvInfoList, EvInfoPageNumber, d);
      PRINT_FRAME_VARIABLE(EvInfoList, EvInfoEntry1, 0X);
      PRINT_FRAME_VARIABLE(EvInfoList, EvInfoEntry2, 0X);
      PRINT_FRAME_VARIABLE(EvInfoList, EvInfoEntry3, 0X);
      PRINT_FRAME_VARIABLE(EvInfoList, EvInfoEntry4, 0X);
      PRINT_FRAME_VARIABLE(EvInfoList, EvInfoEntry5, 0X);
      PRINT_FRAME_VARIABLE(EvInfoList, EvInfoEntry6, 0X);
      break;
    //case 13:
    //  StErrorList_p = frame;
    //  PRINT_FRAME_VARIABLE(StErrorList, StSelectedVersion, d);
    //  PRINT_FRAME_VARIABLE(StErrorList, StErrorPageNumber, d);
    //  PRINT_FRAME_VARIABLE(StErrorList, StErrorEntry1, 0X);
    //  PRINT_FRAME_VARIABLE(StErrorList, StErrorEntry2, 0X);
    //  PRINT_FRAME_VARIABLE(StErrorList, StErrorEntry3, 0X);
    //  PRINT_FRAME_VARIABLE(StErrorList, StErrorEntry4, 0X);
    //  PRINT_FRAME_VARIABLE(StErrorList, StErrorEntry5, 0X);
    //  PRINT_FRAME_VARIABLE(StErrorList, StErrorEntry6, 0X);
    //  break;
    //case 14:
    //  EvErrorList_p = frame;
    //  PRINT_FRAME_VARIABLE(EvErrorList, EvSelectedVersion, d);
    //  PRINT_FRAME_VARIABLE(EvErrorList, EvErrorPageNumber, d);
    //  PRINT_FRAME_VARIABLE(EvErrorList, EvErrorEntry1, 0X);
    //  PRINT_FRAME_VARIABLE(EvErrorList, EvErrorEntry2, 0X);
    //  PRINT_FRAME_VARIABLE(EvErrorList, EvErrorEntry3, 0X);
    //  PRINT_FRAME_VARIABLE(EvErrorList, EvErrorEntry4, 0X);
    //  PRINT_FRAME_VARIABLE(EvErrorList, EvErrorEntry5, 0X);
    //  PRINT_FRAME_VARIABLE(EvErrorList, EvErrorEntry6, 0X);
    //  break;
    case 15:
      SeID_p = frame;
      PRINT_FRAME_VARIABLE(SeID, SeIDPageNumber, d);
      PRINT_FRAME_VARIABLE(SeID, SeIDByteA, 0X);
      PRINT_FRAME_VARIABLE(SeID, SeIDByteB, 0X);
      PRINT_FRAME_VARIABLE(SeID, SeIDByteC, 0X);
      PRINT_FRAME_VARIABLE(SeID, SeIDByteD, 0X);
      PRINT_FRAME_VARIABLE(SeID, SeIDByteE, 0X);
      PRINT_FRAME_VARIABLE(SeID, SeIDByteF, 0X);
      PRINT_FRAME_VARIABLE(SeID, SeIDByteG, 0X);
      break;
    case 16:
      EvID_p = frame;
      PRINT_FRAME_VARIABLE(EvID, EvIDPageNumber, d);
      PRINT_FRAME_VARIABLE(EvID, EvIDByteA, 0X);
      PRINT_FRAME_VARIABLE(EvID, EvIDByteB, 0X);
      PRINT_FRAME_VARIABLE(EvID, EvIDByteC, 0X);
      PRINT_FRAME_VARIABLE(EvID, EvIDByteD, 0X);
      PRINT_FRAME_VARIABLE(EvID, EvIDByteE, 0X);
      PRINT_FRAME_VARIABLE(EvID, EvIDByteF, 0X);
      PRINT_FRAME_VARIABLE(EvID, EvIDByteG, 0X);
      break;
  }
}

void print_all_frames(generic_frame_t * frames)  {
  for(int i = 0; i< NUMBER_OF_FRAMES; i++)  {
      print_generic_frame(i, frames + i);
  }
}

void init_all_frames(generic_frame_t * frames)  {
  for(int i = 0; i< NUMBER_OF_FRAMES; i++)  {
    (frames + i)->flag = 0;
    for (int j = 0; j < 8; j++) {
      (frames + i)->data[j] = 0xFF;
    }
  }
}


//utils.c
void mem_init(void *obj_p, uint8_t size, uint8_t value) {
  uint8_t i, *obj_u8_p = obj_p;

  for (i = 0; i < size; i++) {
    obj_u8_p[i] = value;
  }
}

void mem_copy(void *source_p, void *dest_p, uint8_t size) {
  uint8_t i, *source_u8_p = source_p, *dest_u8_p = dest_p;

  for (i = 0; i < size; i++) {
    dest_u8_p[i] = source_u8_p[i];
  }
}

void DetermineEvState(uint8_t ch, DETERMINE_STATE_CODE code) {(void)0;}
void DetermineEvseState(uint8_t ch, DETERMINE_STATE_CODE code) {(void)0;}