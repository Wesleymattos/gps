#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>

// Firebase configuration
#define FIREBASE_HOST "rastreamentogps-56e5c-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyAlxDBMrFveQx1b1fTu1WMvP7qX_yFARx8"
#define WIFI_SSID "FAMÍLIA MATTOS"
#define WIFI_PASSWORD "1498877676l"

float latitude = -30.0330; // Latitude inicial de Porto Alegre
float longitude = -51.2300; // Longitude inicial de Porto Alegre

// Rota fictícia representando a movimentação em Porto Alegre
// Você pode adicionar mais pontos conforme a necessidade
float route[5][2] = {
  {-30.0330, -51.2300},  // Ponto inicial
  {-30.0340, -51.2350},  // Ponto 1
  {-30.0350, -51.2400},  // Ponto 2
  {-30.0360, -51.2450},  // Ponto 3
  {-30.0370, -51.2500}   // Ponto 4
};

int routeIndex = 0; // Índice da rota para simular movimento

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected: ");
  Serial.println(WiFi.localIP());

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  // Simula o movimento da rota com base nos pontos definidos
  latitude = route[routeIndex][0];
  longitude = route[routeIndex][1];

  // Envia os valores de latitude e longitude para o Firebase
  Firebase.setFloat("/localizacoes/dog1/latitude", latitude);
  Firebase.setFloat("/localizacoes/dog1/longitude", longitude);

  // Verifica se ocorreu algum erro
  if (Firebase.failed()) {
    Serial.print("Error setting values: ");
    Serial.println(Firebase.error());
    return;
  }

  // Exibe no Monitor Serial
  Serial.print("Latitude: ");
  Serial.println(latitude);
  Serial.print("Longitude: ");
  Serial.println(longitude);

  // Atualiza o índice para o próximo ponto na rota
  routeIndex = (routeIndex + 1) % 5; // Vai de 0 a 4, voltando ao início da rota

  // Aguarda 5 segundos para a próxima atualização
  delay(5000);
}
