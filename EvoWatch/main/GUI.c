/*
 * GUI.c
 *
 *  Created on: 7 mars 2021
 *      Author: wasso
 */

#include "GUI.h"
#include "Images.h"

extern Time CurrentTime;
Weather weather;

static lv_style_t TextStyle, TopbarStyle, TabStyle , CompassText;
static lv_color_t TextColor = LV_COLOR_MAGENTA;
lv_obj_t *tabview;

static QueueHandle_t  Button_Queue = NULL;
static void IRAM_ATTR gpio_button_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(Button_Queue, &gpio_num, NULL);
}
//LV_IMG_DECLARE(wifi);


void GUI_MainTask(void *pvParameter) {

    lv_init();
    lvgl_driver_init();

    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

    /*display buffer*/
    static lv_disp_buf_t disp_buf;
    lv_color_t* buf1 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1 != NULL);
    lv_color_t* buf2 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf2 != NULL);
    lv_disp_buf_init(&disp_buf, buf1, buf2, DISP_BUF_SIZE);    /*Initialize the display buffer*/

    /*copy the rendered image to the display*/
    lv_disp_drv_t disp_drv;               /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);          /*Basic initialization*/
    disp_drv.flush_cb = disp_driver_flush;    /*Set your driver function*/
    disp_drv.buffer = &disp_buf;          /*Assign the buffer to the display*/
    lv_disp_drv_register(&disp_drv);      /*Finally register the driver*/

    //Button interruptions
	gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    io_conf.pin_bit_mask = GPIO_BUTTON_PIN_SEL;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    Button_Queue = xQueueCreate(10, sizeof(uint32_t));
	xTaskCreate(GUI_Handle_Button, "Change_screen", 2048, NULL, 10, NULL);
	gpio_install_isr_service(0);
	gpio_isr_handler_add(GPIO_BUTTON,gpio_button_isr_handler, (void*) GPIO_BUTTON);

    /*styles*/
    GUI_Styles();
    tabview = lv_tabview_create(lv_scr_act(), NULL);
    lv_tabview_set_anim_time(tabview, 9000 / portTICK_PERIOD_MS);
    lv_tabview_set_btns_pos(tabview, LV_TABVIEW_TAB_POS_NONE);
    xTaskCreate(GUI_TopbarTask, "Top_Bar", 1024*2, (void *)0, 10, NULL);
    xTaskCreate(GUI_clock_screen,"clock_screen",2048*2,(void *)0,9,NULL);
    while(lv_tabview_get_tab_count(tabview) == 0) vTaskDelay(100 / portTICK_PERIOD_MS);
    xTaskCreate(GUI_calendar_screen, "Calendar", 1024*2, (void *)0, 8, NULL);
    while(lv_tabview_get_tab_count(tabview) == 1) vTaskDelay(100 / portTICK_PERIOD_MS);
    xTaskCreate(GUI_SensorsTask, "Sensor_Screen", 4096*2, (void *)0, 8, NULL);
    while(lv_tabview_get_tab_count(tabview) == 2) vTaskDelay(100 / portTICK_PERIOD_MS);
    xTaskCreate(GUI_compass, "compass", 1024*2, (void *)0, 9, NULL);
    while(lv_tabview_get_tab_count(tabview) == 3) vTaskDelay(100 / portTICK_PERIOD_MS);
    xTaskCreate(GUI_weather, "weather", 4096*2, (void *)0, 1, NULL);

    /*const esp_timer_create_args_t weather_timer_args = {
            .callback = &weather_cb,
            .name = "weather_timer"
	};
	esp_timer_handle_t weather_timer;
	ESP_ERROR_CHECK(esp_timer_create(&weather_timer_args, &weather_timer));
	ESP_ERROR_CHECK(esp_timer_start_periodic(weather_timer,10000000/portTICK_PERIOD_MS));*/

    while (1) {
        /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
        vTaskDelay(pdMS_TO_TICKS(10));
        lv_task_handler();
    }

    free(buf1);
    free(buf2);
    vTaskDelete(NULL);
}

void lv_tick_task(void *arg) {
    (void) arg;

    lv_tick_inc(LV_TICK_PERIOD_MS);
}

void GUI_SensorsTask(void *arg)
{
	{
	    /*Create a Tab view object*/

	    lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "Sensors");

		lv_obj_t * temp = lv_img_create(tab2, NULL);
		lv_img_set_src(temp, &temperature);
		lv_obj_align(temp, tab2, LV_ALIGN_IN_TOP_LEFT, 5, 20);
		lv_obj_t *T = lv_label_create(tab2, NULL);
		char tempbuff[100];

		lv_obj_align(T, tab2, LV_ALIGN_IN_TOP_LEFT, 88, 55);

		lv_obj_t * Hum = lv_img_create(tab2, NULL);
		lv_img_set_src(Hum, &humidite);
		lv_obj_align(Hum, tab2, LV_ALIGN_IN_LEFT_MID, 3, 50);
		lv_obj_t *H = lv_label_create(tab2, NULL);
		char humbuff[100];
		lv_obj_align(H, tab2, LV_ALIGN_IN_LEFT_MID, 88, 55);

		lv_obj_t * Press = lv_img_create(tab2, NULL);
		lv_img_set_src(Press, &pression);
		lv_obj_align(Press, tab2, LV_ALIGN_CENTER, 40, 45);
		lv_obj_t *P = lv_label_create(tab2, NULL);
		char pressbuff[100];
		lv_obj_align(P, tab2, LV_ALIGN_CENTER, 100, 55);

		lv_obj_t * currentweather = lv_img_create(tab2, NULL);
		lv_img_set_src(currentweather, &d01);
		lv_obj_align(currentweather, tab2, LV_ALIGN_IN_TOP_MID, 50, 20);

		lv_obj_t *TemperatureW = lv_label_create(tab2, NULL);
		lv_obj_align(TemperatureW, tab2, LV_ALIGN_IN_TOP_MID, 103, 27);

		lv_obj_t *TemperatureDES = lv_label_create(tab2, NULL);
		lv_obj_align(TemperatureDES, tab2, LV_ALIGN_IN_TOP_MID, 73, 80);

		lv_obj_t *Temperatureminmax = lv_label_create(tab2, NULL);
		lv_obj_align(Temperatureminmax, tab2, LV_ALIGN_IN_TOP_MID, 60, 65);


		char Tbuff[100]={0};


		while (1) {
			/* Delay 1 tick (assumes FreeRTOS tick is 10ms */
			vTaskDelay(1000 / portTICK_PERIOD_MS);
			sprintf(tempbuff," %0.2f %cC",temperature_degC,0xB0);
			lv_label_set_text(T, tempbuff);
		    lv_obj_add_style(T, LV_OBJ_PART_MAIN, &CompassText);

			sprintf(humbuff," %0.0f %%",weather.humidity);
			lv_label_set_text(H, humbuff);
		    lv_obj_add_style(H, LV_OBJ_PART_MAIN, &CompassText);

			sprintf(pressbuff," %0.0f hPa", Pressure_hPa);
			lv_label_set_text(P, pressbuff);
			lv_obj_add_style(P, LV_OBJ_PART_MAIN, &CompassText);

			if(!strcmp(weather.icon,"01d")) lv_img_set_src(currentweather, &d01);
			    			else if(!strcmp(weather.icon,"01n")) lv_img_set_src(currentweather, &n01);
			    			else if(!strcmp(weather.icon,"02d")) lv_img_set_src(currentweather, &d02);
			    			else if(!strcmp(weather.icon,"02n")) lv_img_set_src(currentweather, &n02);
			    			else if(!strcmp(weather.icon,"03d")) lv_img_set_src(currentweather, &d03);
			    			else if(!strcmp(weather.icon,"03n")) lv_img_set_src(currentweather, &d03);
			    			else if(!strcmp(weather.icon,"04d")) lv_img_set_src(currentweather, &d04);
			    			else if(!strcmp(weather.icon,"04n")) lv_img_set_src(currentweather, &d04);
			    			else if(!strcmp(weather.icon,"09d")) lv_img_set_src(currentweather, &d09);
			    			else if(!strcmp(weather.icon,"09n")) lv_img_set_src(currentweather, &d09);
			    			else if(!strcmp(weather.icon,"10d")) lv_img_set_src(currentweather, &d10);
			    			else if(!strcmp(weather.icon,"10n")) lv_img_set_src(currentweather, &n10);
			    			else if(!strcmp(weather.icon,"11d")) lv_img_set_src(currentweather, &d11);
			    			else if(!strcmp(weather.icon,"11n")) lv_img_set_src(currentweather, &d11);
			    			else if(!strcmp(weather.icon,"13d")) lv_img_set_src(currentweather, &d13);
			    			else if(!strcmp(weather.icon,"13n")) lv_img_set_src(currentweather, &d13);
			    			else if(!strcmp(weather.icon,"50d")) lv_img_set_src(currentweather, &d50);
			    			else if(!strcmp(weather.icon,"50n")) lv_img_set_src(currentweather, &d50);

			sprintf(Tbuff," %0.2f C", weather.temp - 273 );
			lv_label_set_text(TemperatureW, Tbuff);
			lv_obj_add_style(TemperatureW, LV_OBJ_PART_MAIN, &CompassText);

			sprintf(Tbuff, weather.description );
			lv_label_set_text(TemperatureDES, Tbuff);
			lv_obj_add_style(TemperatureDES, LV_OBJ_PART_MAIN, &CompassText);

			sprintf(Tbuff," %0.1f / %0.1f C", weather.temp_max- 273,weather.temp_min- 273  );
			lv_label_set_text(Temperatureminmax, Tbuff);
			lv_obj_add_style(Temperatureminmax, LV_OBJ_PART_MAIN, &CompassText);

		}

	}



}

void GUI_TopbarTask()
{
	uint8_t FirstTime = 1;
	//container
	lv_obj_t * TopBar;
	TopBar = lv_cont_create(lv_scr_act(), NULL);
	lv_obj_align(TopBar, NULL, LV_ALIGN_IN_TOP_LEFT, -5, -5);
	lv_obj_set_width(TopBar, SCREEN_WIDTH + 10);
	lv_obj_add_style(TopBar, LV_OBJ_PART_MAIN, &TopbarStyle);
	lv_obj_set_height(TopBar, 30);

	//time
	lv_obj_t *time = lv_label_create(TopBar, NULL);
	char timebuff[20];
	lv_obj_align(time, NULL, LV_ALIGN_IN_LEFT_MID, 5, 2);

	//date
	lv_obj_t *date = lv_label_create(TopBar, NULL);
	lv_label_set_align(date, LV_LABEL_ALIGN_RIGHT);
	char datebuff[20];
	lv_obj_align(date, NULL, LV_ALIGN_IN_RIGHT_MID, -56, 2);

	//WiFi Icon

	lv_obj_t * WifiIcon = lv_img_create(TopBar, NULL);

	lv_obj_set_size(WifiIcon, 25, 25);
	lv_img_set_auto_size(WifiIcon, true);

	while(1)
	{
		if (WiFiConnected && FirstTime)
		{
			//NTP_GetTime();
			FirstTime = 0;
		}

		sprintf(timebuff," %02d:%02d", CurrentTime.hours,CurrentTime.minutes);
		lv_label_set_text(time, timebuff);

		sprintf(datebuff,"%02d/%02d/20%02d ", CurrentTime.date, CurrentTime.months + 1, CurrentTime.years);
				lv_label_set_text(date, datebuff);

		if(WiFiConnected) lv_img_set_src(WifiIcon, &wifi);
		else lv_img_set_src(WifiIcon, &ncwifi);
		lv_obj_align(WifiIcon, TopBar, LV_ALIGN_CENTER, 0, 0);

		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void GUI_clock_screen()
{
    lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "Clock");
    lv_obj_add_style(tabview, LV_OBJ_PART_MAIN, &TabStyle);
    //lv_obj_t *tab3 = lv_tabview_add_tab(tabview, "Compass");

    lv_obj_set_y(tabview, 15);
    /*Add content to the tabs*/
	lv_obj_t * clock = lv_img_create(tab1, NULL);
	lv_img_set_src(clock, &Montre);
	lv_obj_align(clock, tab1, LV_ALIGN_CENTER, 0, 0);



	lv_obj_t * min = lv_img_create(clock, NULL);
	lv_img_set_src(min, &minute);
	lv_obj_align(min, tab1, LV_ALIGN_CENTER, 0, 0);

	lv_obj_t * h = lv_img_create(clock, NULL);
	lv_img_set_src(h, &heure);
	lv_obj_align(h, tab1, LV_ALIGN_CENTER, 0, 0);

	lv_obj_t * sec = lv_img_create(clock, NULL);
	lv_img_set_src(sec, &seconde);
	lv_obj_align(sec, tab1, LV_ALIGN_CENTER, 0, 0);

	while(1)
	{
		lv_img_set_angle(sec, 3600*(float)CurrentTime.seconds/(float)60);
		lv_img_set_angle(min, 3600*(float)CurrentTime.minutes/(float)60);
		lv_img_set_angle(h, 3600*((float)(CurrentTime.hours+(float)CurrentTime.minutes/60)/(float)12));
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void GUI_Handle_Button()
{
    uint32_t io_num;
    for(;;)
    {
        if(xQueueReceive(Button_Queue, &io_num, portMAX_DELAY))
        {
        	if(lv_tabview_get_tab_act(tabview) + 1 >= lv_tabview_get_tab_count(tabview))
        		lv_tabview_set_tab_act(tabview, 0, LV_ANIM_ON);
        	else
        		lv_tabview_set_tab_act(tabview, lv_tabview_get_tab_act(tabview) + 1, LV_ANIM_ON);
        }
    }
}
void GUI_compass()
{
    lv_obj_t *tab4 = lv_tabview_add_tab(tabview, "Clock");
    lv_obj_set_y(tabview, 15);

    /*Add content to the tabs*/
	lv_obj_t * compass_ = lv_img_create(tab4, NULL);
	lv_img_set_src(compass_, &compass);
	lv_obj_align(compass_, tab4, LV_ALIGN_CENTER, 0, 0);

	lv_obj_t * border_ = lv_img_create(tab4, NULL);
	lv_img_set_src(border_, &border);
	lv_obj_align(border_, tab4, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * orientation=lv_label_create(tab4, NULL);
    lv_obj_set_size(orientation, 500, 100);
    lv_obj_align(orientation, tab4, LV_ALIGN_CENTER, 10, 0);
    lv_obj_add_style(orientation, LV_OBJ_PART_MAIN, &CompassText);
	lv_label_set_text(orientation,"N");
	while(1)
	{
		int angle = (int)(180 * MagnetoAngle/M_PI);
		lv_img_set_angle(compass_, angle*10);

		switch(angle)
		{
		case -2 ... 2:
			lv_label_set_text(orientation,"N");
			break;
		case 3 ... 87:
			lv_label_set_text(orientation,"NE");
			break;
		case 88 ... 92:
			lv_label_set_text(orientation, "E");
			break;
		case 93 ... 177:
			lv_label_set_text(orientation, "SE");
			break;
		case 178 ... 180:
			lv_label_set_text(orientation, "S");
			break;
		case -180 ... -178:
			lv_label_set_text(orientation, "S");
			break;
		case -177 ... -93:
			lv_label_set_text(orientation, "SW");
			break;
		case -92 ... -88:
			lv_label_set_text(orientation, "W");
			break;
		case -87 ... -3:
			lv_label_set_text(orientation, "NW");
			break;
		default:
			lv_label_set_text(orientation, "3asba");
			break;
		}
		lv_obj_align(orientation, tab4, LV_ALIGN_CENTER, 0, 0);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void GUI_calendar_screen()
{


	lv_obj_t *tab3 = lv_tabview_add_tab(tabview, "Compass");

    lv_obj_t  * calendar = lv_calendar_create(tab3, NULL);
    lv_obj_set_size(calendar, 200, 200);
    lv_obj_align(calendar, NULL, LV_ALIGN_CENTER, 0, 0);

    /*Make the date number smaller to be sure they fit into their area*/
    lv_obj_set_style_local_text_font(calendar, LV_CALENDAR_PART_DATE, LV_STATE_DEFAULT, lv_theme_get_font_small());

    /*Set today's date*/
    lv_calendar_date_t today;
    while(1)
    	{
    	    today.year =  CurrentTime.years+2000;
    	    today.month =  CurrentTime.months+1;
    	    today.day = CurrentTime.date;

			lv_calendar_set_today_date(calendar, &today);
			lv_calendar_set_showed_date(calendar, &today);

			vTaskDelay(3600000/ portTICK_PERIOD_MS);
    	}
}

void GUI_weather()
{
	    lv_obj_t *tab5 = lv_tabview_add_tab(tabview, "Weather");
		lv_obj_t * step = lv_img_create(tab5, NULL);
		lv_img_set_src(step, &steps);
		lv_obj_align(step, tab5, LV_ALIGN_CENTER, 0, 0);
		lv_obj_t *S = lv_label_create(tab5, NULL);
		char stepbuff[100];
		lv_obj_align(S, tab5, LV_ALIGN_CENTER, 0, 50);
    while(1)
    	{
			sprintf(stepbuff," %d ",steps_count);
			lv_label_set_text(S, stepbuff);
		    lv_obj_add_style(S, LV_OBJ_PART_MAIN, &CompassText);
			vTaskDelay(1000/ portTICK_PERIOD_MS);
    	}
}
void GUI_Styles()
{
	lv_style_init(&TextStyle);
	lv_style_set_text_color(&TextStyle, LV_STATE_DEFAULT, TextColor);

	lv_style_init(&TopbarStyle);
	lv_style_set_text_color(&TopbarStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_bg_color(&TopbarStyle, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	//lv_style_set_bg_opa(&TopbarStyle, LV_STATE_DEFAULT, 125);

	lv_style_init(&TabStyle);
	lv_style_set_bg_color(&TabStyle, LV_STATE_DEFAULT, LV_COLOR_BLACK);

	lv_style_init(&CompassText);
	lv_style_set_text_color(&CompassText, LV_STATE_DEFAULT, LV_COLOR_WHITE);

}
