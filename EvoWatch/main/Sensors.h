/*
 * Sensors.h
 *
 *  Created on: 4 mars 2021
 *      Author: wasso
 */

#ifndef MAIN_SENSORS_H_
#define MAIN_SENSORS_H_

#include "stts751_reg.h"
#include "lps22hh_reg.h"
#include "lsm6dso_reg.h"
#include "lis2mdl_reg.h"
#include "i2c_init.h"

#include "string.h"
#include "math.h"


#define STTS751_ADDR_7BITS 0x4a
#define LPS22H_ADDR_7BITS 0x5D
#define LSM6DSO_ADDR_7BITS 0x6B
#define LIS2mdl_ADD_7BITS 0x1E


typedef union{
  int16_t i16bit;
  uint8_t u8bit[2];
} axis1bit16_t;

typedef union{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} axis3bit16_t;


extern const char *TAG;
extern TaskHandle_t TempHandle;
extern TaskHandle_t PressHandle;
extern TaskHandle_t MovementHandle;

esp_err_t i2c_master_init(void);

//Temperature Vars
extern float temperature_degC;
//stts751_id_t Temp_Sensor;
static stmdev_ctx_t dev_temp;
//axis1bit16_t data_raw_temperature;

//Temperature Functions
int32_t i2c_temp_read(uint8_t i2c_num, uint8_t regaddr, uint8_t *data_rd, uint16_t size);
int32_t i2c_temp_write(uint8_t i2c_num, uint8_t regaddr, uint8_t *data_wr, uint16_t size);
void get_temp_task(void *args);
void Temperature_task(void *args);
void Temp_Init(stmdev_ctx_t* dev_temp);

//Pressure Vars
extern float Pressure_hPa;
//uint8_t Press_Sensor;
static stmdev_ctx_t dev_press;
//uint32_t data_raw_press;

//Pressure Functions
int32_t i2c_press_read(uint8_t i2c_num, uint8_t regaddr, uint8_t *data_rd, uint16_t size);
int32_t i2c_press_write(uint8_t i2c_num, uint8_t regaddr, uint8_t *data_wr, uint16_t size);
void get_press_task(void *args);
void Pressure_task(void *args);
void Pressure_Init(stmdev_ctx_t* dev_press);

//Accelerometer & Gyro Vars
//uint8_t Movement_Sensor;
static stmdev_ctx_t dev_Movement;
//axis3bit16_t data_raw_acceleration;
//axis3bit16_t data_raw_angular_rate;
extern float acceleration_mg[3];
extern float angular_rate_mdps[3];
extern uint16_t steps_count;

//Accelerometer & Gyr Functions
int32_t i2c_move_read(uint8_t i2c_num, uint8_t regaddr, uint8_t *data_rd, uint16_t size);
int32_t i2c_move_write(uint8_t i2c_num, uint8_t regaddr, uint8_t *data_wr, uint16_t size);
void Movement_task(void *args);
void Movement_Init(stmdev_ctx_t* dev_move);

//Magneto Vars
static stmdev_ctx_t dev_Magneto;
extern float MagnetoAngle;

//Magneto Functions
int32_t i2c_magneto_read(uint8_t i2c_num, uint8_t regaddr, uint8_t *data_rd, uint16_t size);
int32_t i2c_magneto_write(uint8_t i2c_num, uint8_t regaddr, uint8_t *data_wr, uint16_t size);
void Magnetometer_task(void *args);
void Magneto_Init(stmdev_ctx_t* dev_move);

#endif /* MAIN_SENSORS_H_ */
