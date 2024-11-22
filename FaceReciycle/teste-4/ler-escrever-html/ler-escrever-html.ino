#include <WiFi.h>
#include <WebServer.h>

// Definições de pinos
#define LED_PIN 2         // LED controlado pelo botão na página
#define TRIG_PIN 21       // Pino TRIG do sensor ultrassônico
#define ECHO_PIN 22       // Pino ECHO do sensor ultrassônico
#define LED_PRESET_PIN 5  // LED que será acionado ao atingir a distância configurada

// Variáveis globais
bool ledState = false;     // Estado do LED
float presetDistance = 10; // Valor inicial do preset de distância
WebServer server(80);      // Criação do servidor web

// Configuração do Access Point
const char* ssid = "ESP32_AP";
const char* password = "12345678";

// Função para medir distância
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = (duration * 0.034) / 2; // Calcula distância em cm
  return distance;
}

// Função para gerar a página HTML
String generateHTML(float distance) {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>ESP32 Control</title>
      <style>
        body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }
        button { padding: 10px 20px; font-size: 18px; margin: 10px; }
        input { padding: 10px; font-size: 18px; width: 100px; }
      </style>
      <script>
        function toggleLED() {
          fetch('/toggle').then(response => response.text());
        }
        function setPreset() {
          const value = document.getElementById('preset').value;
          fetch('/setPreset?distance=' + value).then(response => response.text());
        }
      </script>
    </head>
    <body>
      <h1>ESP32 Control Page</h1>
      <button onclick="toggleLED()">Toggle LED</button>
      <p>Distance: <span id="distance">)rawliteral" + String(distance) + R"rawliteral( cm</span></p>
      <p>
        Set Preset Distance: 
        <input type="number" id="preset" value=")rawliteral" + String(presetDistance) + R"rawliteral(">
        <button onclick="setPreset()">Set</button>
      </p>
    </body>
    </html>
  )rawliteral";
  return html;
}

// Rota principal
void handleRoot() {
  float distance = getDistance();
  String html = generateHTML(distance);
  server.send(200, "text/html", html);
}

// Rota para alternar o LED
void handleToggleLED() {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState);
  server.send(200, "text/plain", "LED toggled");
}

// Rota para definir a distância presetada
void handleSetPreset() {
  if (server.hasArg("distance")) {
    presetDistance = server.arg("distance").toFloat();
  }
  server.send(200, "text/plain", "Preset distance set");
}

void setup() {
  // Inicialização dos pinos
  pinMode(LED_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PRESET_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(LED_PRESET_PIN, LOW);

  // Inicializa o Access Point
  WiFi.softAP(ssid, password);
  Serial.begin(115200);
  Serial.println("Access Point Started!");
  Serial.println("Connect to Wi-Fi network: " + String(ssid));
  Serial.println("IP Address: " + WiFi.softAPIP().toString());

  // Configuração das rotas
  server.on("/", handleRoot);
  server.on("/toggle", handleToggleLED);
  server.on("/setPreset", handleSetPreset);

  // Inicia o servidor
  server.begin();
}

void loop() {
  // Atualiza o servidor
  server.handleClient();

  // Verifica a distância em tempo real e controla o LED do preset
  float distance = getDistance();
  if (distance <= presetDistance && distance > 0) {
    digitalWrite(LED_PRESET_PIN, HIGH);
  } else {
    digitalWrite(LED_PRESET_PIN, LOW);
  }
}
