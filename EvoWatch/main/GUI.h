/*
 * GUI.h
 *
 *  Created on: 7 mars 2021
 *      Author: wasso
 */

#ifndef MAIN_GUI_H_
#define MAIN_GUI_H_

#include "lvgl.h"
#include "disp_driver.h"
#include "lvgl_helpers.h"
#include "RTC.h"
#include "Weather.h"

#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include "esp_timer.h"
#include "driver/gpio.h"
#include "freertos/queue.h"
#include "math.h"
#include "string.h"

#define LV_TICK_PERIOD_MS 1
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define GPIO_BUTTON     0
#define GPIO_BUTTON_PIN_SEL  (1ULL<<GPIO_BUTTON)

void lv_tick_task(void *arg);
void GUI_MainTask(void *pvParameter);
void GUI_SensorsTask(void *arg);
void GUI_TopbarTask();
void GUI_Styles();
void GUI_clock_screen();
void GUI_Handle_Button();
void GUI_compass();
void GUI_calendar_screen();
void GUI_weather();

extern float temperature_degC;
extern float Pressure_hPa;
extern uint8_t WiFiConnected;
extern float MagnetoAngle;
extern uint16_t steps_count;
#endif /* MAIN_GUI_H_ */
