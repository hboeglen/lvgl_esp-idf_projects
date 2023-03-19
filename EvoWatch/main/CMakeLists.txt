idf_component_register(SRCS "main.c" "GUI.c"  
"./components/lvgl_esp32_drivers/lvgl_tft/disp_driver.c"
                    INCLUDE_DIRS "." "components/lvgl" "components" "components/lvgl_esp32_drivers" "components/lvgl_esp32_drivers/lvgl_tft" 
                    EMBED_TXTFILES "certs/cacert.pem"
                                   "certs/prvtkey.pem")
