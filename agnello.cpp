#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
// Caracteres personalizados
byte shit[8]    = { B00000, B10001, B00000, B00000, B01110, B10001, B00000, B00000 };
byte smile[8]   = { B00000, B10001, B00000, B00000, B10001, B01110, B00000, B00000 };
byte serious[8] = { B00000, B10001, B00000, B00000, B11111, B00000, B00000, B00000 };
byte ponto[8]   = { B00000, B00000, B00000, B00000, B00100, B01010, B00100, B00000 };
byte lupa2[8]   = { B00011, B00110, B00100, B00100, B01111, B11100, B11000, B10000 };
byte lupa1[8]   = { B00000, B00000, B00000, B00000, B00000, B00000, B00001, B00001 };
byte lupa3[8]   = { B10000, B01000, B01000, B01000, B10000, B00000, B00000, B00000 };
 
// Pinos
const int LDR = A0;
const int ledVerde = 12;
const int ledAmarelo = 11;
const int ledVermelho = 10;
const int alarme = 13;
 
// Configuração do tempo de experimento em milissegundos
const unsigned long DURACAO_EXPERIMENTO = 10000;  // 10 segundos
 
// Controle do experimento
unsigned long tempoInicial = 0;
unsigned long tempoExperimento = DURACAO_EXPERIMENTO;
int maxLuminosidade = 0;
int minLuminosidade = 1023;
bool experimentoFinalizado = false;
 
// Média de luminosidade
long somaLuminosidade = 0;  // Soma das porcentagens
int contagemLeituras = 0;   // Quantidade de leituras
 
// Controle de alarme
unsigned long tempoAlarme = 0;
bool buzzerTocando = false;
bool jaSoouNaZonaVermelha = false;
 
void setup() {
  lcd.init();
  lcd.backlight();
 
  // Criar caracteres personalizados
  lcd.createChar(0, shit);
  lcd.createChar(1, smile);
  lcd.createChar(2, serious);
  lcd.createChar(5, ponto);
  lcd.createChar(6, lupa1);
  lcd.createChar(7, lupa2);
  lcd.createChar(8, lupa3);
 
  // Animação "Insight"
  lcd.setCursor(4, 1); lcd.print("I"); delay(350);
  lcd.setCursor(5, 1); lcd.print("n"); delay(350);
  lcd.setCursor(6, 1); lcd.print("s"); delay(350);
  lcd.setCursor(7, 1); lcd.print("i"); delay(350);
  lcd.setCursor(8, 1); lcd.print("g"); delay(350);
  lcd.setCursor(9, 1); lcd.print("h"); delay(350);
  lcd.setCursor(10, 1); lcd.print("t"); delay(350);
  lcd.setCursor(11, 1); lcd.write(byte(6));
  lcd.setCursor(12, 1); lcd.write(byte(7));
  lcd.setCursor(13, 1); lcd.write(byte(8));
 
  delay(2000);
  lcd.clear();
 
  const char* msg = "Bem-vindo(a)!";
  int posInicial = (16 - strlen(msg)) / 2;
  for (int i = 0; i < strlen(msg); i++) {
    lcd.setCursor(posInicial + i, 1);
    lcd.print(msg[i]);
    delay(200);
  }
 
  delay(2000);
  lcd.clear();
 
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(alarme, OUTPUT);
 
  Serial.begin(9600);
  tempoInicial = millis();
}
 
void loop() {
  int valorLDR = analogRead(LDR);
  int porcentagem = map(valorLDR, 0, 1023, 0, 100);
 
  unsigned long tempoAtual = millis();
 
  if (!experimentoFinalizado) {
    if (valorLDR > maxLuminosidade) maxLuminosidade = valorLDR;
    if (valorLDR < minLuminosidade) minLuminosidade = valorLDR;
 
    somaLuminosidade += porcentagem;   // Acumula porcentagens
    contagemLeituras++;                // Conta cada leitura
  }
 
  lcd.setCursor(0, 0);
  lcd.print("Luminosidade:    ");
  lcd.setCursor(0, 1);
 
  if (porcentagem >= 50) {
    lcd.print("Status: Bom      ");
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(ledVermelho, LOW);
    noTone(alarme);
    buzzerTocando = false;
    jaSoouNaZonaVermelha = false;
  } else if (porcentagem >= 30) {
    lcd.print("Status: Medio    ");
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, HIGH);
    noTone(alarme);
    buzzerTocando = false;
    jaSoouNaZonaVermelha = false;
  } else {
    lcd.print("Status: Ruim     ");
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(ledVermelho, HIGH);
 
    if (!jaSoouNaZonaVermelha && !buzzerTocando) {
      tone(alarme, 1000);
      tempoAlarme = millis();
      buzzerTocando = true;
      jaSoouNaZonaVermelha = true;
    }
 
    if (buzzerTocando && millis() - tempoAlarme >= 3000) {
      noTone(alarme);
      buzzerTocando = false;
    }
  }
 
  if (!experimentoFinalizado && tempoAtual - tempoInicial >= tempoExperimento) {
    experimentoFinalizado = true;
 
    // Corrigido: mapeia corretamente de 0–1023 para 0–100
    int porcentagemMaxima = map(maxLuminosidade, 0, 1023, 0, 100);
    int porcentagemMinima = map(minLuminosidade, 0, 1023, 0, 100);
 
    // Calcula a média corretamente
    int mediaLuminosidade = contagemLeituras > 0 ? somaLuminosidade / contagemLeituras : 0;
 
    Serial.println("Relatório do Experimento:");
    Serial.print("Luminosidade Máxima: ");
    Serial.print(porcentagemMaxima); Serial.println("%");
    Serial.print("Luminosidade Mínima: ");
    Serial.print(porcentagemMinima); Serial.println("%");
    Serial.print("Média de Luminosidade: ");
    Serial.print(mediaLuminosidade); Serial.println("%");
 
 
 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Relatorio");
    lcd.setCursor(0, 1);
    lcd.print("Enviado:");
 
    delay(5000);
    resetExperimento();
  }
 
  delay(500);
}
 
void resetExperimento() {
  maxLuminosidade = 0;
  minLuminosidade = 1023;
  somaLuminosidade = 0;
  contagemLeituras = 0;
  tempoInicial = millis();
  experimentoFinalizado = false;
}
