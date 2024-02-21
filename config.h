#ifndef _CONFIG_H
#define _CONFIG_H

#define CH_A_ONLY
//Behavior is undefined if neither LIN nor PWM are enabled
#define PWM_CHARGE_ENABLE
#define LIN_CHARGE_ENABLE

//#define SINGLE_PHASE
//#define TYPE_I_COUPLER              //As of now, this does the same thing as SINGLE_PHASE + uses Type I prox levels

#define INITIALIZATION_DELAY 1
#define RTI_TICKS_PER_SECOND 1000ul  //1000/1.08ms  This still runs long for some reason  FIXME?
#define LOCK_CHECK_DELAY     250

//EV
#define CONNECTION_CLEAR_TIME 2*60*RTI_TICKS_PER_SECOND   //Time between connections to assume a new connection is not a restart
#define RAMPDOWN_TIME         2*RTI_TICKS_PER_SECOND      //Time to wait for inverter to stop drawing current before requesting disconnect in non-emergency situation
#define VMU_WAKEUP_TIME       60*RTI_TICKS_PER_SECOND     //Time to wait for ECU to wake up and request charge before deasserting wakeup signal

//Supported Protocol Versions.  2 is always supported, 0 is controlled by PWM_CHARGE_ENABLE
//Order controls preference for EV, 2 is always last
#define PVER_PWM       0
#define PVER_BASE      2
#define PVER_SLASH_1   3
#define PVER_SLASH_1P  103
#define PVER_SLASH_2   252
#define PVER_TEST_A    4
#define PVER_TEST_B    5
#define PVER_TEST_C    6
#define PVER_TEST_D    7
#define PVER_TEST_E    8
#define PVER_TEST_F    9
#define PVER_TEST_G    10
#define PVER_TEST_H    11

//#define SUPPORTED_PVERSIONS  PVER_SLASH_2, PVER_TEST_H, PVER_TEST_G, PVER_TEST_E, PVER_TEST_F, PVER_TEST_A, PVER_TEST_B, PVER_TEST_C, PVER_TEST_D, PVER_SLASH_1,
#define SUPPORTED_PVERSIONS  PVER_SLASH_1,
//#define SUPPORTED_PVERSIONS

//Connection Types
#define CONNTP_EV_CASE_A      0
#define CONNTP_SE_OUTLET      0
#define CONNTP_TYPE_1         1
#define CONNTP_TYPE_2         2
#define CONNTP_CCS_1          3
#define CONNTP_CCS_2          4
#define CONNTP_3400           5
#define CONNTP_GB_T           6
#define CONNTP_ERROR          0xFE
#define CONNTP_NOTSPECIFIED   0xFF

/*<Ratings*/
//Voltages are *10, Currents are *1

//SE Settings
#define SE_FREQUENCY SixtyHz
#define SE_NOMINAL_VOLTAGE_LL 2080
#define SE_NOMINAL_VOLTAGE_LN 1200
#define SE_MAX_CURRENT_L 16
#define SE_MAX_CURRENT_N 16
#define SE_CONNTP CONNTP_TYPE_2

//EV settings
#define EV_FREQUENCIES FiftyOrSixtyHz
#define EV_MAX_VOLTAGE_LL 4800
#define EV_MAX_VOLTAGE_LN 2770
#define EV_MIN_VOLTAGE_LL 2080
#define EV_MIN_VOLTAGE_LN 1200
#define EV_MAX_CURRENT_L 32
#define EV_MAX_CURRENT_N 32
#define EV_MIN_CURRENT_L 0
#define EV_CONNTP CONNTP_TYPE_2

/****<DO NOT MODIFY******/
#if defined(SINGLE_PHASE) || defined(TYPE_I_COUPLER)
  #undef  SE_NOMINAL_VOLTAGE_LL
  #undef  EV_MAX_VOLTAGE_LL
  #undef  EV_MIN_VOLTAGE_LL
  #define SE_NOMINAL_VOLTAGE_LL 0xFFFF
  #define EV_MAX_VOLTAGE_LL 0xFFFF
  #define EV_MIN_VOLTAGE_LL 0xFFFF
#endif
/****DO NOT MODIFY>******/ 
/*Ratings>*/

/*<emulator specific*/
#define CYCLE_TIME  20  //ms
/*emulator specific>*/

#endif
