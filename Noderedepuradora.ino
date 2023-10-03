
#define ESP32_RTOS

#include <esp_system.h>
#include "OTA.h"
#include "Definiciones.h"

#include <WiFi.h>
#include <PubSubClient.h>


WiFiClient wifiClient;
PubSubClient mqttclient(wifiClient);

#include <HardwareSerial.h>
//HardwareSerial Serial(0);  // Crea una instancia de HardwareSerial asociada al puerto 0 pin: GPIO3  (RX1) y GPIO1  (TX0)
HardwareSerial BTHC06(1);         // Crea una instancia de HardwareSerial asociada al puerto 1 pin: GPIO16 (RX2) y GPIO17 (TX1)
HardwareSerial ArduinoSerial(2);  // Crea una instancia de HardwareSerial asociada al puerto 2 pin: GPIO18 (RX2) y GPIO19 (TX2)

#include <ESP32Time.h> 

#include "Comunicaciones.h"
#include "Ordenes_Nodered.h"

// Create an NTPClient to get the time
const char *ntpServer = "pool.ntp.org";
const long  gmtOffset_sec       = 3600;
       int  daylightOffset_sec  = 3600;

ESP32Time rtc;
struct tm timeinfo;

//·································································································································
//·································································································································

void setup() {

    // Serial.begin(9600, SERIAL_8N1, 3, 1);                                 // Inicia el puerto serie USB
    // Serial.setTimeout(100);
    ArduinoSerial.begin(9600, SERIAL_8N1, 15, 2);       //  3, 1 (15,2)Inicia el puerto serie 0 con los pines GPIO3  (RX0) y GPIO1  (TX0)
    ArduinoSerial.setTimeout(10);
    BTHC06.begin(9600, SERIAL_8N1, 16, 17);             // Inicia el puerto serie 2 con los pines GPIO16 (RX1) y GPIO17 (TX1)
    BTHC06.setTimeout(10);
    
    WiFi_conect();

    setupOTA(Device, ssid, password);
    for (size_t i = 0; i < 200; i++) {delayMicroseconds(10000);}
    
    mqttclient.setServer(mqttServer, mqttPort);
    mqttclient.setCallback(callback);
    
    // reconnect();

    // actualiza la hora de internet
    Actualiza_WebTime();
    time_mqtt_live=time(NULL);
    now = time(NULL);

    // Comprueba la conexión Wifi.Y RECONECTA si se ha perdido.
    // xTaskCreatePinnedToCore(keepWiFiAlive, "keepWiFiAlive", 1023, NULL, 1, NULL, 1);
    // xTaskCreatePinnedToCore(delayX, "delayX", 1000, NULL, 0, NULL, 0);

    // Serial.println("#### run ####");
    // Serial.println("OTA run OTA");
}
//·································································································································
//·································································································································

void loop() {

    ArduinoOTA.handle();

    if (WiFi.status()!=WL_CONNECTED) {WiFi_conect();}

    if (WiFi.status()==WL_CONNECTED && !mqttclient.connected()) {reconnect();}
    
    now = time(NULL);

    ordenes_Nodered();
    
    // Captura datos serial de arduino y los guarda en buffer.
    String data;
    if (ArduinoSerial.available()) {
        for (size_t i = 0; i < 9; i++) {
            // Captura trama dedatos desde Arduino Pro Mini
            data = ArduinoSerial.readStringUntil('\n'); 
            // debug
            // if (data.indexOf('I') != -1) { TelnetStream.println(data); }
            // buffer temporal
            From_Arduino[i] = data;
            // Bypass datos serie a BT HC06
            // BTHC06.write(data.c_str(), data.length()); // c_str()  puntero constante a la representación de caracteres de una cadena (string).
            BTHC06.print(data + '\n');

            data = "";
        }
    }

    // actualiza cada 24h timeweb
    static int cycle3sg;
    if(now-cycle3sg > 3) {
        cycle3sg=now;
        // encuentra los mensajes que contienen "I" y los manda a node red
        String Dato;
        for (size_t i = 0; i < 9; i++) {
            Dato = From_Arduino[i];
            if (Dato.indexOf('I') != -1) {  
                for (size_t j = 0; j < 3; j++) {
                    if(Dato==StatusON[j])       {send_Nodered((Devices[j]).c_str() , String ( "on").c_str());}
                    else if(Dato==StatusOFF[j]) {send_Nodered((Devices[j]).c_str() , String ( "off").c_str());}/* code */
                }
            }
            // send_Nodered((Devices[i]).c_str() , String ( From_Arduino[i]).c_str());
        }
    }

    // captura desde BTHC06 y lo reenvia a arduino
    if (BTHC06.available()) {
        String dataAPK = BTHC06.readStringUntil('\n');         // Lee un byte del puerto serie GPIO15
        // ArduinoSerial.write(data.c_str(), data.length());  // Envía el byte al puerto serie GPIO2
        ArduinoSerial.print(dataAPK + '\n');
        dataAPK = "";
    }


    // envia estado del dispositivo a node red
    static int statustime=time(NULL);
    if (now-statustime > nodered_ciclo) {
        statustime=time(NULL);
        int TempValue = analogRead(tmp36Pin);           // lee temperatura
        // TelnetStream.print( "TempValue: ");
        // TelnetStream.println( TempValue);
        float voltage = TempValue  / 1023.0;     // Convierte el valor a voltaje (3.3V en ESP32)
        // TelnetStream.print( "voltage: ");
        // TelnetStream.println( voltage);
        temperatureC = (voltage - 0.5) * 100;   // Convierte el voltaje a temperatura en grados Celsius
        // TelnetStream.print( "temperatureC: ");
        TelnetStream.println( temperatureC);
        status_device();
    }

    // actualiza cada 24h timeweb
    static int cycle24h;
    if(now-cycle24h > 24*60*60) {
        cycle24h=now;
        Actualiza_WebTime();  // UNA VEZ AL DIA
    }

    // delay(100);
    mqttclient.loop();
}
//###########################################################################################

// manda a node-red el estado del sistema
void status_device() {

    mqttclient.publish( (TopicDevice+"/RSSI").c_str() , String ( WiFi.RSSI()).c_str() );
    mqttclient.publish( (TopicDevice+"/IP").c_str() , String ( WiFi.localIP().toString()).c_str() );
    mqttclient.publish( (TopicDevice+"/MAC").c_str() , String ( WiFi.macAddress()).c_str() );
    mqttclient.publish( (TopicDevice+"/WIFI").c_str() , String ( WiFi.SSID()).c_str() );
    mqttclient.publish( (TopicDevice+"/TEMP").c_str() , String ( temperatureC).c_str() );
    TelnetStream.print( WiFi.RSSI());
    TelnetStream.print( "-");
    TelnetStream.print( WiFi.localIP().toString());
    TelnetStream.print( "-");
    TelnetStream.println(WiFi.macAddress());
    // TelnetStream.println(rtc.getTime()    );

    //    ArduinoSerial.println(rtc.getTime() );
}

// publica datos a nodered
void send_Nodered(String topic, String value) {
    mqttclient.publish( (topic).c_str(), String ( value).c_str());
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
    if(getLocalTime(&timeinfo)) { rtc.setTimeStruct(timeinfo ); now = time(NULL);}
}


