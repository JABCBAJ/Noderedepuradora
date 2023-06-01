// Dispositivo remoto en la red, a de coincidir con el topico en nodered "device/#"
String TopicDevice      = "xxxxxx";  // porton prototype
const char* Device      = "XXXXXX";  // PORTON PROTOTIPO
// String TopicDevice      = "prototype";  // porton prototype
// const char* Device      = "PROTOTIPO";  // PORTON PROTOTIPO

const char* ssid        = "VIRUSLAN";
const char* password    = "1Q2W3E4R5T6Y7U8I9O0P$";

#define Wifi_Reconect 100

#define mqttServer    "192.168.1.59"
#define mqttPort      1883

int nodered_ciclo = 10; // nº de segundos entre envios a node red
//____________________________________________________________________________________________________
//

// datos string desde Mqtt
String receive_Payload;
String receive_Topic;

String value,topic;
