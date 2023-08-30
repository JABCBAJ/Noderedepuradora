// inicia WiFi y nodered

/*...........................................................................
    inicia WiFi */
void WiFi_conect() {
    
    // WiFi.mode(WIFI_STA);
    ssid_list=!ssid_list;
    ssid = ssidX[ssid_list];

    int countdown;
    Serial.print("\nconnecting to wifi-" + (String) ssid_list);
    countdown=millis();     // hay que utilizar milis, porque sin Wifi no Devicesa time(NULL).
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED && (millis() - countdown) < 20000) {
        Serial.print(".");
        for (size_t i = 0; i < 20; i++) {delayMicroseconds(10000);}
        if (WiFi.status() == WL_CONNECTED) {  WiFi.hostname(hostname); Serial.println(WiFi.SSID());}
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
        mqttclient.subscribe((TopicDevice+"/MOTOR").c_str() );
        mqttclient.subscribe((TopicDevice+"/CLORO").c_str() );
        mqttclient.subscribe((TopicDevice+"/FOCO").c_str() );
        mqttclient.subscribe((TopicDevice+"/TIMER").c_str() );
        // subscribe topics
        // ***************************************************
        flag_mqtt_ok=true;
    // }
    // else {
    //     // TelnetStream.print("Error de conexión MQTT, rc=");
    //     Serial.print("Error de conexión MQTT, rc=");
    //     // TelnetStream.print(mqttclient.state());
    //     Serial.print(mqttclient.state());
    //     // TelnetStream.println(" Reintentando en 5 segundos...");
    //     Serial.println(" Reintentando en 5 segundos...");
    //     delay(1000);
        
    //     WiFiClient mqttclient;
    //     if (mqttclient.connect("192.168.1.59", 1883)) {
    //     Serial.println("-- Conectado a DOMOTICA --");
    //     //mqttclient.stop();
    //     } else {Serial.println(" -- No se pudo conectar a Internet -- ");}

    //     if(WiFi.status()!=WL_CONNECTED) { WiFi_conect();}
    }
}
