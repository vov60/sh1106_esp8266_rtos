/*
 * esp8266_i2c.c
 *
 *  Created on: Nov 16, 2026
 *      Author: vov
 */

/**
 * display SH1106 ;
 * protocol I2C;
 * 4 pins:VCC,GND,SDA,SCK
 *ESP8266-07;1MB flash
 *
 */
#ifndef U8G_8266_I2C_H_
#define U8G_8266_I2C_H_
#include "u8g2.h"

#include <driver/gpio.h>

#include <driver/i2c.h>


#define U8G2_ESP32_HAL_UNDEFINED (-1)

#define I2C_MASTER_NUM              I2C_NUM_0         //  I2C port number for master dev
#define I2C_MASTER_TX_BUF_DISABLE   0     			 //  I2C master do not need buffer
#define I2C_MASTER_RX_BUF_DISABLE   0     			 //  I2C master do not need buffer
#define I2C_MASTER_STRETCH_TIME     150   			 //  I2C master clock frequency
#define ACK_CHECK_EN                0x1         	//  I2C master will check ack from slave
#define ACK_CHECK_DIS              0x0              //  I2C master will not check ack from slave
#define I2C_SCL                     5
#define I2C_SDA                     4
#define I2C_DISPLAY_ADR           (0x3C<<1)
#define I2C_CLC_STRETCH				150

i2c_cmd_handle_t    handle_i2c;

///**
// * must be initialized in user_main
// */
//typedef struct {
//	gpio_num_t clk;
//	gpio_num_t mosi;
//	gpio_num_t sda; // data for I²C
//	gpio_num_t scl; // clock for I²C
//	gpio_num_t cs;
//	gpio_num_t reset;
//	gpio_num_t dc;
//} u8g2_esp8266_hal_t ;

#define U8G2_ESP8266_HAL_DEFAULT {U8G2_ESP32_HAL_UNDEFINED, U8G2_ESP32_HAL_UNDEFINED, U8G2_ESP32_HAL_UNDEFINED, U8G2_ESP32_HAL_UNDEFINED, U8G2_ESP32_HAL_UNDEFINED, U8G2_ESP32_HAL_UNDEFINED, U8G2_ESP32_HAL_UNDEFINED }


uint8_t u8g2_esp8266_spi_byte_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8g2_esp8266_i2c_byte_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8g2_esp8266_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
#endif /*U8G_8266_I2C_H_ */
