/* Simple HTTP + SSL Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>

#include "GUI.h"

extern TaskHandle_t WeatherTaskHandle;
const char *TAG = "Smart_Watch";


float temperature_degC;
float Pressure_hPa;
float acceleration_mg[3];
float angular_rate_mdps[3];
float MagnetoAngle;
Time CurrentTime;
uint16_t steps_count;
uint8_t WiFiConnected = 0;

TaskHandle_t TempHandle = NULL;
TaskHandle_t PressHandle = NULL;
TaskHandle_t MovementHandle = NULL;

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    //ESP_ERROR_CHECK(esp_netif_init());
    //ESP_ERROR_CHECK(esp_event_loop_create_default());
    //ESP_ERROR_CHECK(i2c_master_init());

  //  WiFi_Connect();

    //Temp_Init(&dev_temp);
    //Pressure_Init(&dev_press);
    //Movement_Init(&dev_Movement);
    //Magneto_Init(&dev_Magneto);

    //xTaskCreate(get_rtc_task, "RTC_Time_task", 1024 * 2, (void *)0, 20, NULL);
    /* start Temperature task */
	//xTaskCreate(Temperature_task, "i2c_Temp_task", 1024*2, (void *)0, 10, &TempHandle);
	/* start Pressure task */
	//xTaskCreate(Pressure_task, "i2c_Press_task", 1024*2, (void *)0, 10, &PressHandle);
	/* start Movement task */
	//xTaskCreate(Movement_task, "i2c_Movement_task", 2048*2, (void *)0, 10, &MovementHandle);
	/* start Magneto task */
	//xTaskCreate(Magnetometer_task, "i2c_Magneto_task", 1024*2, (void *)0, 10, &MovementHandle);
	/*GUI Task*/
	xTaskCreatePinnedToCore(GUI_MainTask, "gui", 16000*2, NULL, 15, NULL, 1);
	//xTaskCreate(Weather_Get, "Weather", 8192, (void *)0, 1, &WeatherTaskHandle);

}
