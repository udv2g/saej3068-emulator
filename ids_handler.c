#include "lin-cp.h"
#include "globals.h"
#include "ids_handler.h"
#include <time.h>

uint8_t ev_id_status[2] = {ID_INCOMPLETE, ID_INCOMPLETE};
uint8_t se_id_status[2] = {ID_INCOMPLETE, ID_INCOMPLETE};

///>Declarations

#define EVSTATUSINLETLATCH_RELEASED 0x00
#define EVSTATUSINLETLATCH_ENGAGED 0x01
#define EVSTATUSINLETLATCH_ERROR 0x02
#define EVSTATUSINLETLATCH_NOT_AVAILABLE 0x03
#define EVSTATUSINLETOVERRIDE_NOT_ACTIVE 0x00
#define EVSTATUSINLETOVERRIDE_ACTIVE 0x01
#define EVSTATUSINLETOVERRIDE_ERROR 0x02
#define EVSTATUSINLETOVERRIDE_NOT_AVAILABLE 0x03
#define EVSTATUSINLETLOCK_UNLOCKED 0x00
#define EVSTATUSINLETLOCK_LOCKED 0x01
#define EVSTATUSINLETLOCK_IN_TRANSITION 0x02
#define EVSTATUSINLETLOCK_NO_LOCK 0x05
#define EVSTATUSINLETLOCK_ERROR 0x06
#define EVSTATUSINLETLOCK_NOT_AVAILABLE 0x07
#define EVSTATUSCELLVOLTDIFF_WITHIN_ACCEPTABLE_LIMITS 0x00
#define EVSTATUSCELLVOLTDIFF_CAN_BE_CORRECTED 0x01
#define EVSTATUSCELLVOLTDIFF_MAINTENANCE_CAN_RESTORE_PERFORMANCE 0x02
#define EVSTATUSCELLVOLTDIFF_MAINTENANCE_CANNOT_RESTORE_PERFORMANCE 0x03
#define EVSTATUSCELLVOLTDIFF_PERFORMANCE_RESTORATION_UNKNOWN 0x04
#define EVSTATUSCELLVOLTDIFF_ERROR 0x0E
#define EVSTATUSCELLVOLTDIFF_NOT_AVAILABLE 0x0F
#define EVSTATUSCELLBAL_BALANCED 0x00
#define EVSTATUSCELLBAL_UNBALANCED 0x01
#define EVSTATUSCELLBAL_ERROR 0x02
#define EVSTATUSCELLBAL_NOT_AVAILABLE 0x03
#define EVACTIVECELLBAL_NOT_ACTIVE 0x00
#define EVACTIVECELLBAL_ACTIVE 0x01
#define EVACTIVECELLBAL_ERROR 0x02
#define EVACTIVECELLBAL_NOT_AVAILABLE 0x03

#define SEEVSTATUSOUTLETOVERRIDE_NOT_ACTIVE 0x00
#define SEEVSTATUSOUTLETOVERRIDE_ACTIVE 0x01
#define SEEVSTATUSOUTLETOVERRIDE_ERROR 0x02
#define SEEVSTATUSOUTLETOVERRIDE_NOT_AVAILABLE 0x03
#define SEEVSTATUSOUTLETLOCK_UNLOCKED 0x00
#define SEEVSTATUSOUTLETLOCK_LOCKED 0x01
#define SEEVSTATUSOUTLETLOCK_IN_TRANSITION 0x02
#define SEEVSTATUSOUTLETLOCK_NO_LOCK 0x05
#define SEEVSTATUSOUTLETLOCK_ERROR 0x06
#define SEEVSTATUSOUTLETLOCK_NOT_AVAILABLE 0x07
#define SERMTMGMTSTATUS_CONNECTED 0x00
#define SERMTMGMTSTATUS_CONNECTING 0x01
#define SERMTMGMTSTATUS_NOT_CONNECTED 0x02
#define SERMTMGMTSTATUS_FALLBACK 0x03
#define SERMTMGMTSTATUS_LOCAL 0x04
#define SERMTMGMTSTATUS_ERROR 0xFE
#define SERMTMGMTSTATUS_NONE_OR_STATUS_UNKNOWN 0xFF
#define SEEVTRIPSTATUS_NOT_PRESENT 0x00
#define SEEVTRIPSTATUS_FOLLOWING 0x01
#define SEEVTRIPSTATUS_CANNOT_BE_MET 0x02
#define SEEVTRIPSTATUS_INVALID 0xFE
#define SEEVTRIPSTATUS_NOT_SUPPORTED 0xFF
#define SESETRIPSTATUS_NOT_PRESENT 0x00
#define SESETRIPSTATUS_FOLLOWING 0x01
#define SESETRIPSTATUS_CANNOT_BE_MET 0x02
#define SESETRIPSTATUS_EV_PRECEDENCE 0x03
#define SESETRIPSTATUS_CANNOT_RETRIEVE 0xFE
#define SESETRIPSTATUS_NOT_SUPPORTED 0xFF

typedef struct  {
  uint8_t EvVIN[18];
  uint8_t EvEMAID[19];
  uint8_t EvEVCCID[71];
  uint8_t EvDriverID[22];
  uint8_t EvVehicleName[22];
  uint8_t EvFirmwareRevision[29];
  uint8_t EvManufacturer[43];
} ev_ids_strings_t;

typedef struct  {
  uint8_t SeEVSEID[41];
  uint8_t SeSECCID[71];
  uint8_t SeSerialNum[43];
  uint8_t SeFirmwareRevision[29];
  uint8_t SeManufacturer[43];
  uint8_t SePublicName[43];
} se_ids_strings_t;

typedef struct  {
  uint32_t EvOdometer;
  uint8_t EvStatusInletLatch;
  uint8_t EvStatusInletOverride;
  uint8_t EvStatusInletLock;
  uint8_t EvNumberJ2012Dtcs;
  uint8_t EvJ2012DtcStatus;
  uint8_t EvJ2012DtcCount;
  uint32_t EvHVESSDishargeCapacity;
  uint32_t EvHVESSChargeCapacity;
  uint32_t EvEnergyForDeparture;
  uint16_t EvTimeToDeparture;
  uint16_t EvHVESSRange;
  uint16_t EvFuelRange;
  uint16_t EvEVTimeToEnergyForDept;
  uint16_t EvDurMin;
  uint16_t EvChaDurMax;
  uint16_t EvDschDurMax;
  uint8_t EvTimeReqNum;
  uint16_t EvEVTimeToRange;
  uint16_t EvEVTimeToEnergy;
  uint16_t EvHVESSVoltage;
  uint16_t EvHVESSCurrent;
  uint8_t EvHVESSHealth;
  uint8_t EvHVESSUserSOC;
  uint16_t EvACActivePower;
  uint16_t EvACReactivePower;
  uint8_t EvACFrequency;
  uint16_t EvL1NVolts;
  uint16_t EvL2NVolts;
  uint16_t EvL3NVolts;
  uint16_t EvAmbientTemp;
  uint16_t EvCabinTemp;
  uint16_t EvHVESSCellTemp;
  uint16_t EvMaxHVESSTemp;
  uint16_t EvMinHVESSTemp;
  uint8_t EvHVESSElecTemp;
  uint16_t EvMaxHVESSCellVolt;
  uint16_t EvMinHVESSCellVolt;
  uint16_t EvNumHVESSCellBalancing;
  uint8_t EvStatusCellVoltDiff;
  uint8_t EvStatusCellBal;
  uint8_t EvActiveCellBal;
  uint16_t EvChargerTemp;
  uint16_t EvMaxChargerTemp;
  uint8_t EvInletTemp;
  uint8_t EvHVESSTemp;
} ev_data_vars_t;

typedef struct  {
  uint8_t EvJ2012Dtc[6];
} ev_data_strings_t;

typedef struct  {
  uint16_t SeAmbientTemp;
  uint8_t SeConnectorTemp;
  uint8_t SeOutletTemp;
  uint8_t SeEvStatusOutletOverride;
  uint8_t SeEvStatusOutletLock;
  uint8_t SeRmtMgmtStatus;
  uint8_t SeEvTripStatus;
  uint8_t SeSeTripStatus;
  uint8_t SeExptTripPerct;
  uint8_t SeTimeReqNum;
  uint16_t SeHVESSRangeCalc;
  uint32_t SeHVESSEnergyCalc;
} se_data_vars_t;

typedef struct  {
  ev_ids_strings_t  s;
} ev_ids_t;

typedef struct  {
  se_ids_strings_t  s;
} se_ids_t;

typedef struct  {
  ev_data_strings_t s;
  ev_data_vars_t    v;
} ev_data_t;

typedef struct  {
  se_data_vars_t    v;
} se_data_t;

#ifdef EV_CONFIG
  ev_ids_t ids_xmit_buff_1_A = {{
    {'V','I','N','g','o','e','s','H','e','r','e','2','3','4','5','6','7','\0'},
    {'E','M','A','I','D','g','o','e','s','H','e','r','e','4','5','6','7','8','\0'},
    {'E','V','C','C','I','D','g','o','e','s','H','e','r','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'D','r','i','v','e','r',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'V','e','h','i','c','l','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'F','i','r','m','w','a','r','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'M','a','n','u','f','a','c','t','u','r','e','r','G','o','e','s','H','e','r','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
  }};
  ev_ids_t ids_xmit_buff_2_A = {{
    {'V','I','N','g','o','e','s','H','e','r','e','2','3','4','5','6','7','\0'},
    {'E','M','A','I','D','g','o','e','s','H','e','r','e','4','5','6','7','8','\0'},
    {'E','V','C','C','I','D','g','o','e','s','H','e','r','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'D','r','i','v','e','r',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'V','e','h','i','c','l','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'F','i','r','m','w','a','r','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'M','a','n','u','f','a','c','t','u','r','e','r','G','o','e','s','H','e','r','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
  }};
  ev_ids_t ids_xmit_buff_1_B = {{
    {'V','I','N','g','o','e','s','H','e','r','e','2','3','4','5','6','7','\0'},
    {'E','M','A','I','D','g','o','e','s','H','e','r','e','4','5','6','7','8','\0'},
    {'E','V','C','C','I','D','g','o','e','s','H','e','r','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'D','r','i','v','e','r',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'V','e','h','i','c','l','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'F','i','r','m','w','a','r','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'M','a','n','u','f','a','c','t','u','r','e','r','G','o','e','s','H','e','r','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
  }};
  ev_ids_t ids_xmit_buff_2_B = {{
    {'V','I','N','g','o','e','s','H','e','r','e','2','3','4','5','6','7','\0'},
    {'E','M','A','I','D','g','o','e','s','H','e','r','e','4','5','6','7','8','\0'},
    {'E','V','C','C','I','D','g','o','e','s','H','e','r','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'D','r','i','v','e','r',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'V','e','h','i','c','l','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'F','i','r','m','w','a','r','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'M','a','n','u','f','a','c','t','u','r','e','r','G','o','e','s','H','e','r','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
  }};
  ev_data_t data_xmit_buff_1_A = {{
    {'A','S','C','I','I','\0'},
  }};
  ev_data_t data_xmit_buff_2_A = {{
    {'A','S','C','I','I','\0'},
  }};
  ev_data_t data_xmit_buff_1_B = {{
    {'A','S','C','I','I','\0'},
  }};
  ev_data_t data_xmit_buff_2_B = {{
    {'A','S','C','I','I','\0'},
  }};
  ev_ids_t *act_ids_xmit[2], *inact_ids_xmit[2];
  se_ids_t ids_rcv_buff_1_A, ids_rcv_buff_2_A, ids_rcv_buff_1_B, ids_rcv_buff_2_B, *act_ids_rcv[2], *inact_ids_rcv[2];
  ev_data_t *act_data_xmit[2], *inact_data_xmit[2];
  se_data_t data_rcv_buff_1_A, data_rcv_buff_2_A, data_rcv_buff_1_B, data_rcv_buff_2_B, *act_data_rcv[2], *inact_data_rcv[2];
#else
  se_ids_t ids_xmit_buff_1_A = {{
    {'E','V','S','E','I','D',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'S','E','C','C','I','D','g','o','e','s','H','e','r','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'A','1','2','3','4','5','6','7','B','8','9','0','1','2','3','4',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'F','i','r','m','w','a','r','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'M','a','n','u','f','a','c','t','u','r','e','r',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'M','y','A','w','e','s','o','m','e','C','h','a','r','g','i','n','g','S','t','a','t','i','o','n',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
  }};
  se_ids_t ids_xmit_buff_2_A = {{
    {'E','V','S','E','I','D',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'S','E','C','C','I','D','g','o','e','s','H','e','r','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'A','1','2','3','4','5','6','7','B','8','9','0','1','2','3','4',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'F','i','r','m','w','a','r','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'M','a','n','u','f','a','c','t','u','r','e','r',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'M','y','A','w','e','s','o','m','e','C','h','a','r','g','i','n','g','S','t','a','t','i','o','n',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
  }};
  se_ids_t ids_xmit_buff_1_B = {{
    {'E','V','S','E','I','D',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'S','E','C','C','I','D','g','o','e','s','H','e','r','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'A','1','2','3','4','5','6','7','B','8','9','0','1','2','3','4',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'F','i','r','m','w','a','r','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'M','a','n','u','f','a','c','t','u','r','e','r',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'M','y','A','w','e','s','o','m','e','C','h','a','r','g','i','n','g','S','t','a','t','i','o','n',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
  }};
  se_ids_t ids_xmit_buff_2_B = {{
    {'E','V','S','E','I','D',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'S','E','C','C','I','D','g','o','e','s','H','e','r','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'A','1','2','3','4','5','6','7','B','8','9','0','1','2','3','4',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'F','i','r','m','w','a','r','e',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'M','a','n','u','f','a','c','t','u','r','e','r',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
    {'M','y','A','w','e','s','o','m','e','C','h','a','r','g','i','n','g','S','t','a','t','i','o','n',0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'\0'},
  }};
  se_data_t data_xmit_buff_1_A = {{
  }};
  se_data_t data_xmit_buff_2_A = {{
  }};
  se_data_t data_xmit_buff_1_B = {{
  }};
  se_data_t data_xmit_buff_2_B = {{
  }};
  se_ids_t *act_ids_xmit[2], *inact_ids_xmit[2];
  ev_ids_t ids_rcv_buff_1_A, ids_rcv_buff_2_A, ids_rcv_buff_1_B, ids_rcv_buff_2_B, *act_ids_rcv[2], *inact_ids_rcv[2];
  se_data_t *act_data_xmit[2], *inact_data_xmit[2];
  ev_data_t data_rcv_buff_1_A, data_rcv_buff_2_A, data_rcv_buff_1_B, data_rcv_buff_2_B, *act_data_rcv[2], *inact_data_rcv[2];
#endif

///<Declarations

///>pgs_to_send
#ifdef EV_CONFIG
#define NUM_ID_PGS2SND 33
uint8_t id_pages_to_send[NUM_ID_PGS2SND] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, };
#define NUM_DATA_PGS2SND 15
uint8_t data_pages_to_send[NUM_DATA_PGS2SND] = { 0, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, };
#else
#define NUM_ID_PGS2SND 39
uint8_t id_pages_to_send[NUM_ID_PGS2SND] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, };
#define NUM_DATA_PGS2SND 4
uint8_t data_pages_to_send[NUM_DATA_PGS2SND] = { 0, 97, 98, 99, };
#endif
///<pgs_to_send

static bool first_data[2] = {false, false};

void copy_buffer_data(uint8_t * source, uint8_t * destination, uint8_t size, bool string) {
  uint8_t i;

  for(i=0; i<size; i++) {
    destination[i] = source[i];
    if(string)  {
      if (source[i] == 0xFF)  {
        destination[i] = '\0';
      }
    }
  }
}

void invalidate_bytes_page(uint8_t * buffer, uint8_t mask)  {
  uint8_t i;

  if (mask != 0)  {
    for(i=6; i<=6; i--) { //>=0 not working because unsigned?
      if (mask & 0b00000001)  {
        buffer[i] = 0xFF;
      }
      mask = mask >> 1;
    }
  }
}

///>ids_init()

#ifdef EV_CONFIG
void clear_ids_xmit_buff(ev_ids_t* ids_xmit_buff)  {
}
void clear_ids_rcv_buff(se_ids_t* ids_rcv_buff)  {
  mem_init(&(ids_rcv_buff->s), (uint8_t)sizeof(se_ids_strings_t), 0x00);
}
void clear_data_xmit_buff(ev_data_t* data_xmit_buff)  {
  mem_init(&(data_xmit_buff->v), (uint8_t)sizeof(ev_data_vars_t), 0xFF);
  mem_init(&(data_xmit_buff->s), (uint8_t)sizeof(ev_data_strings_t), 0x00);
}
void clear_data_rcv_buff(se_data_t* data_rcv_buff)  {
  mem_init(&(data_rcv_buff->v), (uint8_t)sizeof(se_data_vars_t), 0xFF);
}
#else
void clear_ids_xmit_buff(se_ids_t* ids_xmit_buff)  {
}
void clear_ids_rcv_buff(ev_ids_t* ids_rcv_buff)  {
  mem_init(&(ids_rcv_buff->s), (uint8_t)sizeof(ev_ids_strings_t), 0x00);
}
void clear_data_xmit_buff(se_data_t* data_xmit_buff)  {
  mem_init(&(data_xmit_buff->v), (uint8_t)sizeof(se_data_vars_t), 0xFF);
}
void clear_data_rcv_buff(ev_data_t* data_rcv_buff)  {
  mem_init(&(data_rcv_buff->v), (uint8_t)sizeof(ev_data_vars_t), 0xFF);
  mem_init(&(data_rcv_buff->s), (uint8_t)sizeof(ev_data_strings_t), 0x00);
}
#endif


void ids_init(uint8_t ch) {
  se_id_status[ch] = ID_INCOMPLETE;
  ev_id_status[ch] = ID_INCOMPLETE;
  first_data[ch]= false;

  act_ids_xmit[ch] = ch ? &ids_xmit_buff_1_B : &ids_xmit_buff_1_A;
  inact_ids_xmit[ch] = ch ? &ids_xmit_buff_2_B : &ids_xmit_buff_2_A;
  act_ids_rcv[ch] = ch ? &ids_rcv_buff_1_B : &ids_rcv_buff_1_A;
  inact_ids_rcv[ch] = ch ? &ids_rcv_buff_2_B : &ids_rcv_buff_2_A;

  act_data_xmit[ch] = ch ? &data_xmit_buff_1_B : &data_xmit_buff_1_A;
  inact_data_xmit[ch] = ch ? &data_xmit_buff_2_B : &data_xmit_buff_2_A;
  act_data_rcv[ch] = ch ? &data_rcv_buff_1_B : &data_rcv_buff_1_A;
  inact_data_rcv[ch] = ch ? &data_rcv_buff_2_B : &data_rcv_buff_2_A;

  clear_ids_xmit_buff(act_ids_xmit[ch]);
  clear_ids_xmit_buff(inact_ids_xmit[ch]);
  clear_ids_rcv_buff(act_ids_rcv[ch]);
  clear_ids_rcv_buff(inact_ids_rcv[ch]);
  clear_data_xmit_buff(act_data_xmit[ch]);
  clear_data_xmit_buff(inact_data_xmit[ch]);
  clear_data_rcv_buff(act_data_rcv[ch]);
  clear_data_rcv_buff(inact_data_rcv[ch]);

  id_xmit(ch, 0); //preload the control page

  srand((unsigned) time(NULL));   //seed random number generator

#ifdef EV_CONFIG
  #define PREPEND (act_ids_xmit[ch]->v).
  #undef PREPEND
  #define PREPEND (act_data_xmit[ch]->v).
  PREPEND EvOdometer = (uint32_t)(rand() & 0xFFFFFFFF);
  printf("EvOdometer %.3lf km\n", ((double)PREPEND EvOdometer * 0.125000));
  PREPEND EvStatusInletLatch = (uint8_t)(rand() & 0b00000011);
  printf("EvStatusInletLatch 0x%02X Enum\n", (PREPEND EvStatusInletLatch));
  PREPEND EvStatusInletOverride = (uint8_t)(rand() & 0b00000011);
  printf("EvStatusInletOverride 0x%02X Enum\n", (PREPEND EvStatusInletOverride));
  PREPEND EvStatusInletLock = (uint8_t)(rand() & 0b00000111);
  printf("EvStatusInletLock 0x%02X Enum\n", (PREPEND EvStatusInletLock));
  PREPEND EvNumberJ2012Dtcs = (uint8_t)(rand() & 0xFF);
  printf("EvNumberJ2012Dtcs %d count\n", (PREPEND EvNumberJ2012Dtcs));
  PREPEND EvJ2012DtcStatus = (uint8_t)(rand() & 0b00000001);
  printf("EvJ2012DtcStatus %d bit\n", (PREPEND EvJ2012DtcStatus));
  PREPEND EvJ2012DtcCount = (uint8_t)(rand() & 0b01111111);
  printf("EvJ2012DtcCount %d count\n", (PREPEND EvJ2012DtcCount));
  PREPEND EvHVESSDishargeCapacity = (uint32_t)(rand() & 0xFFFFFF);
  printf("EvHVESSDishargeCapacity %.3lf kWh\n", ((double)PREPEND EvHVESSDishargeCapacity * 0.001000));
  PREPEND EvHVESSChargeCapacity = (uint32_t)(rand() & 0xFFFFFF);
  printf("EvHVESSChargeCapacity %.3lf kWh\n", ((double)PREPEND EvHVESSChargeCapacity * 0.001000));
  PREPEND EvEnergyForDeparture = (uint32_t)(rand() & 0xFFFFFF);
  printf("EvEnergyForDeparture %.3lf kWh\n", ((double)PREPEND EvEnergyForDeparture * 0.001000));
  PREPEND EvTimeToDeparture = (uint16_t)(rand() & 0xFFFF);
  printf("EvTimeToDeparture %d min\n", (PREPEND EvTimeToDeparture));
  PREPEND EvHVESSRange = (uint16_t)(rand() & 0xFFFF);
  printf("EvHVESSRange %d km\n", (PREPEND EvHVESSRange));
  PREPEND EvFuelRange = (uint16_t)(rand() & 0xFFFF);
  printf("EvFuelRange %d km\n", (PREPEND EvFuelRange));
  PREPEND EvEVTimeToEnergyForDept = (uint16_t)(rand() & 0xFFFF);
  printf("EvEVTimeToEnergyForDept %d min\n", (PREPEND EvEVTimeToEnergyForDept));
  PREPEND EvDurMin = (uint16_t)(rand() & 0xFFFF);
  printf("EvDurMin %d s\n", (PREPEND EvDurMin * 10));
  PREPEND EvChaDurMax = (uint16_t)(rand() & 0xFFFF);
  printf("EvChaDurMax %d s\n", (PREPEND EvChaDurMax * 10));
  PREPEND EvDschDurMax = (uint16_t)(rand() & 0xFFFF);
  printf("EvDschDurMax %d s\n", (PREPEND EvDschDurMax * 10));
  PREPEND EvTimeReqNum = (uint8_t)(rand() & 0xFF);
  printf("EvTimeReqNum 0x%02X \n", (PREPEND EvTimeReqNum));
  PREPEND EvEVTimeToRange = (uint16_t)(rand() & 0xFFFF);
  printf("EvEVTimeToRange %d min\n", (PREPEND EvEVTimeToRange));
  PREPEND EvEVTimeToEnergy = (uint16_t)(rand() & 0xFFFF);
  printf("EvEVTimeToEnergy %d min\n", (PREPEND EvEVTimeToEnergy));
  PREPEND EvHVESSVoltage = (uint16_t)(rand() & 0xFFFF);
  printf("EvHVESSVoltage %.3lf \n", ((double)PREPEND EvHVESSVoltage * 0.050000));
  PREPEND EvHVESSCurrent = (uint16_t)(rand() & 0xFFFF);
  printf("EvHVESSCurrent %.3lf \n", ((double)PREPEND EvHVESSCurrent * 0.050000) - 1600);
  PREPEND EvHVESSHealth = (uint8_t)(rand() & 0xFF);
  printf("EvHVESSHealth %.3lf %%\n", ((double)PREPEND EvHVESSHealth * 0.400000));
  PREPEND EvHVESSUserSOC = (uint8_t)(rand() & 0xFF);
  printf("EvHVESSUserSOC %.3lf %%\n", ((double)PREPEND EvHVESSUserSOC * 0.400000));
  PREPEND EvACActivePower = (uint16_t)(rand() & 0xFFFF);
  printf("EvACActivePower %d W\n", (PREPEND EvACActivePower * 16) - 500);
  PREPEND EvACReactivePower = (uint16_t)(rand() & 0xFFFF);
  printf("EvACReactivePower %d VA\n", (PREPEND EvACReactivePower * 16) - 500);
  PREPEND EvACFrequency = (uint8_t)(rand() & 0xFF);
  printf("EvACFrequency %.3lf Hz\n", ((double)PREPEND EvACFrequency * 0.100000) - -42.500000);
  PREPEND EvL1NVolts = (uint16_t)(rand() & 0xFFFF);
  printf("EvL1NVolts %.3lf Volts\n", ((double)PREPEND EvL1NVolts * 0.050000));
  PREPEND EvL2NVolts = (uint16_t)(rand() & 0xFFFF);
  printf("EvL2NVolts %.3lf Volts\n", ((double)PREPEND EvL2NVolts * 0.050000));
  PREPEND EvL3NVolts = (uint16_t)(rand() & 0xFFFF);
  printf("EvL3NVolts %.3lf Volts\n", ((double)PREPEND EvL3NVolts * 0.050000));
  PREPEND EvAmbientTemp = (uint16_t)(rand() & 0xFFFF);
  printf("EvAmbientTemp %.3lf C\n", ((double)PREPEND EvAmbientTemp * 0.031250) - 273);
  PREPEND EvCabinTemp = (uint16_t)(rand() & 0xFFFF);
  printf("EvCabinTemp %.3lf C\n", ((double)PREPEND EvCabinTemp * 0.031250) - 273);
  PREPEND EvHVESSCellTemp = (uint16_t)(rand() & 0xFFFF);
  printf("EvHVESSCellTemp %.3lf C\n", ((double)PREPEND EvHVESSCellTemp * 0.031250) - 273);
  PREPEND EvMaxHVESSTemp = (uint16_t)(rand() & 0xFFFF);
  printf("EvMaxHVESSTemp %.3lf C\n", ((double)PREPEND EvMaxHVESSTemp * 0.031250) - 273);
  PREPEND EvMinHVESSTemp = (uint16_t)(rand() & 0xFFFF);
  printf("EvMinHVESSTemp %.3lf C\n", ((double)PREPEND EvMinHVESSTemp * 0.031250) - 273);
  PREPEND EvHVESSElecTemp = (uint8_t)(rand() & 0xFF);
  printf("EvHVESSElecTemp %d C\n", (PREPEND EvHVESSElecTemp) - 40);
  PREPEND EvMaxHVESSCellVolt = (uint16_t)(rand() & 0xFFFF);
  printf("EvMaxHVESSCellVolt %.3lf V\n", ((double)PREPEND EvMaxHVESSCellVolt * 0.001000));
  PREPEND EvMinHVESSCellVolt = (uint16_t)(rand() & 0xFFFF);
  printf("EvMinHVESSCellVolt %.3lf V\n", ((double)PREPEND EvMinHVESSCellVolt * 0.001000));
  PREPEND EvNumHVESSCellBalancing = (uint16_t)(rand() & 0xFFFF);
  printf("EvNumHVESSCellBalancing %d \n", (PREPEND EvNumHVESSCellBalancing));
  PREPEND EvStatusCellVoltDiff = (uint8_t)(rand() & 0b00001111);
  printf("EvStatusCellVoltDiff 0x%02X Enum\n", (PREPEND EvStatusCellVoltDiff));
  PREPEND EvStatusCellBal = (uint8_t)(rand() & 0b00000011);
  printf("EvStatusCellBal 0x%02X Enum\n", (PREPEND EvStatusCellBal));
  PREPEND EvActiveCellBal = (uint8_t)(rand() & 0b00000011);
  printf("EvActiveCellBal 0x%02X Enum\n", (PREPEND EvActiveCellBal));
  PREPEND EvChargerTemp = (uint16_t)(rand() & 0xFFFF);
  printf("EvChargerTemp %.3lf C\n", ((double)PREPEND EvChargerTemp * 0.031250) - 273);
  PREPEND EvMaxChargerTemp = (uint16_t)(rand() & 0xFFFF);
  printf("EvMaxChargerTemp %.3lf C\n", ((double)PREPEND EvMaxChargerTemp * 0.031250) - 273);
  PREPEND EvInletTemp = (uint8_t)(rand() & 0xFF);
  printf("EvInletTemp %d C\n", (PREPEND EvInletTemp) - 40);
  PREPEND EvHVESSTemp = (uint8_t)(rand() & 0xFF);
  printf("EvHVESSTemp %d C\n", (PREPEND EvHVESSTemp) - 40);
  #undef PREPEND
#else
  #define PREPEND (act_ids_xmit[ch]->v).
  #undef PREPEND
  #define PREPEND (act_data_xmit[ch]->v).
  PREPEND SeAmbientTemp = (uint16_t)(rand() & 0xFFFF);
  printf("SeAmbientTemp %.3lf C\n", ((double)PREPEND SeAmbientTemp * 0.031250) - 273);
  PREPEND SeConnectorTemp = (uint8_t)(rand() & 0xFF);
  printf("SeConnectorTemp %d C\n", (PREPEND SeConnectorTemp) - 40);
  PREPEND SeOutletTemp = (uint8_t)(rand() & 0xFF);
  printf("SeOutletTemp %d C\n", (PREPEND SeOutletTemp) - 40);
  PREPEND SeEvStatusOutletOverride = (uint8_t)(rand() & 0b00000011);
  printf("SeEvStatusOutletOverride 0x%02X Enum\n", (PREPEND SeEvStatusOutletOverride));
  PREPEND SeEvStatusOutletLock = (uint8_t)(rand() & 0b00000111);
  printf("SeEvStatusOutletLock 0x%02X Enum\n", (PREPEND SeEvStatusOutletLock));
  PREPEND SeRmtMgmtStatus = (uint8_t)(rand() & 0xFF);
  printf("SeRmtMgmtStatus 0x%02X Enum\n", (PREPEND SeRmtMgmtStatus));
  PREPEND SeEvTripStatus = (uint8_t)(rand() & 0xFF);
  printf("SeEvTripStatus 0x%02X Enum\n", (PREPEND SeEvTripStatus));
  PREPEND SeSeTripStatus = (uint8_t)(rand() & 0xFF);
  printf("SeSeTripStatus 0x%02X Enum\n", (PREPEND SeSeTripStatus));
  PREPEND SeExptTripPerct = (uint8_t)(rand() & 0xFF);
  printf("SeExptTripPerct %.3lf %%\n", ((double)PREPEND SeExptTripPerct * 0.400000));
  PREPEND SeTimeReqNum = (uint8_t)(rand() & 0xFF);
  printf("SeTimeReqNum %d count\n", (PREPEND SeTimeReqNum));
  PREPEND SeHVESSRangeCalc = (uint16_t)(rand() & 0xFFFF);
  printf("SeHVESSRangeCalc %d km\n", (PREPEND SeHVESSRangeCalc));
  PREPEND SeHVESSEnergyCalc = (uint32_t)(rand() & 0xFFFFFF);
  printf("SeHVESSEnergyCalc %.3lf kWh\n", ((double)PREPEND SeHVESSEnergyCalc * 0.001000));
  #undef PREPEND
#endif
  printf("======================================================================\n");
}

///<ids_init()

#ifdef EV_CONFIG

#define LC_ID_STATUS(ch)           ev_id_status[ch]
#define RM_ID_STATUS(ch)           se_id_status[ch]
#define READ_LC_PAGE_NUMBER(ch)    LR(ch, l_u8, EvIDPage)
#define READ_RM_PAGE_NUMBER(ch)    LR(ch, l_u8, SeIDPage)
#define READ_RM_BYTE_A(ch)         LR(ch, l_u8, SeIDByteA)

#else

#define LC_ID_STATUS(ch)           se_id_status[ch]
#define RM_ID_STATUS(ch)           ev_id_status[ch]
#define READ_LC_PAGE_NUMBER(ch)    LR(ch, l_u8, SeIDPage)
#define READ_RM_PAGE_NUMBER(ch)    LR(ch, l_u8, EvIDPage)
#define READ_RM_BYTE_A(ch)         LR(ch, l_u8, EvIDByteA)

#endif

void on_id_frame_receipt(uint8_t ch)  {
  if (READ_RM_PAGE_NUMBER(ch) == 0)  {   //make sure we are always reading the status
    RM_ID_STATUS(ch) = READ_RM_BYTE_A(ch);
  }

  if (LC_ID_STATUS(ch) == ID_INCOMPLETE && id_parse(ch)) {
    LC_ID_STATUS(ch) = ID_COMPLETE;

#ifdef EV_CONFIG
    PrintConsoleString("---EVSEID:",0);
    PrintConsoleString((inact_ids_rcv[ch]->s).SeEVSEID,0);
    PrintConsoleString(", Serial:", 0);
    PrintConsoleString((inact_ids_rcv[ch]->s).SeSerialNum, 0);
    PrintConsoleString("\r\n---SECCID:", 0);
    PrintConsoleString((inact_ids_rcv[ch]->s).SeSECCID, 0);
    PrintConsoleString("\r\n---Manu:",0);
    PrintConsoleString((inact_ids_rcv[ch]->s).SeManufacturer,0);
    PrintConsoleString(", Firmware:", 0);
    PrintConsoleString((inact_ids_rcv[ch]->s).SeFirmwareRevision, 0);
    PrintConsoleString("\r\n---Public:", 0);
    PrintConsoleString((inact_ids_rcv[ch]->s).SePublicName, 0);
    PrintConsoleString("\r\n", 0);
#else
    PrintConsoleString("---VIN:", 0);
    PrintConsoleString((inact_ids_rcv[ch]->s).EvVIN, 0);
    PrintConsoleString(", EMAID:", 0);
    PrintConsoleString((inact_ids_rcv[ch]->s).EvEMAID, 0);
    PrintConsoleString(", Drv:", 0);
    PrintConsoleString((inact_ids_rcv[ch]->s).EvDriverID, 0);
    PrintConsoleString(", Veh:", 0);
    PrintConsoleString((inact_ids_rcv[ch]->s).EvVehicleName, 0);
    PrintConsoleString("\r\n---EVCCID:", 0);
    PrintConsoleString((inact_ids_rcv[ch]->s).EvEVCCID, 0);
    PrintConsoleString("\r\n---Manu:", 0);
    PrintConsoleString((inact_ids_rcv[ch]->s).EvManufacturer, 0);
    PrintConsoleString(", Firmware:", 0);
    PrintConsoleString((inact_ids_rcv[ch]->s).EvFirmwareRevision, 0);
    PrintConsoleString("\r\n", 0);
#endif
  }

  if (LC_ID_STATUS(ch) == ID_COMPLETE && (RM_ID_STATUS(ch) == ID_COMPLETE || RM_ID_STATUS(ch) == DATA))  {
    PrintConsoleString("====ID COMPLETE!!!!===\r\n",0);
    LC_ID_STATUS(ch) = DATA;
    first_data[ch] = true;
  }

  if (LC_ID_STATUS(ch) == DATA ) {
    if (RM_ID_STATUS(ch) == DATA)  {
      data_parse(ch);
    }
  }
}

void on_id_frame_xmit(uint8_t ch) {
  static uint8_t xmit_pgs_indx[2] = {0, 0};
  void* temp_ptr;

  if (LC_ID_STATUS(ch) == ID_INCOMPLETE || LC_ID_STATUS(ch) == ID_COMPLETE) {
    if (xmit_pgs_indx[ch] == NUM_ID_PGS2SND-1)  {
      xmit_pgs_indx[ch] = 0;
      temp_ptr = inact_ids_xmit[ch];            //swap id xmit buffers
      inact_ids_xmit[ch] = act_ids_xmit[ch];
      act_ids_xmit[ch] = temp_ptr;
      //don't clear id xmit buffers ad info is static
    } else  {
      xmit_pgs_indx[ch]++;
    }
    id_xmit(ch, id_pages_to_send[xmit_pgs_indx[ch]]);
  }

  if (LC_ID_STATUS(ch) == DATA ) {
    if (first_data[ch])  {
      xmit_pgs_indx[ch] = 0;
      first_data[ch] = false;
    } else  {
      //printf("Success!\n");
      if (xmit_pgs_indx[ch] == NUM_DATA_PGS2SND-1)  {
        xmit_pgs_indx[ch] = 0;
        temp_ptr = inact_data_xmit[ch];            //swap data xmit buffers
        inact_data_xmit[ch] = act_data_xmit[ch];
        act_data_xmit[ch] = temp_ptr;
        clear_data_xmit_buff(act_data_xmit[ch]);  //clear data xmit buffer
      } else  {
        xmit_pgs_indx[ch]++;
      }
    }
    data_xmit(ch, data_pages_to_send[xmit_pgs_indx[ch]]);
  }
}

#ifdef EV_CONFIG

bool id_parse(uint8_t ch) {
  static uint8_t buffer[7];
  static uint8_t pgs_to_read[2] = {255, 255};
  static uint8_t pgs_read[2] = {0, 0};
  static uint8_t last_pg[2] = {255, 255};

  uint8_t copy_size = 7;
  uint8_t * source_ptr = buffer;
  uint8_t * dest_ptr = NULL;
  bool is_string = true;

  void * temp_ptr;

  buffer[0] = LR(ch, l_u8, SeIDByteA);
  buffer[1] = LR(ch, l_u8, SeIDByteB);
  buffer[2] = LR(ch, l_u8, SeIDByteC);
  buffer[3] = LR(ch, l_u8, SeIDByteD);
  buffer[4] = LR(ch, l_u8, SeIDByteE);
  buffer[5] = LR(ch, l_u8, SeIDByteF);
  buffer[6] = LR(ch, l_u8, SeIDByteG);

  //printf("-->%c%c\n", buffer[0], buffer[1]);
  //printf("page:%d\n", l_u8_rd_LI0_EvIDPage());

  pgs_read[ch]++;

  ///>ev_id_parse
  switch(LR(ch, l_u8, SeIDPage)) {
    case 0:
      pgs_to_read[ch] = buffer[1];
      se_id_status[ch] = buffer[0];
      last_pg[ch] = buffer[3];
      pgs_read[ch] = 1;
        clear_ids_rcv_buff(act_ids_rcv[ch]);  //clear rcv buffer on control page to remove data from corrupted cycle
      break;
    case 1:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeEVSEID;
      break;
    case 2:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeEVSEID + 7;
      break;
    case 3:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeEVSEID + 14;
      break;
    case 4:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeEVSEID + 21;
      break;
    case 5:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeEVSEID + 28;
      break;
    case 6:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeEVSEID + 35;
      copy_size = 5;
      break;
    case 7:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeSECCID;
      break;
    case 8:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeSECCID + 7;
      break;
    case 9:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeSECCID + 14;
      break;
    case 10:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeSECCID + 21;
      break;
    case 11:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeSECCID + 28;
      break;
    case 12:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeSECCID + 35;
      break;
    case 13:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeSECCID + 42;
      break;
    case 14:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeSECCID + 49;
      break;
    case 15:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeSECCID + 56;
      break;
    case 16:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeSECCID + 63;
      break;
    case 17:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeSerialNum;
      break;
    case 18:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeSerialNum + 7;
      break;
    case 19:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeSerialNum + 14;
      break;
    case 20:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeSerialNum + 21;
      break;
    case 21:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeSerialNum + 28;
      break;
    case 22:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeSerialNum + 35;
      break;
    case 23:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeFirmwareRevision;
      break;
    case 24:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeFirmwareRevision + 7;
      break;
    case 25:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeFirmwareRevision + 14;
      break;
    case 26:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeFirmwareRevision + 21;
      break;
    case 27:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeManufacturer;
      break;
    case 28:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeManufacturer + 7;
      break;
    case 29:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeManufacturer + 14;
      break;
    case 30:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeManufacturer + 21;
      break;
    case 31:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeManufacturer + 28;
      break;
    case 32:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SeManufacturer + 35;
      break;
    case 33:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SePublicName;
      break;
    case 34:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SePublicName + 7;
      break;
    case 35:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SePublicName + 14;
      break;
    case 36:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SePublicName + 21;
      break;
    case 37:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SePublicName + 28;
      break;
    case 38:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).SePublicName + 35;
      break;
  }
  ///<ev_id_parse

  //printf("pointer:%p\n",dest_ptr);

  //fflush(stdout);

  if (dest_ptr != NULL) {
    copy_buffer_data(source_ptr, dest_ptr, copy_size, is_string);
  }

  if ((pgs_read[ch] == pgs_to_read[ch]) && (LR(ch, l_u8, SeIDPage) == last_pg[ch]))  {
    temp_ptr = inact_ids_rcv[ch];            //swap id rcv buffers
    inact_ids_rcv[ch] = act_ids_rcv[ch];
    act_ids_rcv[ch] = temp_ptr;
    clear_ids_rcv_buff(act_ids_rcv[ch]);    //clear new buffer
    return true;
  } else  {
    return false;
  }
}

void id_xmit(uint8_t ch, uint8_t page){
  static uint8_t buffer[7];
  uint8_t mask = 0;

  uint8_t copy_size = 7;
  uint8_t * dest_ptr = buffer;
  uint8_t * source_ptr = NULL;

  ///>ev_id_xmit
  switch(page) {
    case 0:
      buffer[0] = ev_id_status[ch];
      buffer[1] = 33; //pages to be sent
      buffer[2] =  1; //first (non zero) page
      buffer[3] = 38; //last page
      buffer[4] = 0x00; //CRC neither sent nor read
      mask = 0b0000011;
      break;
    case 1:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvVIN;
      break;
    case 2:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvVIN + 7;
      break;
    case 3:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvVIN + 14;
      copy_size = 3;
      mask = 0b0001111;
      break;
    case 4:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvEMAID;
      break;
    case 5:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvEMAID + 7;
      break;
    case 6:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvEMAID + 14;
      copy_size = 4;
      mask = 0b0000111;
      break;
    case 7:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvEVCCID;
      break;
    case 8:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvEVCCID + 7;
      break;
    case 9:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvEVCCID + 14;
      break;
    case 10:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvEVCCID + 21;
      break;
    case 11:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvEVCCID + 28;
      break;
    case 12:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvEVCCID + 35;
      break;
    case 13:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvEVCCID + 42;
      break;
    case 14:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvEVCCID + 49;
      break;
    case 15:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvEVCCID + 56;
      break;
    case 16:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvEVCCID + 63;
      break;
    case 23:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvDriverID;
      break;
    case 24:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvDriverID + 7;
      break;
    case 25:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvDriverID + 14;
      break;
    case 26:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvVehicleName;
      break;
    case 27:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvVehicleName + 7;
      break;
    case 28:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvVehicleName + 14;
      break;
    case 29:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvFirmwareRevision;
      break;
    case 30:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvFirmwareRevision + 7;
      break;
    case 31:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvFirmwareRevision + 14;
      break;
    case 32:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvFirmwareRevision + 21;
      break;
    case 33:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvManufacturer;
      break;
    case 34:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvManufacturer + 7;
      break;
    case 35:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvManufacturer + 14;
      break;
    case 36:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvManufacturer + 21;
      break;
    case 37:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvManufacturer + 28;
      break;
    case 38:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).EvManufacturer + 35;
      break;
  }
  ///<ev_id_xmit


  if (source_ptr != NULL) {
    copy_buffer_data(source_ptr, dest_ptr, copy_size, true);
  }

  invalidate_bytes_page(buffer, mask);

  LW(ch, l_u8, EvIDPage, page);
  LW(ch, l_u8, EvIDByteA, buffer[0]);
  LW(ch, l_u8, EvIDByteB, buffer[1]);
  LW(ch, l_u8, EvIDByteC, buffer[2]);
  LW(ch, l_u8, EvIDByteD, buffer[3]);
  LW(ch, l_u8, EvIDByteE, buffer[4]);
  LW(ch, l_u8, EvIDByteF, buffer[5]);
  LW(ch, l_u8, EvIDByteG, buffer[6]);
}

void data_xmit(uint8_t ch, uint8_t page) {
  static uint8_t buffer[7];
  uint8_t mask = 0;

  uint8_t copy_size = 7;
  uint8_t * dest_ptr = buffer;
  uint8_t * source_ptr = NULL;

  ///>ev_data_xmit
  switch(page) {
    case 0:
      buffer[0] = ev_id_status[ch];
      buffer[1] = 15; //pages to be sent
      buffer[2] = 97; //first (non zero) page
      buffer[3] = 110; //last page
      buffer[4] = 0x00; //CRC neither sent nor read
      mask = 0b0000011;
      break;
    case 97:
      buffer[0] = ((act_data_xmit[ch]->v).EvOdometer) & 0xFF;
      buffer[1] = ((act_data_xmit[ch]->v).EvOdometer >> 8) & 0xFF;
      buffer[2] = ((act_data_xmit[ch]->v).EvOdometer >> 16) & 0xFF;
      buffer[3] = ((act_data_xmit[ch]->v).EvOdometer >> 24) & 0xFF;
      buffer[4] = 0xFF;
      buffer[4] &= ~((~(act_data_xmit[ch]->v).EvStatusInletLatch & 0b00000011) << 0);
      buffer[4] &= ~((~(act_data_xmit[ch]->v).EvStatusInletOverride & 0b00000011) << 2);
      buffer[4] &= ~((~(act_data_xmit[ch]->v).EvStatusInletLock & 0b00000111) << 4);
      mask = 0b0000011;
      break;
    case 98:
      buffer[0] = ((act_data_xmit[ch]->v).EvNumberJ2012Dtcs) & 0xFF;
      source_ptr = (uint8_t *) (act_data_xmit[ch]->s).EvJ2012Dtc;
      dest_ptr = buffer + 1;
      copy_size = 5;
      buffer[6] = 0xFF;
      buffer[6] &= ~((~(act_data_xmit[ch]->v).EvJ2012DtcStatus & 0b00000001) << 0);
      buffer[6] &= ~((~(act_data_xmit[ch]->v).EvJ2012DtcCount & 0b01111111) << 1);
      break;
    case 99:
      buffer[0] = ((act_data_xmit[ch]->v).EvHVESSDishargeCapacity) & 0xFF;
      buffer[1] = ((act_data_xmit[ch]->v).EvHVESSDishargeCapacity >> 8) & 0xFF;
      buffer[2] = ((act_data_xmit[ch]->v).EvHVESSDishargeCapacity >> 16) & 0xFF;
      buffer[3] = ((act_data_xmit[ch]->v).EvHVESSChargeCapacity) & 0xFF;
      buffer[4] = ((act_data_xmit[ch]->v).EvHVESSChargeCapacity >> 8) & 0xFF;
      buffer[5] = ((act_data_xmit[ch]->v).EvHVESSChargeCapacity >> 16) & 0xFF;
      mask = 0b0000001;
      break;
    case 100:
      buffer[0] = ((act_data_xmit[ch]->v).EvEnergyForDeparture) & 0xFF;
      buffer[1] = ((act_data_xmit[ch]->v).EvEnergyForDeparture >> 8) & 0xFF;
      buffer[2] = ((act_data_xmit[ch]->v).EvEnergyForDeparture >> 16) & 0xFF;
      buffer[3] = ((act_data_xmit[ch]->v).EvTimeToDeparture) & 0xFF;
      buffer[4] = ((act_data_xmit[ch]->v).EvTimeToDeparture >> 8) & 0xFF;
      mask = 0b0000011;
      break;
    case 101:
      buffer[0] = ((act_data_xmit[ch]->v).EvHVESSRange) & 0xFF;
      buffer[1] = ((act_data_xmit[ch]->v).EvHVESSRange >> 8) & 0xFF;
      buffer[2] = ((act_data_xmit[ch]->v).EvFuelRange) & 0xFF;
      buffer[3] = ((act_data_xmit[ch]->v).EvFuelRange >> 8) & 0xFF;
      buffer[4] = ((act_data_xmit[ch]->v).EvEVTimeToEnergyForDept) & 0xFF;
      buffer[5] = ((act_data_xmit[ch]->v).EvEVTimeToEnergyForDept >> 8) & 0xFF;
      mask = 0b0000001;
      break;
    case 102:
      buffer[0] = ((act_data_xmit[ch]->v).EvDurMin) & 0xFF;
      buffer[1] = ((act_data_xmit[ch]->v).EvDurMin >> 8) & 0xFF;
      buffer[2] = ((act_data_xmit[ch]->v).EvChaDurMax) & 0xFF;
      buffer[3] = ((act_data_xmit[ch]->v).EvChaDurMax >> 8) & 0xFF;
      buffer[4] = ((act_data_xmit[ch]->v).EvDschDurMax) & 0xFF;
      buffer[5] = ((act_data_xmit[ch]->v).EvDschDurMax >> 8) & 0xFF;
      mask = 0b0000001;
      break;
    case 103:
      buffer[0] = ((act_data_xmit[ch]->v).EvTimeReqNum) & 0xFF;
      buffer[1] = ((act_data_xmit[ch]->v).EvEVTimeToRange) & 0xFF;
      buffer[2] = ((act_data_xmit[ch]->v).EvEVTimeToRange >> 8) & 0xFF;
      buffer[3] = ((act_data_xmit[ch]->v).EvEVTimeToEnergy) & 0xFF;
      buffer[4] = ((act_data_xmit[ch]->v).EvEVTimeToEnergy >> 8) & 0xFF;
      mask = 0b0000011;
      break;
    case 104:
      buffer[0] = ((act_data_xmit[ch]->v).EvHVESSVoltage) & 0xFF;
      buffer[1] = ((act_data_xmit[ch]->v).EvHVESSVoltage >> 8) & 0xFF;
      buffer[2] = ((act_data_xmit[ch]->v).EvHVESSCurrent) & 0xFF;
      buffer[3] = ((act_data_xmit[ch]->v).EvHVESSCurrent >> 8) & 0xFF;
      buffer[4] = ((act_data_xmit[ch]->v).EvHVESSHealth) & 0xFF;
      buffer[5] = ((act_data_xmit[ch]->v).EvHVESSUserSOC) & 0xFF;
      mask = 0b0000001;
      break;
    case 105:
      buffer[0] = ((act_data_xmit[ch]->v).EvACActivePower) & 0xFF;
      buffer[1] = ((act_data_xmit[ch]->v).EvACActivePower >> 8) & 0xFF;
      buffer[2] = ((act_data_xmit[ch]->v).EvACReactivePower) & 0xFF;
      buffer[3] = ((act_data_xmit[ch]->v).EvACReactivePower >> 8) & 0xFF;
      buffer[4] = ((act_data_xmit[ch]->v).EvACFrequency) & 0xFF;
      mask = 0b0000011;
      break;
    case 106:
      buffer[0] = ((act_data_xmit[ch]->v).EvL1NVolts) & 0xFF;
      buffer[1] = ((act_data_xmit[ch]->v).EvL1NVolts >> 8) & 0xFF;
      buffer[2] = ((act_data_xmit[ch]->v).EvL2NVolts) & 0xFF;
      buffer[3] = ((act_data_xmit[ch]->v).EvL2NVolts >> 8) & 0xFF;
      buffer[4] = ((act_data_xmit[ch]->v).EvL3NVolts) & 0xFF;
      buffer[5] = ((act_data_xmit[ch]->v).EvL3NVolts >> 8) & 0xFF;
      mask = 0b0000001;
      break;
    case 107:
      buffer[0] = ((act_data_xmit[ch]->v).EvAmbientTemp) & 0xFF;
      buffer[1] = ((act_data_xmit[ch]->v).EvAmbientTemp >> 8) & 0xFF;
      buffer[2] = ((act_data_xmit[ch]->v).EvCabinTemp) & 0xFF;
      buffer[3] = ((act_data_xmit[ch]->v).EvCabinTemp >> 8) & 0xFF;
      mask = 0b0000111;
      break;
    case 108:
      buffer[0] = ((act_data_xmit[ch]->v).EvHVESSCellTemp) & 0xFF;
      buffer[1] = ((act_data_xmit[ch]->v).EvHVESSCellTemp >> 8) & 0xFF;
      buffer[2] = ((act_data_xmit[ch]->v).EvMaxHVESSTemp) & 0xFF;
      buffer[3] = ((act_data_xmit[ch]->v).EvMaxHVESSTemp >> 8) & 0xFF;
      buffer[4] = ((act_data_xmit[ch]->v).EvMinHVESSTemp) & 0xFF;
      buffer[5] = ((act_data_xmit[ch]->v).EvMinHVESSTemp >> 8) & 0xFF;
      buffer[6] = ((act_data_xmit[ch]->v).EvHVESSElecTemp) & 0xFF;
      break;
    case 109:
      buffer[0] = ((act_data_xmit[ch]->v).EvMaxHVESSCellVolt) & 0xFF;
      buffer[1] = ((act_data_xmit[ch]->v).EvMaxHVESSCellVolt >> 8) & 0xFF;
      buffer[2] = ((act_data_xmit[ch]->v).EvMinHVESSCellVolt) & 0xFF;
      buffer[3] = ((act_data_xmit[ch]->v).EvMinHVESSCellVolt >> 8) & 0xFF;
      buffer[4] = ((act_data_xmit[ch]->v).EvNumHVESSCellBalancing) & 0xFF;
      buffer[5] = ((act_data_xmit[ch]->v).EvNumHVESSCellBalancing >> 8) & 0xFF;
      buffer[6] = 0xFF;
      buffer[6] &= ~((~(act_data_xmit[ch]->v).EvStatusCellVoltDiff & 0b00001111) << 0);
      buffer[6] &= ~((~(act_data_xmit[ch]->v).EvStatusCellBal & 0b00000011) << 4);
      buffer[6] &= ~((~(act_data_xmit[ch]->v).EvActiveCellBal & 0b00000011) << 6);
      break;
    case 110:
      buffer[0] = ((act_data_xmit[ch]->v).EvChargerTemp) & 0xFF;
      buffer[1] = ((act_data_xmit[ch]->v).EvChargerTemp >> 8) & 0xFF;
      buffer[2] = ((act_data_xmit[ch]->v).EvMaxChargerTemp) & 0xFF;
      buffer[3] = ((act_data_xmit[ch]->v).EvMaxChargerTemp >> 8) & 0xFF;
      buffer[4] = ((act_data_xmit[ch]->v).EvInletTemp) & 0xFF;
      buffer[5] = ((act_data_xmit[ch]->v).EvHVESSTemp) & 0xFF;
      mask = 0b0000001;
      break;
  }
  ///<ev_data_xmit

  if (source_ptr != NULL) {
    copy_buffer_data(source_ptr, dest_ptr, copy_size, false);
  }

  invalidate_bytes_page(buffer, mask);

  LW(ch, l_u8, EvIDPage, page);
  LW(ch, l_u8, EvIDByteA, buffer[0]);
  LW(ch, l_u8, EvIDByteB, buffer[1]);
  LW(ch, l_u8, EvIDByteC, buffer[2]);
  LW(ch, l_u8, EvIDByteD, buffer[3]);
  LW(ch, l_u8, EvIDByteE, buffer[4]);
  LW(ch, l_u8, EvIDByteF, buffer[5]);
  LW(ch, l_u8, EvIDByteG, buffer[6]);
}

void data_parse(uint8_t ch)  {
  static uint8_t buffer[7];
  static uint8_t pgs_to_read[2] = {255, 255};
  static uint8_t pgs_read[2] = {0, 0};
  static uint8_t last_pg[2] = {255, 255};

  uint8_t copy_size = 7;
  uint8_t * source_ptr = buffer;
  uint8_t * dest_ptr = NULL;
  bool is_string = true;

  void* temp_ptr;

  buffer[0] = LR(ch, l_u8, SeIDByteA);
  buffer[1] = LR(ch, l_u8, SeIDByteB);
  buffer[2] = LR(ch, l_u8, SeIDByteC);
  buffer[3] = LR(ch, l_u8, SeIDByteD);
  buffer[4] = LR(ch, l_u8, SeIDByteE);
  buffer[5] = LR(ch, l_u8, SeIDByteF);
  buffer[6] = LR(ch, l_u8, SeIDByteG);

  pgs_read[ch]++;

  ///>ev_data_parse
  switch(LR(ch, l_u8, SeIDPage)) {
    case 0:
      pgs_to_read[ch] = buffer[1];
      se_id_status[ch] = buffer[0];
      last_pg[ch] = buffer[3];
      pgs_read[ch] = 1;
        clear_data_rcv_buff(act_data_rcv[ch]);  //clear rcv buffer on control page to remove data from corrupted cycle
      break;
    case 97:
      (act_data_rcv[ch]->v).SeAmbientTemp = ((uint16_t)buffer[1] << 8) | ((uint16_t)buffer[0]);
      printf("SeAmbientTemp %.3lf C\n", ((double)(act_data_rcv[ch]->v).SeAmbientTemp * 0.031250) - 273);
      (act_data_rcv[ch]->v).SeConnectorTemp = ((uint8_t)buffer[2]);
      printf("SeConnectorTemp %d C\n", ((act_data_rcv[ch]->v).SeConnectorTemp) - 40);
      (act_data_rcv[ch]->v).SeOutletTemp = ((uint8_t)buffer[3]);
      printf("SeOutletTemp %d C\n", ((act_data_rcv[ch]->v).SeOutletTemp) - 40);
      (act_data_rcv[ch]->v).SeEvStatusOutletOverride = ((buffer[4] >> 0) & 0b00000011);
      printf("SeEvStatusOutletOverride 0x%02X Enum\n", ((act_data_rcv[ch]->v).SeEvStatusOutletOverride));
      (act_data_rcv[ch]->v).SeEvStatusOutletLock = ((buffer[4] >> 2) & 0b00000111);
      printf("SeEvStatusOutletLock 0x%02X Enum\n", ((act_data_rcv[ch]->v).SeEvStatusOutletLock));
      break;
    case 98:
      (act_data_rcv[ch]->v).SeRmtMgmtStatus = ((uint8_t)buffer[0]);
      printf("SeRmtMgmtStatus 0x%02X Enum\n", ((act_data_rcv[ch]->v).SeRmtMgmtStatus));
      (act_data_rcv[ch]->v).SeEvTripStatus = ((uint8_t)buffer[1]);
      printf("SeEvTripStatus 0x%02X Enum\n", ((act_data_rcv[ch]->v).SeEvTripStatus));
      (act_data_rcv[ch]->v).SeSeTripStatus = ((uint8_t)buffer[2]);
      printf("SeSeTripStatus 0x%02X Enum\n", ((act_data_rcv[ch]->v).SeSeTripStatus));
      (act_data_rcv[ch]->v).SeExptTripPerct = ((uint8_t)buffer[3]);
      printf("SeExptTripPerct %.3lf %%\n", ((double)(act_data_rcv[ch]->v).SeExptTripPerct * 0.400000));
      break;
    case 99:
      (act_data_rcv[ch]->v).SeTimeReqNum = ((uint8_t)buffer[0]);
      printf("SeTimeReqNum %d count\n", ((act_data_rcv[ch]->v).SeTimeReqNum));
      (act_data_rcv[ch]->v).SeHVESSRangeCalc = ((uint16_t)buffer[2] << 8) | ((uint16_t)buffer[1]);
      printf("SeHVESSRangeCalc %d km\n", ((act_data_rcv[ch]->v).SeHVESSRangeCalc));
      (act_data_rcv[ch]->v).SeHVESSEnergyCalc = ((uint32_t)buffer[5] << 16) | ((uint32_t)buffer[4] << 8) | ((uint32_t)buffer[3]);
      printf("SeHVESSEnergyCalc %.3lf kWh\n", ((double)(act_data_rcv[ch]->v).SeHVESSEnergyCalc * 0.001000));
      break;
  }
  ///<ev_data_parse

  if (dest_ptr != NULL) {
    copy_buffer_data(source_ptr, dest_ptr, copy_size, is_string);
  }

  if ((pgs_read[ch] == pgs_to_read[ch]) && (LR(ch, l_u8, EvIDPage) == last_pg[ch]))  {
    temp_ptr = inact_data_rcv[ch];           //swap data rcv buffers
    inact_data_rcv[ch] = act_data_rcv[ch];
    act_data_rcv[ch] = temp_ptr;
    clear_data_rcv_buff(act_data_rcv[ch]);   //clear new buffer
  }
}
#else
bool id_parse(uint8_t ch) {
  static uint8_t buffer[7];
  static uint8_t pgs_to_read[2] = {255, 255};
  static uint8_t pgs_read[2] = {0, };
  static uint8_t last_pg[2] = {255, 255};

  uint8_t copy_size = 7;
  uint8_t * source_ptr = buffer;
  uint8_t * dest_ptr = NULL;
  bool is_string = true;

  void* temp_ptr;

  buffer[0] = LR(ch, l_u8, EvIDByteA);
  buffer[1] = LR(ch, l_u8, EvIDByteB);
  buffer[2] = LR(ch, l_u8, EvIDByteC);
  buffer[3] = LR(ch, l_u8, EvIDByteD);
  buffer[4] = LR(ch, l_u8, EvIDByteE);
  buffer[5] = LR(ch, l_u8, EvIDByteF);
  buffer[6] = LR(ch, l_u8, EvIDByteG);

  //printf("-->%c%c\n", buffer[0], buffer[1]);
  //printf("page:%d\n", l_u8_rd_LI0_EvIDPage());

  pgs_read[ch]++;

  ///>se_id_parse
  switch(LR(ch, l_u8, EvIDPage)) {
    case 0:
      pgs_to_read[ch] = buffer[1];
      ev_id_status[ch] = buffer[0];
      last_pg[ch] = buffer[3];
      pgs_read[ch] = 1;
        clear_ids_rcv_buff(act_ids_rcv[ch]);  //clear rcv buffer on control page to remove data from corrupted cycle
      break;
    case 1:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvVIN;
      break;
    case 2:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvVIN + 7;
      break;
    case 3:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvVIN + 14;
      copy_size = 3;
      break;
    case 4:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvEMAID;
      break;
    case 5:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvEMAID + 7;
      break;
    case 6:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvEMAID + 14;
      copy_size = 4;
      break;
    case 7:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvEVCCID;
      break;
    case 8:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvEVCCID + 7;
      break;
    case 9:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvEVCCID + 14;
      break;
    case 10:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvEVCCID + 21;
      break;
    case 11:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvEVCCID + 28;
      break;
    case 12:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvEVCCID + 35;
      break;
    case 13:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvEVCCID + 42;
      break;
    case 14:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvEVCCID + 49;
      break;
    case 15:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvEVCCID + 56;
      break;
    case 16:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvEVCCID + 63;
      break;
    case 23:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvDriverID;
      break;
    case 24:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvDriverID + 7;
      break;
    case 25:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvDriverID + 14;
      break;
    case 26:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvVehicleName;
      break;
    case 27:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvVehicleName + 7;
      break;
    case 28:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvVehicleName + 14;
      break;
    case 29:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvFirmwareRevision;
      break;
    case 30:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvFirmwareRevision + 7;
      break;
    case 31:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvFirmwareRevision + 14;
      break;
    case 32:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvFirmwareRevision + 21;
      break;
    case 33:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvManufacturer;
      break;
    case 34:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvManufacturer + 7;
      break;
    case 35:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvManufacturer + 14;
      break;
    case 36:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvManufacturer + 21;
      break;
    case 37:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvManufacturer + 28;
      break;
    case 38:
      dest_ptr = (uint8_t *) (act_ids_rcv[ch]->s).EvManufacturer + 35;
      break;
  }
  ///<se_id_parse

  //printf("pointer:%p\n",dest_ptr);

  //fflush(stdout);

  if (dest_ptr != NULL) {
    copy_buffer_data(source_ptr, dest_ptr, copy_size, is_string);
  }

  if ((pgs_read[ch] == pgs_to_read[ch]) && (LR(ch, l_u8, EvIDPage) == last_pg[ch]))  {
    temp_ptr = inact_ids_rcv[ch];            //swap id rcv buffers
    inact_ids_rcv[ch] = act_ids_rcv[ch];
    act_ids_rcv[ch] = temp_ptr;
    clear_ids_rcv_buff(act_ids_rcv[ch]);    //clear new buffer
    return true;
  } else  {
    return false;
  }
}

void id_xmit(uint8_t ch, uint8_t page){
  static uint8_t buffer[7];
  uint8_t mask = 0;

  uint8_t copy_size = 7;
  uint8_t * dest_ptr = buffer;
  uint8_t * source_ptr = NULL;

  ///>se_id_xmit
  switch(page) {
    case 0:
      buffer[0] = se_id_status[ch];
      buffer[1] = 39; //pages to be sent
      buffer[2] =  1; //first (non zero) page
      buffer[3] = 38; //last page
      buffer[4] = 0x00; //CRC neither sent nor read
      mask = 0b0000011;
      break;
    case 1:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeEVSEID;
      break;
    case 2:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeEVSEID + 7;
      break;
    case 3:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeEVSEID + 14;
      break;
    case 4:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeEVSEID + 21;
      break;
    case 5:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeEVSEID + 28;
      break;
    case 6:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeEVSEID + 35;
      copy_size = 5;
      mask = 0b0000011;
      break;
    case 7:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeSECCID;
      break;
    case 8:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeSECCID + 7;
      break;
    case 9:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeSECCID + 14;
      break;
    case 10:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeSECCID + 21;
      break;
    case 11:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeSECCID + 28;
      break;
    case 12:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeSECCID + 35;
      break;
    case 13:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeSECCID + 42;
      break;
    case 14:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeSECCID + 49;
      break;
    case 15:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeSECCID + 56;
      break;
    case 16:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeSECCID + 63;
      break;
    case 17:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeSerialNum;
      break;
    case 18:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeSerialNum + 7;
      break;
    case 19:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeSerialNum + 14;
      break;
    case 20:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeSerialNum + 21;
      break;
    case 21:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeSerialNum + 28;
      break;
    case 22:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeSerialNum + 35;
      break;
    case 23:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeFirmwareRevision;
      break;
    case 24:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeFirmwareRevision + 7;
      break;
    case 25:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeFirmwareRevision + 14;
      break;
    case 26:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeFirmwareRevision + 21;
      break;
    case 27:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeManufacturer;
      break;
    case 28:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeManufacturer + 7;
      break;
    case 29:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeManufacturer + 14;
      break;
    case 30:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeManufacturer + 21;
      break;
    case 31:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeManufacturer + 28;
      break;
    case 32:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SeManufacturer + 35;
      break;
    case 33:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SePublicName;
      break;
    case 34:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SePublicName + 7;
      break;
    case 35:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SePublicName + 14;
      break;
    case 36:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SePublicName + 21;
      break;
    case 37:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SePublicName + 28;
      break;
    case 38:
      source_ptr = (uint8_t *) (act_ids_xmit[ch]->s).SePublicName + 35;
      break;
  }
  ///<se_id_xmit


  if (source_ptr != NULL) {
    copy_buffer_data(source_ptr, dest_ptr, copy_size, true);
  }

  invalidate_bytes_page(buffer, mask);

  LW(ch, l_u8, SeIDPage, page);
  LW(ch, l_u8, SeIDByteA, buffer[0]);
  LW(ch, l_u8, SeIDByteB, buffer[1]);
  LW(ch, l_u8, SeIDByteC, buffer[2]);
  LW(ch, l_u8, SeIDByteD, buffer[3]);
  LW(ch, l_u8, SeIDByteE, buffer[4]);
  LW(ch, l_u8, SeIDByteF, buffer[5]);
  LW(ch, l_u8, SeIDByteG, buffer[6]);
}

void data_xmit(uint8_t ch, uint8_t page) {
  static uint8_t buffer[7];
  uint8_t mask = 0;
  uint32_t temp;

  uint8_t copy_size = 7;
  uint8_t * dest_ptr = buffer;
  uint8_t * source_ptr = NULL;

  ///>se_data_xmit
  switch(page) {
    case 0:
      buffer[0] = se_id_status[ch];
      buffer[1] =  4; //pages to be sent
      buffer[2] = 97; //first (non zero) page
      buffer[3] = 99; //last page
      buffer[4] = 0x00; //CRC neither sent nor read
      mask = 0b0000011;
      break;
    case 97:
      buffer[0] = ((act_data_xmit[ch]->v).SeAmbientTemp) & 0xFF;
      buffer[1] = ((act_data_xmit[ch]->v).SeAmbientTemp >> 8) & 0xFF;
      buffer[2] = ((act_data_xmit[ch]->v).SeConnectorTemp) & 0xFF;
      buffer[3] = ((act_data_xmit[ch]->v).SeOutletTemp) & 0xFF;
      buffer[4] = 0xFF;
      buffer[4] &= ~((~(act_data_xmit[ch]->v).SeEvStatusOutletOverride & 0b00000011) << 0);
      buffer[4] &= ~((~(act_data_xmit[ch]->v).SeEvStatusOutletLock & 0b00000111) << 2);
      mask = 0b0000011;
      break;
    case 98:
      buffer[0] = ((act_data_xmit[ch]->v).SeRmtMgmtStatus) & 0xFF;
      buffer[1] = ((act_data_xmit[ch]->v).SeEvTripStatus) & 0xFF;
      buffer[2] = ((act_data_xmit[ch]->v).SeSeTripStatus) & 0xFF;
      buffer[3] = ((act_data_xmit[ch]->v).SeExptTripPerct) & 0xFF;
      mask = 0b0000111;
      break;
    case 99:
      buffer[0] = ((act_data_xmit[ch]->v).SeTimeReqNum) & 0xFF;
      buffer[1] = ((act_data_xmit[ch]->v).SeHVESSRangeCalc) & 0xFF;
      buffer[2] = ((act_data_xmit[ch]->v).SeHVESSRangeCalc >> 8) & 0xFF;
      buffer[3] = ((act_data_xmit[ch]->v).SeHVESSEnergyCalc) & 0xFF;
      buffer[4] = ((act_data_xmit[ch]->v).SeHVESSEnergyCalc >> 8) & 0xFF;
      buffer[5] = ((act_data_xmit[ch]->v).SeHVESSEnergyCalc >> 16) & 0xFF;
      mask = 0b0000001;
      break;
  }
  ///<se_data_xmit

  if (source_ptr != NULL) {
    copy_buffer_data(source_ptr, dest_ptr, copy_size, false);
  }

  invalidate_bytes_page(buffer, mask);

  LW(ch, l_u8, SeIDPage, page);
  LW(ch, l_u8, SeIDByteA, buffer[0]);
  LW(ch, l_u8, SeIDByteB, buffer[1]);
  LW(ch, l_u8, SeIDByteC, buffer[2]);
  LW(ch, l_u8, SeIDByteD, buffer[3]);
  LW(ch, l_u8, SeIDByteE, buffer[4]);
  LW(ch, l_u8, SeIDByteF, buffer[5]);
  LW(ch, l_u8, SeIDByteG, buffer[6]);
}

void data_parse(uint8_t ch)  {
  static uint8_t buffer[7];
  static uint8_t pgs_to_read[2] = {255, 255};
  static uint8_t pgs_read[2] = {0, 0};
  static uint8_t last_pg[2] = {255, 255};

  uint8_t copy_size = 7;
  uint8_t * source_ptr = buffer;
  uint8_t * dest_ptr = NULL;
  bool is_string = true;

  void* temp_ptr;

  buffer[0] = LR(ch, l_u8, EvIDByteA);
  buffer[1] = LR(ch, l_u8, EvIDByteB);
  buffer[2] = LR(ch, l_u8, EvIDByteC);
  buffer[3] = LR(ch, l_u8, EvIDByteD);
  buffer[4] = LR(ch, l_u8, EvIDByteE);
  buffer[5] = LR(ch, l_u8, EvIDByteF);
  buffer[6] = LR(ch, l_u8, EvIDByteG);

  pgs_read[ch]++;

  ///>se_data_parse
  switch(LR(ch, l_u8, EvIDPage)) {
    case 0:
      pgs_to_read[ch] = buffer[1];
      ev_id_status[ch] = buffer[0];
      last_pg[ch] = buffer[3];
      pgs_read[ch] = 1;
        clear_data_rcv_buff(act_data_rcv[ch]);  //clear rcv buffer on control page to remove data from corrupted cycle
      break;
    case 97:
      (act_data_rcv[ch]->v).EvOdometer = ((uint32_t)buffer[3] << 24) | ((uint32_t)buffer[2] << 16) | ((uint32_t)buffer[1] << 8) | ((uint32_t)buffer[0]);
      printf("EvOdometer %.3lf km\n", ((double)(act_data_rcv[ch]->v).EvOdometer * 0.125000));
      (act_data_rcv[ch]->v).EvStatusInletLatch = ((buffer[4] >> 0) & 0b00000011);
      printf("EvStatusInletLatch 0x%02X Enum\n", ((act_data_rcv[ch]->v).EvStatusInletLatch));
      (act_data_rcv[ch]->v).EvStatusInletOverride = ((buffer[4] >> 2) & 0b00000011);
      printf("EvStatusInletOverride 0x%02X Enum\n", ((act_data_rcv[ch]->v).EvStatusInletOverride));
      (act_data_rcv[ch]->v).EvStatusInletLock = ((buffer[4] >> 4) & 0b00000111);
      printf("EvStatusInletLock 0x%02X Enum\n", ((act_data_rcv[ch]->v).EvStatusInletLock));
      break;
    case 98:
      (act_data_rcv[ch]->v).EvNumberJ2012Dtcs = ((uint8_t)buffer[0]);
      printf("EvNumberJ2012Dtcs %d count\n", ((act_data_rcv[ch]->v).EvNumberJ2012Dtcs));
      dest_ptr = (uint8_t *) (act_data_rcv[ch]->s).EvJ2012Dtc;
      source_ptr = buffer + 1;
      copy_size = 5;
      (act_data_rcv[ch]->v).EvJ2012DtcStatus = ((buffer[6] >> 0) & 0b00000001);
      printf("EvJ2012DtcStatus %d bit\n", ((act_data_rcv[ch]->v).EvJ2012DtcStatus));
      (act_data_rcv[ch]->v).EvJ2012DtcCount = ((buffer[6] >> 1) & 0b01111111);
      printf("EvJ2012DtcCount %d count\n", ((act_data_rcv[ch]->v).EvJ2012DtcCount));
      break;
    case 99:
      (act_data_rcv[ch]->v).EvHVESSDishargeCapacity = ((uint32_t)buffer[2] << 16) | ((uint32_t)buffer[1] << 8) | ((uint32_t)buffer[0]);
      printf("EvHVESSDishargeCapacity %.3lf kWh\n", ((double)(act_data_rcv[ch]->v).EvHVESSDishargeCapacity * 0.001000));
      (act_data_rcv[ch]->v).EvHVESSChargeCapacity = ((uint32_t)buffer[5] << 16) | ((uint32_t)buffer[4] << 8) | ((uint32_t)buffer[3]);
      printf("EvHVESSChargeCapacity %.3lf kWh\n", ((double)(act_data_rcv[ch]->v).EvHVESSChargeCapacity * 0.001000));
      break;
    case 100:
      (act_data_rcv[ch]->v).EvEnergyForDeparture = ((uint32_t)buffer[2] << 16) | ((uint32_t)buffer[1] << 8) | ((uint32_t)buffer[0]);
      printf("EvEnergyForDeparture %.3lf kWh\n", ((double)(act_data_rcv[ch]->v).EvEnergyForDeparture * 0.001000));
      (act_data_rcv[ch]->v).EvTimeToDeparture = ((uint16_t)buffer[4] << 8) | ((uint16_t)buffer[3]);
      printf("EvTimeToDeparture %d min\n", ((act_data_rcv[ch]->v).EvTimeToDeparture));
      break;
    case 101:
      (act_data_rcv[ch]->v).EvHVESSRange = ((uint16_t)buffer[1] << 8) | ((uint16_t)buffer[0]);
      printf("EvHVESSRange %d km\n", ((act_data_rcv[ch]->v).EvHVESSRange));
      (act_data_rcv[ch]->v).EvFuelRange = ((uint16_t)buffer[3] << 8) | ((uint16_t)buffer[2]);
      printf("EvFuelRange %d km\n", ((act_data_rcv[ch]->v).EvFuelRange));
      (act_data_rcv[ch]->v).EvEVTimeToEnergyForDept = ((uint16_t)buffer[5] << 8) | ((uint16_t)buffer[4]);
      printf("EvEVTimeToEnergyForDept %d min\n", ((act_data_rcv[ch]->v).EvEVTimeToEnergyForDept));
      break;
    case 102:
      (act_data_rcv[ch]->v).EvDurMin = ((uint16_t)buffer[1] << 8) | ((uint16_t)buffer[0]);
      printf("EvDurMin %d s\n", ((act_data_rcv[ch]->v).EvDurMin * 10));
      (act_data_rcv[ch]->v).EvChaDurMax = ((uint16_t)buffer[3] << 8) | ((uint16_t)buffer[2]);
      printf("EvChaDurMax %d s\n", ((act_data_rcv[ch]->v).EvChaDurMax * 10));
      (act_data_rcv[ch]->v).EvDschDurMax = ((uint16_t)buffer[5] << 8) | ((uint16_t)buffer[4]);
      printf("EvDschDurMax %d s\n", ((act_data_rcv[ch]->v).EvDschDurMax * 10));
      break;
    case 103:
      (act_data_rcv[ch]->v).EvTimeReqNum = ((uint8_t)buffer[0]);
      printf("EvTimeReqNum 0x%02X \n", ((act_data_rcv[ch]->v).EvTimeReqNum));
      (act_data_rcv[ch]->v).EvEVTimeToRange = ((uint16_t)buffer[2] << 8) | ((uint16_t)buffer[1]);
      printf("EvEVTimeToRange %d min\n", ((act_data_rcv[ch]->v).EvEVTimeToRange));
      (act_data_rcv[ch]->v).EvEVTimeToEnergy = ((uint16_t)buffer[4] << 8) | ((uint16_t)buffer[3]);
      printf("EvEVTimeToEnergy %d min\n", ((act_data_rcv[ch]->v).EvEVTimeToEnergy));
      break;
    case 104:
      (act_data_rcv[ch]->v).EvHVESSVoltage = ((uint16_t)buffer[1] << 8) | ((uint16_t)buffer[0]);
      printf("EvHVESSVoltage %.3lf \n", ((double)(act_data_rcv[ch]->v).EvHVESSVoltage * 0.050000));
      (act_data_rcv[ch]->v).EvHVESSCurrent = ((uint16_t)buffer[3] << 8) | ((uint16_t)buffer[2]);
      printf("EvHVESSCurrent %.3lf \n", ((double)(act_data_rcv[ch]->v).EvHVESSCurrent * 0.050000) - 1600);
      (act_data_rcv[ch]->v).EvHVESSHealth = ((uint8_t)buffer[4]);
      printf("EvHVESSHealth %.3lf %%\n", ((double)(act_data_rcv[ch]->v).EvHVESSHealth * 0.400000));
      (act_data_rcv[ch]->v).EvHVESSUserSOC = ((uint8_t)buffer[5]);
      printf("EvHVESSUserSOC %.3lf %%\n", ((double)(act_data_rcv[ch]->v).EvHVESSUserSOC * 0.400000));
      break;
    case 105:
      (act_data_rcv[ch]->v).EvACActivePower = ((uint16_t)buffer[1] << 8) | ((uint16_t)buffer[0]);
      printf("EvACActivePower %d W\n", ((act_data_rcv[ch]->v).EvACActivePower * 16) - 500);
      (act_data_rcv[ch]->v).EvACReactivePower = ((uint16_t)buffer[3] << 8) | ((uint16_t)buffer[2]);
      printf("EvACReactivePower %d VA\n", ((act_data_rcv[ch]->v).EvACReactivePower * 16) - 500);
      (act_data_rcv[ch]->v).EvACFrequency = ((uint8_t)buffer[4]);
      printf("EvACFrequency %.3lf Hz\n", ((double)(act_data_rcv[ch]->v).EvACFrequency * 0.100000) - -42.500000);
      break;
    case 106:
      (act_data_rcv[ch]->v).EvL1NVolts = ((uint16_t)buffer[1] << 8) | ((uint16_t)buffer[0]);
      printf("EvL1NVolts %.3lf Volts\n", ((double)(act_data_rcv[ch]->v).EvL1NVolts * 0.050000));
      (act_data_rcv[ch]->v).EvL2NVolts = ((uint16_t)buffer[3] << 8) | ((uint16_t)buffer[2]);
      printf("EvL2NVolts %.3lf Volts\n", ((double)(act_data_rcv[ch]->v).EvL2NVolts * 0.050000));
      (act_data_rcv[ch]->v).EvL3NVolts = ((uint16_t)buffer[5] << 8) | ((uint16_t)buffer[4]);
      printf("EvL3NVolts %.3lf Volts\n", ((double)(act_data_rcv[ch]->v).EvL3NVolts * 0.050000));
      break;
    case 107:
      (act_data_rcv[ch]->v).EvAmbientTemp = ((uint16_t)buffer[1] << 8) | ((uint16_t)buffer[0]);
      printf("EvAmbientTemp %.3lf C\n", ((double)(act_data_rcv[ch]->v).EvAmbientTemp * 0.031250) - 273);
      (act_data_rcv[ch]->v).EvCabinTemp = ((uint16_t)buffer[3] << 8) | ((uint16_t)buffer[2]);
      printf("EvCabinTemp %.3lf C\n", ((double)(act_data_rcv[ch]->v).EvCabinTemp * 0.031250) - 273);
      break;
    case 108:
      (act_data_rcv[ch]->v).EvHVESSCellTemp = ((uint16_t)buffer[1] << 8) | ((uint16_t)buffer[0]);
      printf("EvHVESSCellTemp %.3lf C\n", ((double)(act_data_rcv[ch]->v).EvHVESSCellTemp * 0.031250) - 273);
      (act_data_rcv[ch]->v).EvMaxHVESSTemp = ((uint16_t)buffer[3] << 8) | ((uint16_t)buffer[2]);
      printf("EvMaxHVESSTemp %.3lf C\n", ((double)(act_data_rcv[ch]->v).EvMaxHVESSTemp * 0.031250) - 273);
      (act_data_rcv[ch]->v).EvMinHVESSTemp = ((uint16_t)buffer[5] << 8) | ((uint16_t)buffer[4]);
      printf("EvMinHVESSTemp %.3lf C\n", ((double)(act_data_rcv[ch]->v).EvMinHVESSTemp * 0.031250) - 273);
      (act_data_rcv[ch]->v).EvHVESSElecTemp = ((uint8_t)buffer[6]);
      printf("EvHVESSElecTemp %d C\n", ((act_data_rcv[ch]->v).EvHVESSElecTemp) - 40);
      break;
    case 109:
      (act_data_rcv[ch]->v).EvMaxHVESSCellVolt = ((uint16_t)buffer[1] << 8) | ((uint16_t)buffer[0]);
      printf("EvMaxHVESSCellVolt %.3lf V\n", ((double)(act_data_rcv[ch]->v).EvMaxHVESSCellVolt * 0.001000));
      (act_data_rcv[ch]->v).EvMinHVESSCellVolt = ((uint16_t)buffer[3] << 8) | ((uint16_t)buffer[2]);
      printf("EvMinHVESSCellVolt %.3lf V\n", ((double)(act_data_rcv[ch]->v).EvMinHVESSCellVolt * 0.001000));
      (act_data_rcv[ch]->v).EvNumHVESSCellBalancing = ((uint16_t)buffer[5] << 8) | ((uint16_t)buffer[4]);
      printf("EvNumHVESSCellBalancing %d \n", ((act_data_rcv[ch]->v).EvNumHVESSCellBalancing));
      (act_data_rcv[ch]->v).EvStatusCellVoltDiff = ((buffer[6] >> 0) & 0b00001111);
      printf("EvStatusCellVoltDiff 0x%02X Enum\n", ((act_data_rcv[ch]->v).EvStatusCellVoltDiff));
      (act_data_rcv[ch]->v).EvStatusCellBal = ((buffer[6] >> 4) & 0b00000011);
      printf("EvStatusCellBal 0x%02X Enum\n", ((act_data_rcv[ch]->v).EvStatusCellBal));
      (act_data_rcv[ch]->v).EvActiveCellBal = ((buffer[6] >> 6) & 0b00000011);
      printf("EvActiveCellBal 0x%02X Enum\n", ((act_data_rcv[ch]->v).EvActiveCellBal));
      break;
    case 110:
      (act_data_rcv[ch]->v).EvChargerTemp = ((uint16_t)buffer[1] << 8) | ((uint16_t)buffer[0]);
      printf("EvChargerTemp %.3lf C\n", ((double)(act_data_rcv[ch]->v).EvChargerTemp * 0.031250) - 273);
      (act_data_rcv[ch]->v).EvMaxChargerTemp = ((uint16_t)buffer[3] << 8) | ((uint16_t)buffer[2]);
      printf("EvMaxChargerTemp %.3lf C\n", ((double)(act_data_rcv[ch]->v).EvMaxChargerTemp * 0.031250) - 273);
      (act_data_rcv[ch]->v).EvInletTemp = ((uint8_t)buffer[4]);
      printf("EvInletTemp %d C\n", ((act_data_rcv[ch]->v).EvInletTemp) - 40);
      (act_data_rcv[ch]->v).EvHVESSTemp = ((uint8_t)buffer[5]);
      printf("EvHVESSTemp %d C\n", ((act_data_rcv[ch]->v).EvHVESSTemp) - 40);
      break;
  }
  ///<se_data_parse

  if (dest_ptr != NULL) {
    copy_buffer_data(source_ptr, dest_ptr, copy_size, is_string);
  }

  if ((pgs_read[ch] == pgs_to_read[ch]) && (LR(ch, l_u8, SeIDPage) == last_pg[ch]))  {
    temp_ptr = inact_data_rcv[ch];           //swap data rcv buffers
    inact_data_rcv[ch] = act_data_rcv[ch];
    act_data_rcv[ch] = temp_ptr;
    clear_data_rcv_buff(act_data_rcv[ch]);   //clear new buffer
  }
}
#endif
