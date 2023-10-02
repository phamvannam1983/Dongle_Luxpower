/*
    NamPham
*/
//////////// CLOUD ////////////
#include <WiFi.h>
//#include "HTTPClient.h"
static uint8_t g_eth_connected = 0;

#include "PubSubClient_3_1.h"
// Add your MQTT Broker IP address, example:
const char* mqtt_server = "192.168.31.166"; 
//const char* mqtt_server = "YOUR_MQTT_BROKER_IP_ADDRESS";
const char* topic = "lxp/BA31605780";    //printer_mqtt
int port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

//#include <Arduino_JSON.h>
#include "rtu_crc.h"

#define WIFI_SSID "objm"
#define WIFI_PASSWORD "sunrise2406"

int g_connect_wifi;
char g_id[16];

///////////// RS485 object /////////////
HardwareSerial MySerial1(1);
#define RS485   MySerial1

#define ARR_485       500
#define ADD_LEN       14
unsigned char g_rs485_data[ARR_485];
int           g_timeout_rs485_rx = 0;
unsigned char data_1[18]  = {0x00, 0x04, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x7F, 0x00, 0x76, 0x5E};
unsigned char data_2[18]  = {0x00, 0x04, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x7F, 0x00, 0x7F, 0x00, 0x6F, 0x8A};
unsigned char data_3[18]  = {0x00, 0x04, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0xFE, 0x00, 0x7F, 0x00, 0x47, 0xB6};

unsigned char data[300] = {0x01, 0x04, 0x33, 0x32, 0x32, 0x33, 0x36, 0x33, 0x32, 0x36, 0x31, 0x39, 0x00, 0x00, 0xFE, 0x04, 0x00, 0x02, 0x09, 0x00, 0x00, 0x3C, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDD, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x69, 0x08, 0x00, 0x00, 0x00, 0x00, 0x8D, 0x13, 0x76, 0x01, 0x00, 0x00, 0xDC, 0x00, 0x00, 0x00, 0x5C, 0x08, 0x04, 0x20, 0x0A, 0x00, 0x8D, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x49, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x41, 0x0E, 0x25, 0x00,    0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x20, 0x00,     0x23, 0x00, 0x22, 0x00, 0x00, 0x00, 0x19, 0x47, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x32, 0x03, 0xA8, 0x0E, 0x03, 0xF0, 0x60, 0x2D, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x01, 0x00, 0x00, 0x33, 0x32, 0x32, 0x33, 0x36, 0x33, 0x32, 0x36, 0x31, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x47, 0x00, 0x09, 0xC1, 0x7C, 0x14, 0x00, 0x54, 0x60};


//////////// GPIO object ////////////
// Button Pin
#define TRUE          1
#define FAIL          0
#define LOOP          50
#define ON            LOW  
#define OF            HIGH

#define   LED_1       2  //6 out -> not run
#define   LED_0       0
#define   OUT_0       23
#define   OUT_1       22
#define   INP_0       36
#define   INP_1       39
#define   INP_2       34
#define   INP_3       35
#define   IN_1       32
#define   IN_2       33
#define   IN_3       25
#define   IN_4       26
#define   IN_5       27
#define   IN_6       14
#define   IN_7       12
#define   IN_8       13

/////////////// Other ///////////////
#include "time.h"

const char* ntpServer = "1.vn.pool.ntp.org";  //nnn pool.ntp.org
const long  gmtOffset_sec = 3600 * 7; //nnn *1
const int   daylightOffset_sec = 3600 * 0;  

struct  tm timeinfo;
time_t  utc, utc_;
int     g_sts_read_utc = OFF;
/////////////// Other ///////////////
int           g_time_rs = 0, g_time_led = 0;
TaskHandle_t  Task0;

/////////////////////////////////////////////////////////////////////////////
#define ARR_JSON      2000
#define ARR_DATA      300
char g_json[ARR_JSON];

struct st_ReadInputAll {
    unsigned short status;
    //nom(Parse = "Utils::le_i16_div10")]
    float v_pv_1;
    //nom(Parse = "Utils::le_i16_div10")]
    float v_pv_2;
    //nom(Parse = "Utils::le_i16_div10")]
    float v_pv_3;
    //nom(Parse = "Utils::le_i16_div10")]
    float v_bat;

    unsigned char soc;
    unsigned char soh;
    //nom(SkipBefore(2))] // InternalFault
    //nom(Ignore)]
    unsigned short p_pv;
    unsigned short p_pv_1;
    unsigned short p_pv_2;
    unsigned short p_pv_3;
    //nom(Ignore)]
    //nnn unsigned short p_battery;
    unsigned short p_charge;
    unsigned short p_discharge;

    //nom(Parse = "Utils::le_i16_div10")]
    float v_ac_r;
    //nom(Parse = "Utils::le_i16_div10")]
    float v_ac_s;
    //nom(Parse = "Utils::le_i16_div10")]
    float v_ac_t;
    //nom(Parse = "Utils::le_i16_div100")]
    float f_ac;

    unsigned short p_inv;
    unsigned short p_rec;

    //nom(SkipBefore(2))] // IinvRMS
    //nom(Parse = "Utils::le_i16_div1000")]
    float pf;

    //nom(Parse = "Utils::le_i16_div10")]
    float v_eps_r;
    //nom(Parse = "Utils::le_i16_div10")]
    float v_eps_s;
    //nom(Parse = "Utils::le_i16_div10")]
    float v_eps_t;
    //nom(Parse = "Utils::le_i16_div100")]
    float f_eps;
    unsigned short p_eps;
    unsigned short s_eps;
    //nom(Ignore)]
    //nnn unsigned short p_grid;
    unsigned short p_to_grid;
    unsigned short p_to_user;
    unsigned short p_load;  //new

    //nom(Ignore)]
    float e_pv_day;
    //nom(Parse = "Utils::le_i16_div10")]
    float e_pv_day_1;
    //nom(Parse = "Utils::le_i16_div10")]
    float e_pv_day_2;
    //nom(Parse = "Utils::le_i16_div10")]
    float e_pv_day_3;

    //nom(Parse = "Utils::le_i16_div10")]
    float e_inv_day;
    //nom(Parse = "Utils::le_i16_div10")]
    float e_rec_day;
    //nom(Parse = "Utils::le_i16_div10")]
    float e_chg_day;
    //nom(Parse = "Utils::le_i16_div10")]
    float e_dischg_day;
    //nom(Parse = "Utils::le_i16_div10")]
    float e_eps_day;
    //nom(Parse = "Utils::le_i16_div10")]
    float e_to_grid_day;
    //nom(Parse = "Utils::le_i16_div10")]
    float e_to_user_day;
    float e_load_day;   //new

    //nom(Parse = "Utils::le_i16_div10")]
    float v_bus_1;
    //nom(Parse = "Utils::le_i16_div10")]
    float v_bus_2;
    /////////////////////////////////////

    //nom(Ignore)]
    float e_pv_all;
    //nom(Parse = "Utils::le_u32_div10")]
    float e_pv_all_1;
    //nom(Parse = "Utils::le_u32_div10")]
    float e_pv_all_2;
    //nom(Parse = "Utils::le_u32_div10")]
    float e_pv_all_3;

    //nom(Parse = "Utils::le_u32_div10")]
    float e_inv_all;
    //nom(Parse = "Utils::le_u32_div10")]
    float e_rec_all;
    //nom(Parse = "Utils::le_u32_div10")]
    float e_chg_all;
    //nom(Parse = "Utils::le_u32_div10")]
    float e_dischg_all;
    //nom(Parse = "Utils::le_u32_div10")]
    float e_eps_all;
    //nom(Parse = "Utils::le_u32_div10")]
    float e_to_grid_all;
    //nom(Parse = "Utils::le_u32_div10")]
    float e_to_user_all;
    float e_load_all;   //new

    //nom(SkipBefore(8))] // 4 byte fault code, 4 byte warning code
    unsigned short t_inner;
    unsigned short t_rad_1;
    unsigned short t_rad_2;
    unsigned short t_bat;
    //nom(SkipBefore(2))] // reserved - radiator 3?
    unsigned long runtime;
    /////////////////////////////////////

    // 18 bytes of auto_test stuff here I'm not doing yet
    //nom(SkipBefore(18))] // auto_test stuff, TODO..
    //nom(SkipBefore(2))] // bat_brand, bat_com_type
    //nom(Parse = "Utils::le_i16_div100")]
    float max_chg_curr;
    //nom(Parse = "Utils::le_i16_div100")]
    float max_dischg_curr;
    //nom(Parse = "Utils::le_i16_div10")]
    float charge_volt_ref;
    //nom(Parse = "Utils::le_i16_div10")]
    float dischg_cut_volt;

    unsigned short bat_status_0;
    unsigned short bat_status_1;
    unsigned short bat_status_2;
    unsigned short bat_status_3;
    unsigned short bat_status_4;
    unsigned short bat_status_5;
    unsigned short bat_status_6;
    unsigned short bat_status_7;
    unsigned short bat_status_8;
    unsigned short bat_status_9;
    unsigned short bat_status_inv;

    unsigned short bat_count;
    unsigned short bat_capacity;

    //nom(Parse = "Utils::le_i16_div100")]
    float bat_current;

    unsigned short bms_event_1; // FaultCode_BMS
    unsigned short bms_event_2; // WarningCode_BMS

    // TODO: probably floats but need non-zero sample data to check. just guessing at the div100.
    //nom(Parse = "Utils::le_i16_div100")]
    float max_cell_voltage;
    //nom(Parse = "Utils::le_i16_div100")]
    float min_cell_voltage;
    //nom(Parse = "Utils::le_i16_div100")]
    float max_cell_temp;
    //nom(Parse = "Utils::le_i16_div100")]
    float min_cell_temp;

    unsigned short bms_fw_update_state;

    unsigned short cycle_count;

    //nom(Parse = "Utils::le_i16_div10")]
    float vbat_inv;

    // 14 bytes I'm not sure what they are; possibly generator stuff
    //nom(SkipBefore(14))]
    // following are for influx capability only
    //nom(Parse = "Utils::current_time_for_nom")]
    unsigned long time;
    //nom(Ignore)]
    unsigned long datalog;
};
struct st_ReadInput1 {
    unsigned char frame[16];  //nnn
    unsigned short status;
    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short v_pv_1;
    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short v_pv_2;
    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short v_pv_3;
    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short v_bat;

    unsigned char soc;
    unsigned char soh;
    //[nom(SkipBefore(2))] // InternalFault
    unsigned short empty;   //nnn
    //[nom(Ignore)]
    //nnn unsigned short p_pv;
    unsigned short p_pv_1;
    unsigned short p_pv_2;
    unsigned short p_pv_3;
    //[nom(Ignore)]
    //nnn unsigned short p_battery;
    unsigned short p_charge;
    unsigned short p_discharge;

    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short v_ac_r;
    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short v_ac_s;
    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short v_ac_t;
    //[nom(Parse = "Utils::le_i16_div100")]
    unsigned short f_ac;

    unsigned short p_inv;
    unsigned short p_rec;

    //[nom(SkipBefore(2))]  // IinvRMS
    unsigned short empty0;   //nnn
    //[nom(Parse = "Utils::le_i16_div1000")]
    unsigned short pf;

    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short v_eps_r;
    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short v_eps_s;
    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short v_eps_t;
    //[nom(Parse = "Utils::le_i16_div100")]
    unsigned short f_eps;
    unsigned short p_eps;
    unsigned short s_eps;
    //[nom(Ignore)]
    //nnn unsigned short p_grid;
    unsigned short p_to_grid;
    unsigned short p_to_user;

    //[nom(Ignore)]
    //nnn unsigned short e_pv_day;
    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short e_pv_day_1;
    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short e_pv_day_2;
    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short e_pv_day_3;

    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short e_inv_day;
    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short e_rec_day;
    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short e_chg_day;
    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short e_dischg_day;
    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short e_eps_day;
    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short e_to_grid_day;
    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short e_to_user_day;

    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short v_bus_1;
    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short v_bus_2;
  //nnn 1-96
  /////////////////////////////////nnn
  //[nom(Parse = "Utils::le_u32_div10")]
  unsigned long e_pv_all_1;
  //[nom(Parse = "Utils::le_u32_div10")]
  unsigned long e_pv_all_2;
  //[nom(Parse = "Utils::le_u32_div10")]
  unsigned long e_pv_all_3;

  //[nom(Parse = "Utils::le_u32_div10")]
  unsigned long e_inv_all;
  //[nom(Parse = "Utils::le_u32_div10")]
  unsigned long e_rec_all;
  //[nom(Parse = "Utils::le_u32_div10")]
  unsigned long e_chg_all;
  //[nom(Parse = "Utils::le_u32_div10")]
  unsigned long e_dischg_all;
  //[nom(Parse = "Utils::le_u32_div10")]
  unsigned long e_eps_all;
  //[nom(Parse = "Utils::le_u32_div10")]
  unsigned long e_to_grid_all;
  //[nom(Parse = "Utils::le_u32_div10")]
  unsigned long e_to_user_all;

  //[nom(SkipBefore(8))] // 4 byte fault code, 4 byte warning code
  unsigned short empty1;   //nnn
  unsigned short empty2;   //nnn
  unsigned short empty3;   //nnn
  unsigned short empty4;   //nnn

  unsigned short t_inner;
  unsigned short t_rad_1;
  unsigned short t_rad_2;
  unsigned short t_bat;

  //[nom(SkipBefore(2))] // reserved
  unsigned short empty5;   //nnn
  unsigned short runtime_l;
  unsigned short runtime_h;
                            
  //97-158byte 
  /////////////////////////////////nnn
  //[nom(SkipBefore(18))] // auto_test stuff, TODO..
  //[nom(SkipBefore(2))] // bat_brand, bat_com_type
  unsigned short empty6;   //nnn
  unsigned short empty7;   //nnn
  unsigned short empty8;   //nnn
  unsigned short empty9;   //nnn
  unsigned short empty10;   //nnn
  unsigned short empty11;   //nnn
  unsigned short empty12;   //nnn
  unsigned short empty13;   //nnn
  unsigned short empty14;   //nnn
  unsigned short empty15;   //nnn
  //[nom(Parse = "Utils::le_i16_div100")]
  unsigned short max_chg_curr;
  //[nom(Parse = "Utils::le_i16_div100")]
  unsigned short max_dischg_curr;
  //[nom(Parse = "Utils::le_i16_div10")]
  unsigned short charge_volt_ref;
  //[nom(Parse = "Utils::le_i16_div10")]
  unsigned short dischg_cut_volt;
  unsigned short bat_status_0;
  unsigned short bat_status_1;
  unsigned short bat_status_2;
  unsigned short bat_status_3;
  unsigned short bat_status_4;
  unsigned short bat_status_5;
  unsigned short bat_status_6;
  unsigned short bat_status_7;
  unsigned short bat_status_8;
  unsigned short bat_status_9;
  unsigned short bat_status_inv;
  unsigned short bat_count;
  unsigned short bat_capacity;
  //[nom(Parse = "Utils::le_i16_div100")]
  unsigned short bat_current;
  unsigned short bms_event_1;
  unsigned short bms_event_2;
  // TODO: probably floats but need non-zero sample data to check. just guessing at the div100.
  //[nom(Parse = "Utils::le_i16_div100")]
  unsigned short max_cell_voltage;
  //[nom(Parse = "Utils::le_i16_div100")]
  unsigned short min_cell_voltage;
  //[nom(Parse = "Utils::le_i16_div100")]
  unsigned short max_cell_temp;
  //[nom(Parse = "Utils::le_i16_div100")]
  unsigned short min_cell_temp;
  unsigned short bms_fw_update_state;
  unsigned short cycle_count;
  //[nom(Parse = "Utils::le_i16_div10")]
  unsigned short vbat_inv;
  /////////////////////////////////nnn
    // 14 bytes I'm not sure what they are; possibly generator stuff
    //[nom(SkipBefore(14))]
    unsigned short empty16;   //nnn
    unsigned short empty17;   //nnn
    unsigned short empty18;   //nnn
    unsigned short empty19;   //nnn
    unsigned short empty20;   //nnn
    unsigned short empty21;   //nnn
    unsigned short empty22;   //nnn
    //following are for influx capability only
    //[nom(Parse = "Utils::current_time_for_nom")]
    unsigned long time;
    //[nom(Ignore)]
    unsigned long datalog;
    unsigned short crc;

    unsigned long runtime;
};
struct st_ReadInput2 {
    unsigned char frame[16];  //nnn
    //[nom(Ignore)]
    unsigned long e_pv_all;
    //[nom(Parse = "Utils::le_u32_div10")]
    unsigned long e_pv_all_1;
    //[nom(Parse = "Utils::le_u32_div10")]
    unsigned long e_pv_all_2;
    //[nom(Parse = "Utils::le_u32_div10")]
    unsigned long e_pv_all_3;

    //[nom(Parse = "Utils::le_u32_div10")]
    unsigned long e_inv_all;
    //[nom(Parse = "Utils::le_u32_div10")]
    unsigned long e_rec_all;
    //[nom(Parse = "Utils::le_u32_div10")]
    unsigned long e_chg_all;
    //[nom(Parse = "Utils::le_u32_div10")]
    unsigned long e_dischg_all;
    //[nom(Parse = "Utils::le_u32_div10")]
    unsigned long e_eps_all;
    //[nom(Parse = "Utils::le_u32_div10")]
    unsigned long e_to_grid_all;
    //[nom(Parse = "Utils::le_u32_div10")]
    unsigned long e_to_user_all;

    //[nom(SkipBefore(8))] // 4 byte fault code, 4 byte warning code
    unsigned short t_inner;
    unsigned short t_rad_1;
    unsigned short t_rad_2;
    unsigned short t_bat;

    //[nom(SkipBefore(2))] // reserved
    unsigned long runtime;
};
struct st_ReadInput3 {
    unsigned char frame[16];  //nnn
    //[nom(SkipBefore(18))] // auto_test stuff, TODO..
    //[nom(SkipBefore(2))] // bat_brand, bat_com_type
    //[nom(Parse = "Utils::le_i16_div100")]
    unsigned short max_chg_curr;
    //[nom(Parse = "Utils::le_i16_div100")]
    unsigned short max_dischg_curr;
    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short charge_volt_ref;
    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short dischg_cut_volt;

    unsigned short bat_status_0;
    unsigned short bat_status_1;
    unsigned short bat_status_2;
    unsigned short bat_status_3;
    unsigned short bat_status_4;
    unsigned short bat_status_5;
    unsigned short bat_status_6;
    unsigned short bat_status_7;
    unsigned short bat_status_8;
    unsigned short bat_status_9;
    unsigned short bat_status_inv;

    unsigned short bat_count;
    unsigned short bat_capacity;

    //[nom(Parse = "Utils::le_i16_div100")]
    unsigned short bat_current;

    unsigned short bms_event_1;
    unsigned short bms_event_2;

    // TODO: probably floats but need non-zero sample data to check. just guessing at the div100.
    //[nom(Parse = "Utils::le_i16_div100")]
    unsigned short max_cell_voltage;
    //[nom(Parse = "Utils::le_i16_div100")]
    unsigned short min_cell_voltage;
    //[nom(Parse = "Utils::le_i16_div100")]
    unsigned short max_cell_temp;
    //[nom(Parse = "Utils::le_i16_div100")]
    unsigned short min_cell_temp;

    unsigned short bms_fw_update_state;

    unsigned short cycle_count;

    //[nom(Parse = "Utils::le_i16_div10")]
    unsigned short vbat_inv;
};
union u_Read1 {
  unsigned char   data[ARR_DATA];
  st_ReadInput1   input;
};
union u_Read2 {
  unsigned char   data[ARR_DATA];
  st_ReadInput2   input;
};
union u_Read3 {
  unsigned char   data[ARR_DATA];
  st_ReadInput3   input;
};

u_Read1           g_input_1; 
u_Read2           g_input_2;
u_Read3           g_input_3;
st_ReadInputAll   g_input_all;
/////////////////////////////////////////////////////////////////////////////////////////////////
// Timer int ////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
    portENTER_CRITICAL_ISR(&timerMux);

    //RS485
    if(g_timeout_rs485_rx > 0) g_timeout_rs485_rx--;

    //System Reset
    g_time_rs++;
    if(g_time_rs >= 15000) esp_restart(); //15s

    //Blink Led
    g_time_led++;
    if(g_time_led == 500)  {digitalWrite(LED_0, LOW);}
    if(g_time_led == 1000) {digitalWrite(LED_0, HIGH); g_time_led = 0;}
    
    portEXIT_CRITICAL_ISR(&timerMux);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// mqtt ///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void callback(char* topic, byte* message, unsigned int len) {
  
    Serial.printf("\ng_incomingPacket len: %d, ", len);
    if(message[0] == '0') digitalWrite(LED_1, LOW);
    else                  digitalWrite(LED_1, HIGH);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
// process data /////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
unsigned short crc16(byte *buffer, int buffer_length)
{
  unsigned char crc_hi = 0xFF; /* high CRC byte initialized */
  unsigned char crc_lo = 0xFF; /* low CRC byte initialized */
  unsigned int i; /* will index into CRC lookup */

  /* pass through message buffer */
  while (buffer_length--) {
    i = crc_hi ^ *buffer++; /* calculate the CRC  */
    crc_hi = crc_lo ^ table_crc_hi[i];
    crc_lo = table_crc_lo[i];
  }

  return (crc_hi << 8 | crc_lo);
}
/////////////////////////////////////////////////////////////////////////////
void Decode_input_all(void)
{
    ///////////////// 111 ////////////////////
    g_input_all.status = g_input_1.input.status;
    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.v_pv_1 = (float)(g_input_1.input.v_pv_1)/10.0;
    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.v_pv_2 = (float)(g_input_1.input.v_pv_2)/10.0;
    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.v_pv_3 = (float)(g_input_1.input.v_pv_3)/10.0;
    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.v_bat = (float)(g_input_1.input.v_bat)/10.0;

    g_input_all.soc = g_input_1.input.soc;
    g_input_all.soh = g_input_1.input.soh;
    //nom(SkipBefore(2))] // InternalFault
    //nom(Ignore)]
    g_input_all.p_pv = g_input_1.input.p_pv_1 + g_input_1.input.p_pv_2 + g_input_1.input.p_pv_3;
    g_input_all.p_pv_1 = g_input_1.input.p_pv_1;
    g_input_all.p_pv_2 = g_input_1.input.p_pv_2;
    g_input_all.p_pv_3 = g_input_1.input.p_pv_3;
    //nom(Ignore)]
    //nnn g_input_all.p_battery = g_input_1.input.p_battery;
    g_input_all.p_charge = g_input_1.input.p_charge;
    g_input_all.p_discharge = g_input_1.input.p_discharge;

    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.v_ac_r = (float)(g_input_1.input.v_ac_r)/10.0;
    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.v_ac_s = (float)(g_input_1.input.v_ac_s)/10.0;
    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.v_ac_t = (float)(g_input_1.input.v_ac_t)/10.0;
    //nom(Parse = "Utils::le_i16_div100")]
    g_input_all.f_ac = (float)(g_input_1.input.f_ac)/100.0;

    g_input_all.p_inv = g_input_1.input.p_inv;
    g_input_all.p_rec = g_input_1.input.p_rec;

    //nom(SkipBefore(2))] // IinvRMS
    //nom(Parse = "Utils::le_i16_div1000")]
    g_input_all.pf = (float)(g_input_1.input.pf)/1000.0;

    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.v_eps_r = (float)(g_input_1.input.v_eps_r)/10.0;
    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.v_eps_s = (float)(g_input_1.input.v_eps_s)/10.0;
    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.v_eps_t = (float)(g_input_1.input.v_eps_t)/10.0;
    //nom(Parse = "Utils::le_i16_div100")]
    g_input_all.f_eps = (float)(g_input_1.input.f_eps)/100.0;
    g_input_all.p_eps = g_input_1.input.p_eps;
    g_input_all.s_eps = g_input_1.input.s_eps;
    //nom(Ignore)]
    //nnn g_input_all.p_grid = g_input_1.input.p_grid;
    g_input_all.p_to_grid = g_input_1.input.p_to_grid;
    g_input_all.p_to_user = g_input_1.input.p_to_user;
    g_input_all.p_load = g_input_all.p_inv + g_input_all.p_rec + g_input_all.p_to_user; //new

    //nom(Ignore)]
    g_input_all.e_pv_day = (float)(g_input_1.input.e_pv_day_1 + g_input_1.input.e_pv_day_2 + g_input_1.input.e_pv_day_3)/10.0;
    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.e_pv_day_1 = (float)(g_input_1.input.e_pv_day_1)/10.0;
    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.e_pv_day_2 = (float)(g_input_1.input.e_pv_day_2)/10.0;
    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.e_pv_day_3 = (float)(g_input_1.input.e_pv_day_3)/10.0;

    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.e_inv_day = (float)(g_input_1.input.e_inv_day)/10.0;
    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.e_rec_day = (float)(g_input_1.input.e_rec_day)/10.0;
    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.e_chg_day = (float)(g_input_1.input.e_chg_day)/10.0;
    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.e_dischg_day = (float)(g_input_1.input.e_dischg_day)/10.0;
    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.e_eps_day = (float)(g_input_1.input.e_eps_day)/10.0;
    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.e_to_grid_day = (float)(g_input_1.input.e_to_grid_day)/10.0;
    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.e_to_user_day = (float)(g_input_1.input.e_to_user_day)/10.0;
    g_input_all.e_load_day = g_input_all.e_inv_day + g_input_all.e_rec_day + g_input_all.e_to_user_day; //new

    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.v_bus_1 = (float)(g_input_1.input.v_bus_1)/10.0;
    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.v_bus_2 = (float)(g_input_1.input.v_bus_2)/10.0;

    ////////////////// 222 ///////////////////
    //nom(Ignore)]
    g_input_all.e_pv_all = (float)(g_input_1.input.e_pv_all_1 + g_input_1.input.e_pv_all_2 + g_input_1.input.e_pv_all_3)/10;
    //nom(Parse = "Utils::le_u32_div10")]
    g_input_all.e_pv_all_1 = (float)(g_input_1.input.e_pv_all_1)/10.0;
    //nom(Parse = "Utils::le_u32_div10")]
    g_input_all.e_pv_all_2 = (float)(g_input_1.input.e_pv_all_2)/10.0;
    //nom(Parse = "Utils::le_u32_div10")]
    g_input_all.e_pv_all_3 = (float)(g_input_1.input.e_pv_all_3)/10.0;

    //nom(Parse = "Utils::le_u32_div10")]
    g_input_all.e_inv_all = (float)(g_input_1.input.e_inv_all)/10.0;
    //nom(Parse = "Utils::le_u32_div10")]
    g_input_all.e_rec_all = (float)(g_input_1.input.e_rec_all)/10.0;
    //nom(Parse = "Utils::le_u32_div10")]
    g_input_all.e_chg_all = (float)(g_input_1.input.e_chg_all)/10.0;
    //nom(Parse = "Utils::le_u32_div10")]
    g_input_all.e_dischg_all = (float)(g_input_1.input.e_dischg_all)/10.0;
    //nom(Parse = "Utils::le_u32_div10")]
    g_input_all.e_eps_all = (float)(g_input_1.input.e_eps_all)/10.0;
    //nom(Parse = "Utils::le_u32_div10")]
    g_input_all.e_to_grid_all = (float)(g_input_1.input.e_to_grid_all)/10.0;
    //nom(Parse = "Utils::le_u32_div10")]
    g_input_all.e_to_user_all = (float)(g_input_1.input.e_to_user_all)/10.0;
    g_input_all.e_load_all = g_input_all.e_inv_all + g_input_all.e_rec_all + g_input_all.e_to_user_all; //new

    //nom(SkipBefore(8))] // 4 byte fault code, 4 byte warning code
    g_input_all.t_inner = g_input_1.input.t_inner;
    g_input_all.t_rad_1 = g_input_1.input.t_rad_1;
    g_input_all.t_rad_2 = g_input_1.input.t_rad_2;
    g_input_all.t_bat = g_input_1.input.t_bat;
    //nom(SkipBefore(2))] // reserved - radiator 3?
    g_input_all.runtime = (g_input_1.input.runtime_h<<16) + g_input_1.input.runtime_l ;

    ///////////////// 333 ////////////////////
    // 18 bytes of auto_test stuff here I'm not doing yet
    //nom(SkipBefore(18))] // auto_test stuff, TODO..
    //nom(SkipBefore(2))] // bat_brand, bat_com_type
    //nom(Parse = "Utils::le_i16_div100")]
    g_input_all.max_chg_curr = (float)(g_input_1.input.max_chg_curr)/100.0;
    //nom(Parse = "Utils::le_i16_div100")]
    g_input_all.max_dischg_curr = (float)(g_input_1.input.max_dischg_curr)/100.0;
    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.charge_volt_ref = (float)(g_input_1.input.charge_volt_ref)/10.0;
    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.dischg_cut_volt = (float)(g_input_1.input.dischg_cut_volt)/10.0;

    g_input_all.bat_status_0 = g_input_1.input.bat_status_0;
    g_input_all.bat_status_1 = g_input_1.input.bat_status_1;
    g_input_all.bat_status_2 = g_input_1.input.bat_status_2;
    g_input_all.bat_status_3 = g_input_1.input.bat_status_3;
    g_input_all.bat_status_4 = g_input_1.input.bat_status_4;
    g_input_all.bat_status_5 = g_input_1.input.bat_status_5;
    g_input_all.bat_status_6 = g_input_1.input.bat_status_6;
    g_input_all.bat_status_7 = g_input_1.input.bat_status_7;
    g_input_all.bat_status_8 = g_input_1.input.bat_status_8;
    g_input_all.bat_status_9 = g_input_1.input.bat_status_9;
    g_input_all.bat_status_inv = g_input_1.input.bat_status_inv;
    g_input_all.bat_count      = g_input_1.input.bat_count;
    g_input_all.bat_capacity   = g_input_1.input.bat_capacity;

    //nom(Parse = "Utils::le_i16_div100")]
    g_input_all.bat_current = (float)(g_input_1.input.bat_current)/100.0;

    g_input_all.bms_event_1 = g_input_1.input.bms_event_1; // FaultCode_BMS
    g_input_all.bms_event_2 = g_input_1.input.bms_event_2; // WarningCode_BMS

    // TODO: probably floats but need non-zero sample data to check. just guessing at the div100.
    //nom(Parse = "Utils::le_i16_div100")]
    g_input_all.max_cell_voltage = (float)(g_input_1.input.max_cell_voltage)/100.0;
    //nom(Parse = "Utils::le_i16_div100")]
    g_input_all.min_cell_voltage = (float)(g_input_1.input.min_cell_voltage)/100.0;
    //nom(Parse = "Utils::le_i16_div100")]
    g_input_all.max_cell_temp = (float)(g_input_1.input.max_cell_temp)/100.0;
    //nom(Parse = "Utils::le_i16_div100")]
    g_input_all.min_cell_temp = (float)(g_input_1.input.min_cell_temp)/100.0;

    g_input_all.bms_fw_update_state = g_input_1.input.bms_fw_update_state;
    g_input_all.cycle_count = g_input_1.input.cycle_count;

    //nom(Parse = "Utils::le_i16_div10")]
    g_input_all.vbat_inv = (float)(g_input_1.input.vbat_inv)/10.0;

    // 14 bytes I'm not sure what they are; possibly generator stuff
    //nom(SkipBefore(14))]
    // following are for influx capability only
    //nom(Parse = "Utils::current_time_for_nom")]
    g_input_all.time = 0;
    //nom(Ignore)]
    g_input_all.datalog = 0;
}
/////////////////////////////////////////////////////////////////////////////
void export_json(unsigned long long utc_time, char *id_inveter)
{  
  char field_json[64];

  memset(g_json, 0, 2000); 
  strcat(g_json, "{");        memset(field_json, 0, 64); sprintf(field_json, "\"status\":%d,", g_input_all.status);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"v_pv_1\":%.1f,", g_input_all.v_pv_1);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"v_pv_2\":%.1f,", g_input_all.v_pv_2);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"v_pv_3\":%.1f,", g_input_all.v_pv_3);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"v_bat\":%.1f,", g_input_all.v_bat);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"soc\":%d,", g_input_all.soc);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"soh\":%d,", g_input_all.soh);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"p_pv\":%d,", g_input_all.p_pv);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"p_pv_1\":%d,", g_input_all.p_pv_1);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"p_pv_2\":%d,", g_input_all.p_pv_2);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"p_pv_3\":%d,", g_input_all.p_pv_3);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"p_charge\":%d,", g_input_all.p_charge);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"p_discharge\":%d,", g_input_all.p_discharge);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"v_ac_r\":%.1f,", g_input_all.v_ac_r);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"v_ac_s\":%.1f,", g_input_all.v_ac_s);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"v_ac_t\":%.1f,", g_input_all.v_ac_t);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"f_ac\":%.2f,", g_input_all.f_ac);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"p_inv\":%d,", g_input_all.p_inv);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"p_rec\":%d,", g_input_all.p_rec);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"pf\":%d,", g_input_all.pf);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"v_eps_r\":%.1f,", g_input_all.v_eps_r);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"v_eps_s\":%.1f,", g_input_all.v_eps_s);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"v_eps_t\":%.1f,", g_input_all.v_eps_t);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"f_eps\":%.2f,", g_input_all.f_eps);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"p_eps\":%d,", g_input_all.p_eps);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"s_eps\":%d,", g_input_all.s_eps);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"p_to_grid\":%d,", g_input_all.p_to_grid);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"p_to_user\":%d,", g_input_all.p_to_user);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"p_load\":%d,", g_input_all.p_load); //new
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_pv_day\":%.1f,", g_input_all.e_pv_day);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_pv_day_1\":%.1f,", g_input_all.e_pv_day_1);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_pv_day_2\":%.1f,", g_input_all.e_pv_day_2);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_pv_day_3\":%.1f,", g_input_all.e_pv_day_3);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_inv_day\":%.1f,", g_input_all.e_inv_day);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_rec_day\":%.1f,", g_input_all.e_rec_day);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_chg_day\":%.1f,", g_input_all.e_chg_day);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_dischg_day\":%.1f,", g_input_all.e_dischg_day);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_eps_day\":%.1f,", g_input_all.e_eps_day);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_to_grid_day\":%.1f,", g_input_all.e_to_grid_day);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_to_user_day\":%.1f,", g_input_all.e_to_user_day);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_load_day\":%.1f,", g_input_all.e_load_day); //new
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"v_bus_1\":%.1f,", g_input_all.v_bus_1);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"v_bus_2\":%.1f,", g_input_all.v_bus_2);
    /////////////////////////////////////
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_pv_all\":%.1f,", g_input_all.e_pv_all);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_pv_all_1\":%.1f,", g_input_all.e_pv_all_1);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_pv_all_2\":%.1f,", g_input_all.e_pv_all_2);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_pv_all_3\":%.1f,", g_input_all.e_pv_all_3);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_inv_all\":%.1f,", g_input_all.e_inv_all);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_rec_all\":%.1f,", g_input_all.e_rec_all);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_chg_all\":%.1f,", g_input_all.e_chg_all);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_dischg_all\":%.1f,", g_input_all.e_dischg_all);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_eps_all\":%.1f,", g_input_all.e_eps_all);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_to_grid_all\":%.1f,", g_input_all.e_to_grid_all);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_to_user_all\":%.1f,", g_input_all.e_to_user_all);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"e_load_all\":%.1f,", g_input_all.e_load_all); //new
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"t_inner\":%d,", g_input_all.t_inner);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"t_rad_1\":%d,", g_input_all.t_rad_1);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"t_rad_2\":%d,", g_input_all.t_rad_2);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"t_bat\":%d,", g_input_all.t_bat);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"runtime\":%d,", g_input_all.runtime);
    /////////////////////////////////////
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"max_chg_curr\":%.1f,", g_input_all.max_chg_curr);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"max_dischg_curr\":%.1f,", g_input_all.max_dischg_curr);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"charge_volt_ref\":%.1f,", g_input_all.charge_volt_ref);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"dischg_cut_volt\":%.1f,", g_input_all.dischg_cut_volt);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"bat_status_0\":%d,", g_input_all.bat_status_0);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"bat_status_1\":%d,", g_input_all.bat_status_1);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"bat_status_2\":%d,", g_input_all.bat_status_2);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"bat_status_3\":%d,", g_input_all.bat_status_3);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"bat_status_4\":%d,", g_input_all.bat_status_4);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"bat_status_5\":%d,", g_input_all.bat_status_5);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"bat_status_6\":%d,", g_input_all.bat_status_6);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"bat_status_7\":%d,", g_input_all.bat_status_7);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"bat_status_8\":%d,", g_input_all.bat_status_8);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"bat_status_9\":%d,", g_input_all.bat_status_9);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"bat_status_inv\":%d,", g_input_all.bat_status_inv);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"bat_count\":%d,", g_input_all.bat_count);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"bat_capacity\":%d,", g_input_all.bat_capacity);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"bat_current\":%.1f,", g_input_all.bat_current);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"bms_event_1\":%d,", g_input_all.bms_event_1);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"bms_event_2\":%d,", g_input_all.bms_event_2);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"max_cell_voltage\":%.1f,", g_input_all.max_cell_voltage);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"min_cell_voltage\":%.1f,", g_input_all.min_cell_voltage);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"max_cell_temp\":%.1f,", g_input_all.max_cell_temp);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"min_cell_temp\":%.1f,", g_input_all.min_cell_temp);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"bms_fw_update_state\":%d,", g_input_all.bms_fw_update_state);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"cycle_count\":%d,", g_input_all.cycle_count);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"vbat_inv\":%.1f,", g_input_all.vbat_inv);
  //strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"time\":%d,", g_input_all.time);
  //strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"datalog\":%d,", g_input_all.datalog);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"time\":%d,", utc_time);
  strcat(g_json, field_json); memset(field_json, 0, 64); sprintf(field_json, "\"datalog\":%s", id_inveter);
  strcat(g_json, field_json);
  strcat(g_json, "}");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup & Main /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
    int i_s;
    pinMode(LED_0, OUTPUT);
    pinMode(LED_1, OUTPUT);
    pinMode(INP_0, INPUT_PULLUP);
    pinMode(INP_1, INPUT_PULLUP);
    pinMode(INP_2, INPUT_PULLUP);
    pinMode(INP_3, INPUT_PULLUP);
    pinMode(OUT_0, OUTPUT);
    pinMode(OUT_1, OUTPUT);
    digitalWrite(LED_0, 1);  digitalWrite(LED_1, 1); digitalWrite(OUT_1, 1); digitalWrite(OUT_0, 1);
    
    //waiting 1s
    for (i_s = 0; i_s < 10; i_s++) {
      digitalWrite(LED_0, LOW);
      delay(50);
      digitalWrite(LED_0, HIGH);
      delay(50);
    }

    //init Reader & UART
    Serial.begin(115200); 
    Serial.println("\nXSolar begin:");
    RS485.begin(19200, SERIAL_8N1, 16, 17); // RX, TX
    memset(g_id, 0, 16);
    sprintf(g_id, "%012llX", ESP.getEfuseMac());
    Serial.printf("MAC ID: %s\n", g_id);
    //Serial.printf("\nCHIP MAC: %012llX\n", ESP.getChipId());
    //Serial.printf("\nCHIP MAC: %012llx\n", ESP.getEfuseMac());
    
    //Connect Wifi
	WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
	WiFi.mode(WIFI_STA); //init wifi mode
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    delay(100);
    i_s = 0; g_connect_wifi = TRUE;
    while (WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      digitalWrite(LED_0, LOW);
      delay(100);
      digitalWrite(LED_0, HIGH);
      delay(100);
      i_s++;
      if(i_s > 35) { //7 sec
        g_connect_wifi = FAIL;
        break; 
      }
    }
    if(g_connect_wifi == TRUE) {
      Serial.print("\nConnected with IP: ");
      Serial.println(WiFi.localIP());
      Serial.println();
      //Connect mqtt
      client.setServer(mqtt_server, port);
      client.subscribe(topic);
      client.setCallback(callback);
      i_s = 0; 
      while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(g_id)) {  //ESP8266Client
          Serial.println("connected");
          // Subscribe
          client.setServer(mqtt_server, port);
          client.subscribe(topic);
          client.setCallback(callback);
        } else {
          Serial.print("failed, rc=");
          Serial.print(client.state());
          Serial.println(" try again in 5 seconds");
          // Wait 5 seconds before retrying
          delay(5000);
          i_s++;
          if(i_s >= 3) {
            Serial.println("ESP32 reset...");
            delay(100); esp_restart(); delay(100); 
          }
        }
      }
    }
    else {
      Serial.println("ESP32 reset...");
      delay(100); esp_restart(); delay(100);
    }

    //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
    xTaskCreatePinnedToCore(
                    Task0code,   /* Task function. */
                    "Task0",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */    //0 is lowest
                    &Task0,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
    delay(500); 
    
    //interrupt timer 1ms
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 1000, true); //1ms          //1.000.000 ~ 1s
    timerAlarmEnable(timer);

    // We configure the NTP server
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
    }
    time(&utc);
    utc_ = utc;
    Serial.print("\nUTC: ");
    Serial.println(utc);
    Serial.println(&timeinfo, "\n%A, %B %d %Y %H:%M:%S");

    //Clean    
    while(RS485.available())   RS485.read();

    Serial.println("Start!"); 
}
/////////////////////////////////////////////////////////////////////////
void Task0code( void * pvParameters ){
for(;;){         
    //watchdog timer    
    g_time_rs = 0;
    
    //UTC
    if(g_sts_read_utc == OFF){
      utc++;
    }
    delay(1000);
}}
/////////////////////////////////////////////////////////////////////////
void loop()
{
  static unsigned short i, j, len, crc, loop = 29;
  static char id_inverter_[64];

  while(1){
    if(utc >= (utc_ + 3600)) { //~1h
      g_sts_read_utc = ON;
      delay(10);
      if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
      }
      time(&utc);
      utc_ = utc;
      Serial.print("\nUTC: ");
      Serial.println(utc);
      Serial.println(&timeinfo, "\n%A, %B %d %Y %H:%M:%S");
      g_sts_read_utc = OFF;
    }
    if(loop < 30) { //60 ~ 60s
        loop++;

        /
        client.loop();
    }
    else {
        loop = 0;

        ///// read data /////
        for(i=0; i< 18; i++) { //input serial RS485
          RS485.write(data_1[i]);
        }
        i = 1000;
        g_timeout_rs485_rx = 10;  //ms
        j = 0;
        while(i--) {
          if(RS485.available()) {
            g_rs485_data[j] = RS485.read();
            j++;
            while(g_timeout_rs485_rx) {
              if(RS485.available()) {
                g_rs485_data[j] = RS485.read();
                j++;
                g_timeout_rs485_rx = 10;  //ms
              }
            }
            i = 1;
          }
          delay(1);
        }
            //Test
            //for(i=0; i< ARR_485; i++) { //input serial RS485
            //  g_rs485_data[i] = data[i]; j = 300;
            //}
        ///// check data /////
        if(j > 255) {
          memcpy(g_input_1.data+1, g_rs485_data, ARR_DATA-1);
          //memcpy(g_input_2.data, data_2, ARR_DATA);
          //memcpy(g_input_3.data, data_3, ARR_DATA);

          //data len: 0xFE ~ 254 byte
          //frame: 15 byte
          // => position crc: 254 + 15 = 269 & 270
          len = g_rs485_data[ADD_LEN]+15;
          i = crc16(g_rs485_data, len);
          crc = g_rs485_data[len]<<8; 
          crc = crc + g_rs485_data[len+1];
          if(i == crc) {
            Serial.printf("\nCRC ok: %X", i);
            Serial.printf("\nCRC position: %X %X", g_rs485_data[269], g_rs485_data[270]);

            ///// decode data /////
            memset(id_inverter_, 0, 64);
            sprintf(id_inverter_, "\"%s\"", "BA31605780");
            Decode_input_all();
            //printf_input_all();
            export_json(utc, id_inverter_);
            Serial.printf("\n\nJSON: %s\n\n", g_json);

            ///// sent data /////
            client.publish("lxp/BA31605780", g_json);
          }
          else {
            Serial.printf("\nCRC Fail: %X", i);
          }
        }
    }
    delay(1000);
  }  
}




