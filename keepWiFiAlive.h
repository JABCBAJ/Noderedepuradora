

// matiene conectado a la conexion WIFI y nodered
void keepWiFiAlive(void * parameter){
    
    for(;;){

        if(WiFi.status()!=WL_CONNECTED) {WiFi_conect();}

        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}


