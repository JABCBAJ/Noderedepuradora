//-----------------------------------------------------------------------------------------------
// ACTIVA O DESACTIVA DISPOSITIVOS SEGUN ORDENES DEsE NODE-RED
//-----------------------------------------------------------------------------------------------

bool ordenes_Nodered() {

    // solo para test
    if(receive_Topic!="") {

        TelnetStream.print("<<< topic  : ");
        TelnetStream.println(receive_Topic);
        TelnetStream.print("<<< payload: ");
        TelnetStream.println(receive_Payload);

        // Serial.println(receive_Topic);
        // localiza si la orden enviada esta en la lista de encendido o apagado
        for (int i = 0; i < sizeof(Devices) / sizeof(Devices[0]); i++) {
            if (receive_Topic==Topic_Devices[i]) {
                if (receive_Payload=="off") {
                    ArduinoSerial.print(OrdenesON[i]); //  + '\n'
                    Serial.write((uint8_t*)OrdenesON[i].c_str(), OrdenesON[i].length());
                    // Serial.print("xxxx");
                }
                else if (receive_Payload=="on") {
                    ArduinoSerial.print(OrdenesOFF[i]);
                    // Serial.write((uint8_t*)OrdenesOFF[i].c_str(), OrdenesOFF[i].length());
                    // Serial.print("xxxx");
                }
            }   
        }
        
        // 
        if (receive_Topic==TopicDevice+"/Mqtt_Live") {
            if (receive_Payload=="true") {time_mqtt_live=now;}
        }

        // borra buffer mqtt
        receive_Topic=""; receive_Payload="";
    }
}

