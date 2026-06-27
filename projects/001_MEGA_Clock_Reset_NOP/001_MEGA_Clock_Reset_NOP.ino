/*
  Z80 Clock + Reset + NOP bus
  Arduino Mega 2560

  D8  / PH5 -> CLOCK hacia Z80 pin 6
  D10 / PB4 -> /RESET hacia Z80 pin 26

  D22-D29 / PA0-PA7 -> bus de datos Z80 D0-D7
  Todos en LOW para entregar 00h = NOP.
*/

const int Z80_CLK   = 8;   // D8  / PH5
const int Z80_RESET = 10;  // D10 / PB4

bool clkState = LOW;
unsigned long lastClockToggle = 0;

// Frecuencia inicial: 100 Hz
unsigned long halfPeriodMicros = 5000;

void setup() {
  // Clock y reset
  pinMode(Z80_CLK, OUTPUT);
  pinMode(Z80_RESET, OUTPUT);

  digitalWrite(Z80_CLK, LOW);

  // Mantener el Z80 reseteado mientras preparamos el bus
  digitalWrite(Z80_RESET, LOW);

  // Puerto A completo como salida:
  // Arduino Mega D22-D29 = PA0-PA7
  DDRA = 0xFF;    // PA0-PA7 como salidas
  PORTA = 0x00;   // D0-D7 en LOW -> byte 00h -> NOP

  Serial.begin(115200);

  Serial.println();
  Serial.println("Z80 Clock + Reset + NOP bus");
  Serial.println("--------------------------------");
  Serial.println("D8  / PH5 -> CLOCK");
  Serial.println("D10 / PB4 -> /RESET");
  Serial.println("D22-D29 / PA0-PA7 -> Z80 D0-D7 = 00h");
  Serial.println();

  Serial.println("Manteniendo /RESET en LOW durante 2 segundos...");
  delay(2000);

  digitalWrite(Z80_RESET, HIGH);

  Serial.println("/RESET liberado");
  Serial.println();
  Serial.println("Comandos:");
  Serial.println("1 = clock 1 Hz");
  Serial.println("2 = clock 10 Hz");
  Serial.println("3 = clock 100 Hz");
  Serial.println("4 = clock 1 kHz");
  Serial.println("r = pulso de reset");
}

void loop() {
  generarClock();
  leerComandos();
}

void generarClock() {
  unsigned long now = micros();

  if (now - lastClockToggle >= halfPeriodMicros) {
    lastClockToggle = now;
    clkState = !clkState;
    digitalWrite(Z80_CLK, clkState);
  }
}

void leerComandos() {
  if (!Serial.available()) {
    return;
  }

  char c = Serial.read();

  if (c == '1') {
    halfPeriodMicros = 500000;
    Serial.println("Clock: 1 Hz");
  }

  if (c == '2') {
    halfPeriodMicros = 50000;
    Serial.println("Clock: 10 Hz");
  }

  if (c == '3') {
    halfPeriodMicros = 5000;
    Serial.println("Clock: 100 Hz");
  }

  if (c == '4') {
    halfPeriodMicros = 500;
    Serial.println("Clock: 1 kHz");
  }

  if (c == 'r' || c == 'R') {
    Serial.println("Pulso de reset...");

    digitalWrite(Z80_RESET, LOW);
    delay(100);
    digitalWrite(Z80_RESET, HIGH);

    Serial.println("/RESET liberado");
  }
}