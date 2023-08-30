
// matiene conectado a la conexion WIFI y nodered
void keepWiFiAlive(void * parameter){
    
    for(;;){

        vTaskDelay(1000 / portTICK_PERIOD_MS);

        while (WiFi.status() != WL_CONNECTED) {

            ssid_list=!ssid_list;
            ssid = ssidX[ssid_list];

            int countdown;
            countdown=millis();     // hay que utilizar milis, porque sin Wifi no Devicesa time(NULL).
            Serial.print("\nconnecting to wifi-" + (String) ssid_list);
            
            WiFi.begin(ssid, password);

            while (WiFi.status() != WL_CONNECTED && (millis() - countdown) < 20000) {
                Serial.print(".");
                for (size_t i = 0; i < 30; i++) {delayMicroseconds(10000);}
                if (WiFi.status() == WL_CONNECTED) {  
                    WiFi.hostname(hostname); 
                    Serial.println(WiFi.SSID());
                    Serial.println(WiFi.localIP());
                }
            } 

            if(now-time_mqtt_live > 120) { 
                time_mqtt_live=now;
                delay(2000);
                ESP.restart();
            }
        }
    }
}

