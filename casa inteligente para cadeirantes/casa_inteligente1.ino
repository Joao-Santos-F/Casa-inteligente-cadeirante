#include <Servo.h>

// Variáveis globais
const int ledAmarelo = 13;
const int PIR = 2;
const int mq135 = A0;
const int buzzer = 12;
const int servoPin = 9;
const int rele = 8; // Relé para a lâmpada

// Controle de detecção de gás
int contador = 0;
int tempoFumaca = 0;
bool alarmeAtivo = false;

// Controle do alarme de dois tons
unsigned long ultimaTrocaDeTom = 0;
bool tomAlto = true;

// Servo motor
Servo servo;
int estadoPIR = LOW;

void setup() {
  Serial.begin(9600);

  pinMode(ledAmarelo, OUTPUT);
  pinMode(PIR, INPUT);
  pinMode(mq135, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(rele, OUTPUT); // Configura o relé como saída

  servo.attach(servoPin);
  servo.write(0); // Porta começa fechada

  Serial.println("Calibrando o sensor PIR...");
  Serial.println("Por favor, nao se mova na frente do sensor.");
  delay(10000);
  Serial.println("Sensor PIR calibrado e pronto!");
}

// Função para controlar o alarme com dois tons
void alarme_dois_tons() {
  unsigned long tempoAtual = millis();

  if (tempoAtual - ultimaTrocaDeTom >= 500) { // Alterna o tom a cada 500ms
    ultimaTrocaDeTom = tempoAtual;

    if (tomAlto) {
      tone(buzzer, 2000); // Tom alto
    } else {
      tone(buzzer, 1000); // Tom baixo
    }

    tomAlto = !tomAlto;
  }
}

// Função para detecção de movimento com PIR e controle do servo e da lâmpada
void acenderLEDAoDetectarPresenca() {
  estadoPIR = digitalRead(PIR);

  if (estadoPIR == HIGH) {
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(rele, HIGH); // Liga a lâmpada
    Serial.println("Movimento detectado! LED aceso, lampada ligada e porta abrindo.");
    servo.write(90); // Abre a porta
    delay(5000); // Mantém aberta por 5 segundos
  } else {
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(rele, LOW); // Desliga a lâmpada
    servo.write(0); // Fecha a porta
    Serial.println("Sem movimento. LED apagado, lampada desligada e porta fechada.");
  }
}

// Função para detecção de gás
void verificarVazamentoDeGas() {
  int estadoMQ135 = analogRead(mq135);

  if (estadoMQ135 > 700) {
    tempoFumaca++;

    Serial.println("Fumaca detectada...");

    if (tempoFumaca >= 3 && !alarmeAtivo) { // Após 3 leituras consecutivas
      Serial.println("Fumaca detectada por 3 segundos. Acionando alarme!");
      alarmeAtivo = true;
    }
  } else {
    if (alarmeAtivo) {
      Serial.println("Fumaca sumiu. Desligando alarme.");
      alarmeAtivo = false;
      noTone(buzzer);
    }
    tempoFumaca = 0;
    Serial.println("Ambiente limpo.");
  }

  delay(1000); // Espera 1 segundo entre leituras de gás
}

void loop() {
  acenderLEDAoDetectarPresenca();
  verificarVazamentoDeGas();

  if (alarmeAtivo) {
    alarme_dois_tons();
  }
}
