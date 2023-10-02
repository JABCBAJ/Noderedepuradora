// inicia WiFi y nodered

/*...........................................................................
    inicia WiFi */
void WiFi_conect() {
    
    // WiFi.mode(WIFI_STA);
    ssid_list=!ssid_list;
    ssid = ssidX[ssid_list];

    int countdown;
    countdown=millis();     // hay que utilizar milis, porque sin Wifi no Devicesa time(NULL).
    Serial.print("\nconnecting to wifi-" + (String) ssid_list);
    
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED && (millis() - countdown) < 9000) {
        Serial.print(".");
        for (size_t i = 0; i < 30; i++) {delayMicroseconds(10000);}
        if (WiFi.status() == WL_CONNECTED) {  
            WiFi.hostname(hostname); 
            Serial.println(WiFi.SSID());
            Serial.println(WiFi.localIP());
        }
    }
}
//...........................................................................
// Subscribe callback which is called when every packet has come
void callback(char* topic, byte* payload, unsigned int length) {
        receive_Topic    = topic;
        for (int i = 0; i < length; i++) {receive_Payload += (char)payload[i];} 
        receive_Payload  = receive_Payload;
}
//...........................................................................
// reconecta con MQTT
void reconnect() {
   
    Serial.println("Intentando conexión MQTT...");

    mqttclient.connect(hostname);

    if (mqttclient.connected()) {

        Serial.println("Conectado al servidor MQTT");
        // ***************************************************
        // subscribe topics
        mqttclient.subscribe((TopicDevice+"/Mqtt_Live").c_str() );
        mqttclient.subscribe((TopicDevice+"/motor").c_str() );
        mqttclient.subscribe((TopicDevice+"/cloro").c_str() );
        mqttclient.subscribe((TopicDevice+"/foco").c_str() );
        mqttclient.subscribe((TopicDevice+"/timer").c_str() );
        // subscribe topics
        // ***************************************************
    }
}
