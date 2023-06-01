// inicia WiFi y nodered

/*...........................................................................
    inicia WiFi */
void WiFi_conect() {
    int countdown;
    Serial.print("\nconnecting to wifi...");
    countdown=millis();     // hay que utilizar milis, porque sin Wifi no funciona time(NULL).
    while (WiFi.status() != WL_CONNECTED && (millis() - countdown) < 3000) {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        Serial.print(".");
        delay(Wifi_Reconect);
    }
}
//...........................................................................
// Subscribe callback which is called when every packet has come
void callback(char* topic, byte* payload, unsigned int length) {
        receive_Topic    = topic;
        for (int i = 0; i < length; i++) {receive_Payload += (char)payload[i];} 
        receive_Payload  = receive_Payload;
        Serial.print(receive_Topic);Serial.print("-");Serial.println(receive_Payload);
}
//...........................................................................
// reconecta con MQTT
void reconnect() {
    while (!client.connected()) {
        Serial.println("Intentando conexión MQTT...");

        if (client.connect("ESP32Client")) {
            
            Serial.println("Conectado al servidor MQTT");
            // ***************************************************
            // subscribe topics
            client.subscribe((TopicDevice+"/XXXXXXXXX").c_str() );
            client.subscribe((TopicDevice+"/XXXXXXXXX").c_str() );
            client.subscribe((TopicDevice+"/XXXXXXXXX").c_str() );
            client.subscribe((TopicDevice+"/XXXXXXXXX").c_str() );
            client.subscribe((TopicDevice+"/XXXXXXXXX").c_str() );
            // subscribe topics
            // ***************************************************
            
        }
        else {
            TelnetStream.print("Error de conexión MQTT, rc=");
            Serial.print("Error de conexión MQTT, rc=");
            TelnetStream.print(client.state());
            Serial.print(client.state());
            TelnetStream.println(" Reintentando en 5 segundos...");
            Serial.println(" Reintentando en 5 segundos...");
            delay(5000);
        }
    }
}
