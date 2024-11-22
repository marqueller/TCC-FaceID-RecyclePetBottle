#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32-Access-Point"; // Nome da rede
const char* password = "123456789"; // Senha da rede

WebServer server(80); // Cria um servidor na porta 80
const int ledPin = 2; // Pino onde o LED está conectado

// Função para lidar com a raiz do servidor
void handleRoot() {
  String html = "<html><body><h1>Controle de LED</h1>";
  html += "<p><a href=\"/on\"><button>Ligado</button></a></p>";
  html += "<p><a href=\"/off\"><button>Desligado</button></a></p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Função para ligar o LED
void handleOn() {
  digitalWrite(ledPin, HIGH); // Liga o LED
  server.sendHeader("Location", "/"); // Redireciona para a página principal
  server.send(303); // Responde com um redirecionamento
}

// Função para desligar o LED
void handleOff() {
  digitalWrite(ledPin, LOW); // Desliga o LED
  server.sendHeader("Location", "/"); // Redireciona para a página principal
  server.send(303); // Responde com um redirecionamento
}

void setup() {
  Serial.begin(115200);
  
  // Configura o pino do LED
  pinMode(ledPin, OUTPUT);
  
  // Inicia o ponto de acesso
  WiFi.softAP(ssid, password);
  
  Serial.println("Ponto de acesso iniciado");
  Serial.print("IP do AP: ");
  Serial.println(WiFi.softAPIP()); // Mostra o IP do ponto de acesso

  // Define as rotas do servidor
  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  
  // Inicia o servidor
  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  server.handleClient(); // Trata as requisições do cliente
}