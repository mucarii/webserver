#include <ESP8266WiFi.h>

const char* ssid = "  SEU_WIFI";      // Insira o nome da sua rede Wi-Fi
const char* password = "SENHA_WIFI"; // Insira a senha da sua rede Wi-Fi
const char* server = "www.exemplo.com";  // Insira o domínio do servidor da Locaweb
const int httpPort = 80;                 // Porta HTTP padrão

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT); // Define o pino do LED embutido como saída
  digitalWrite(LED_BUILTIN, LOW); // Desliga o LED no início

  delay(10);

  WiFi.begin(ssid, password);   // Criar conexao com Wi-Fi

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conexão Wi-Fi estabelecida!");
}

void loop() {
  WiFiClient client;

  if (!client.connect(server, httpPort)) {
    Serial.println("Falha na conexão com o servidor");
    delay(5000); // Espera 5 segundos antes de tentar novamente
    return;
  }

  // Faz a requisição GET
  client.print(String("GET /caminho/do/arquivo.txt HTTP/1.1\r\n") +  // inserir o caminho completo do arquivo ou recurso que você deseja acessar no servidor hospedado na Locaweb.
               "Host: " + server + "\r\n" +
               "Connection: close\r\n\r\n");

  // Aguarda a resposta do servidor
  while (client.connected()) {
    if (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);

      // Verifica se a resposta do servidor contém o comando "ligar"
      if (line.indexOf("ligar") != -1) {
        digitalWrite(LED_BUILTIN, HIGH); // Ligar o LED embutido
      }

      // Verifica se a resposta do servidor contém o comando "desligar"
      if (line.indexOf("desligar") != -1) {
        digitalWrite(LED_BUILTIN, LOW); // Desligar o LED embutido
      }
    }
  }

  Serial.println();
  Serial.println("Conexão encerrada");
  delay(5000); // Espera 5 segundos antes de fazer a próxima requisição
}
