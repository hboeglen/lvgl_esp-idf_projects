/*
 * Weather.h
 *
 *  Created on: 27 mars 2021
 *      Author: wasso
 */

#ifndef MAIN_WEATHER_H_
#define MAIN_WEATHER_H_

#include <string.h>
#include <stdlib.h>
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_tls.h"
//#include "mjson.h"

typedef struct {
	char description[30];
	char icon[5];
	double temp;
	double temp_min;
	double temp_max;
	double humidity;
}Weather;

#define MAX_HTTP_OUTPUT_BUFFER 2048



extern const char *TAG;

//esp_err_t _http_event_handler(esp_http_client_event_t *evt);
//void Weather_Get(void* args);
//void weather_cb(void *arg);

#endif /* MAIN_WEATHER_H_ */
