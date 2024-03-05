//Long Button Press: Return to main menu
#include <Arduino.h>
#include <U8g2lib.h>
#include <MUIU8g2.h>
#include <Versatile_RotaryEncoder.h>
#include <Adafruit_NeoPixel.h>
#include <Ethernet.h>
#include <Artnet.h>

#define MUI_U8G2_V_PADDING 1
#define clk 6
#define dt 7
#define sw 8

Versatile_RotaryEncoder versatile_encoder(clk, dt, sw);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, 22, NEO_RGB);
U8G2_ST7567_OS12864_F_4W_SW_SPI u8g2(U8G2_R2,/* clock=*/ 2, /* data=*/ 3, /* cs=*/ 5, /* dc=*/ 4, /* reset=*/ 11);
MUIU8G2 mui;

Artnet artnet;
byte ip[] = {192, 168, 1, 131};
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

uint8_t exit_code = 1; 


uint8_t is_redraw = 1;
uint8_t rotate_event = 0; // 0 = not turning, 1 = CW, 2 = CCW
uint8_t press_event = 0; // 0 = not pushed, 1 = pushed
uint8_t long_press_event = 0; // 0 = not pushed, 1 = pushed

int octet1;
int octet2;
int octet3;
int octet4;
byte newip[]= {0,0,0,0};

uint16_t protocol_idx = 0;
uint8_t octet1_A      = ip[0]/100;
uint8_t octet1_B      = (ip[0]-(octet1_A*100))/10;
uint8_t octet1_C      = ip[0]-(octet1_A*100)-(octet1_B*10);
uint8_t octet2_A      = ip[1]/100;
uint8_t octet2_B      = (ip[1]-(octet2_A*100))/10;
uint8_t octet2_C      = ip[1]-(octet2_A*100)-(octet2_B*10);
uint8_t octet3_A      = ip[2]/100;
uint8_t octet3_B      = (ip[2]-(octet3_A*100))/10;
uint8_t octet3_C      = ip[2]-(octet3_A*100)-(octet3_B*10);
uint8_t octet4_A      = ip[3]/100;
uint8_t octet4_B      = (ip[3]-(octet4_A*100))/10;
uint8_t octet4_C      = ip[3]-(octet4_A*100)-(octet4_B*10);
uint8_t uni100_value  = 0;
uint8_t uni10_value   = 0;
uint8_t uni1_value    = 0;
uint8_t ch100_value   = 0;
uint8_t ch10_value    = 0;
uint8_t ch1_value     = 0;
const char *protocols[] = { "Artnet", "sACN"};

uint16_t protocols_name_list_get_cnt(void *data) {
  return sizeof(protocols)/sizeof(*protocols);    
}

const char *protocols_name_list_get_str(void *data, uint16_t index) {
  return protocols[index];
}

uint8_t mui_hrule(mui_t *ui, uint8_t msg) {
  if ( msg == MUIF_MSG_DRAW ) {
      u8g2.drawHLine(0, mui_get_y(ui), u8g2.getDisplayWidth());
  }
  return 0;
}

uint8_t mui_update_info(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_draw_button_utf(ui, U8G2_BTN_HCENTER |mui_u8g2_get_fi_flags(ui), 0, 1, MUI_U8G2_V_PADDING, ui->text);
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
        Serial.println("update info");
  octet1 = octet1_A*100 + octet1_B*10 + octet1_C;
  octet2 = octet2_A*100 + octet2_B*10 + octet2_C;
  octet3 = octet3_A*100 + octet3_B*10 + octet3_C;
  octet4 = octet4_A*100 + octet4_B*10 + octet4_C;
  newip[0] = octet1;
  newip[1] = octet2;
  newip[2] = octet3;
  newip[3] = octet4;
  if(newip != ip){
    Serial.println("new ip != ip");
    memcpy(ip , newip, sizeof(newip));
    Ethernet.begin(mac, ip);
    artnet.begin(mac, ip);
    Serial.println(Ethernet.localIP());
  }
      //return mui_GotoForm(ui, ui->arg, 0);
      return mui_GotoFormAutoCursorPosition(ui, ui->arg);
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;    
  }
  return 0;
}

muif_t muif_list[] = {
  MUIF_U8G2_FONT_STYLE(0, u8g2_font_helvR08_tr),        /* regular font */
  MUIF_U8G2_FONT_STYLE(1, u8g2_font_helvB08_tr),        /* bold font */
  MUIF_RO("HR", mui_hrule),
  MUIF_U8G2_LABEL(),
  MUIF_RO("GP",mui_u8g2_goto_data),  
  MUIF_BUTTON("GC", mui_u8g2_goto_form_w1_pi),
  MUIF_U8G2_U8_MIN_MAX("UA", &uni100_value, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("UB", &uni10_value, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("UC", &uni1_value, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("CA", &ch100_value, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("CB", &ch10_value, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("CC", &ch1_value, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("IA", &octet1_A, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("IB", &octet1_B, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("IC", &octet1_C, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("ID", &octet2_A, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("IE", &octet2_B, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("IF", &octet2_C, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("IG", &octet3_A, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("IH", &octet3_B, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("II", &octet3_C, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("IJ", &octet4_A, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("IK", &octet4_B, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("IL", &octet4_C, 0, 9, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U16_LIST("NA", &protocol_idx, NULL, protocols_name_list_get_str, protocols_name_list_get_cnt, mui_u8g2_u16_list_line_wa_mud_pi),
  MUIF_VARIABLE("G0",&exit_code,mui_update_info)
};

fds_t fds_data[] = 
MUI_FORM(1)
MUI_STYLE(0)
MUI_LABEL(5, 8, "MechaPhoton pan tilt")
MUI_XY("HR", 0,11)
MUI_DATA("GP",
    MUI_10 "PROTOCOL|"
    MUI_12 "IP ADDRESS|"
    MUI_13 "UNIVERSE|"
    MUI_14 "CHANNEL|"
)
MUI_XYA("GC", 5, 24, 0) 
MUI_XYA("GC", 5, 36, 1) 
MUI_XYA("GC", 5, 48, 2) 
MUI_XYA("GC", 5, 60, 3) 

MUI_FORM(10)
MUI_STYLE(0)
MUI_LABEL(5, 8, "Protocol")
MUI_XY("HR", 0,11)
MUI_LABEL(5,49, "Protocol:")
MUI_XYA("NA", 50, 49, 44)
MUI_XYT("G0", 114, 60, " Ok ") 

MUI_FORM(12)
MUI_STYLE(0)
MUI_LABEL(5, 8, "IP ADDRESS")
MUI_XY("HR", 0,11)
MUI_XY("IA", 10, 23)
MUI_XY("IB", 18, 23)
MUI_XY("IC", 26, 23)
//MUI_LABEL(35,23, ".")
MUI_XY("ID", 40, 23)
MUI_XY("IE", 48, 23)
MUI_XY("IF", 56, 23)
//MUI_LABEL(65,23, ".")
MUI_XY("IG", 70, 23)
MUI_XY("IH", 78, 23)
MUI_XY("II", 86, 23)
//MUI_LABEL(95,23, ".")
MUI_XY("IJ", 100, 23)
MUI_XY("IK", 108, 23)
MUI_XY("IL", 116, 23)
MUI_XYAT("G0", 114, 60, 1, " Ok ") 

MUI_FORM(13)
MUI_STYLE(0)
MUI_LABEL(5, 8, "UNIVERSE")
MUI_XY("HR", 0,11)
MUI_XY("UA", 50, 23)
MUI_XY("UB", 60, 23)
MUI_XY("UC", 70, 23)
MUI_XYAT("G0", 114, 60, 1, " Ok ") 

MUI_FORM(14)
MUI_STYLE(0)
MUI_LABEL(5, 8, "START CHANNEL")
MUI_XY("HR", 0,11)
MUI_XY("CA", 50, 23)
MUI_XY("CB", 60, 23)
MUI_XY("CC", 70, 23)
MUI_XYAT("G0", 114, 60, 1, " Ok ") 
;

void handleRotate(int8_t rotation) {
  if ( rotation > 0 )
    rotate_event = 2; // CW
  else
    rotate_event = 1; // CCW
}

void handlePressRelease() {
  press_event = 1;
}

void handleLongPressRelease() {
  long_press_event = 1;
}

void setup(void) {

  Serial.begin(9600);
  while (! Serial); 
  Serial.println("serial init");
  versatile_encoder.setHandleRotate(handleRotate);
  versatile_encoder.setHandlePressRelease(handlePressRelease);
  versatile_encoder.setHandleLongPressRelease(handleLongPressRelease);
  
  u8g2.begin();
  mui.begin(u8g2, fds_data, muif_list, sizeof(muif_list)/sizeof(muif_t));
  mui.gotoForm(/* form_id= */ 1, /* initial_cursor_position= */ 0);

  Ethernet.init(17);
  Ethernet.begin(mac, ip);
  artnet.begin(mac, ip);
  artnet.setArtDmxCallback(onDmxFrame);
  Serial.println("artnet init");
  Serial.println(Ethernet.localIP());

  strip.begin();
  strip.setBrightness(150);
  strip.setPixelColor(0, strip.Color(150, 150, 0));
  strip.setPixelColor(1, strip.Color(150, 150, 0));
  strip.setPixelColor(2, strip.Color(150, 150, 0));
  strip.show();
}

void handle_events(void) {
  // 0 = not pushed, 1 = pushed  
  if ( press_event == 1 ) {
    mui.sendSelect();
    is_redraw = 1;
    press_event = 0;
  }

  // 0 = not pushed, 1 = pushed  
  if ( long_press_event == 1 ) {
    mui.sendSelectWithExecuteOnSelectFieldSearch();
    is_redraw = 1;
    long_press_event = 0;
  }
  
  // 0 = not turning, 1 = CW, 2 = CCW
  if ( rotate_event == 1 ) {
    mui.nextField();
    is_redraw = 1;
    rotate_event = 0;
  }
  
  if ( rotate_event == 2 ) {
    mui.prevField();
    is_redraw = 1;
    rotate_event = 0;
  }    
}

void loop(void) {
  artnet.read();
  /* check whether the menu is active */
  if ( mui.isFormActive() ) {
    /* update the display content, if the redraw flag is set */
    if ( is_redraw ) {
      u8g2.firstPage();
      do {
          versatile_encoder.ReadEncoder(); // Do the encoder reading and processing
          mui.draw();
          versatile_encoder.ReadEncoder(); // Do the encoder reading and processing
      } while( u8g2.nextPage() );
      is_redraw = 0;                    /* clear the redraw flag */
    }
    versatile_encoder.ReadEncoder(); // Do the encoder reading and processing
    handle_events();
  } else {
      /* the menu should never become inactive, but if so, then restart the menu system */
      mui.gotoForm(/* form_id= */ 1, /* initial_cursor_position= */ 0);
  }
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data, IPAddress remoteIP){
  //Serial.println("frame");
  strip.setPixelColor(0, strip.Color(0, 0, data[0]));
  strip.setPixelColor(1, strip.Color(0, 0, data[0]));
  strip.setPixelColor(2, strip.Color(0, 0, data[0]));
  strip.show();
}


