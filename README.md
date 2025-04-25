
# Projeto: Sistema de Monitoramento de Luminosidade com Arduino

## Descrição
Este projeto é um sistema de monitoramento de luminosidade ambiente utilizando um sensor LDR (Light Dependent Resistor), LEDs, buzzer (alarme sonoro) e um display LCD com comunicação I2C.

O objetivo é:
- Monitorar a luminosidade em tempo real.
- Indicar o nível de luminosidade através de LEDs (verde, amarelo e vermelho).
- Acionar um alarme sonoro em condições de luminosidade ruim.
- Exibir mensagens e status no display LCD.
- Realizar um experimento de 30 segundos coletando máximas e mínimas leituras.
- Após 30 segundos, apresentar um relatório via monitor serial e LCD.

## Componentes Utilizados
- Arduino Uno
- Sensor de Luminosidade (LDR)
- 1 LED verde
- 1 LED amarelo
- 1 LED vermelho
- 1 buzzer ativo
- 1 Display LCD 16x2 com módulo I2C
- Resistores
- Protoboard
- Cabos jumper

## Ligações (Wiring)
- **LDR**:
  - Um terminal ao 5V
  - Outro terminal ao A0 (com resistor pull-down para o GND)
- **LEDs**:
  - LED Verde: pino 12
  - LED Amarelo: pino 11
  - LED Vermelho: pino 10
- **Buzzer**: pino 13
- **LCD I2C**:
  - VCC -> 5V
  - GND -> GND
  - SDA -> A4
  - SCL -> A5

## Funcionamento
1. Ao iniciar, o LCD mostra uma animação de "Insight" seguido de uma mensagem de boas-vindas.
2. O sistema começa a monitorar a luminosidade:
   - Se a luminosidade for **≥50%**, o status é "Bom" (LED verde aceso).
   - Se estiver entre **30% e 49%**, o status é "Médio" (LED amarelo aceso).
   - Se for **<30%**, o status é "Ruim" (LED vermelho e amarelo acesos + buzzer soa por 3 segundos).
3. Durante o experimento de 30 segundos, os valores máximos e mínimos de luminosidade são armazenados.
4. Ao final dos 30 segundos:
   - O experimento é finalizado.
   - O LCD exibe "Experimento Finalizado".
   - O monitor serial mostra o relatório de luminosidade máxima e mínima.
   - O sistema é automaticamente resetado para novo experimento.

## Personalizações no LCD
- Caracteres personalizados (emojis, ícones) foram criados para uma melhor experiência visual.
- Animações foram implementadas com delays para efeito de digitação.

## Código-fonte
O código foi escrito em C++ para Arduino, utilizando as bibliotecas:
- `Wire.h` (comunicação I2C)
- `LiquidCrystal_I2C.h` (controle do display LCD)

## Como Usar
1. Monte o circuito conforme o esquema.
2. Carregue o código no Arduino usando a IDE do Arduino.
3. Abra o monitor serial para acompanhar o relatório final.
4. Observe a mudança de LEDs e o comportamento do alarme conforme variação da luz.

## Observações
- Se desejar alterar o tempo de duração do experimento, modifique a variável `tempoExperimento` no código.
- O endereço I2C do LCD pode variar (0x27 ou 0x3F). Ajuste no código conforme necessidade.

## Imagem do Circuito

![Circuito Montado](attachment:/mnt/data/88c1bb74-ac53-4a68-9cb8-d3f48c6f3a83.png)

---
Projeto desenvolvido para monitoramento didático de luminosidade utilizando Arduino.
