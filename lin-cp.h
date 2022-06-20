#ifndef LIN_CP_H
#define LIN_CP_H

#include <stdio.h>
#include "lincp-emu.h"

typedef uint8_t l_u8;
typedef uint16_t l_u16;
typedef uint32_t l_u32;

#define A 0
#define B 1
#define CH_A_ONLY

//Write functions for signals in multiple frames

#define l_u8_wr_LI0_SeStatusVer(x)        SeVersionList_p->SeStatusVer = (x);\
                                          SeStatus_p->SeStatusVer = (x)
#define l_u8_wr_LI0_SeStatusInit(x)       SeVersionList_p->SeStatusInit = (x);\
                                          SeStatus_p->SeStatusInit = (x)
#define l_u8_wr_LI0_SeStatusOp(x)         SeVersionList_p->SeStatusOp = (x);\
                                          SeStatus_p->SeStatusOp = (x)
#define l_bool_wr_LI0_EvResponseError(x)  EvVersionList_p->EvResponseError = (x);\
                                          EvStatus_p->EvResponseError = (x)
#define l_u8_wr_LI0_EvStatusVer(x)        EvVersionList_p->EvStatusVer = (x);\
                                          EvStatus_p->EvStatusVer = (x)
#define l_u8_wr_LI0_EvStatusInit(x)       EvVersionList_p->EvStatusInit = (x);\
                                          EvStatus_p->EvStatusInit = (x)
#define l_u8_wr_LI0_EvStatusOp(x)         EvVersionList_p->EvStatusOp = (x);\
                                          EvStatus_p->EvStatusOp = (x)
#define l_bool_wr_LI0_EvAwake(x)          EvVersionList_p->EvAwake = (x);\
                                          EvStatus_p->EvAwake = (x)
#define l_bool_wr_LI1_EvAwake(x)          (void)0

#define l_u8_wr_LI0_SeSelectedVersion(x)  SeVersionList_p->SeSelectedVersion = (x);\
                                          SeStatus_p->SeSelectedVersion = (x);\
                                          SeNomVoltages_p->SeSelectedVersion = (x);\
                                          SeMaxCurrentList_p->SeSelectedVersion = (x);\
                                          SeInfoList_p->SeSelectedVersion = (x);\
//                                          StErrorList_p->StSelectedVersion = (x)

#define l_u8_wr_LI0_EvSelectedVersion(x)  EvVersionList_p->EvSelectedVersion = (x);\
                                          EvStatus_p->EvSelectedVersion = (x);\
                                          EvPresentCurrentList_p->EvSelectedVersion = (x);\
                                          EvMaxVoltageList_p->EvSelectedVersion = (x);\
                                          EvMinVoltageList_p->EvSelectedVersion = (x);\
                                          EvMaxMinCurrents_p->EvSelectedVersion = (x);\
                                          EvInfoList_p->EvSelectedVersion = (x);\
//                                          EvErrorList_p->EvSelectedVersion = (x)

//SeVersionList, 0
#define l_u8_rd_LI0_SeSelectedVersion_SeVersionList() SeVersionList_p->SeSelectedVersion
#define l_u8_rd_LI0_SeStatusVer_SeVersionList()       SeVersionList_p->SeStatusVer
#define l_u8_rd_LI0_SeStatusInit_SeVersionList()      SeVersionList_p->SeStatusInit
#define l_u8_rd_LI0_SeStatusOp_SeVersionList()        SeVersionList_p->SeStatusOp
#define l_u8_rd_LI0_SeVersionPageNumber()  SeVersionList_p->SeVersionPageNumber
#define l_u8_wr_LI0_SeVersionPageNumber(x) SeVersionList_p->SeVersionPageNumber = (x)
#define l_u8_rd_LI0_SeSupportedVersion1()  SeVersionList_p->SeSupportedVersion1
#define l_u8_wr_LI0_SeSupportedVersion1(x) SeVersionList_p->SeSupportedVersion1 = (x)
#define l_u8_rd_LI0_SeSupportedVersion2()  SeVersionList_p->SeSupportedVersion2
#define l_u8_wr_LI0_SeSupportedVersion2(x) SeVersionList_p->SeSupportedVersion2 = (x)
#define l_u8_rd_LI0_SeSupportedVersion3()  SeVersionList_p->SeSupportedVersion3
#define l_u8_wr_LI0_SeSupportedVersion3(x) SeVersionList_p->SeSupportedVersion3 = (x)
#define l_u8_rd_LI0_SeSupportedVersion4()  SeVersionList_p->SeSupportedVersion4
#define l_u8_wr_LI0_SeSupportedVersion4(x) SeVersionList_p->SeSupportedVersion4 = (x)
#define l_u8_rd_LI0_SeSupportedVersion5()  SeVersionList_p->SeSupportedVersion5
#define l_u8_wr_LI0_SeSupportedVersion5(x) SeVersionList_p->SeSupportedVersion5 = (x)
#define l_flg_tst_LI0_SeVersionList_flag() all_frames[0].flag
#define l_flg_clr_LI0_SeVersionList_flag() all_frames[0].flag = false

//EvVersionList, 1
#define l_u8_rd_LI0_EvSelectedVersion_EvVersionList() EvVersionList_p->EvSelectedVersion
#define l_bool_rd_LI0_EvResponseError_EvVersionList() EvVersionList_p->EvResponseError
#define l_u8_rd_LI0_EvStatusVer_EvVersionList()       EvVersionList_p->EvStatusVer
#define l_u8_rd_LI0_EvStatusInit_EvVersionList()      EvVersionList_p->EvStatusInit
#define l_u8_rd_LI0_EvStatusOp_EvVersionList()        EvVersionList_p->EvStatusOp
#define l_bool_rd_LI0_EvAwake_EvVersionList()         EvVersionList_p->EvAwake
#define l_u8_rd_LI0_EvVersionPageNumber()  EvVersionList_p->EvVersionPageNumber
#define l_u8_wr_LI0_EvVersionPageNumber(x) EvVersionList_p->EvVersionPageNumber = (x)
#define l_u8_rd_LI0_EvSupportedVersion1()  EvVersionList_p->EvSupportedVersion1
#define l_u8_wr_LI0_EvSupportedVersion1(x) EvVersionList_p->EvSupportedVersion1 = (x)
#define l_u8_rd_LI0_EvSupportedVersion2()  EvVersionList_p->EvSupportedVersion2
#define l_u8_wr_LI0_EvSupportedVersion2(x) EvVersionList_p->EvSupportedVersion2 = (x)
#define l_u8_rd_LI0_EvSupportedVersion3()  EvVersionList_p->EvSupportedVersion3
#define l_u8_wr_LI0_EvSupportedVersion3(x) EvVersionList_p->EvSupportedVersion3 = (x)
#define l_u8_rd_LI0_EvSupportedVersion4()  EvVersionList_p->EvSupportedVersion4
#define l_u8_wr_LI0_EvSupportedVersion4(x) EvVersionList_p->EvSupportedVersion4 = (x)
#define l_u8_rd_LI0_EvSupportedVersion5()  EvVersionList_p->EvSupportedVersion5
#define l_u8_wr_LI0_EvSupportedVersion5(x) EvVersionList_p->EvSupportedVersion5 = (x)
#define l_flg_tst_LI0_EvVersionList_flag() all_frames[1].flag
#define l_flg_clr_LI0_EvVersionList_flag() all_frames[1].flag = false

//SeStatus, 2
#define l_u8_rd_LI0_SeSelectedVersion_SeStatus() SeStatus_p->SeSelectedVersion
#define l_u8_rd_LI0_SeStatusVer_SeStatus()       SeStatus_p->SeStatusVer
#define l_u8_rd_LI0_SeStatusInit_SeStatus()      SeStatus_p->SeStatusInit
#define l_u8_rd_LI0_SeStatusOp_SeStatus()        SeStatus_p->SeStatusOp
#define l_u8_rd_LI0_SeAvailableCurrentL1()  SeStatus_p->SeAvailableCurrentL1
#define l_u8_wr_LI0_SeAvailableCurrentL1(x) SeStatus_p->SeAvailableCurrentL1 = (x)
#define l_u8_rd_LI0_SeAvailableCurrentL2()  SeStatus_p->SeAvailableCurrentL2
#define l_u8_wr_LI0_SeAvailableCurrentL2(x) SeStatus_p->SeAvailableCurrentL2 = (x)
#define l_u8_rd_LI0_SeAvailableCurrentL3()  SeStatus_p->SeAvailableCurrentL3
#define l_u8_wr_LI0_SeAvailableCurrentL3(x) SeStatus_p->SeAvailableCurrentL3 = (x)
#define l_u8_rd_LI0_SeAvailableCurrentN()   SeStatus_p->SeAvailableCurrentN
#define l_u8_wr_LI0_SeAvailableCurrentN(x)  SeStatus_p->SeAvailableCurrentN = (x)
#define l_flg_tst_LI0_SeStatus_flag()       all_frames[2].flag
#define l_flg_clr_LI0_SeStatus_flag()       all_frames[2].flag = false

//EvStatus, 3
#define l_u8_rd_LI0_EvSelectedVersion_EvStatus() EvStatus_p->EvSelectedVersion
#define l_bool_rd_LI0_EvResponseError_EvStatus() EvStatus_p->EvResponseError
#define l_u8_rd_LI0_EvStatusVer_EvStatus()       EvStatus_p->EvStatusVer
#define l_u8_rd_LI0_EvStatusInit_EvStatus()      EvStatus_p->EvStatusInit
#define l_u8_rd_LI0_EvStatusOp_EvStatus()        EvStatus_p->EvStatusOp
#define l_bool_rd_LI0_EvAwake_EvStatus()         EvStatus_p->EvAwake
#define l_u8_rd_LI0_EvRequestedCurrentL1()  EvStatus_p->EvRequestedCurrentL1
#define l_u8_wr_LI0_EvRequestedCurrentL1(x) EvStatus_p->EvRequestedCurrentL1 = (x)
#define l_u8_rd_LI0_EvRequestedCurrentL2()  EvStatus_p->EvRequestedCurrentL2
#define l_u8_wr_LI0_EvRequestedCurrentL2(x) EvStatus_p->EvRequestedCurrentL2 = (x)
#define l_u8_rd_LI0_EvRequestedCurrentL3()  EvStatus_p->EvRequestedCurrentL3
#define l_u8_wr_LI0_EvRequestedCurrentL3(x) EvStatus_p->EvRequestedCurrentL3 = (x)
#define l_u8_rd_LI0_EvRequestedCurrentN()   EvStatus_p->EvRequestedCurrentN
#define l_u8_wr_LI0_EvRequestedCurrentN(x)  EvStatus_p->EvRequestedCurrentN = (x)
#define l_flg_tst_LI0_EvStatus_flag()       all_frames[3].flag
#define l_flg_clr_LI0_EvStatus_flag()       all_frames[3].flag = false

//EvPresentCurrentList, 4
#define l_u8_rd_LI0_EvSelectedVersionEvPresentCurrentList() EvPresentCurrentList_p->EvSelectedVersion
#define l_u8_rd_LI0_EvPresentCurrentL1()          EvPresentCurrentList_p->EvPresentCurrentL1
#define l_u8_wr_LI0_EvPresentCurrentL1(x)         EvPresentCurrentList_p->EvPresentCurrentL1 = (x)
#define l_u8_rd_LI0_EvPresentCurrentL2()          EvPresentCurrentList_p->EvPresentCurrentL2
#define l_u8_wr_LI0_EvPresentCurrentL2(x)         EvPresentCurrentList_p->EvPresentCurrentL2 = (x)
#define l_u8_rd_LI0_EvPresentCurrentL3()          EvPresentCurrentList_p->EvPresentCurrentL3
#define l_u8_wr_LI0_EvPresentCurrentL3(x)         EvPresentCurrentList_p->EvPresentCurrentL3 = (x)
#define l_u8_rd_LI0_EvPresentCurrentN()           EvPresentCurrentList_p->EvPresentCurrentN
#define l_u8_wr_LI0_EvPresentCurrentN(x)          EvPresentCurrentList_p->EvPresentCurrentN = (x)
#define l_flg_tst_LI0_EvPresentCurrentList_flag() all_frames[4].flag
#define l_flg_clr_LI0_EvPresentCurrentList_flag() all_frames[4].flag = false

//SeNomVoltages, 5
#define l_u8_rd_LI0_SeSelectedVersion_SeNomVoltages() SeNomVoltages_p->SeSelectedVersion
#define l_u16_rd_LI0_SeNomVoltageL1N()     SeNomVoltages_p->SeNomVoltageL1N
#define l_u16_wr_LI0_SeNomVoltageL1N(x)    SeNomVoltages_p->SeNomVoltageL1N = (x)
#define l_u16_rd_LI0_SeNomVoltageLL()      SeNomVoltages_p->SeNomVoltageLL
#define l_u16_wr_LI0_SeNomVoltageLL(x)     SeNomVoltages_p->SeNomVoltageLL = (x)
#define l_u8_rd_LI0_SeFrequency()          SeNomVoltages_p->SeFrequency
#define l_u8_wr_LI0_SeFrequency(x)         SeNomVoltages_p->SeFrequency = (x)
#define l_flg_tst_LI0_SeNomVoltages_flag() all_frames[5].flag
#define l_flg_clr_LI0_SeNomVoltages_flag() all_frames[5].flag = false

//SeMaxCurrentList, 6
#define l_u8_rd_LI0_SeSelectedVersion_SeMaxCurrentList() SeMaxCurrentList_p->SeSelectedVersion
#define l_u8_rd_LI0_SeMaxCurrentL1()          SeMaxCurrentList_p->SeMaxCurrentL1
#define l_u8_wr_LI0_SeMaxCurrentL1(x)         SeMaxCurrentList_p->SeMaxCurrentL1 = (x)
#define l_u8_rd_LI0_SeMaxCurrentL2()          SeMaxCurrentList_p->SeMaxCurrentL2
#define l_u8_wr_LI0_SeMaxCurrentL2(x)         SeMaxCurrentList_p->SeMaxCurrentL2 = (x)
#define l_u8_rd_LI0_SeMaxCurrentL3()          SeMaxCurrentList_p->SeMaxCurrentL3
#define l_u8_wr_LI0_SeMaxCurrentL3(x)         SeMaxCurrentList_p->SeMaxCurrentL3 = (x)
#define l_u8_rd_LI0_SeMaxCurrentN()           SeMaxCurrentList_p->SeMaxCurrentN
#define l_u8_wr_LI0_SeMaxCurrentN(x)          SeMaxCurrentList_p->SeMaxCurrentN = (x)
#define l_flg_tst_LI0_SeMaxCurrentList_flag() all_frames[6].flag
#define l_flg_clr_LI0_SeMaxCurrentList_flag() all_frames[6].flag = false

//EvMaxVoltageList, 7
#define l_u8_rd_LI0_EvSelectedVersion_EvMaxVoltageList() EvMaxVoltageList_p->EvSelectedVersion
#define l_u16_rd_LI0_EvMaxVoltageL1N()        EvMaxVoltageList_p->EvMaxVoltageL1N
#define l_u16_wr_LI0_EvMaxVoltageL1N(x)       EvMaxVoltageList_p->EvMaxVoltageL1N = (x)
#define l_u16_rd_LI0_EvMaxVoltageLL()         EvMaxVoltageList_p->EvMaxVoltageLL
#define l_u16_wr_LI0_EvMaxVoltageLL(x)        EvMaxVoltageList_p->EvMaxVoltageLL = (x)
#define l_u8_rd_LI0_EvFrequencies()           EvMaxVoltageList_p->EvFrequencies
#define l_u8_wr_LI0_EvFrequencies(x)          EvMaxVoltageList_p->EvFrequencies = (x)
#define l_flg_tst_LI0_EvMaxVoltageList_flag() all_frames[7].flag
#define l_flg_clr_LI0_EvMaxVoltageList_flag() all_frames[7].flag = false

//EvMinVoltageList, 8
#define l_u8_rd_LI0_EvSelectedVersion_EvMinVoltageList() EvMinVoltageList_p->EvSelectedVersion
#define l_u16_rd_LI0_EvMinVoltageL1N()        EvMinVoltageList_p->EvMinVoltageL1N
#define l_u16_wr_LI0_EvMinVoltageL1N(x)       EvMinVoltageList_p->EvMinVoltageL1N = (x)
#define l_u16_rd_LI0_EvMinVoltageLL()         EvMinVoltageList_p->EvMinVoltageLL
#define l_u16_wr_LI0_EvMinVoltageLL(x)        EvMinVoltageList_p->EvMinVoltageLL = (x)
#define l_flg_tst_LI0_EvMinVoltageList_flag() all_frames[8].flag
#define l_flg_clr_LI0_EvMinVoltageList_flag() all_frames[8].flag = false

//EvMaxMinCurrents, 9
#define l_u8_rd_LI0_EvSelectedVersion_EvMaxMinCurrents() EvMaxMinCurrents_p->EvSelectedVersion
#define l_u8_rd_LI0_EvMaxCurrentL1()          EvMaxMinCurrents_p->EvMaxCurrentL1
#define l_u8_wr_LI0_EvMaxCurrentL1(x)         EvMaxMinCurrents_p->EvMaxCurrentL1 = (x)
#define l_u8_rd_LI0_EvMaxCurrentL2()          EvMaxMinCurrents_p->EvMaxCurrentL2
#define l_u8_wr_LI0_EvMaxCurrentL2(x)         EvMaxMinCurrents_p->EvMaxCurrentL2 = (x)
#define l_u8_rd_LI0_EvMaxCurrentL3()          EvMaxMinCurrents_p->EvMaxCurrentL3
#define l_u8_wr_LI0_EvMaxCurrentL3(x)         EvMaxMinCurrents_p->EvMaxCurrentL3 = (x)
#define l_u8_rd_LI0_EvMaxCurrentN()           EvMaxMinCurrents_p->EvMaxCurrentN
#define l_u8_wr_LI0_EvMaxCurrentN(x)          EvMaxMinCurrents_p->EvMaxCurrentN = (x)
#define l_u8_rd_LI0_EvMinCurrentL1()          EvMaxMinCurrents_p->EvMinCurrentL1
#define l_u8_wr_LI0_EvMinCurrentL1(x)         EvMaxMinCurrents_p->EvMinCurrentL1 = (x)
#define l_u8_rd_LI0_EvMinCurrentL2()          EvMaxMinCurrents_p->EvMinCurrentL2
#define l_u8_wr_LI0_EvMinCurrentL2(x)         EvMaxMinCurrents_p->EvMinCurrentL2 = (x)
#define l_u8_rd_LI0_EvMinCurrentL3()          EvMaxMinCurrents_p->EvMinCurrentL3
#define l_u8_wr_LI0_EvMinCurrentL3(x)         EvMaxMinCurrents_p->EvMinCurrentL3 = (x)
#define l_flg_tst_LI0_EvMaxMinCurrents_flag() all_frames[9].flag
#define l_flg_clr_LI0_EvMaxMinCurrents_flag() all_frames[9].flag = false

//SeInfoList, 11
#define l_u8_rd_LI0_SeSelectedVersion_SeInfoList() SeInfoList_p->SeSelectedVersion
#define l_u8_rd_LI0_SeInfoPageNumber()      SeInfoList_p->SeInfoPageNumber
#define l_u8_wr_LI0_SeInfoPageNumber(x)     SeInfoList_p->SeInfoPageNumber = (x)
#define l_u8_rd_LI0_SeInfoEntry1()          SeInfoList_p->SeInfoEntry1
#define l_u8_wr_LI0_SeInfoEntry1(x)         SeInfoList_p->SeInfoEntry1 = (x)
#define l_u8_rd_LI0_SeInfoEntry2()          SeInfoList_p->SeInfoEntry2
#define l_u8_wr_LI0_SeInfoEntry2(x)         SeInfoList_p->SeInfoEntry2 = (x)
#define l_u8_rd_LI0_SeInfoEntry3()          SeInfoList_p->SeInfoEntry3
#define l_u8_wr_LI0_SeInfoEntry3(x)         SeInfoList_p->SeInfoEntry3 = (x)
#define l_u8_rd_LI0_SeInfoEntry4()          SeInfoList_p->SeInfoEntry4
#define l_u8_wr_LI0_SeInfoEntry4(x)         SeInfoList_p->SeInfoEntry4 = (x)
#define l_u8_rd_LI0_SeInfoEntry5()          SeInfoList_p->SeInfoEntry5
#define l_u8_wr_LI0_SeInfoEntry5(x)         SeInfoList_p->SeInfoEntry5 = (x)
#define l_u8_rd_LI0_SeInfoEntry6()          SeInfoList_p->SeInfoEntry6
#define l_u8_wr_LI0_SeInfoEntry6(x)         SeInfoList_p->SeInfoEntry6 = (x)
#define l_flg_tst_LI0_SeInfoList_flag()     all_frames[11].flag
#define l_flg_clr_LI0_SeInfoList_flag()     all_frames[11].flag = false

//EvInfoList, 12
#define l_u8_rd_LI0_EvSelectedVersion_EvInfoList() EvInfoList_p->EvSelectedVersion
#define l_u8_rd_LI0_EvInfoPageNumber()     EvInfoList_p->EvInfoPageNumber
#define l_u8_wr_LI0_EvInfoPageNumber(x)    EvInfoList_p->EvInfoPageNumber = (x)
#define l_u8_rd_LI0_EvInfoEntry1()         EvInfoList_p->EvInfoEntry1
#define l_u8_wr_LI0_EvInfoEntry1(x)        EvInfoList_p->EvInfoEntry1 = (x)
#define l_u8_rd_LI0_EvInfoEntry2()         EvInfoList_p->EvInfoEntry2
#define l_u8_wr_LI0_EvInfoEntry2(x)        EvInfoList_p->EvInfoEntry2 = (x)
#define l_u8_rd_LI0_EvInfoEntry3()         EvInfoList_p->EvInfoEntry3
#define l_u8_wr_LI0_EvInfoEntry3(x)        EvInfoList_p->EvInfoEntry3 = (x)
#define l_u8_rd_LI0_EvInfoEntry4()         EvInfoList_p->EvInfoEntry4
#define l_u8_wr_LI0_EvInfoEntry4(x)        EvInfoList_p->EvInfoEntry4 = (x)
#define l_u8_rd_LI0_EvInfoEntry5()         EvInfoList_p->EvInfoEntry5
#define l_u8_wr_LI0_EvInfoEntry5(x)        EvInfoList_p->EvInfoEntry5 = (x)
#define l_u8_rd_LI0_EvInfoEntry6()         EvInfoList_p->EvInfoEntry6
#define l_u8_wr_LI0_EvInfoEntry6(x)        EvInfoList_p->EvInfoEntry6 = (x)
#define l_flg_tst_LI0_EvInfoList_flag()    all_frames[12].flag
#define l_flg_clr_LI0_EvInfoList_flag()    all_frames[12].flag = false

//StErrorList, 13
#define l_u8_rd_LI0_SeSelectedVersion_StErrorList() StErrorList_p->StSelectedVersion
#define l_u8_rd_LI0_StErrorPageNumber()    StErrorList_p->StErrorPageNumber
#define l_u8_wr_LI0_StErrorPageNumber(x)   StErrorList_p->StErrorPageNumber = (x)
#define l_u8_rd_LI0_StErrorEntry1()        StErrorList_p->StErrorEntry1
#define l_u8_wr_LI0_StErrorEntry1(x)       StErrorList_p->StErrorEntry1 = (x)
#define l_u8_rd_LI0_StErrorEntry2()        StErrorList_p->StErrorEntry2
#define l_u8_wr_LI0_StErrorEntry2(x)       StErrorList_p->StErrorEntry2 = (x)
#define l_u8_rd_LI0_StErrorEntry3()        StErrorList_p->StErrorEntry3
#define l_u8_wr_LI0_StErrorEntry3(x)       StErrorList_p->StErrorEntry3 = (x)
#define l_u8_rd_LI0_StErrorEntry4()        StErrorList_p->StErrorEntry4
#define l_u8_wr_LI0_StErrorEntry4(x)       StErrorList_p->StErrorEntry4 = (x)
#define l_u8_rd_LI0_StErrorEntry5()        StErrorList_p->StErrorEntry5
#define l_u8_wr_LI0_StErrorEntry5(x)       StErrorList_p->StErrorEntry5 = (x)
#define l_u8_rd_LI0_StErrorEntry6()        StErrorList_p->StErrorEntry6
#define l_u8_wr_LI0_StErrorEntry6(x)       StErrorList_p->StErrorEntry6 = (x)
#define l_flg_clr_LI0_SeErrorList_flag()   all_frames[13].flag = false
#define l_flg_tst_LI0_SeErrorList_flag()   all_frames[13].flag

//EvErrorList, 14
#define l_u8_rd_LI0_EvSelectedVersion_EvErrorList() EvErrorList_p->EvSelectedVersion
#define l_u8_rd_LI0_EvErrorPageNumber()     EvErrorList_p->EvErrorPageNumber
#define l_u8_wr_LI0_EvErrorPageNumber(x)    EvErrorList_p->EvErrorPageNumber = (x)
#define l_u8_rd_LI0_EvErrorEntry1()         EvErrorList_p->EvErrorEntry1
#define l_u8_wr_LI0_EvErrorEntry1(x)        EvErrorList_p->EvErrorEntry1 = (x)
#define l_u8_rd_LI0_EvErrorEntry2()         EvErrorList_p->EvErrorEntry2
#define l_u8_wr_LI0_EvErrorEntry2(x)        EvErrorList_p->EvErrorEntry2 = (x)
#define l_u8_rd_LI0_EvErrorEntry3()         EvErrorList_p->EvErrorEntry3
#define l_u8_wr_LI0_EvErrorEntry3(x)        EvErrorList_p->EvErrorEntry3 = (x)
#define l_u8_rd_LI0_EvErrorEntry4()         EvErrorList_p->EvErrorEntry4
#define l_u8_wr_LI0_EvErrorEntry4(x)        EvErrorList_p->EvErrorEntry4 = (x)
#define l_u8_rd_LI0_EvErrorEntry5()         EvErrorList_p->EvErrorEntry5
#define l_u8_wr_LI0_EvErrorEntry5(x)        EvErrorList_p->EvErrorEntry5 = (x)
#define l_u8_rd_LI0_EvErrorEntry6()         EvErrorList_p->EvErrorEntry6
#define l_u8_wr_LI0_EvErrorEntry6(x)        EvErrorList_p->EvErrorEntry6 = (x)
#define l_flg_tst_LI0_EvErrorList_flag()    all_frames[14].flag
#define l_flg_clr_LI0_EvErrorList_flag()    all_frames[14].flag = false

//SeID, 15
#define l_u8_rd_LI0_SeIDPageNumber()       SeID_p->SeIDPageNumber
#define l_u8_wr_LI0_SeIDPageNumber(x)      SeID_p->SeIDPageNumber = (x)
#define l_u8_rd_LI0_SeIDByteA()            SeID_p->SeIDByteA
#define l_u8_wr_LI0_SeIDByteA(x)           SeID_p->SeIDByteA = (x)
#define l_u8_rd_LI0_SeIDByteB()            SeID_p->SeIDByteB
#define l_u8_wr_LI0_SeIDByteB(x)           SeID_p->SeIDByteB = (x)
#define l_u8_rd_LI0_SeIDByteC()            SeID_p->SeIDByteC
#define l_u8_wr_LI0_SeIDByteC(x)           SeID_p->SeIDByteC = (x)
#define l_u8_rd_LI0_SeIDByteD()            SeID_p->SeIDByteD
#define l_u8_wr_LI0_SeIDByteD(x)           SeID_p->SeIDByteD = (x)
#define l_u8_rd_LI0_SeIDByteE()            SeID_p->SeIDByteE
#define l_u8_wr_LI0_SeIDByteE(x)           SeID_p->SeIDByteE = (x)
#define l_u8_rd_LI0_SeIDByteF()            SeID_p->SeIDByteF
#define l_u8_wr_LI0_SeIDByteF(x)           SeID_p->SeIDByteF = (x)
#define l_u8_rd_LI0_SeIDByteG()            SeID_p->SeIDByteG
#define l_u8_wr_LI0_SeIDByteG(x)           SeID_p->SeIDByteG = (x)
#define l_flg_clr_LI0_SeID_flag()          all_frames[15].flag = false
#define l_flg_tst_LI0_SeID_flag()          all_frames[15].flag

//EvIDt, 16
#define l_u8_rd_LI0_EvIDPageNumber()        EvID_p->EvIDPageNumber
#define l_u8_wr_LI0_EvIDPageNumber(x)       EvID_p->EvIDPageNumber = (x)
#define l_u8_rd_LI0_EvIDByteA()             EvID_p->EvIDByteA
#define l_u8_wr_LI0_EvIDByteA(x)            EvID_p->EvIDByteA = (x)
#define l_u8_rd_LI0_EvIDByteB()             EvID_p->EvIDByteB
#define l_u8_wr_LI0_EvIDByteB(x)            EvID_p->EvIDByteB = (x)
#define l_u8_rd_LI0_EvIDByteC()             EvID_p->EvIDByteC
#define l_u8_wr_LI0_EvIDByteC(x)            EvID_p->EvIDByteC = (x)
#define l_u8_rd_LI0_EvIDByteD()             EvID_p->EvIDByteD
#define l_u8_wr_LI0_EvIDByteD(x)            EvID_p->EvIDByteD = (x)
#define l_u8_rd_LI0_EvIDByteE()             EvID_p->EvIDByteE
#define l_u8_wr_LI0_EvIDByteE(x)            EvID_p->EvIDByteE = (x)
#define l_u8_rd_LI0_EvIDByteF()             EvID_p->EvIDByteF
#define l_u8_wr_LI0_EvIDByteF(x)            EvID_p->EvIDByteF = (x)
#define l_u8_rd_LI0_EvIDByteG()             EvID_p->EvIDByteG
#define l_u8_wr_LI0_EvIDByteG(x)            EvID_p->EvIDByteG = (x)
#define l_flg_tst_LI0_EvID_flag()           all_frames[16].flag
#define l_flg_clr_LI0_EvID_flag()           all_frames[16].flag = false

#define l_sch_set(it,sch,entry) *schedule_picker_p = (sch)

#define l_sys_init()    (void)0     //probably make these actually do something
#define l_ifc_init(it)  (void)0

//FIXME
#define schedule_if_unscheduled(a,b,c) (void)0
#define unschedule(a)                  (void)0

#define TRUE  true
#define FALSE false

#define EMPTY
#define LV(t,rw,it,v,a) t##_##rw##_##it##_##v(a)
#define LF(tc,it,v) l_flg_##tc##_##it##_##v##_flag()
#define LS(it,sch,a) l_sch_set(it,it##_##sch,a)
#define LW(it,t,v,a) LV(t,wr,LI0,v,a)
#define LR(it,t,v) LV(t,rd,LI0,v,EMPTY)
#define LFT(it,v) LF(tst,LI0,v)
#define LFC(it,v) LF(clr,LI0,v)
#define LSS(it,sch,a) LS(LI0,sch,a)


/***************************************************************************/
typedef enum {StartNull, RunNull, StartVer, RunVer, StartInit, RunInit, StartOp, RunOp,
      StartErr, RunErr, StartSleep, RunSleep} SCHEDULE_PICKER;

typedef enum {continueOnLIN, startLIN, stopLIN} SCHEDULE_PICKER_EVENTS;

enum {FiftyHz = 1, SixtyHz, FiftyOrSixtyHz};

#define  RTI_TICKS_PER_LINCP_LOOP  20             //  How often the LIN-CP state machine is serviced

//9.3 -- Table 16
#define  T_SESTART     RTI_TICKS_PER_SECOND/2     //  9.4.1.3                       SE max time to start LIN Schedule
#define  T_EVSTART     1*RTI_TICKS_PER_SECOND     //  9.4.1.4                       EV max time to start responding
#define  T_PWM         1*RTI_TICKS_PER_SECOND     //  9.5.4.1                       SE max time to establish LIN-CP
#define  T_EVCLOSE     3*RTI_TICKS_PER_SECOND     //  9.7.2.3                       EV max time to close S2
#define  T_SECLOSE     3*RTI_TICKS_PER_SECOND     //  9.7.2.4                       SE max time to close the Contactor
#define  T_SEADAPT     10*RTI_TICKS_PER_SECOND    //  9.7.3.2                       SE max time to adapt SeAvailableCurrents to external limit (for example, energy management system)
#define  T_EVADAPT     5*RTI_TICKS_PER_SECOND     //  9.7.3.5                       EV max time to adapt load current to SeAvailableCurrents
#define  T_SEOPEN      3*RTI_TICKS_PER_SECOND     //  9.7.4.2   10.7.2.2            SE max time to open the Contactor
#define  T_RAMPDOWN    6*RTI_TICKS_PER_SECOND     //  9.7.5.1   10.2.2.1            SE min time to wait before interrupting supply
#define  T_EVOPEN      3*RTI_TICKS_PER_SECOND     //  9.7.5.2   9.8.2.1   10.7.1.1  EV max time to open S2
#define  T_UNLOCK      3*RTI_TICKS_PER_SECOND     //  9.8.1.1   9.8.2.3             EV max time to unlock Inlet
#define  T_VER         5*RTI_TICKS_PER_SECOND     //  10.3.1.1  10.3.2.1            SE and EV timeout before Version selection fails
#define  T_INIT        5*RTI_TICKS_PER_SECOND     //  10.4.1.1  10.4.2.1            SE and EV timeout before initialization fails
#define  T_NOLIN       2*RTI_TICKS_PER_SECOND     //  10.7.1.1  10.7.1.2            EV min time to wait before opening S2, no LIN Headers
                                                  //  10.7.2.1  10.7.2.2            SE min time to wait before opening the Contactor, no LIN Responses
#define  T_SE_12       RTI_TICKS_PER_SECOND/10    //  10.8.4.1                      SE max time to open the Contactor, SE detects CP Level 12

#define  L_SESTART     T_SESTART /RTI_TICKS_PER_LINCP_LOOP
#define  L_EVSTART     T_EVSTART /RTI_TICKS_PER_LINCP_LOOP
#define  L_PWM         T_PWM     /RTI_TICKS_PER_LINCP_LOOP
#define  L_EVCLOSE     T_EVCLOSE /RTI_TICKS_PER_LINCP_LOOP
#define  L_SECLOSE     T_SECLOSE /RTI_TICKS_PER_LINCP_LOOP
#define  L_SEADAPT     T_SEADAPT /RTI_TICKS_PER_LINCP_LOOP
#define  L_EVADAPT     T_EVADAPT /RTI_TICKS_PER_LINCP_LOOP
#define  L_SEOPEN      T_SEOPEN  /RTI_TICKS_PER_LINCP_LOOP
#define  L_RAMPDOWN    T_RAMPDOWN/RTI_TICKS_PER_LINCP_LOOP
#define  L_EVOPEN      T_EVOPEN  /RTI_TICKS_PER_LINCP_LOOP
#define  L_UNLOCK      T_UNLOCK  /RTI_TICKS_PER_LINCP_LOOP
#define  L_VER         T_VER     /RTI_TICKS_PER_LINCP_LOOP
#define  L_INIT        T_INIT    /RTI_TICKS_PER_LINCP_LOOP
#define  L_NOLIN       T_NOLIN   /RTI_TICKS_PER_LINCP_LOOP
#define  L_SE_12       T_SE_12   /RTI_TICKS_PER_LINCP_LOOP

void InitializeLINCP(void);
void DetermineLINCPState(uint8_t ch, SCHEDULE_PICKER_EVENTS SchedulePickerMessage);

typedef struct {
  uint8_t pwm;
  uint8_t C1_L1;
  uint8_t C2_L2;
  uint8_t C3_L3;
  uint8_t C4_N;
} three_phase_currents_t;

//config.h
#define PWM_CHARGE_ENABLE

#define INITIALIZATION_DELAY 10000
#define RTI_TICKS_PER_SECOND 926ul  //1000/1.08ms  This still runs long for some reason  FIXME?
#define LOCK_CHECK_DELAY     250
#define CONNECTION_CLEAR_TIME 2*60*RTI_TICKS_PER_SECOND   //Time between connections to assume a new connection, not a restart (EV only)

/*<Ratings*/
//Voltages are *10, Currents are *1
#define MAX_RATED_CURRENT 80

//Supported Protocol Versions.  2 is always supported, 0 is controlled by PWM_CHARGE_ENABLE
//Order controls preference for EV, 2 is always last
#define PVER_PWM     0
#define PVER_BASE    2
#define PVER_SLASH_1 3
#define PVER_SLASH_2 252
#define PVER_TEST_A  4
#define PVER_TEST_B  5
#define PVER_TEST_C  6
#define PVER_TEST_D  7
#define PVER_TEST_E  8
#define PVER_TEST_F  9
#define PVER_TEST_G  10
#define PVER_TEST_H  11

//#define SUPPORTED_PVERSIONS  PVER_SLASH_2, PVER_TEST_H, PVER_TEST_G, PVER_TEST_E, PVER_TEST_F, PVER_TEST_A, PVER_TEST_B, PVER_TEST_C, PVER_TEST_D, PVER_SLASH_1,
#define SUPPORTED_PVERSIONS  PVER_SLASH_1,
//#define SUPPORTED_PVERSIONS

//EVSE
#define EVSE_INCREASE_SET_C_WAIT_PERIOD 100
#define EVSE_DECREASE_SET_C_WAIT_PERIOD 10
#define EVSE_MIN_C 6
#define EVSE_MAX_C 30
#define EVSE_DEFAULT_C 30
#define EVSE_MIN_D 6
#define EVSE_MAX_D 30
#define EVSE_DEFAULT_D 30
#define OVER_CURRENT_DEFAULT 112
#define OVER_CURRENT_MIN 110
#define OVER_CURRENT_MAX 125
#define EVSE_MAIN_LOOP     1000

//SE Settings
#define SE_FREQUENCY SixtyHz
#define SE_NOMINAL_VOLTAGE_LL 2080
#define SE_NOMINAL_VOLTAGE_LN 1200
#define SE_MAX_CURRENT_L 16
#define SE_MAX_CURRENT_N 16

//EV settings
#define EV_FREQUENCIES FiftyOrSixtyHz
#define EV_MAX_VOLTAGE_LL 480*11    //+10%
#define EV_MAX_VOLTAGE_LN 277*11
#define EV_MIN_VOLTAGE_LL 208*9     //-10%
#define EV_MIN_VOLTAGE_LN 120*9
#define EV_MAX_CURRENT_L 32
#define EV_MAX_CURRENT_N 32
#define EV_MIN_CURRENT_L 6 
/*Rating>*/


#endif
