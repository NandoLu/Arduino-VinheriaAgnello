#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o display LCD com endereço I2C 0x27 e tamanho 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definição de caracteres personalizados para o LCD
byte shit[8]    = { B00000, B10001, B00000, B00000, B01110, B10001, B00000, B00000 }; // Ícone de expressão
byte smile[8]   = { B00000, B10001, B00000, B00000, B10001, B01110, B00000, B00000 }; // Ícone de sorriso
byte serious[8] = { B00000, B10001, B00000, B00000, B11111, B00000, B00000, B00000 }; // Ícone sério
byte ponto[8]   = { B00000, B00000, B00000, B00000, B00100, B01010, B00100, B00000 }; // Ponto decorativo
byte lupa2[8]   = { B00011, B00110, B00100, B00100, B01111, B11100, B11000, B10000 }; // Ícone de lupa
byte lupa1[8]   = { B00000, B00000, B00000, B00000, B00000, B00000, B00001, B00001 }; // Complemento de lupa
byte lupa3[8]   = { B10000, B01000, B01000, B01000, B10000, B00000, B00000, B00000 }; // Complemento de lupa

// Definição dos pinos conectados ao LDR, LEDs e alarme (buzzer)
const int LDR = A0;
const int ledVerde = 10;
const int ledAmarelo = 11;
const int ledVermelho = 12;
const int alarme = 13;

// Controle de tempo do experimento e inicialização de variáveis
unsigned long tempoInicial = 0;      // Armazena o tempo inicial do experimento
unsigned long tempoExperimento = 30000; // Duração do experimento em milissegundos
int maxLuminosidade = 0;             // Valor máximo de luminosidade registrado
int minLuminosidade = 1023;          // Valor mínimo de luminosidade registrado
bool experimentoFinalizado = false;  // Indica se o experimento foi finalizado

// Controle do alarme (buzzer)
unsigned long tempoAlarme = 0;       // Tempo de ativação do alarme
bool buzzerTocando = false;          // Indica se o alarme está tocando
bool jaSoouNaZonaVermelha = false;   // Impede que o alarme toque repetidamente na zona vermelha

// Soma e quantidade de leituras para cálculo da média
long somaLuminosidade = 0;           // Soma dos valores de luminosidade
int quantidadeLeituras = 0;          // Quantidade de leituras realizadas

void setup() {
  lcd.init();                        // Inicializa o LCD
  lcd.backlight();                   // Liga o backlight do LCD

  // Criar caracteres personalizados no LCD
  lcd.createChar(0, shit);
  lcd.createChar(1, smile);
  lcd.createChar(2, serious);
  lcd.createChar(5, ponto);
  lcd.createChar(6, lupa1);
  lcd.createChar(7, lupa2);
  lcd.createChar(8, lupa3);

  // Exibe animação "Insight" no LCD
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

  delay(2000); // Aguarda 2 segundos após exibir a animação

  // Exibe mensagem de boas-vindas com efeito de digitação
  lcd.clear();
  const char* msg = "Bem-vindo(a)!";
  int posInicial = (16 - strlen(msg)) / 2; // Centraliza a mensagem no LCD

  for (int i = 0; i < strlen(msg); i++) {
    lcd.setCursor(posInicial + i, 1); // Exibe cada caractere na linha inferior
    lcd.print(msg[i]);
    delay(200);                      // Intervalo entre cada caractere
  }

  delay(2000); // Aguarda 2 segundos antes de limpar o LCD e iniciar o experimento
  lcd.clear();

  // Configura os pinos como saída
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(alarme, OUTPUT);

  Serial.begin(9600);                // Inicializa comunicação serial
  tempoInicial = millis();           // Registra o tempo inicial do sistema
}

void loop() {
  int valorLDR = analogRead(LDR);    // Lê o valor do sensor de luminosidade (LDR)
  int porcentagem = map(valorLDR, 0, 1023, 0, 100); // Converte para porcentagem de luminosidade

  unsigned long tempoAtual = millis(); // Registra o tempo atual

  // Atualiza valores máximos, mínimos e soma leituras durante o experimento
  if (!experimentoFinalizado) {
    if (valorLDR > maxLuminosidade) maxLuminosidade = valorLDR;
    if (valorLDR < minLuminosidade) minLuminosidade = valorLDR;

    somaLuminosidade += valorLDR;
    quantidadeLeituras++;
  }

  // Atualiza status de luminosidade no LCD e controla LEDs e alarme
  lcd.setCursor(0, 0);
  lcd.print("Luminosidade:    ");
  lcd.setCursor(0, 1);

  if (porcentagem >= 50) {
    lcd.print("Status: Bom      ");         // Exibe status "Bom"
    digitalWrite(ledVerde, HIGH);           // Liga LED verde
    digitalWrite(ledAmarelo, LOW);          // Desliga LED amarelo
    digitalWrite(ledVermelho, LOW);         // Desliga LED vermelho
    noTone(alarme);                         // Garante que o alarme está desligado
    buzzerTocando = false;
    jaSoouNaZonaVermelha = false;

  } else if (porcentagem >= 30) {
    lcd.print("Status: Medio    ");         // Exibe status "Médio"
    digitalWrite(ledVerde, LOW);            // Desliga LED verde
    digitalWrite(ledAmarelo, HIGH);         // Liga LED amarelo
    digitalWrite(ledVermelho, LOW);         // Desliga LED vermelho
    noTone(alarme);                         // Garante que o alarme está desligado
    buzzerTocando = false;
    jaSoouNaZonaVermelha = false;

  } else {
    lcd.print("Status: Ruim     ");         // Exibe status "Ruim"
    digitalWrite(ledVerde, LOW);            // Desliga LED verde
    digitalWrite(ledAmarelo, LOW);          // Desliga LED amarelo
    digitalWrite(ledVermelho, HIGH);        // Liga LED vermelho

    if (!jaSoouNaZonaVermelha && !buzzerTocando) {
      tone(alarme, 1000);                   // Liga o buzzer com frequência de 1000Hz
      tempoAlarme = millis();               // Registra o tempo de início do alarme
      buzzerTocando = true;
      jaSoouNaZonaVermelha = true;
    }

    if (buzzerTocando && millis() - tempoAlarme >= 3000) {
      noTone(alarme);                       // Desliga o buzzer após 3 segundos
      buzzerTocando = false;
    }
  }

  // Finaliza o experimento e gera relatório no monitor serial
  if (!experimentoFinalizado && tempoAtual - tempoInicial >= tempoExperimento) {
    experimentoFinalizado = true;

    int porcentagemMinima = map(maxLuminosidade, 0, 1023, 100, 0); // Calcula porcentagem mínima
    int porcentagemMaxima = map(minLuminosidade, 0, 1023, 100, 0); // Calcula porcentagem máxima
    int mediaLuminosidade = somaLuminosidade / quantidadeLeituras; // Calcula média de luminosidade
    int porcentagemMedia = map(mediaLuminosidade, 0, 1023, 100, 0); // Converte a média para porcentagem

    // Exibe o relatório no Monitor Serial
    Serial.println("Relatorio:");
    Serial.print("Luminosidade Máxima: ");
    Serial.print(porcentagemMaxima); // Mostra a luminosidade máxima em porcentagem
    Serial.println("%");
    Serial.print("Luminosidade Mínima: ");
    Serial.print(porcentagemMinima); // Mostra a luminosidade mínima em porcentagem
    Serial.println("%");
    Serial.print("Luminosidade Média: ");
    Serial.print(porcentagemMedia); // Mostra a luminosidade média em porcentagem
    Serial.println("%");

    // Exibe no LCD que o relatório foi enviado
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Relatorio:");
    lcd.setCursor(0, 1);
    lcd.print("Enviado");

    delay(5000); // Aguarda 5 segundos antes de resetar o experimento
    resetExperimento(); // Chama a função para reiniciar as variáveis
  }

  delay(500); // Intervalo entre as leituras do sensor
}

void resetExperimento() {
  maxLuminosidade = 0; // Reseta o valor máximo de luminosidade
  minLuminosidade = 1023; // Reseta o valor mínimo de luminosidade
  somaLuminosidade = 0; // Reseta a soma das leituras
  quantidadeLeituras = 0; // Reseta a quantidade de leituras realizadas
  tempoInicial = millis(); // Reinicia o tempo inicial do experimento
  experimentoFinalizado = false; // Permite um novo experimento
}
