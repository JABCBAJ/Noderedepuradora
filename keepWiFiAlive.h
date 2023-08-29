
// matiene conectado a la conexion WIFI y nodered
void keepWiFiAlive(void * parameter){
    
    for(;;){

        vTaskDelay(20000 / portTICK_PERIOD_MS);

        if(WiFi.status()!=WL_CONNECTED) { 
            //WiFi.disconnect(true); 
            //for (size_t i = 0; i < 100; i++) {delayMicroseconds(10000);}
            ssid_list = !ssid_list;
            WiFi_conect();
            Serial.print("wifi ssid");

        }

        if(now-time_mqtt_live > 60) { 
            time_mqtt_live=now;
            delay(2000);
            ESP.restart();
        }

    }
}

