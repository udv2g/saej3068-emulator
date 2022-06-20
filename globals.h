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
extern struct evse_state_t evse_stateA;
#define evse_state_S(ch) evse_stateA
/*********EVSE>**********/

/*********<LIN**********/
extern uint8_t LINPermitVoltage[2];

extern three_phase_currents_t SeAvailAmps[2];
extern l_u8 SeStatusVer[2], SeStatusInit[2], SeStatusOp[2]; // 3 = Not Available

extern l_u16 SeNomVoltsLN[2], SeNomVoltsLL[2];
extern l_u8 SeFrequency[2], EvFrequencies[2];
/*********LIN>**********/

#endif