#ifndef LINCP_EMU_H
#define LINCP_EMU_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define NUMBER_OF_FRAMES 17 //64 for complete LIN

#define GENERATE_TEXT_LOG
#define GENERATE_ASC_LOG

/****<DO NOT MODIFY******/
#ifndef EV_CONFIG
  #define SE_CONFIG
#endif
/****DO NOT MODIFY>******/

#define FRAME_SIZE(x) #x, sizeof(x)
#define FRAME_POINTER(frame) frame##_t * frame##_p
#define FRAME_ASSIGN(frame, number) frame##_p = (frame##_t *) all_frames[number].data
#define ASSIGN_ALL_FRAMES()                 \
  FRAME_ASSIGN(SeVersionList, 0);           \
  FRAME_ASSIGN(EvVersionList, 1);           \
  FRAME_ASSIGN(SeStatus, 2);                \
  FRAME_ASSIGN(EvStatus, 3);                \
  FRAME_ASSIGN(EvPresentCurrents, 4);       \
  FRAME_ASSIGN(SeNomVoltages, 5);           \
  FRAME_ASSIGN(SeMaxCurrents, 6);           \
  FRAME_ASSIGN(EvMaxVoltages, 7);           \
  FRAME_ASSIGN(EvMinVoltages, 8);           \
  FRAME_ASSIGN(EvMaxMinCurrents, 9);        \
  FRAME_ASSIGN(SeInfoList, 11);             \
  FRAME_ASSIGN(EvInfoList, 12);             \
  FRAME_ASSIGN(SeID, 15);                   \
  FRAME_ASSIGN(EvID, 16);

#define CREATE_FRAME_POINTERS()             \
  FRAME_POINTER(SeVersionList);             \
  FRAME_POINTER(EvVersionList);             \
  FRAME_POINTER(SeStatus);                  \
  FRAME_POINTER(EvStatus);                  \
  FRAME_POINTER(EvPresentCurrents);         \
  FRAME_POINTER(SeNomVoltages);             \
  FRAME_POINTER(SeMaxCurrents);             \
  FRAME_POINTER(EvMaxVoltages);             \
  FRAME_POINTER(EvMinVoltages);             \
  FRAME_POINTER(EvMaxMinCurrents);          \
  FRAME_POINTER(SeInfoList);                \
  FRAME_POINTER(EvInfoList);                \
  FRAME_POINTER(SeID);                      \
  FRAME_POINTER(EvID);

typedef struct {
  bool flag;
  uint8_t data[8];
} generic_frame_t;

//Frame: 0, SeVersionList
typedef struct {
  uint8_t SeSelectedVersion;
  unsigned int : 1;
  unsigned int SeStatusOp : 2;
  unsigned int SeStatusInit : 2;
  unsigned int SeStatusVer : 2;
  unsigned int : 1;
  uint8_t SeVersionPageNumber;
  uint8_t SeSupportedVersion1;
  uint8_t SeSupportedVersion2;
  uint8_t SeSupportedVersion3;
  uint8_t SeSupportedVersion4;
  uint8_t SeSupportedVersion5;
} SeVersionList_t;

//Frame: 1, EvVersionList
typedef struct {
  uint8_t EvSelectedVersion;
  unsigned int EvAwake: 1;
  unsigned int EvStatusOp : 2;
  unsigned int EvStatusInit : 2;
  unsigned int EvStatusVer : 2;
  unsigned int EvResponseError: 1;
  uint8_t EvVersionPageNumber;
  uint8_t EvSupportedVersion1;
  uint8_t EvSupportedVersion2;
  uint8_t EvSupportedVersion3;
  uint8_t EvSupportedVersion4;
  uint8_t EvSupportedVersion5;
} EvVersionList_t;

//Frame: 2, SeStatus
typedef struct {
  uint8_t SeSelectedVersion;
  unsigned int : 1;
  unsigned int SeStatusOp : 2;
  unsigned int SeStatusInit : 2;
  unsigned int SeStatusVer : 2;
  unsigned int : 1;
  uint8_t SeAvailableCurrentL1;
  uint8_t SeAvailableCurrentL2;
  uint8_t SeAvailableCurrentL3;
  uint8_t SeAvailableCurrentN;
  uint16_t reserved;
} SeStatus_t;

//Frame: 3, EvStatus
typedef struct {
  uint8_t EvSelectedVersion;
  unsigned int EvAwake: 1;
  unsigned int EvStatusOp : 2;
  unsigned int EvStatusInit : 2;
  unsigned int EvStatusVer : 2;
  unsigned int EvResponseError: 1;
  uint8_t EvRequestedCurrentL1;
  uint8_t EvRequestedCurrentL2;
  uint8_t EvRequestedCurrentL3;
  uint8_t EvRequestedCurrentN;
  uint16_t reserved;
} EvStatus_t;

//Frame: 4, EvPresentCurrents
typedef struct {
  uint8_t EvSelectedVersion;
  uint8_t EvPresentCurrentL1;
  uint8_t EvPresentCurrentL2;
  uint8_t EvPresentCurrentL3;
  uint8_t EvPresentCurrentN;
  uint8_t reserved1;
  uint16_t reserved2;
} EvPresentCurrents_t;

//Frame: 5, SeNomVoltages
typedef struct {
  uint8_t SeSelectedVersion;
  uint16_t SeNomVoltageL1N;
  uint16_t SeNomVoltageLL;
  uint8_t SeFrequency;
  uint16_t reserved;
} __attribute__((__packed__)) SeNomVoltages_t;

//Frame: 6, SeMaxCurrents
typedef struct {
  uint8_t SeSelectedVersion;
  uint8_t SeMaxCurrentL1;
  uint8_t SeMaxCurrentL2;
  uint8_t SeMaxCurrentL3;
  uint8_t SeMaxCurrentN;
  uint8_t SeConnectionType;
  uint16_t reserved;
} SeMaxCurrents_t;

//Frame: 7, EvMaxVoltages
typedef struct {
  uint8_t EvSelectedVersion;
  uint16_t EvMaxVoltageL1N;
  uint16_t EvMaxVoltageLL;
  uint8_t EvFrequencies;
  uint16_t reserved;
} __attribute__((__packed__)) EvMaxVoltages_t;

//Frame: 8, EvMinVoltages
typedef struct {
  uint8_t EvSelectedVersion;
  uint16_t EvMinVoltageL1N;
  uint16_t EvMinVoltageLL;
  uint8_t EvConnectionType;
  uint16_t reserved;
} __attribute__((__packed__)) EvMinVoltages_t;

//Frame: 9, EvMaxMinCurrents
typedef struct {
  uint8_t EvSelectedVersion;
  uint8_t EvMaxCurrentL1;
  uint8_t EvMaxCurrentL2;
  uint8_t EvMaxCurrentL3;
  uint8_t EvMaxCurrentN;
  uint8_t EvMinCurrentL1;
  uint8_t EvMinCurrentL2;
  uint8_t EvMinCurrentL3;
} EvMaxMinCurrents_t;

//Frame: 10, CaProperties

//Frame: 11, SeInfoList
typedef struct {
  uint8_t SeSelectedVersion;
  uint8_t SeInfoPageNumber;
  uint8_t SeInfoEntry1;
  uint8_t SeInfoEntry2;
  uint8_t SeInfoEntry3;
  uint8_t SeInfoEntry4;
  uint8_t SeInfoEntry5;
  uint8_t SeInfoEntry6;
} SeInfoList_t;

//Frame: 12, EvInfoList
typedef struct {
  uint8_t EvSelectedVersion;
  uint8_t EvInfoPageNumber;
  uint8_t EvInfoEntry1;
  uint8_t EvInfoEntry2;
  uint8_t EvInfoEntry3;
  uint8_t EvInfoEntry4;
  uint8_t EvInfoEntry5;
  uint8_t EvInfoEntry6;
} EvInfoList_t;

//Frame: 13, StErrorList
typedef struct {
  uint8_t StSelectedVersion;
  uint8_t StErrorPageNumber;
  uint8_t StErrorEntry1;
  uint8_t StErrorEntry2;
  uint8_t StErrorEntry3;
  uint8_t StErrorEntry4;
  uint8_t StErrorEntry5;
  uint8_t StErrorEntry6;
} StErrorList_t;

//Frame: 14, EvErrorList
typedef struct {
  uint8_t EvSelectedVersion;
  uint8_t EvErrorPageNumber;
  uint8_t EvErrorEntry1;
  uint8_t EvErrorEntry2;
  uint8_t EvErrorEntry3;
  uint8_t EvErrorEntry4;
  uint8_t EvErrorEntry5;
  uint8_t EvErrorEntry6;
} EvErrorList_t;

//Frame: 15, SeID
typedef struct {
  uint8_t SeIDPageNumber;
  uint8_t SeIDByteA;
  uint8_t SeIDByteB;
  uint8_t SeIDByteC;
  uint8_t SeIDByteD;
  uint8_t SeIDByteE;
  uint8_t SeIDByteF;
  uint8_t SeIDByteG;
} SeID_t;

//Frame: 16, EvID
typedef struct {
  uint8_t EvIDPageNumber;
  uint8_t EvIDByteA;
  uint8_t EvIDByteB;
  uint8_t EvIDByteC;
  uint8_t EvIDByteD;
  uint8_t EvIDByteE;
  uint8_t EvIDByteF;
  uint8_t EvIDByteG;
} EvID_t;


extern FRAME_POINTER(SeVersionList);
extern FRAME_POINTER(EvVersionList);
extern FRAME_POINTER(SeStatus);
extern FRAME_POINTER(EvStatus);
extern FRAME_POINTER(EvPresentCurrents);
extern FRAME_POINTER(SeNomVoltages);
extern FRAME_POINTER(SeMaxCurrents);
extern FRAME_POINTER(EvMaxVoltages);
extern FRAME_POINTER(EvMinVoltages);
extern FRAME_POINTER(EvMaxMinCurrents);
extern FRAME_POINTER(SeInfoList);
extern FRAME_POINTER(EvInfoList);
extern FRAME_POINTER(SeID);
extern FRAME_POINTER(EvID);

typedef enum { DoNothing = 0, CopySeToEv, CopyEvToSe } schedule_action_t;
typedef enum { LI0_LIN_NULL_SCHEDULE = 0, LI0_GOTO_SLEEP_SCHEDULE, LI0_Ver, LI0_Init, LI0_Op, LI0_Op3 } schedule_picker_t;

void verify_type_sizes();
void print_generic_frame(uint8_t frame_number, generic_frame_t * frame);
void print_LIN_record(FILE * stream, double time_end, uint8_t frame_number, generic_frame_t * frame);
void print_specific_frame(FILE * stream, uint8_t frame_number, void * frame);
void print_all_frames(generic_frame_t * frames);
void init_all_frames(generic_frame_t * frames);

extern schedule_action_t schedule_ver[];
extern schedule_action_t schedule_init[];
extern schedule_action_t schedule_op_base[];
extern schedule_action_t schedule_op_slash1[];

extern generic_frame_t *all_frames;
extern schedule_picker_t *schedule_picker_p;

#define EV_FRAME_MEMORY_IDENTIFIER "/ev_memory_location_identifier"
#define SE_FRAME_MEMORY_IDENTIFIER "/se_memory_location_identifier"
#define SE_SCHEDULE_MEMORY_IDENTIFIER "/se_schedule_mem_loc_id"

#ifdef EV_CONFIG
#define FRAME_MEMORY_IDENTIFIER EV_FRAME_MEMORY_IDENTIFIER
#else
#define FRAME_MEMORY_IDENTIFIER SE_FRAME_MEMORY_IDENTIFIER
#endif

typedef enum {
  NO_CODE=0,
  ERROR_DETECTED,
  BSC_ESTABLISHED,
  BSC_FAILED,
  BSC_TIMEOUT,
  DELAY_FINISHED,
  HIDE,
  PWM_DISABLE_TILL_UNPLUG,
  PWM_DISABLE_TILL_CLEARED,
  PWM_ENABLE,
} DETERMINE_STATE_CODE;

void DetermineEvState(uint8_t ch, DETERMINE_STATE_CODE code);
void DetermineEvseState(uint8_t ch, DETERMINE_STATE_CODE code);

//https://talyian.github.io/ansicolors/
#define ANSI_COLOR_RED     "\x1b[38;5;197m"
#define ANSI_COLOR_GREEN   "\x1b[38;5;82m"
#define ANSI_COLOR_YELLOW  "\x1b[38;5;227m"
#define ANSI_COLOR_CYAN    "\x1b[38;5;123m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define CONSOLE_TEXT(command) do { printf(ANSI_COLOR_YELLOW); command; printf(ANSI_COLOR_RESET); } while(false)
#define COMMAND_TEXT(command) do { printf(ANSI_COLOR_GREEN); command; printf(ANSI_COLOR_RESET); } while(false)

#define PrintConsoleString(string,length) CONSOLE_TEXT(printf("%s", string) )
#define PrintConsoleChar(char) CONSOLE_TEXT( printf("%c", char))
#define PrintConsoleHex(number) CONSOLE_TEXT( if ((number) > 0xFF) { printf("%04X", number); } else { printf("%02X", number); } )
#define PrintConsoleHexFixedW(number) CONSOLE_TEXT( printf("%04X", number) )
#define PrintConsoleLongHex(number) CONSOLE_TEXT( printf("%08X", number) )
#define PrintTenthResDec(value) CONSOLE_TEXT( printf("%.1f", ((float) (value))/10) )

#define PrintCmdsString(string,length) COMMAND_TEXT(printf("%s", string) )
#define PrintCmdsHex(number) COMMAND_TEXT( if ((number) > 0xFF) { printf("%04X", number); } else { printf("%02X", number); } )

//utils.h
void mem_init(void *obj_p, uint8_t size, uint8_t value);
void mem_copy(void *source_p, void *dest_p, uint8_t size);

#endif