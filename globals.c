#include <stdint.h>
#include <stdbool.h>
#include "lin-cp.h"
#include "globals.h"
#include "ids_handler.h"
#include "j3072_handler.h"

/*********<EVSE**********/
struct evse_state_t evse_state[1] = {{ 1, {6, 6, 6, 6, 6}, {80, 80, 80, 80, 80}, {30, 30, 30, 30, 30}, {30, 30, 30, 30, 30}}};
/*********EVSE>**********/

/*********<LIN**********/
uint8_t LINPermitVoltage[2];
three_phase_currents_t SeAvailAmps[2];
l_u8 SeStatusVer[2] = {2, 2}, SeStatusInit[2] = {2, 2}, SeStatusOp[2] = {2, 2}; // 3 = Not Available
l_u16 SeNomVoltsLN[2] = {0xFFFF, 0xFFFF}, SeNomVoltsLL[2] = {0xFFFF, 0xFFFF};   // Not Available
l_u8 SeFrequency[2] = {254, 254}, EvFrequencies[2] = {254, 254};                // 2 is 60 Hz
/*********LIN>**********/

/*********<SLASH1**********/
uint8_t ev_id_status[2] = {ID_INCOMPLETE, ID_INCOMPLETE};
uint8_t se_id_status[2] = {ID_INCOMPLETE, ID_INCOMPLETE};
/*********SLASH1>**********/

/*********<SLASH2**********/
uint8_t ev_j3072_status[2] = {CERT_INCOMPLETE, CERT_INCOMPLETE};
uint8_t se_j3072_status[2] = {CERT_INCOMPLETE, CERT_INCOMPLETE};
/*********SLASH2>**********/