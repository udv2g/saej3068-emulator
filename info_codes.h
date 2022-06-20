#ifndef _INFO_CODES_H                                              
#define _INFO_CODES_H                                              
                                                                   
#define EVREASONCODE_UNSPECIFIED                        0x00       //Unspecified reason or other reason                                                                                                                                   
#define EVREASONCODE_STSTATUSOP_NOT_READY               0x01       //Charging station is signalling StStatusOp = not_ready. For further information see which StReasonCode signals are set.                                               
#define EVREASONCODE_EV_ENERGY_MANAGEMENT_FAULT         0x02       //Energy management reason at EV, e.g. battery is full or charging is scheduled to start at a later point in time. Simultaneously, EvRequestedCurrent will be set to 0.
#define EVREASONCODE_USER_STOP                          0x03       //Charging stopped by user, e.g. S3 button or unlock button pressed.                                                                                                   
#define EVREASONCODE_MAXIMUM_CURRENT_TOO_LOW            0x04       //Maximum allowable current too low.                                                                                                                                   
#define EVINFOENTRY_VERSION_FAILS                       0x11       //EV determines that Version selection fails 10.3.1                                                                                                                    
#define EVINFOENTRY_INITIALIZATION_FAILS                0x12       //EV determines that initialization fails 10.4.1                                                                                                                       
#define EVINFOENTRY_VERSION_RESTART                     0x13       //EV determines to restart the Control Sequence to reselect version 10.2.1                                                                                             
#define EVINFOENTRY_BIT_RATE_RESTART                    0x14       //EV determines to restart the Control Sequence to request a change of bit rate 10.2.1                                                                                 
#define EVINFOENTRY_MCU_RESET_RESTART                   0x15       //EV needs to restart the Control Sequence after an MCU reset 10.6.2                                                                                                   
#define EVINFOENTRY_PILOT_FAULT_RESTART                 0x16       //EV needs to restart the Control Sequence after detecting CP Level 0 while the proximity circuit is connected (e.g., SE 10.5 control power outage)                    
#define EVINFOENTRY_NO_LIN                              0x17       //EV detects no LIN Headers for longer than T noLIN 10.7.1                                                                                                             
#define EVINFOENTRY_INT_FAULT_RESTART                   0x18       //EV determines to restart the Control Sequence after detecting a recoverable internal fault 10.7.2                                                                    
#define EVINFOENTRY_INT_FAULT_TERMINATION               0x19       //EV will terminate charging, an unrecoverable internal fault was detected 10.8.1                                                                                      
#define EVINFOENTRY_AVAILABLE_CURRENT_TOO_LOW           0x1A       //Maximum available current is too low 10.4.1                                                                                                                          
#define EVINFOENTRY_AVAILABLE_VOLTAGE_TOO_HIGH          0x1B       //Minimum available voltage is too high 10.4.1                                                                                                                         
#define EVINFOENTRY_FREQUENCY_MISMATCH                  0x1C       //Frequency does not match 10.4.1                                                                                                                                      
#define EVINFOENTRY_EMS_CHARGING_DELAYED                0x1D       //Charging delayed due to energy management system 9.7.2.1                                                                                                             
#define EVINFOENTRY_ISOLATION_FAULT                     0x1E       //EV isolation fault                                                                                                                                                   
#define EVINFOENTRY_CHARGING_DELAYED                    0x1F       //EV is not immediately ready to receive AC voltage 9.7.2.1                                                                                                            
#define EVINFOENTRY_INTERRUPT_CLEAN                     0x20       //EV requests the AC supply voltage to be interrupted by reducing the load current to < 1A, writing EvStatusOp = Deny_V and opening S2 9.7.4                           
#define EVINFOENTRY_INTERRUPT_DIRTY                     0x21       //EV requests the AC supply voltage to be interrupted by immediately opening S2 9.7.4                                                                                  
#define EVINFOENTRY_AVAILABLE_VOLTAGE_TOO_LOW           0x22       //Maximum available voltage is too low                                                                                                                                 
#define EVINFOENTRY_EMPTY                               0xFF       //Empty                                                                                                                                                                
                                                                   
#define MAX_EVINFOENTRIES                               24         //Maximum space required to store all EvInfo codes rounded up to the page size (6)                                                                                     
                                                                   
                                                                   
#define STREASONCODE_UNSPECIFIED                        0x00       //Unspecified reason or other reason                                                                                                                                   
#define STREASONCODE_SLEEP                              0x01       //EV is requesting to go to sleep (EvAwake = 0).                                                                                                                       
#define STREASONCODE_ENERGY_UNAVAILABLE                 0x02       //Energy management reason. Simultaneously, all StAllowableCurrent signals will be set to 0. Station will become available again automatically after some time.        
#define STREASONCODE_USER_STOP                          0x03       //Charging stopped by user, e.g. stop button on charging station pressed.                                                                                              
#define SEINFOENTRY_VERSION_FAILS                       0x11       //SE determines that Version selection fails 10.3.2                                                                                                                    
#define SEINFOENTRY_INITIALIZATION_FAILS                0x12       //SE determines that initialization fails 10.4.2                                                                                                                       
#define SEINFOENTRY_VERSION_RESTART                     0x13       //SE determines to restart the Control Sequence to reselect version 10.2.2                                                                                             
#define SEINFOENTRY_BIT_RATE_RESTART                    0x14       //SE determines to restart the Control Sequence to request a change of bit rate 10.2.2                                                                                 
#define SEINFOENTRY_MCU_RESET_RESTART                   0x15       //SE needs to restart the Control Sequence after an MCU reset 10.6.1                                                                                                   
#define SEINFOENTRY_POWER_FAULT_RESTART                 0x16       //SE needs to restart the Control Sequence after a control power outage 10.5                                                                                           
#define SEINFOENTRY_NO_LIN                              0x17       //SE detects no LIN Responses for longer than T noLIN 10.7.2                                                                                                           
#define SEINFOENTRY_INT_FAULT_RESTART                   0x18       //SE needs to restart the Control Sequence after detecting an internal fault 10.8.2                                                                                    
#define SEINFOENTRY_EMS_CHARGING_DELAYED                0x19       //Charging delayed due to energy management system 9.7.2.2                                                                                                             
#define SEINFOENTRY_USER_STOP                           0x1A       //Charging stopped by user, for example the stop button on charging station pressed 1B 16 Maximum available current is too low (SeMaxCurrents < EvMinCurrents) 10.4.2  
#define SEINFOENTRY_AVAILABLE_CURRENT_TOO_LOW           0x1B       //Maximum available current is too low (SeMaxCurrents < EvMinCurrents) 10.4.2                                                                                          
#define SEINFOENTRY_AVAILABLE_VOLTAGE_TOO_HIGH          0x1C       //Minimum available voltage is too high (lowest SeNomVoltage > EvMaxVoltage) 10.4.2                                                                                    
#define SEINFOENTRY_FREQUENCY_MISMATCH                  0x1D       //Frequency does not match 10.4.2                                                                                                                                      
#define SEINFOENTRY_INIT_TIMEOUT                        0x1E       //Initialization timeout at charging station 10.4.2                                                                                                                    
#define SEINFOENTRY_CONNECTOR_OVER_TEMP                 0x1F       //Over-temperature in Connector                                                                                                                                        
#define SEINFOENTRY_OVER_TEMP                           0x20       //Over-temperature internally                                                                                                                                          
#define SEINFOENTRY_TEMP_SENSE_INVALID                  0x21       //Temperature sensor irrational                                                                                                                                        
#define SEINFOENTRY_OVERCURRENT                         0x22       //Overcurrent (EV load current is too high)                                                                                                                            
#define SEINFOENTRY_CURRENT_SENSE_INVALID               0x23       //Current sensor irrational                                                                                                                                            
#define SEINFOENTRY_VOLTAGE_SENSE_INVALID               0x24       //Voltage sensor irrational                                                                                                                                            
#define SEINFOENTRY_PILOT_FAULT                         0x25       //Pilot voltage fault                                                                                                                                                  
#define SEINFOENTRY_CONTACTOR_FAULT                     0x26       //Contactor fault                                                                                                                                                      
#define SEINFOENTRY_INPUT_MISWIRED                      0x27       //Input miswired                                                                                                                                                       
#define SEINFOENTRY_INPUT_OVER_VOLTAGE                  0x28       //Input over voltage                                                                                                                                                   
#define SEINFOENTRY_INPUT_UNDER_VOLTAGE                 0x29       //Input under voltage                                                                                                                                                  
#define SEINFOENTRY_CCID_SELF_TEST_FAULT                0x2A       //CCID self-test fault                                                                                                                                                 
#define SEINFOENTRY_CCID_TRIPPED_RETRY                  0x2B       //CCID tripped -- retry allowed                                                                                                                                        
#define SEINFOENTRY_CCID_TRIPPED_NO_RETRY               0x2C       //CCID tripped -- retry limit exceeded                                                                                                                                 
#define SEINFOENTRY_BREAKER_TRIPPED                     0x2D       //Breaker tripped                                                                                                                                                      
#define SEINFOENTRY_GROUND_MONITOR_FAULT                0x2E       //Ground monitor circuit fault                                                                                                                                         
#define SEINFOENTRY_CONFIGURATION_ERROR                 0x2F       //Station configuration error                                                                                                                                          
#define SEINFOENTRY_GROUND_FAULT                        0x30       //Improper grounding or Ground is not present                                                                                                                          
#define SEINFOENTRY_COMM_FAULT                          0x31       //Problem with EV communications -- Disconnect and restart                                                                                                             
#define SEINFOENTRY_INTERNAL_FAULT                      0x32       //Internal EVSE fault -- Call for service                                                                                                                              
#define SEINFOENTRY_AVAILABLE_VOLTAGE_TOO_LOW           0x33       //Maximum available voltage is too low                                                                                                                                 
#define SEINFOENTRY_EMPTY                               0xFF       //Empty                                                                                                                                                                
                                                                   
#define MAX_SEINFOENTRIES                               42         //Maximum space required to store all SeInfo codes rounded up to the page size (6)                                                                                     
                                                                   
#endif                                                             