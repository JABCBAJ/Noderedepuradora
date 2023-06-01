
#define ESP32_RTOS

#include <esp_system.h>
#include "OTA.h"
#include "Definiciones.h"

//#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient wifiClient;
PubSubClient client(wifiClient);

#include <ESP32Time.h>

#include <TelnetStream.h>       // DEBUG

#include "Comunicaciones.h"
#include "keepWiFiAlive.h"

// Create an NTPClient to get the time
const char *ntpServer = "pool.ntp.org";
const long  gmtOffset_sec       = 3600;
       int  daylightOffset_sec  = 3600;

ESP32Time rtc;
struct tm timeinfo;

//·································································································································
//·································································································································

void setup() {

    WiFi_conect();

    // OTA ESP32 
    setupOTA(Device, ssid, password);

    //comunicaciones();
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

    // inicializa debug
    Serial.begin(115200);     // initialize debug serial port
    // TelnetStream.begin();

    // Comprueba la conexión Wifi.Y RECONECTA si se ha perdido.
    xTaskCreatePinnedToCore(keepWiFiAlive, "keepWiFiAlive", 1023, NULL, 2, NULL, 1);

    // actualiza la hora de internet
    Actualiza_WebTime();

    Serial.println("RESET");
}
//·································································································································
//·································································································································

void loop() {

    ArduinoOTA.handle();

    if (!client.connected()) {reconnect();}
    
    ordenes_nodered();

    // TU CODIGO AQUI
    
    static int ciclo=time(NULL);
    if (time(NULL)-ciclo> nodered_ciclo ) {
        ciclo=time(NULL);
        status_lock();
    }

    client.loop();
}
//###########################################################################################

// manda a node-red el estado del sistema
void status_lock() {
     
    topic=TopicDevice+"XXXXXXXX";
    value="XXXXXX";  // O value=variable;
    send_nodered(topic, value);
    
    client.publish( (TopicDevice+"/MAC" ).c_str()          , String ( WiFi.macAddress()).c_str() );
    client.publish( (TopicDevice+"/WIFI").c_str()         , String ( WiFi.SSID()).c_str() );
    client.publish( (TopicDevice+"/RSSI").c_str()         , String ( WiFi.RSSI()).c_str() );
    client.publish( (TopicDevice+"/IP"  ).c_str()         , String ( WiFi.localIP()).c_str() );

    //Serial.println(rtc.getLocalTime(&timeinfo) );  // getLocalTime(&timeinfo)    rtc.getDateTime(true)
    Serial.println(rtc.getTime() );
  
    TelnetStream.print( WiFi.RSSI());
    TelnetStream.print( "-");
    TelnetStream.print( WiFi.localIP().toString());
    TelnetStream.print( "-");
    TelnetStream.println(WiFi.macAddress());
    TelnetStream.println(rtc.getTime()    );
}

// recibido de nodered
void ordenes_nodered() {

    if(receive_Topic!="") {
        // bloqueo porton
        if (receive_Topic==TopicDevice+"/XXXX") {

            if (receive_Payload=="XXXX" ) {
                
            }
        }
        
        // reinicio total EDSP32 desde Nodered
        if (receive_Topic==TopicDevice+"/reset" && receive_Payload=="true") { 
            delay(2000);
            ESP.restart(); 
        }
        // borra buffer recepcion node-red
        receive_Topic    = "";
        receive_Payload  = "";
    }
}

// envia datos a nodered
void send_nodered( String topic, String value) {
     client.publish( (topic).c_str(), value.c_str() );
}

// actualiza tiempo desde internet
void Actualiza_WebTime() {
    
    // Obtener el último domingo de marzo a las 02:00 horario de verano
    if (timeinfo.tm_mon >= 2 && timeinfo.tm_wday == 0 && timeinfo.tm_mday >= 25 && timeinfo.tm_hour >= 10) {
        // El último domingo de marzo a las 02:00 se ha pasado, se cambia al horario de verano
        Serial.println("Cambiando al horario de verano");
        TelnetStream.println("Cambiando al horario de verano");
        daylightOffset_sec = 3600; // Ajuste horario de verano para España (1 hora)
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    }

    // Obtener el último domingo de octubre a las 03:00 horario de invierno
    if (timeinfo.tm_mon >= 9 && timeinfo.tm_wday == 0 && timeinfo.tm_mday >= 25 && timeinfo.tm_hour >= 3) {
        // El último domingo de octubre a las 03:00 se ha pasado, se cambia al horario de invierno
        Serial.println("Cambiando al horario de invierno");
        TelnetStream.println("Cambiando al horario de verano");
        daylightOffset_sec = 0; // Ajuste horario de invierno para España (0 horas)
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    }

    // actualiza la hora de internet
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    if (!getLocalTime(&timeinfo)) { Serial.println("Error al obtener la hora"); return; }
    if(getLocalTime(&timeinfo)) { rtc.setTimeStruct(timeinfo ); }
}


