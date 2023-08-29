// inicia WiFi y nodered

/*...........................................................................
    inicia WiFi */
void WiFi_conect() {
    
    // WiFi.mode(WIFI_STA);
    ssid = ssidX[!ssid_list];

    int countdown;
    Serial.print("\nconnecting to wifi1...");
    countdown=millis();     // hay que utilizar milis, porque sin Wifi no Devicesa time(NULL).
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED && (millis() - countdown) < 3000) {
        Serial.print(".");
        delay(200);
        if (WiFi.status() == WL_CONNECTED) {  WiFi.hostname(hostname);return; }
    }
    ssid = ssidX[!ssid_list];

    Serial.print("\nconnecting to wifi2...");
    countdown=millis();     // hay que utilizar milis, porque sin Wifi no Devicesa time(NULL).
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED && (millis() - countdown) < 3000) {
        Serial.print(".");
        delay(200);
        if (WiFi.status() == WL_CONNECTED) {  WiFi.hostname(hostname);return; }
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
    while (!client.connected()) {
        Serial.println("Intentando conexión MQTT...");

        if (client.connect(hostname)) {
            
            Serial.println("Conectado al servidor MQTT");
            // ***************************************************
            // subscribe topics
            client.subscribe((TopicDevice+"/Mqtt_Live").c_str() );
            client.subscribe((TopicDevice+"/MOTOR").c_str() );
            client.subscribe((TopicDevice+"/CLORO").c_str() );
            client.subscribe((TopicDevice+"/FOCO").c_str() );
            client.subscribe((TopicDevice+"/TIMER").c_str() );
            // subscribe topics
            // ***************************************************
            flag_mqtt_ok=true;
        }
        else {
            // TelnetStream.print("Error de conexión MQTT, rc=");
            Serial.print("Error de conexión MQTT, rc=");
            // TelnetStream.print(client.state());
            Serial.print(client.state());
            // TelnetStream.println(" Reintentando en 5 segundos...");
            Serial.println(" Reintentando en 5 segundos...");
            delay(5000);
        }
    }
}
