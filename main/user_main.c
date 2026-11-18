/*
 * esp8266_i2c.c
 *
 *  Created on: Nov 16, 2026
 *      Author: vov
 */
/* I2C example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>

#include <freertos/task.h>
#include "freertos/queue.h"

#include <esp_log.h>
#include <esp_system.h>
#include <esp_err.h>

#include <driver/i2c.h>
#include "u8g_8266_I2c.h"
#include "rom/ets_sys.h"
static const char *TAG = "main";

/**
 * TEST CODE BRIEF
 *
 * This example will show you how to use I2C module by running  tasks on i2c bus:
 * init and write data to SH1106 display
 *
 *
 * Pin assignment:
 *
 * - master:
 *    GPIO4 is assigned as the data signal of i2c master port
 *    GPIO5 is assigned as the clock signal of i2c master port
 *
 * Connection:
 *
 * - connect sda/scl of sensor with GPIO4/GPIO5
 * - no need to add external pull-up resistors, driver will enable internal pull-up resistors.
 *
 * Test items:
 *
 * draw  string and box
 */



/**
 * @brief i2c master initialization
 */
//static esp_err_t i2c_init()
//{
//    int i2c_master_port = I2C_MASTER_NUM;
//    i2c_config_t conf;
//    conf.mode = I2C_MODE_MASTER;
//    conf.sda_io_num = I2C_SDA;
//    conf.sda_pullup_en = 1;
//    conf.scl_io_num =  I2C_SCL;
//    conf.scl_pullup_en = 1;
//    conf.clk_stretch_tick = 150; // 300 ticks, Clock stretch is about 210us, you can make changes according to the actual situation.
//    ESP_ERROR_CHECK(i2c_driver_install(i2c_master_port, conf.mode));
//    ESP_ERROR_CHECK(i2c_param_config(i2c_master_port, &conf));
//    return ESP_OK;
//}




static void i2c_task_example(void *arg)

{
	/**
	 * uncoment to see logs
	 */
	//esp_log_level_set(TAG, ESP_LOG_DEBUG);



	u8g2_t u8g2;

	/**
	 * init u8g2
	 * u8g2_Setup_sh1106_i2c_128x64_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation,
	 * 										 u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
	 *
	 *u8g2_Setup_sh1106_i2c_128x64_noname_2(&u8g2, U8G2_R0, u8g2_esp8266_i2c_byte_cb, u8g2_esp8266_gpio_and_delay_cb);
	 */
	u8g2_Setup_sh1106_i2c_128x64_noname_1(&u8g2, U8G2_R0,  u8g2_esp8266_i2c_byte_cb, u8g2_esp8266_gpio_and_delay_cb);
	//u8g2.u8x8.i2c_address= I2C_DISPLAY_ADR;
	u8x8_SetI2CAddress(&u8g2.u8x8, I2C_DISPLAY_ADR);
	 ESP_LOGD(TAG, "u8g2_InitDisplay");
	 ESP_LOGI(TAG, "i2c_bus_clock_100kH=%d",u8g2.u8x8.display_info->i2c_bus_clock_100kHz);

	 	u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,

	 	ESP_LOGD(TAG, "u8g2_SetPowerSave");

	 	u8g2_SetPowerSave(&u8g2, 0); // wake up
	 	u8g2_ClearDisplay(&u8g2);

	 	ESP_LOGD(TAG, "u8g2_SetFont");
	 	ESP_LOGI(TAG, "u8g2_DrawStr");
	 		 u8g2_FirstPage(&u8g2);
	 		    	  do {
	 		    		 u8g2_SetFont(&u8g2,u8g2_font_8x13_mf);
	 		    		u8g2_DrawStr(&u8g2, 2,17,"Hi VOVAN!");
	 		    		 u8g2_DrawBox(&u8g2,5,25,64,35);
	 		    	  } while ( u8g2_NextPage(&u8g2) );
	 		ESP_LOGI(TAG, "u8g2_SendBuffer");
TickType_t delay3s=3000;
	 		vTaskDelay(pdMS_TO_TICKS(delay3s));
	 		u8g2_ClearBuffer(&u8g2);
	 		u8g2_ClearDisplay(&u8g2);
	 		uint8_t m = 24;uint8_t h=0;
    while (1) {
    	 char m_str[5];
    	  strcpy(m_str, u8x8_u8toa(h,2));

    	  strcat(m_str,":");
    	  strcat(m_str, u8x8_u8toa(m,2));/* convert m to a string with two digits */
       u8g2_FirstPage(&u8g2);
    	  do {
    		  u8g2_SetFont(&u8g2,u8g2_font_logisoso34_tr);
    		  u8g2_DrawStr(&u8g2,20,63,m_str);
    	   // u8g2_DrawStr(&u8g2,33,63,":");
    	  //  u8g2_DrawStr(&u8g2,50,63,m_str);
    	  } while ( u8g2_NextPage(&u8g2) );
       vTaskDelay(pdMS_TO_TICKS(1000));
    	   m++;
    	   if ( m == 60 )
    	   { m = 0;
    	   h++;
    	   if(h==60)
    	   h=0;}
    }
	 		vTaskDelete(NULL);
}

void app_main(void)
{ //i2c_init();

    xTaskCreate(i2c_task_example, "i2c_task_example", 4096, NULL, 10, NULL);
}
