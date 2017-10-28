/* HAL-only entry function */
#include "hal_data.h"
#include "hal_entry.h"
#include "i2c.h"
#include "en210.h"
#include "ssd1306_oled.h"

/* LED type structure */
bsp_leds_t g_leds;

void hal_entry(void)
{
    uint16_t partid;
    float  g_temperatureC;
    float  g_temperatureF;
    float  g_humidityP;
    ssp_err_t err;
    const uint8_t ENS210_init_seq[]= {SENS_RUN, H_RUN|T_RUN, H_START|T_START};
    uint16_t xhist=HISTXPOS;   // The x position of data histogram



    /* Get LED information for this board */
    R_BSP_LedsGet(&g_leds);

    for (uint16_t i=0; i<g_leds.led_count; i++){
         g_ioport.p_api->pinWrite(g_leds.p_leds[i], OFF);
    }

    // timer configuration
    err = g_timer.p_api->open(g_timer.p_ctrl,g_timer.p_cfg);
    APP_ERROR_TRAP(err)

    // int I2C Master driver configuration
    err    = g_int_i2c_bus.p_api->open(g_int_i2c_bus.p_ctrl, g_int_i2c_bus.p_cfg);
    APP_ERROR_TRAP(err)

    // ext I2C Master driver configuration
    err    = g_ext_i2c_bus.p_api->open(g_ext_i2c_bus.p_ctrl, g_ext_i2c_bus.p_cfg);
    APP_ERROR_TRAP(err)



    // set internal i2c bus for EN210
     err = g_int_i2c_bus_slave_addr_set(EN210_I2C_ADDRESS);
     APP_ERROR_TRAP(err)

     err = g_int_i2c_bus_write((uint8_t * const) ENS210_init_seq, sizeof(ENS210_init_seq), false);
     APP_ERROR_TRAP(err)

     err = readstatus (&partid);
     APP_ERROR_TRAP(err)


     // set external i2c bus for OLED display
     err = g_ext_i2c_bus_slave_addr_set(SSD1306_I2C_ADDRESS);
     APP_ERROR_TRAP(err)

     // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
     oled_begin(SSD1306_SWITCHCAPVCC);  // initialize with the I2C addr 0x3D (for the 128x64)
     clearBuffer(); // clears the screen and buffer
     oled_display(); // draw the new image.

     while (1) {
         err = en210_get_temp_humd(&g_temperatureC, &g_temperatureF, &g_humidityP);
         APP_ERROR_TRAP(err)

         delay (20);

         // set i2c for OLED display operation
         err = g_ext_i2c_bus_slave_addr_set(SSD1306_I2C_ADDRESS);
         APP_ERROR_TRAP(err)

         // Display "TEMP="
         for (uint8_t i=0; i<5; i++){
              writefontchar(8*i+(HISTXPOS-1), 0, i+1);
         }
         // Clean the old values
         for (uint8_t i=0; i<3; i++){
              writefontchar(40+8*i+(HISTXPOS-1), 0, 0);
         }
         displayNumber(40+(HISTXPOS-1), 0, g_temperatureF);

         drawverticalline(HISTXPOS-1,HISTYPOS-(1+HISTHEIGHT+1),HISTHEIGHT+1);
         drawhorizontalline(HISTXPOS-1,HISTYPOS-1,HISTWIDTH+1);

         for (uint8_t i=0; i< HISTHEIGHT/10+1; i++) {
              drawADot2(HISTXPOS-2, HISTYPOS-1-i*10, 1);
              drawADot2(HISTXPOS-3, HISTYPOS-1-i*10, 1);
              drawADot2(HISTXPOS-4, HISTYPOS-1-i*10, 1);
              displayNumber (0,HISTYPOS-1-i*10-4, MINTEMPHISTF + i * 10 );
         }

         displayHistogram(xhist,g_temperatureF);
         if (xhist < SSD1306_LCDWIDTH){
              xhist++;
         } else {
              xhist = HISTXPOS;
         }
         oled_display(); // draw the new image.
      }
 }

void delay(timer_size_t delaytimeMS) {
    timer_size_t counts;

    g_timer.p_api->reset(g_timer.p_ctrl);
    while (1)
    {
          g_timer.p_api->counterGet(g_timer.p_ctrl, &counts);

          // reaching the
          if (counts > delaytimeMS*COUNTS_PER_MILLISECOND) {
              break;
          }
    }
}
