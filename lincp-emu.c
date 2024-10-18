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
  DoNothing,      //EvPresentCurrents
  DoNothing,      //SeNomVoltages
  DoNothing,      //SeMaxCurrents
  DoNothing,      //EvMaxVoltages
  DoNothing,      //EvMinVoltages
  DoNothing,      //EvMaxMinCurrents
  DoNothing,      //CaProperties
  CopySeToEv,     //SeInfoList
  CopyEvToSe,     //EvInfoList
  DoNothing,      //StErrorList
  DoNothing,      //EvErrorList
  DoNothing,      //SeID
  DoNothing,      //EvID
  DoNothing,      //17
  DoNothing,      //18
  DoNothing,      //19
  DoNothing,      //20
  DoNothing,      //EvModeCtrl
  DoNothing,      //SeModeCtrl
  DoNothing,      //EvJ3072
  DoNothing,      //SeJ3072
  DoNothing,      //SeTargets1
};

schedule_action_t schedule_init[NUMBER_OF_FRAMES] = {
  DoNothing,      //SeVersionList
  DoNothing,      //EvVersionList
  CopySeToEv,     //SeStatus
  CopyEvToSe,     //EvStatus
  DoNothing,      //EvPresentCurrents
  CopySeToEv,     //SeNomVoltages
  CopySeToEv,     //SeMaxCurrents
  CopyEvToSe,     //EvMaxVoltages
  CopyEvToSe,     //EvMinVoltages
  CopyEvToSe,     //EvMaxMinCurrents
  DoNothing,      //CaProperties
  CopySeToEv,     //SeInfoList
  CopyEvToSe,     //EvInfoList
  DoNothing,      //StErrorList
  DoNothing,      //EvErrorList
  DoNothing,      //SeID
  DoNothing,      //EvID
  DoNothing,      //17
  DoNothing,      //18
  DoNothing,      //19
  DoNothing,      //20
  DoNothing,      //EvModeCtrl
  DoNothing,      //SeModeCtrl
  DoNothing,      //EvJ3072
  DoNothing,      //SeJ3072
  DoNothing,      //SeTargets1
};

schedule_action_t schedule_op_base[NUMBER_OF_FRAMES] = {
  DoNothing,      //SeVersionList
  DoNothing,      //EvVersionList
  CopySeToEv,     //SeStatus
  CopyEvToSe,     //EvStatus
  CopyEvToSe,     //EvPresentCurrents
  DoNothing,      //SeNomVoltages
  DoNothing,      //SeMaxCurrents
  DoNothing,      //EvMaxVoltages
  DoNothing,      //EvMinVoltages
  DoNothing,      //EvMaxMinCurrents
  DoNothing,      //CaProperties
  CopySeToEv,     //SeInfoList
  CopyEvToSe,     //EvInfoList
  DoNothing,      //StErrorList
  DoNothing,      //EvErrorList
  DoNothing,      //SeID
  DoNothing,      //EvID
  DoNothing,      //17
  DoNothing,      //18
  DoNothing,      //19
  DoNothing,      //20
  DoNothing,      //EvModeCtrl
  DoNothing,      //SeModeCtrl
  DoNothing,      //EvJ3072
  DoNothing,      //SeJ3072
  DoNothing,      //SeTargets1
};

schedule_action_t schedule_op_slash1[NUMBER_OF_FRAMES] = {
  DoNothing,      //SeVersionList
  DoNothing,      //EvVersionList
  CopySeToEv,     //SeStatus
  CopyEvToSe,     //EvStatus
  CopyEvToSe,     //EvPresentCurrents
  DoNothing,      //SeNomVoltages
  DoNothing,      //SeMaxCurrents
  DoNothing,      //EvMaxVoltages
  DoNothing,      //EvMinVoltages
  DoNothing,      //EvMaxMinCurrents
  DoNothing,      //CaProperties
  CopySeToEv,     //SeInfoList
  CopyEvToSe,     //EvInfoList
  DoNothing,      //StErrorList
  DoNothing,      //EvErrorList
  CopySeToEv,     //SeID
  CopyEvToSe,     //EvID
  DoNothing,      //17
  DoNothing,      //18
  DoNothing,      //19
  DoNothing,      //20
  DoNothing,      //EvModeCtrl
  DoNothing,      //SeModeCtrl
  DoNothing,      //EvJ3072
  DoNothing,      //SeJ3072
  DoNothing,      //SeTargets1
};

schedule_action_t schedule_op_slash2[NUMBER_OF_FRAMES] = {
  DoNothing,      //SeVersionList
  DoNothing,      //EvVersionList
  CopySeToEv,     //SeStatus
  CopyEvToSe,     //EvStatus
  CopyEvToSe,     //EvPresentCurrents
  DoNothing,      //SeNomVoltages
  DoNothing,      //SeMaxCurrents
  DoNothing,      //EvMaxVoltages
  DoNothing,      //EvMinVoltages
  DoNothing,      //EvMaxMinCurrents
  DoNothing,      //CaProperties
  CopySeToEv,     //SeInfoList
  CopyEvToSe,     //EvInfoList
  DoNothing,      //StErrorList
  DoNothing,      //EvErrorList
  CopySeToEv,     //SeID
  CopyEvToSe,     //EvID
  DoNothing,      //17
  DoNothing,      //18
  DoNothing,      //19
  DoNothing,      //20
  CopyEvToSe,      //EvModeCtrl
  CopySeToEv,      //SeModeCtrl
  CopyEvToSe,      //EvJ3072
  CopySeToEv,      //SeJ3072
  CopySeToEv,      //SeTargets1
};

void verify_type_sizes()  {
  printf("%s, %ld\n", FRAME_SIZE(generic_frame_t));
  printf("%s, %ld\n", FRAME_SIZE(SeVersionList_t));
  printf("%s, %ld\n", FRAME_SIZE(EvVersionList_t));
  printf("%s, %ld\n", FRAME_SIZE(SeStatus_t));
  printf("%s, %ld\n", FRAME_SIZE(EvStatus_t));
  printf("%s, %ld\n", FRAME_SIZE(EvPresentCurrents_t));
  printf("%s, %ld\n", FRAME_SIZE(SeNomVoltages_t));
  printf("%s, %ld\n", FRAME_SIZE(SeMaxCurrents_t));
  printf("%s, %ld\n", FRAME_SIZE(EvMaxVoltages_t));
  printf("%s, %ld\n", FRAME_SIZE(EvMinVoltages_t));
  printf("%s, %ld\n", FRAME_SIZE(EvMaxMinCurrents_t));
  printf("%s, %ld\n", FRAME_SIZE(SeInfoList_t));
  printf("%s, %ld\n", FRAME_SIZE(EvInfoList_t));
  printf("%s, %ld\n", FRAME_SIZE(SeID_t));
  printf("%s, %ld\n", FRAME_SIZE(EvID_t));
  printf("%s, %ld\n", FRAME_SIZE(EvModeCtrl_t));
  printf("%s, %ld\n", FRAME_SIZE(SeModeCtrl_t));
  printf("%s, %ld\n", FRAME_SIZE(EvJ3072_t));
  printf("%s, %ld\n", FRAME_SIZE(SeJ3072_t));
  printf("%s, %ld\n", FRAME_SIZE(SeTargets1_t));
}


void print_generic_frame(uint8_t frame_number, generic_frame_t * frame)  {
  printf("Frame:%d, %x, %x, %x, %x, %x, %x, %x, %x%c\n", frame_number, frame->data[0], frame->data[1], frame->data[2], frame->data[3], frame->data[4], frame->data[5], frame->data[6], frame->data[7], frame->flag? '*': ' ');
}

void print_LIN_record(FILE * stream, double time_end, uint8_t frame_number, generic_frame_t * frame)	{
	uint32_t checksum = 0;
	uint8_t p0, p1;

	fprintf(stream, "%11.6f Li %2x              Rx     8 ",time_end, frame_number);
	for (uint8_t i = 0; i<8; i++)	{
		fprintf(stream, "%02x ", frame->data[i]);
		checksum += frame->data[i];
	}
	p0 = (frame_number & 0b00000001) ^ ((frame_number >> 1) & 0b00000001) ^ ((frame_number >> 2) & 0b00000001) ^ ((frame_number >> 4) & 0b00000001);
	p1 = !(((frame_number >> 1) & 0b00000001) ^ ((frame_number >> 3) & 0b00000001) ^ ((frame_number >> 4) & 0b00000001) ^ ((frame_number >> 5) & 0b00000001));
	checksum += ((p1 << 7) | (p0 << 6) | frame_number);
	checksum = ~(checksum%255) & 0xFF;
	fprintf(stream, " checksum = %02x   ", checksum);
	fprintf(stream, "header time =  35, full time = 137  ");
	fprintf(stream, "SOF = %11.6f   BR = 19200  break = 673700 54250    ",time_end-=0.007073);
	fprintf(stream, "EOH = %11.6f    EOB =",time_end += 0.001797);
	time_end += 0.000128;
	for (uint8_t i = 0; i<8; i++)	{
		fprintf(stream, " %11.6f ", time_end += 0.000572);
	}
	fprintf(stream, "   sim = 0    EOF = %11.6f",time_end += 0.000572);
	fprintf(stream, "   RBR = 19200  HBR = 19200.000000  HSO = 22785     RSO = 22750     CSM = enhanced\n");

}

void print_specific_frame(FILE * stream, uint8_t frame_number, void * frame) {
  CREATE_FRAME_POINTERS();
  char * statusVerInit[] = { "Incomplete", "Complete", "Error", "Not_Available" };
  char * statusOp[] = { "Deny_V", "Permit_V", "Error", "Not_Available" };
  char * awake[] = { "Sleep", "Awake" };
  char * error[] = { "No_Error", "Error" };
  char * gridCode[] = {"Not_Supported", "Basic_V2G_settings_A", "Basic_V2X_settings_B", "UL_1741-SA", "IEEE_1547-2018_or_UL_1741-SB", "Error", "No_Request_or_Unconfigured"};
  char * gridCodeMod[] = {"Modified", "Unmodified"};
  char * ctrlModeAck[] = {"Normal_Charging", "CCL", "TC_P-", "TGC_P+-", "TC+R_II&III", "TGC+R_I-IV", "Autonomous_or_External", "Local_EPS_Forming", "Processing", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved"};
  char * evInverterState[] = {"Disconnected_Off", "Deep_sleep_offline", "Deep_sleep_online", "Light_Transparent_sleep", "Active_On", "In_Transition", "Not_Active_for_20+_seconds", "Error", "Not_Available"};
  char * sePwrCtrlAuth[] = {"No_Authorization", "Authorization_to_form_Local_EPS", "Authorization_to_Discharge", "Processing", "Reserved", "Reserved"};

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
      EvPresentCurrents_p = frame;
      PRINT_FRAME_VARIABLE(EvPresentCurrents, EvSelectedVersion, d);
      PRINT_FRAME_VARIABLE(EvPresentCurrents, EvPresentCurrentL1, d);
      PRINT_FRAME_VARIABLE(EvPresentCurrents, EvPresentCurrentL2, d);
      PRINT_FRAME_VARIABLE(EvPresentCurrents, EvPresentCurrentL3, d);
      PRINT_FRAME_VARIABLE(EvPresentCurrents, EvPresentCurrentN, d);
      break;
    case 5:
      SeNomVoltages_p = frame;
      PRINT_FRAME_VARIABLE(SeNomVoltages, SeSelectedVersion, d);
      PRINT_FRAME_VARIABLE(SeNomVoltages, SeNomVoltageL1N, d);
      PRINT_FRAME_VARIABLE(SeNomVoltages, SeNomVoltageLL, d);
      PRINT_FRAME_VARIABLE(SeNomVoltages, SeFrequency, d);
      break;
    case 6:
      SeMaxCurrents_p = frame;
      PRINT_FRAME_VARIABLE(SeMaxCurrents, SeSelectedVersion, d);
      PRINT_FRAME_VARIABLE(SeMaxCurrents, SeMaxCurrentL1, d);
      PRINT_FRAME_VARIABLE(SeMaxCurrents, SeMaxCurrentL2, d);
      PRINT_FRAME_VARIABLE(SeMaxCurrents, SeMaxCurrentL3, d);
      PRINT_FRAME_VARIABLE(SeMaxCurrents, SeMaxCurrentN, d);
      PRINT_FRAME_VARIABLE(SeMaxCurrents, SeConnectionType, 0X);
      break;
    case 7:
      EvMaxVoltages_p = frame;
      PRINT_FRAME_VARIABLE(EvMaxVoltages, EvSelectedVersion, d);
      PRINT_FRAME_VARIABLE(EvMaxVoltages, EvMaxVoltageL1N, d);
      PRINT_FRAME_VARIABLE(EvMaxVoltages, EvMaxVoltageLL, d);
      PRINT_FRAME_VARIABLE(EvMaxVoltages, EvFrequencies, d);
      break;
    case 8:
      EvMinVoltages_p = frame;
      PRINT_FRAME_VARIABLE(EvMinVoltages, EvSelectedVersion, d);
      PRINT_FRAME_VARIABLE(EvMinVoltages, EvMinVoltageL1N, d);
      PRINT_FRAME_VARIABLE(EvMinVoltages, EvMinVoltageLL, d);
      PRINT_FRAME_VARIABLE(EvMinVoltages, EvConnectionType, 0X);
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
      PRINT_FRAME_VARIABLE(SeID, SeIDPage, d);
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
      PRINT_FRAME_VARIABLE(EvID, EvIDPage, d);
      PRINT_FRAME_VARIABLE(EvID, EvIDByteA, 0X);
      PRINT_FRAME_VARIABLE(EvID, EvIDByteB, 0X);
      PRINT_FRAME_VARIABLE(EvID, EvIDByteC, 0X);
      PRINT_FRAME_VARIABLE(EvID, EvIDByteD, 0X);
      PRINT_FRAME_VARIABLE(EvID, EvIDByteE, 0X);
      PRINT_FRAME_VARIABLE(EvID, EvIDByteF, 0X);
      PRINT_FRAME_VARIABLE(EvID, EvIDByteG, 0X);
      break;

    //case 17:
    //  break;
    
    //case 18:
    //  break;

    //case 19:
    //  break;

    //case 20:
    //  break;

    // TODO Update to use enums.
    case 21:
      EvModeCtrl_p = frame;
      PRINT_FRAME_VARIABLE(EvModeCtrl, EvGridCodeStatusMod, 0x);
      PRINT_FRAME_VARIABLE(EvModeCtrl, EvGridCodeStatus, 0x);
      PRINT_FRAME_VARIABLE(EvModeCtrl, EvInverterState, 0x);
      PRINT_FRAME_VARIABLE(EvModeCtrl, EvPwrCtrlModeAck, 0x);
      PRINT_FRAME_VARIABLE(EvModeCtrl, EvPwrCtrlUnitsAvail, 0X);
      PRINT_FRAME_VARIABLE(EvModeCtrl, EvPwrCtrlModesAvail, 0x);
      break;

    // TODO Update to use enums.
    case 22:
      SeModeCtrl_p = frame;
      PRINT_FRAME_VARIABLE(SeModeCtrl, SeGridCodeRequest, 0x);
      PRINT_FRAME_VARIABLE(SeModeCtrl, SeInverterRequest, 0x);
      PRINT_FRAME_VARIABLE(SeModeCtrl, SePwrCtrlMode, 0x);
      PRINT_FRAME_VARIABLE(SeModeCtrl, SePwrCtrlUnits, 0X);
      PRINT_FRAME_VARIABLE(SeModeCtrl, SePwrCtrlAuth, 0x);
      PRINT_FRAME_VARIABLE(SeModeCtrl, SeTimeStamp, 0X);
      break;

    case 23:
      EvJ3072_p = frame;
      PRINT_FRAME_VARIABLE(EvJ3072, EvJ3072Page, d);
      PRINT_FRAME_VARIABLE(EvJ3072, EvJ3072ByteA, 0X);
      PRINT_FRAME_VARIABLE(EvJ3072, EvJ3072ByteB, 0X);
      PRINT_FRAME_VARIABLE(EvJ3072, EvJ3072ByteC, 0X);
      PRINT_FRAME_VARIABLE(EvJ3072, EvJ3072ByteD, 0X);
      PRINT_FRAME_VARIABLE(EvJ3072, EvJ3072ByteE, 0X);
      PRINT_FRAME_VARIABLE(EvJ3072, EvJ3072ByteF, 0X);
      PRINT_FRAME_VARIABLE(EvJ3072, EvJ3072ByteG, 0X);      
      break;

    case 24:
      SeJ3072_p = frame;
      PRINT_FRAME_VARIABLE(SeJ3072, SeJ3072Page, d);
      PRINT_FRAME_VARIABLE(SeJ3072, SeJ3072ByteA, 0X);
      PRINT_FRAME_VARIABLE(SeJ3072, SeJ3072ByteB, 0X);
      PRINT_FRAME_VARIABLE(SeJ3072, SeJ3072ByteC, 0X);
      PRINT_FRAME_VARIABLE(SeJ3072, SeJ3072ByteD, 0X);
      PRINT_FRAME_VARIABLE(SeJ3072, SeJ3072ByteE, 0X);
      PRINT_FRAME_VARIABLE(SeJ3072, SeJ3072ByteF, 0X);
      PRINT_FRAME_VARIABLE(SeJ3072, SeJ3072ByteG, 0X); 
      break;

    case 25:
      SeTargets1_p = frame;
      PRINT_FRAME_VARIABLE(SeTargets1, SeTargets1ElementA, 0X);
      PRINT_FRAME_VARIABLE(SeTargets1, SeTargets1ElementB, 0X);
      PRINT_FRAME_VARIABLE(SeTargets1, SeTargets1ElementC, 0X);
      PRINT_FRAME_VARIABLE(SeTargets1, SeTargets1ElementD, 0X);
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