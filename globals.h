#ifndef __GLOBALS_H_
#define __GLOBALS_H_

#include <stdint.h>
#include <stdbool.h>
#include "lin-cp.h"

struct evse_state_t {
  uint8_t number_heads;

  three_phase_currents_t min_c;
  three_phase_currents_t max_c;
  three_phase_currents_t default_c;
  three_phase_currents_t set_c;
  uint8_t overCurrentTrip;

  uint8_t min_d;
  uint8_t max_d;
  uint8_t default_d;
  uint8_t set_d;

  uint8_t reversePermitted;
  uint8_t controlProhibited;
  uint8_t hosePortUpdate;

  uint8_t command_not_received_count;
  uint16_t nominal_voltage;

  uint8_t duty;
  uint8_t ch;
};

/*********<EVSE**********/
extern struct evse_state_t evse_state[];
/*********EVSE>**********/

/*********<LIN**********/
extern uint8_t LINPermitVoltage[2];

extern three_phase_currents_t SeAvailAmps[2];
extern l_u8 SeStatusVer[2], SeStatusInit[2], SeStatusOp[2]; // 3 = Not Available

extern l_u16 SeNomVoltsLN[2], SeNomVoltsLL[2];
extern l_u8 SeFrequency[2], EvFrequencies[2];
/*********LIN>**********/

/*********<SLASH1**********/
extern uint8_t ev_id_status[2];
extern uint8_t se_id_status[2];
/*********SLASH1>**********/

/*********<SLASH2**********/
extern uint8_t ev_j3072_status[2];
extern uint8_t se_j3072_status[2];
/*********SLASH2>**********/

#endif