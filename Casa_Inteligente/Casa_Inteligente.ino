//bibliotecas
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <DHT11.h>
#include <DHT.h>
#include <DHT_U.h>

// indentacao (atalho) - Shift + Alt + F

//variaveis  - ascender led
const int pir = 2;
const int LedAmarelo = 13;

//variaveis verificar vazamento de gas
const int MQ135 = A0;
const int buzzer = 12;

//variavel de temperatura
#define DHTPINO A1
#define DHTTYPE DHT11

DHT dht(DHTPINO, DHTTYPE);
LiquidCrystal_I2C lcd (0x27, 20, 4);

// servo motor

Servo motor; //variavel do tipo motor

void ascenderLedAoDetectarPresenca() {
  int estadopir = digitalRead(pir);  //le o pino 2

  if ( estadopir == HIGH){
    digitalWrite(LedAmarelo, HIGH);
    Serial.println("Movimento detectado, ligando luzes.");
   } else {
     digitalWrite(LedAmarelo, LOW);
     Serial.println("Nenhum movimento detectado no momento.");

   }
}

void VerificarVazamentoDeGas(){
    int EstadoMQ135 = analogRead(MQ135);

    //Serial.println(EstadoMQ135);
    if (EstadoMQ135 >= 250){
     alarme_dois_tons();
    }
    else {
      noTone(buzzer); //para de tocar o alarme 
    }
}

void alarme_dois_tons() {
  int freqAlta = 2000;
  int freqBaixa = 800;
  int duracaoTom = 250;

  tone(buzzer, freqAlta, duracaoTom);
  delay(duracaoTom);
  tone(buzzer, freqBaixa, duracaoTom);
  delay(duracaoTom);
}

void verificartemperatura(){
 
 float temperatura = dht.readTemperature(); // le a temperatura em celsius, padrao
 float umidade = dht.readHumidity(); // le a umidade 

 lcd.clear(); // limpa o lcd 
 lcd.setCursor(0, 0); // 0 - primeira linha 
 lcd.print("Temperatura: " + String(temperatura) + " C ");
 lcd.setCursor(0, 1); // 1 - segunda linha
 lcd.print("Umidade: " + String(umidade) + " % ");

 //Serial.println("Temperatura: " + String(temperatura) + " C ");
 //Serial.println("Umidade: " + String(umidade) + " % ");
 delay(5000);

}

void AbrirPortaSozinha(){
 motor.write(180);
 delay(1500);

}

void FecharPortaSozinha(){
 motor.write(0);
 delay(1500);

}

void setup() {
  Serial.begin(9600);

  dht.begin(); // inicializando o dth11
  lcd.init(); // inicializando o lcd

  lcd.backlight(); // liga a luz do lcd

  //fala ao programa que o pino 13 sera uma saida/entrada
  pinMode(LedAmarelo, OUTPUT);
  pinMode(pir, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(MQ135, INPUT);
  motor.attach(3);

  //angulo da posicao inicial do motor
  motor.write(0); //por enquanto o angulo e 0

  Serial.println("Aguarde um tempo para os sensores serem calibrados.");
  delay(10000);
  Serial.println("Os sensores foram calibrados com sucesso.");

  lcd.setCursor(0, 0); // primeira linha 
  lcd.print(" Inicializando leituras ");
  lcd.setCursor(0, 1); // segunda linha
  lcd.print(" Voce nao me amou, igual ela que nunca te amou ");
}

void loop() {
  //ascenderLedAoDetectarPresenca();
  //VerificarVazamentoDeGas();
  //alarme_dois_tons();
  //verificartemperatura();
  AbrirPortaSozinha();
  FecharPortaSozinha();
}
