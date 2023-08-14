
// matiene conectado a la conexion WIFI y nodered
void keepWiFiAlive(void * parameter){
    
    for(;;){

        vTaskDelay(3000 / portTICK_PERIOD_MS);

        if(WiFi.status()!=WL_CONNECTED) { 
            WiFi.disconnect(true); 
            for (size_t i = 0; i < 100; i++) {delayMicroseconds(10000);}
            WiFi_conect();
        }

        if(now-time_mqtt_live > 60) { 
            time_mqtt_live=now;
            delay(2000);
            ESP.restart();
        }

    }
}

