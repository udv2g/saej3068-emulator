#include "lin-cp.h"
#include "globals.h"
#include "ids_handler.h"
#include <time.h>

uint8_t se_id_status = ID_INCOMPLETE;
uint8_t ev_id_status = ID_INCOMPLETE;

int32_t ev_odometer;

uint32_t ev_energy_discharge_capacity;
uint32_t ev_energy_charge_capacity;
uint32_t ev_energy_for_departure;
uint16_t ev_time_to_departure;

uint16_t ev_battery_range;
uint16_t ev_fuel_range;
int16_t ev_battery_dc_power;
uint8_t ev_battery_health;

int16_t ev_ambient_temperature;
int16_t ev_cabin_temperature;

int16_t ev_battery_temperature;
int16_t ev_max_battery_temperature;
int16_t ev_min_battery_temperature;

uint16_t ev_max_watt_charge;
uint16_t ev_max_watt_discharge;

uint16_t ev_max_var_supply;
uint16_t ev_max_var_absorb;

int16_t se_trip_elevation;

//THIS IS ONLY  FOR TESTING
void __ids_init() {
  static bool once = false;
  if (once) return;

  srand((unsigned) time(NULL));   //seed random number generator
  //RAND_MAX = 0x7FFF FFFF

#ifdef EV_CONFIG
  //EvOdometer               97    4 bytes   0.125 km/bit                     00       to 526,385,151.875     km
  ev_odometer = rand();
  printf("EvOdometer %.3lfkm\n", (float)ev_odometer * 0.125);

  //EvEnergyDishargeCapacity 98    3 bytes   0.001 kWh per bit                00       to 16449.535 kWh       kWh
  ev_energy_discharge_capacity = rand() & 0xFFFFF;
  //EvEnergyChargeCapacity   98    3 bytes   0.001 kWh per bit                00       to 16449.535 kWh       kWh
  ev_energy_charge_capacity = (rand() & 0xFFFFF);
  printf("Energy Capacity, Discharge: %.3lf kWh, Charge: %.3lfkWh\n", (double) ev_energy_discharge_capacity * 0.001, (double) ev_energy_charge_capacity * 0.001);

  //EvEnergyForDeparture     99    3 bytes   0.001 kWh per bit                00       to 16449.535 kWh       kWh
  ev_energy_for_departure = (rand() & 0xFFFFF);
  printf("Energy for Departure: %.3lfkWh\n", (double) ev_energy_for_departure * 0.001);
  //EvTimeToDeparture        99    2 bytes   1 min/bit                        00       to 64,255 min          min
  ev_time_to_departure = (uint16_t)(rand() & 0xFFFF);
  printf("EvTimeToDeparture %d minutes\n", ev_time_to_departure);

  //EvBatteryRange          100    2 bytes   1 km/bit                         00       to 64,255 km           km
  ev_battery_range = (uint16_t)(rand() & 0xFFFF);
  //EvFuelRange             100    2 bytes   1 km/bit                         00       to 64,255 km           km
  ev_fuel_range = (uint16_t)(rand() & 0xFFFF);
  printf("Range, Battery: %dkm, Fuel: %dkm\n", ev_battery_range, ev_fuel_range);
  //EvBatteryDCPower        100    2 bytes   20 W/bit          -650,000 W     -650,000 to 660,700             WW
  ev_battery_dc_power = (int16_t)(rand() & 0xFFFF);
  printf("Battery DC Power: %dW\n", (int32_t) (ev_battery_dc_power * 20));
  //EvBatteryHealth         100    1 bytes   0.4 %/bit                        00       to 100 %               %
  ev_battery_health = (uint8_t)(rand() & 0xFF);
  printf("Battery Health %.1f%%\n", ev_battery_health * 0.4);

  //EvAmbientTemp           101    2 bytes   0.03125 °C/bit    -273 °C        -273     to 1734.96875 °C       °C
  ev_ambient_temperature = (int16_t)(rand() & 0xFFFF);  if (ev_ambient_temperature < -(273 * 32))  {ev_ambient_temperature = 0;}
  //EvCabinTemp             101    2 bytes   0.03125 °C/bit    -273 °C        -273     to 1734.96875 °C       °C
  ev_cabin_temperature = (int16_t)(rand() & 0xFFFF);  if (ev_cabin_temperature < -(273 * 32))  {ev_cabin_temperature = 0;}
  printf("Environment Temperatures, Ambient: %.3lf°C, Cabin %.3lf°C\n", (double)(ev_ambient_temperature * 0.03125), (double)(ev_cabin_temperature *0.03125));

  //EvBatteryTemp           102    2 bytes   0.03125 °C/bit    -273 °C        -273     to 1734.96875 °C       °C
  ev_battery_temperature = (int16_t)(rand() & 0xFFFF);  if (ev_battery_temperature < -(273 * 32))  {ev_battery_temperature = 0;}
  //EvMaxBatteryTemp        102    2 bytes   0.03125 °C/bit    -273 °C        -273     to 1734.96875 °C       °C
  ev_max_battery_temperature = ev_battery_temperature + (uint16_t)(rand() & 0xFF);
  //EvMinBatteryTemp        102    2 bytes   0.03125 °C/bit    -273 °C        -273     to 1734.96875 °C       °C
  ev_min_battery_temperature = ev_battery_temperature - (uint16_t)(rand() & 0xFF);  if (ev_min_battery_temperature < -(273 * 32))  {ev_min_battery_temperature = 0;}
  printf("Battery Temperatures, Ave: %.3lf°C, Max: %.3lf°C, Min %.3lf°C\n", (double)(ev_battery_temperature * 0.03125), (double)(ev_max_battery_temperature * 0.03125), (double)(ev_min_battery_temperature * 0.03125));

  //EvMaxWattCharge         103    2 bytes   20 W/bit          -650,000 W     -650,000 to 660,700 W           W
  ev_max_watt_charge = (int16_t)(rand() & 0xFFFF);
  //EvMaxWattDischarge      103    2 bytes   20 W/bit          -650,000 W     -650,000 to 660,700 W           W
  ev_max_watt_discharge = (int16_t)(rand() & 0xFFFF);
  printf("Power Limits, Charge: %dW, Discharge: %dW\n", (int32_t) (ev_max_watt_charge * 20), (int32_t) (ev_max_watt_discharge * 20));

  //EvMaxVarSupply          104    2 bytes   20 VA/bit         -650,000 VA    -650,000 to 660,700 VA          VA
  ev_max_var_supply = (int16_t)(rand() & 0xFFFF);
  //EvMaxVarAbsorb          104    2 bytes   20 VA/bit         -650,000 VA    -650,000 to 660,700 VA          VA
  ev_max_var_absorb = (int16_t)(rand() & 0xFFFF);
  printf("Reactive Power Limits, Supply: %dVA, Absorb: %dVA\n", (int32_t) (ev_max_var_supply * 20), (int32_t) (ev_max_var_absorb * 20));

#else
  //SeTripElevation         97     2 bytes   0.125 m/bit       -4000 m        -4000    to 4031.875 m          m
  se_trip_elevation = (int16_t)(rand() & 0xFFFF);
  printf("SeTripElevation %lfm\n", (double) (se_trip_elevation * 0.125));
#endif

  printf("======================================================================\n");
  once = true;
}

static bool first_data = false;

#ifdef EV_CONFIG
#define EVSEID_SIZE 42
#define MANUFACTURER_SIZE 43
char evseid[EVSEID_SIZE] = {0xFF};
char manufacturer[MANUFACTURER_SIZE] = {0xFF};

char * vin = "VINgoesHere234567";
char * emaid = "EMAIDgoesHere45678";
char * driver = "Driver7";
char * vehicle = "Vehicle";
#else
#define VIN_SIZE 21
#define EMAID_SIZE 21
#define DRIVER_VEHICLE_SIZE 8
char vin[VIN_SIZE] = {0xFF};    //must be a multiple of 7 or 1 greater to leave space for \0
char emaid[EMAID_SIZE] = {0xFF};
char driver[DRIVER_VEHICLE_SIZE] = {0xFF};
char vehicle[DRIVER_VEHICLE_SIZE] = {0xFF};

char * evseid = "EVSEIDgoesHere56789012345678901234567890";
char * manufacturer = "ManufacturerGoesHere1234567890123456789012";
#endif


void copy_page(uint8_t * source, uint8_t * destination, bool string) {
  int i;

  for(i=0; i<7; i++) {
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

void ids_init() {
  se_id_status = ID_INCOMPLETE;
  ev_id_status = ID_INCOMPLETE;
  first_data= false;

  __ids_init();

#ifdef EV_CONFIG
  mem_init(evseid, EVSEID_SIZE-1, 0xFF);
  evseid[EVSEID_SIZE-1] = '\0';
  mem_init(manufacturer, MANUFACTURER_SIZE-1, 0xFF);
  manufacturer[MANUFACTURER_SIZE-1] = '\0';
#else
  mem_init(vin, VIN_SIZE-1, 0xFF);
  vin[VIN_SIZE-1] = '\0';
  mem_init(emaid, EMAID_SIZE-1, 0xFF);
  emaid[EMAID_SIZE-1] = '\0';
  mem_init(driver, DRIVER_VEHICLE_SIZE-1, 0xFF);
  driver[DRIVER_VEHICLE_SIZE-1] = '\0';
  mem_init(vehicle, DRIVER_VEHICLE_SIZE-1, 0xFF);
  vehicle[DRIVER_VEHICLE_SIZE-1] = '\0';
#endif
}

#ifdef EV_CONFIG

#define MY_ID_STATUS               ev_id_status
#define THEIR_ID_STATUS            se_id_status
#define READ_MY_PAGE_NUMBER()      l_u8_rd_LI0_EvIDPageNumber()
#define READ_THEIR_PAGE_NUMBER()   l_u8_rd_LI0_SeIDPageNumber()
#define READ_THEIR_BYTE_A()        l_u8_rd_LI0_SeIDByteA()
#define MY_MAX_ID_PAGE             8
#define MY_MAX_DATA_PAGE           104

#else

#define MY_ID_STATUS               se_id_status
#define THEIR_ID_STATUS            ev_id_status
#define READ_MY_PAGE_NUMBER()      l_u8_rd_LI0_SeIDPageNumber()
#define READ_THEIR_PAGE_NUMBER()   l_u8_rd_LI0_EvIDPageNumber()
#define READ_THEIR_BYTE_A()        l_u8_rd_LI0_EvIDByteA()
#define MY_MAX_ID_PAGE             12
#define MY_MAX_DATA_PAGE           97

#endif

void on_id_frame_receipt()  {
  if (READ_THEIR_PAGE_NUMBER() == 0)  {   //make sure we are always reading the status
    THEIR_ID_STATUS = READ_THEIR_BYTE_A();
  }

  if (MY_ID_STATUS == ID_INCOMPLETE && id_parse()) {
    MY_ID_STATUS = ID_COMPLETE;

#ifdef EV_CONFIG
    PrintConsoleString("---EVSEID:",0);
    PrintConsoleString(evseid,0);
    PrintConsoleString("\r\n",0);
    PrintConsoleString("---Manufacturer:",0);
    PrintConsoleString(manufacturer,0);
    PrintConsoleString("\r\n",0);
#else
    PrintConsoleString("---VIN:", 0);
    PrintConsoleString(vin, 0);
    PrintConsoleString(", EMAID:", 0);
    PrintConsoleString(emaid, 0);
    PrintConsoleString(", Drv:", 0);
    PrintConsoleString(driver, 0);
    PrintConsoleString(", Veh:", 0);
    PrintConsoleString(vehicle, 0);
    PrintConsoleString("\r\n", 0);
#endif
  }

  if (MY_ID_STATUS == ID_COMPLETE && (THEIR_ID_STATUS == ID_COMPLETE || THEIR_ID_STATUS == DATA))  {
    PrintConsoleString("====ID COMPLETE!!!!===\r\n",0);
    MY_ID_STATUS = DATA;
    first_data = true;
  }

  if (MY_ID_STATUS == DATA ) {
    if (THEIR_ID_STATUS == DATA)  {
      data_parse();
    }
  }
}

void on_id_frame_xmit() {
  if (MY_ID_STATUS == ID_INCOMPLETE || MY_ID_STATUS == ID_COMPLETE) {
    if (READ_MY_PAGE_NUMBER() == MY_MAX_ID_PAGE)  {
      id_xmit(0);
    } else  {
      id_xmit(READ_MY_PAGE_NUMBER() + 1);
    }
  }

  if (MY_ID_STATUS == DATA ) {
    if (THEIR_ID_STATUS != DATA || first_data)  {
      data_xmit(0);    //repeatedly send page 0 until other side is also in data
      first_data = false;
    } else  {
      //printf("Success!\n");
      if (READ_MY_PAGE_NUMBER() == MY_MAX_DATA_PAGE)  {
        data_xmit(0);
      } else  {
        if (READ_MY_PAGE_NUMBER() == 0)  {
          data_xmit(97);
        } else  {
          data_xmit(READ_MY_PAGE_NUMBER() + 1);
        }
      }
    }
  }
}

#ifdef EV_CONFIG

bool id_parse() {
  static uint8_t buffer[7];
  static uint8_t pgs_to_read = 255;
  static uint8_t pgs_read = 0;
  static uint8_t last_pg = 255;

  uint8_t * dest_ptr = NULL;

  buffer[0] = l_u8_rd_LI0_SeIDByteA();
  buffer[1] = l_u8_rd_LI0_SeIDByteB();
  buffer[2] = l_u8_rd_LI0_SeIDByteC();
  buffer[3] = l_u8_rd_LI0_SeIDByteD();
  buffer[4] = l_u8_rd_LI0_SeIDByteE();
  buffer[5] = l_u8_rd_LI0_SeIDByteF();
  buffer[6] = l_u8_rd_LI0_SeIDByteG();

  //printf("-->%c%c\n", buffer[0], buffer[1]);
  //printf("page:%d\n", l_u8_rd_LI0_EvIDPageNumber());

  pgs_read++;

  switch(l_u8_rd_LI0_SeIDPageNumber()) {
    case 0:
      pgs_to_read = buffer[1];
      se_id_status = buffer[0];
      last_pg = buffer[3];
      pgs_read = 1;
      break;
    case 1:
      dest_ptr = (uint8_t *) evseid;
      break;
    case 2:
      dest_ptr = (uint8_t *) evseid + 7;
      break;
    case 3:
      dest_ptr = (uint8_t *) evseid + 14;
      break;
    case 4:
      dest_ptr = (uint8_t *) evseid + 21;
      break;
    case 5:
      dest_ptr = (uint8_t *) evseid + 28;
      break;
    case 6:
      dest_ptr = (uint8_t *) evseid + 35;
      break;
    case 7:
      dest_ptr = (uint8_t *) manufacturer;
      break;
    case 8:
      dest_ptr = (uint8_t *) manufacturer + 7;
      break;
    case 9:
      dest_ptr = (uint8_t *) manufacturer + 14;
      break;
    case 10:
      dest_ptr = (uint8_t *) manufacturer + 21;
      break;
    case 11:
      dest_ptr = (uint8_t *) manufacturer + 28;
      break;
    case 12:
      dest_ptr = (uint8_t *) manufacturer + 35;
  }

  //printf("pointer:%p\n",dest_ptr);

  //fflush(stdout);

  if (dest_ptr != NULL) {
    copy_page(buffer, dest_ptr, true);
  }

  if ((pgs_read == pgs_to_read) && (l_u8_rd_LI0_SeIDPageNumber() == last_pg))  {
    return true;
  } else  {
    return false;
  }
}

void id_xmit(uint8_t page){
  static uint8_t buffer[7];
  uint8_t mask = 0;

  uint8_t * source_ptr = NULL;

  switch(page) {
    case 0:
      buffer[0] = ev_id_status;
      buffer[1] = 9;  //pages to be sent
      buffer[2] = 1;  //first (non zero) page
      buffer[3] = 8;  //last page
      mask = 0b0000111;
      break;
    case 1:
      source_ptr = (uint8_t *) vin;
      break;
    case 2:
      source_ptr = (uint8_t *) vin + 7;
      break;
    case 3:
      source_ptr = (uint8_t *) vin + 14;
      mask = 0b0001111;
      break;
    case 4:
      source_ptr = (uint8_t *) emaid;
      break;
    case 5:
      source_ptr = (uint8_t *) emaid + 7;
      break;
    case 6:
      source_ptr = (uint8_t *) emaid + 14;
      mask = 0b0000111;
      break;
    case 7:
      source_ptr = (uint8_t *) driver;
      break;
    case 8:
      source_ptr = (uint8_t *) vehicle;
  }


  if (source_ptr != NULL) {
    copy_page(source_ptr, buffer, false);
  }

  invalidate_bytes_page(buffer, mask);

  l_u8_wr_LI0_EvIDPageNumber(page);
  l_u8_wr_LI0_EvIDByteA(buffer[0]);
  l_u8_wr_LI0_EvIDByteB(buffer[1]);
  l_u8_wr_LI0_EvIDByteC(buffer[2]);
  l_u8_wr_LI0_EvIDByteD(buffer[3]);
  l_u8_wr_LI0_EvIDByteE(buffer[4]);
  l_u8_wr_LI0_EvIDByteF(buffer[5]);
  l_u8_wr_LI0_EvIDByteG(buffer[6]);
}

void data_xmit(uint8_t page) {
  static uint8_t buffer[7];
  uint8_t mask = 0;
  int32_t temp;

  switch(page)  {
    case 0:
      buffer[0] = ev_id_status;
      buffer[1] = 9;  //pages to be sent
      buffer[2] = 97;  //first (non zero) page
      buffer[3] = 104;  //last page
      mask = 0b0000111;
      break;
    case 97:
      buffer[0] = (ev_odometer) & 0xFF;
      buffer[1] = (ev_odometer >> 8) & 0xFF;
      buffer[2] = (ev_odometer >> 16) & 0xFF;
      buffer[3] = (ev_odometer >> 24) & 0xFF;
      mask = 0b0000111;
      break;
    case 98:
      buffer[0] = (ev_energy_discharge_capacity) & 0xFF;
      buffer[1] = (ev_energy_discharge_capacity >> 8) & 0xFF;
      buffer[2] = (ev_energy_discharge_capacity >> 16) & 0xFF;
      buffer[3] = (ev_energy_charge_capacity) & 0xFF;
      buffer[4] = (ev_energy_charge_capacity >> 8) & 0xFF;
      buffer[5] = (ev_energy_charge_capacity >> 16) & 0xFF;
      mask = 0b0000001;
      break;
    case 99:
      buffer[0] = (ev_energy_for_departure) & 0xFF;
      buffer[1] = (ev_energy_for_departure >> 8) & 0xFF;
      buffer[2] = (ev_energy_for_departure >> 16) & 0xFF;
      buffer[3] = (ev_time_to_departure) & 0xFF;
      buffer[4] = (ev_time_to_departure >> 8) & 0xFF;
      mask = 0b0000011;
      break;
    case 100:
      buffer[0] = (ev_battery_range) & 0xFF;
      buffer[1] = (ev_battery_range >> 8) & 0xFF;
      buffer[2] = (ev_fuel_range) & 0xFF;
      buffer[3] = (ev_fuel_range >> 8) & 0xFF;
      temp = ev_battery_dc_power + (650000/20);
      buffer[4] = (temp) & 0xFF;
      buffer[5] = (temp >> 8) & 0xFF;
      buffer[6] = ev_battery_health;
      break;
    case 101:
      temp = ev_ambient_temperature + 273*32;  // 1/0.03125 = 32
      buffer[0] = (temp) & 0xFF;
      buffer[1] = (temp >> 8) & 0xFF;
      temp = ev_cabin_temperature + 273*32;
      buffer[2] = (temp) & 0xFF;
      buffer[3] = (temp >> 8) & 0xFF;
      mask = 0b0000111;
      break;
    case 102:
      temp = ev_battery_temperature + 273*32;  // 1/0.03125 = 32
      buffer[0] = (temp) & 0xFF;
      buffer[1] = (temp >> 8) & 0xFF;
      temp = ev_max_battery_temperature + 273*32;
      buffer[2] = (temp) & 0xFF;
      buffer[3] = (temp >> 8) & 0xFF;
      temp = ev_min_battery_temperature + 273*32;
      buffer[4] = (temp) & 0xFF;
      buffer[5] = (temp >> 8) & 0xFF;
      mask = 0b0000001;
      break;
    case 103:
      temp = ev_max_watt_charge + (650000/20);
      buffer[0] = (temp) & 0xFF;
      buffer[1] = (temp >> 8) & 0xFF;
      temp = ev_max_watt_discharge + (650000/20);
      buffer[2] = (temp) & 0xFF;
      buffer[3] = (temp >> 8) & 0xFF;
      mask = 0b0000111;
      break;
    case 104:
      temp = ev_max_var_supply + (650000/20);
      buffer[0] = (temp) & 0xFF;
      buffer[1] = (temp >> 8) & 0xFF;
      temp = ev_max_var_absorb + (650000/20);
      buffer[2] = (temp) & 0xFF;
      buffer[3] = (temp >> 8) & 0xFF;
      mask = 0b0000111;
      break;
  }

  invalidate_bytes_page(buffer, mask);

  l_u8_wr_LI0_EvIDPageNumber(page);
  l_u8_wr_LI0_EvIDByteA(buffer[0]);
  l_u8_wr_LI0_EvIDByteB(buffer[1]);
  l_u8_wr_LI0_EvIDByteC(buffer[2]);
  l_u8_wr_LI0_EvIDByteD(buffer[3]);
  l_u8_wr_LI0_EvIDByteE(buffer[4]);
  l_u8_wr_LI0_EvIDByteF(buffer[5]);
  l_u8_wr_LI0_EvIDByteG(buffer[6]);
}

void data_parse()  {
  static uint8_t buffer[7];
  static uint8_t pgs_to_read = 255;
  static uint8_t pgs_read = 0;
  int16_t temp;

  buffer[0] = l_u8_rd_LI0_SeIDByteA();
  buffer[1] = l_u8_rd_LI0_SeIDByteB();
  buffer[2] = l_u8_rd_LI0_SeIDByteC();
  buffer[3] = l_u8_rd_LI0_SeIDByteD();
  buffer[4] = l_u8_rd_LI0_SeIDByteE();
  buffer[5] = l_u8_rd_LI0_SeIDByteF();
  buffer[6] = l_u8_rd_LI0_SeIDByteG();

  pgs_read++;

  switch(l_u8_rd_LI0_SeIDPageNumber()) {
    case 0:
      pgs_to_read = buffer[1];
      se_id_status = buffer[0];
      pgs_read = 1;
      break;
    case 97:
      temp = ((uint16_t)buffer[1] << 8) | ((uint16_t)buffer[0]);
      se_trip_elevation = temp - 4000*8;
      printf("SeTripElevation %lfm\n", se_trip_elevation * 0.125);
  }
}
#else
bool id_parse() {
  static uint8_t buffer[7];
  static uint8_t pgs_to_read = 255;
  static uint8_t pgs_read = 0;
  static uint8_t last_pg = 255;

  uint8_t * dest_ptr = NULL;

  buffer[0] = l_u8_rd_LI0_EvIDByteA();
  buffer[1] = l_u8_rd_LI0_EvIDByteB();
  buffer[2] = l_u8_rd_LI0_EvIDByteC();
  buffer[3] = l_u8_rd_LI0_EvIDByteD();
  buffer[4] = l_u8_rd_LI0_EvIDByteE();
  buffer[5] = l_u8_rd_LI0_EvIDByteF();
  buffer[6] = l_u8_rd_LI0_EvIDByteG();

  //printf("-->%c%c\n", buffer[0], buffer[1]);
  //printf("page:%d\n", l_u8_rd_LI0_EvIDPageNumber());

  pgs_read++;

  switch(l_u8_rd_LI0_EvIDPageNumber()) {
    case 0:
      pgs_to_read = buffer[1];
      ev_id_status = buffer[0];
      last_pg = buffer[3];
      pgs_read = 1;
      break;
    case 1:
      dest_ptr = (uint8_t *) vin;
      break;
    case 2:
      dest_ptr = (uint8_t *) vin + 7;
      break;
    case 3:
      dest_ptr = (uint8_t *) vin + 14;
      break;
    case 4:
      dest_ptr = (uint8_t *) emaid;
      break;
    case 5:
      dest_ptr = (uint8_t *) emaid + 7;
      break;
    case 6:
      dest_ptr = (uint8_t *) emaid + 14;
      break;
    case 7:
      dest_ptr = (uint8_t *) driver;
      break;
    case 8:
      dest_ptr = (uint8_t *) vehicle;
  }

  //printf("pointer:%p\n",dest_ptr);

  //fflush(stdout);

  if (dest_ptr != NULL) {
    copy_page(buffer, dest_ptr, true);
  }

  if ((pgs_read == pgs_to_read) && (l_u8_rd_LI0_EvIDPageNumber() == last_pg))  {
    return true;
  } else  {
    return false;
  }
}

void id_xmit(uint8_t page){
  static uint8_t buffer[7];
  uint8_t mask = 0;

  uint8_t * source_ptr = NULL;

  switch(page) {
    case 0:
      buffer[0] = se_id_status;
      buffer[1] = 13; //pages to be sent
      buffer[2] = 1;  //first (non zero) page
      buffer[3] = 12; //last page
      mask = 0b0000111;
      break;
    case 1:
      source_ptr = (uint8_t *) evseid;
      break;
    case 2:
      source_ptr = (uint8_t *) evseid + 7;
      break;
    case 3:
      source_ptr = (uint8_t *) evseid + 14;
      break;
    case 4:
      source_ptr = (uint8_t *) evseid + 21;
      break;
    case 5:
      source_ptr = (uint8_t *) evseid + 28;
      break;
    case 6:
      source_ptr = (uint8_t *) evseid + 35;
      mask = 0b0000011;
      break;
    case 7:
      source_ptr = (uint8_t *) manufacturer;
      break;
    case 8:
      source_ptr = (uint8_t *) manufacturer + 7;
      break;
    case 9:
      source_ptr = (uint8_t *) manufacturer + 14;
      break;
    case 10:
      source_ptr = (uint8_t *) manufacturer + 21;
      break;
    case 11:
      source_ptr = (uint8_t *) manufacturer + 28;
      break;
    case 12:
      source_ptr = (uint8_t *) manufacturer + 35;
  }


  if (source_ptr != NULL) {
    copy_page(source_ptr, buffer, false);
  }

  invalidate_bytes_page(buffer, mask);

  l_u8_wr_LI0_SeIDPageNumber(page);
  l_u8_wr_LI0_SeIDByteA(buffer[0]);
  l_u8_wr_LI0_SeIDByteB(buffer[1]);
  l_u8_wr_LI0_SeIDByteC(buffer[2]);
  l_u8_wr_LI0_SeIDByteD(buffer[3]);
  l_u8_wr_LI0_SeIDByteE(buffer[4]);
  l_u8_wr_LI0_SeIDByteF(buffer[5]);
  l_u8_wr_LI0_SeIDByteG(buffer[6]);
}

void data_xmit(uint8_t page) {
  static uint8_t buffer[7];
  uint8_t mask = 0;
  uint32_t temp;

  switch(page)  {
    case 0:
      buffer[0] = se_id_status;
      buffer[1] = 2;  //pages to be sent
      buffer[2] = 97;  //first (non zero) page
      buffer[3] = 97;  //last page
      mask = 0b0000111;
      break;
    case 97:
      temp = se_trip_elevation + 4000*8;            //use offset value to avoid valid 0xFF bytes
      buffer[0] = (uint8_t) (temp & 0xFFFF);        //low byte
      buffer[1] = (uint8_t) ((temp >> 8) & 0xFFFF); //high byte
      mask = 0b0011111;
  }

  invalidate_bytes_page(buffer, mask);

  l_u8_wr_LI0_SeIDPageNumber(page);
  l_u8_wr_LI0_SeIDByteA(buffer[0]);
  l_u8_wr_LI0_SeIDByteB(buffer[1]);
  l_u8_wr_LI0_SeIDByteC(buffer[2]);
  l_u8_wr_LI0_SeIDByteD(buffer[3]);
  l_u8_wr_LI0_SeIDByteE(buffer[4]);
  l_u8_wr_LI0_SeIDByteF(buffer[5]);
  l_u8_wr_LI0_SeIDByteG(buffer[6]);
}

void data_parse()  {
  static uint8_t buffer[7];
  static uint8_t pgs_to_read = 255;
  static uint8_t pgs_read = 0;
  int32_t temp;

  buffer[0] = l_u8_rd_LI0_EvIDByteA();
  buffer[1] = l_u8_rd_LI0_EvIDByteB();
  buffer[2] = l_u8_rd_LI0_EvIDByteC();
  buffer[3] = l_u8_rd_LI0_EvIDByteD();
  buffer[4] = l_u8_rd_LI0_EvIDByteE();
  buffer[5] = l_u8_rd_LI0_EvIDByteF();
  buffer[6] = l_u8_rd_LI0_EvIDByteG();

  pgs_read++;

  switch(l_u8_rd_LI0_EvIDPageNumber()) {
    case 0:
      pgs_to_read = buffer[1];
      se_id_status = buffer[0];
      pgs_read = 1;
      break;
    case 97:
      ev_odometer = ((uint32_t)buffer[3] << 24) | ((uint32_t)buffer[2] << 16) | ((uint32_t)buffer[1] << 8) | ((uint32_t)buffer[0]);
      printf("EvOdometer %.3lfkm\n", (float)ev_odometer * 0.125);
      break;
    case 98:
      ev_energy_discharge_capacity = ((uint32_t)buffer[2] << 16) | ((uint32_t)buffer[1] << 8) | ((uint32_t)buffer[0]);
      ev_energy_charge_capacity = ((uint32_t)buffer[5] << 16) | ((uint32_t)buffer[4] << 8) | ((uint32_t)buffer[3]);
      printf("Energy Capacity, Discharge: %.3lf kWh, Charge: %.3lfkWh\n", (double) ev_energy_discharge_capacity * 0.001, (double) ev_energy_charge_capacity * 0.001);
      break;
    case 99:
      ev_energy_for_departure = ((uint32_t)buffer[2] << 16) | ((uint32_t)buffer[1] << 8) | ((uint32_t)buffer[0]);
      ev_time_to_departure = ((uint32_t)buffer[4] << 8) | ((uint32_t)buffer[3]);
      printf("Energy for Departure: %.3lfkWh\n", (double) ev_energy_for_departure * 0.001);
      printf("EvTimeToDeparture %d minutes\n", ev_time_to_departure);
    break;
    case 100:
      ev_battery_range = ((uint32_t)buffer[1] << 8) | ((uint32_t)buffer[0]);
      ev_fuel_range = ((uint32_t)buffer[3] << 8) | ((uint32_t)buffer[2]);
      temp = (int32_t)((uint32_t)buffer[5] << 8) | ((uint32_t)buffer[4]);
      ev_battery_dc_power = temp - (650000/20);
      ev_battery_health = buffer[6];
      printf("Range, Battery: %dkm, Fuel: %dkm\n", ev_battery_range, ev_fuel_range);
      printf("Battery DC Power: %dW\n", (int32_t) (ev_battery_dc_power * 20));
      printf("Battery Health %.1f%%\n", ev_battery_health * 0.4);
      break;
    case 101:
      temp = (int32_t)((uint32_t)buffer[1] << 8) | ((uint32_t)buffer[0]);
      ev_ambient_temperature = temp - 273*32;
      temp = (int32_t)((uint32_t)buffer[3] << 8) | ((uint32_t)buffer[2]);
      ev_cabin_temperature = temp - 273*32;
      printf("Environment Temperatures, Ambient: %.3lf°C, Cabin %.3lf°C\n", (double)(ev_ambient_temperature * 0.03125), (double)(ev_cabin_temperature *0.03125));
      break;
    case 102:
      temp = (int32_t)((uint32_t)buffer[1] << 8) | ((uint32_t)buffer[0]);
      ev_battery_temperature = temp - 273*32;
      temp = (int32_t)((uint32_t)buffer[3] << 8) | ((uint32_t)buffer[2]);
      ev_max_battery_temperature = temp - 273*32;
      temp = (int32_t)((uint32_t)buffer[5] << 8) | ((uint32_t)buffer[4]);
      ev_min_battery_temperature = temp - 273*32;
      printf("Battery Temperatures, Ave: %.3lf°C, Max: %.3lf°C, Min %.3lf°C\n", (double)(ev_battery_temperature * 0.03125), (double)(ev_max_battery_temperature * 0.03125), (double)(ev_min_battery_temperature * 0.03125));
      break;
    case 103:
      temp = (int32_t)((uint32_t)buffer[1] << 8) | ((uint32_t)buffer[0]);
      ev_max_watt_charge = temp - (650000/20);
      temp = (int32_t)((uint32_t)buffer[3] << 8) | ((uint32_t)buffer[2]);
      ev_max_watt_discharge = temp - (650000/20);
      printf("Power Limits, Charge: %dW, Discharge: %dW\n", (int32_t) (ev_max_watt_charge * 20), (int32_t) (ev_max_watt_discharge * 20));
      break;
    case 104:
      temp = (int32_t)((uint32_t)buffer[1] << 8) | ((uint32_t)buffer[0]);
      ev_max_var_supply = temp - (650000/20);
      temp = (int32_t)((uint32_t)buffer[3] << 8) | ((uint32_t)buffer[2]);
      ev_max_var_absorb = temp - (650000/20);
      printf("Reactive Power Limits, Supply: %dVA, Absorb: %dVA\n", (int32_t) (ev_max_var_supply * 20), (int32_t) (ev_max_var_absorb * 20));
      break;
  }
}
#endif