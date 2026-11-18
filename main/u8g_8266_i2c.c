/*
 * esp8266_i2c.c
 *
 *  Created on: Nov 16, 2026
 *      Author: vov
 */

#include <stdio.h>
#include <string.h>

#include <sdkconfig.h>
#include <esp_log.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "u8g_8266_I2c.h"

static const char *TAG = "u8g2_hal";



//static i2c_cmd_handle_t    handle_i2c;      // I2C handle.


#undef ESP_ERROR_CHECK
#define ESP_ERROR_CHECK(x)   do { esp_err_t rc = (x); if (rc != ESP_OK) { ESP_LOGE("err", "esp_err_t = %d", rc); assert(0 && #x);} } while(0);


 static  esp_err_t i2c_init()
 {
     int i2c_master_port = I2C_MASTER_NUM;
     i2c_config_t conf;

     conf.mode = I2C_MODE_MASTER;
     conf.sda_io_num = I2C_SDA;
     conf.sda_pullup_en = 1;
     conf.scl_io_num =  I2C_SCL;
     conf.scl_pullup_en = 1;
     conf.clk_stretch_tick = I2C_CLC_STRETCH; // 150 ticks, Clock stretch is about 100us, you can make changes according to the actual situation.
     ESP_ERROR_CHECK(i2c_driver_install(i2c_master_port, conf.mode));
     ESP_ERROR_CHECK(i2c_param_config(i2c_master_port, &conf));
     return ESP_OK;
 }




/*
 * HAL callback function as prescribed by the U8G2 library.  This callback is invoked
 * to handle I2C communications.
 */
uint8_t u8g2_esp8266_i2c_byte_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
	ESP_LOGD(TAG, "i2c_cb: Received a msg: %d, arg_int: %d, arg_ptr: %p", msg, arg_int, arg_ptr);

	switch(msg) {

		case U8X8_MSG_BYTE_INIT: {
			i2c_init();  //init I2C
		break;
		}

		case U8X8_MSG_BYTE_SEND:
		{
			uint8_t* data_ptr = (uint8_t*)arg_ptr;
			while( arg_int > 0 ) {

			   ESP_ERROR_CHECK(i2c_master_write_byte(handle_i2c, *data_ptr, ACK_CHECK_EN));
			   data_ptr++;
			  arg_int--;
			}
			break;
		}

		case U8X8_MSG_BYTE_START_TRANSFER: {

			handle_i2c = i2c_cmd_link_create();
			ESP_LOGD(TAG, "Start I2C transfer to %02X.", u8x8_GetI2CAddress(u8x8)>>1);

			ESP_ERROR_CHECK(i2c_master_start(handle_i2c));
			u8x8->i2c_started=1;
			ESP_ERROR_CHECK(i2c_master_write_byte(handle_i2c,(( 0x3c<<1) | I2C_MASTER_WRITE), ACK_CHECK_EN));

			break;
		}

		case U8X8_MSG_BYTE_END_TRANSFER: {
			if(u8x8->i2c_started)
			{ESP_ERROR_CHECK(i2c_master_stop(handle_i2c));
			ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_MASTER_NUM, handle_i2c, 1000/portTICK_PERIOD_MS));
			i2c_cmd_link_delete(handle_i2c);
			u8x8->i2c_started=0;}
			break;
		}
	}
	return 0;
} // u8g2_esp32_i2c_byte_cb

/*
 * HAL callback function as prescribed by the U8G2 library.  This callback is invoked
 * to handle callbacks for GPIO and delay functions.
 */
uint8_t u8g2_esp8266_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
	ESP_LOGD(TAG, "gpio_and_delay_cb: Received a msg: %d, arg_int: %d, arg_ptr: %p", msg, arg_int, arg_ptr);

	switch(msg) {
	// Initialize the GPIO and DELAY HAL functions.  If the pins for DC and RESET have been
	// specified then we define those pins as GPIO outputs.
		case U8X8_MSG_GPIO_AND_DELAY_INIT: {
#undef U8X8_PIN_RESET            //redefine reset pin;no such pin
#define	 U8X8_PIN_RESET U8X8_PIN_NONE;
		}
			break;

	// Delay for the number of milliseconds passed in through arg_int.
		case U8X8_MSG_DELAY_MILLI:
			{
			vTaskDelay(pdMS_TO_TICKS(arg_int));}
			break;

		case U8X8_MSG_DELAY_10MICRO:
			 os_delay_us(10);
			 break;
	}
	return 0;
} // u8g2_esp32_gpio_and_delay_cb
