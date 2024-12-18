#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

// Configurações do Firebase
#define FIREBASE_HOST "rastreamentogps-56e5c-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyAlxDBMrFveQx1b1fTu1WMvP7qX_yFARx8"
#define WIFI_SSID "Wesley"
#define WIFI_PASSWORD "unovermelho"

// Inicialização do GPS
HardwareSerial mySerial(1);
TinyGPSPlus gps;

// Variáveis para Latitude e Longitude
float latitude = 0.0;
float longitude = 0.0;

void setup() {
  Serial.begin(115200);  // Inicializa o monitor serial
  mySerial.begin(9600, SERIAL_8N1, 16, 17);  // RX no GPIO16, TX no GPIO17 (modifique conforme necessário)

  // Conecta à rede Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected: ");
  Serial.println(WiFi.localIP());

  // Inicializa o Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  while (mySerial.available() > 0) {
    gps.encode(mySerial.read());
    
    if (gps.location.isUpdated()) {
      // Obtém a latitude e longitude do GPS
      latitude = gps.location.lat();
      longitude = gps.location.lng();

      // Converte as coordenadas para strings com 8 casas decimais
      String latitudeStr = String(latitude, 8);  // 8 casas decimais
      String longitudeStr = String(longitude, 8); // 8 casas decimais

      // Envia as coordenadas como strings para o Firebase
      Firebase.setString("/localizacoes/dog1/latitude", latitudeStr);
      Firebase.setString("/localizacoes/dog1/longitude", longitudeStr);

      // Verifica se ocorreu erro ao enviar para o Firebase
      if (Firebase.failed()) {
        Serial.print("Erro ao enviar dados: ");
        Serial.println(Firebase.error());
        return;
      }

      // Exibe as coordenadas no Monitor Serial com 8 casas decimais
      Serial.print("Latitude: ");
      Serial.println(latitudeStr);  
      Serial.print("Longitude: ");
      Serial.println(longitudeStr); 
    }
  }

  // Aguarda 1 segundo antes de fazer a próxima leitura
  delay(1000);
}
