//-----------------------------------------------------------------------------------------------
// ACTIVA O DESACTIVA DISPOSITIVOS SEGUN ORDENES DEsE NODE-RED
//-----------------------------------------------------------------------------------------------

bool ordenes_Nodered() {

    // solo para test
    if(receive_Topic!="") {

        // // TelnetStream.print("<<< topic  : ");
        // // TelnetStream.println(receive_Topic);
        // // TelnetStream.print("<<< payload: ");
        // // TelnetStream.println(receive_Payload);

        // Serial.println(receive_Topic);
        // localiza si la orden enviada esta en la lista de encendido o apagado
        for (int i = 0; i < sizeof(Devices) / sizeof(Devices[0]); i++) {
            if (receive_Topic==Devices[i]) {
                if (receive_Payload=="true") {
                    ArduinoSerial.println(OrdenesON[i]);
                    // Serial.print(OrdenesON[i]);
                }
                if (receive_Payload=="false") {
                    ArduinoSerial.println(OrdenesOFF[i]);
                    // Serial.print(OrdenesOFF[i]);
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

