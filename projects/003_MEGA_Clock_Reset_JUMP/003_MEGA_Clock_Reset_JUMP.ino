/*
  Z80 Mini ROM Test - JP 0000h
  Arduino Mega 2560

  D8  / PH5 -> CLOCK hacia Z80 pin 6
  D10 / PB4 -> /RESET hacia Z80 pin 26

  D22-D29 / PA0-PA7 -> bus de datos Z80 D0-D7

  D30 -> Z80 A0, pin 30
  D31 -> Z80 A1, pin 31
  D32 -> Z80 A2, pin 32

  Programa emulado:
    0000h: C3 00 00   JP 0000h
*/

const int Z80_CLK   = 8;    // D8  / PH5
const int Z80_RESET = 10;   // D10 / PB4

const int Z80_A0 = 30;
const int Z80_A1 = 31;
const int Z80_A2 = 32;

bool clkState = LOW;
unsigned long lastClockToggle = 0;

// Frecuencia inicial: 10 Hz
unsigned long halfPeriodMicros = 50000;

// ROM mínima de 8 bytes, indexada por A2 A1 A0
const byte miniRom[8] = {
  0xC3,   // 0000h: JP nn
  0x00,   // 0001h: byte bajo de 0000h
  0x00,   // 0002h: byte alto de 0000h
  0x00,   // 0003h: NOP
  0x00,   // 0004h: NOP
  0x00,   // 0005h: NOP
  0x00,   // 0006h: NOP
  0x00    // 0007h: NOP
};

void setup() {
  pinMode(Z80_CLK, OUTPUT);
  pinMode(Z80_RESET, OUTPUT);

  digitalWrite(Z80_CLK, LOW);
  digitalWrite(Z80_RESET, LOW);

  pinMode(Z80_A0, INPUT);
  pinMode(Z80_A1, INPUT);
  pinMode(Z80_A2, INPUT);

  // Arduino Mega D22-D29 = PA0-PA7
  DDRA = 0xFF;
  PORTA = 0x00;

  Serial.begin(115200);

  Serial.println();
  Serial.println("Z80 Mini ROM Test - JP 0000h");
  Serial.println("--------------------------------");
  Serial.println("Programa emulado:");
  Serial.println("0000h: C3 00 00   JP 0000h");
  Serial.println();
  Serial.println("D8  / PH5 -> CLOCK");
  Serial.println("D10 / PB4 -> /RESET");
  Serial.println("D22-D29 / PA0-PA7 -> Z80 D0-D7");
  Serial.println("D30 -> Z80 A0");
  Serial.println("D31 -> Z80 A1");
  Serial.println("D32 -> Z80 A2");
  Serial.println();

  Serial.println("Manteniendo /RESET en LOW durante 2 segundos...");
  delay(2000);

  actualizarBusDatos();

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
  actualizarBusDatos();
  generarClock();
  leerComandos();
}

void actualizarBusDatos() {
  byte addr = leerDireccionBaja();
  byte data = miniRom[addr];

  PORTA = data;
}

byte leerDireccionBaja() {
  byte a0 = digitalRead(Z80_A0) ? 1 : 0;
  byte a1 = digitalRead(Z80_A1) ? 1 : 0;
  byte a2 = digitalRead(Z80_A2) ? 1 : 0;

  return (a2 << 2) | (a1 << 1) | a0;
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
    resetZ80();
  }
}

void resetZ80() {
  digitalWrite(Z80_RESET, LOW);
  delay(100);

  actualizarBusDatos();

  digitalWrite(Z80_RESET, HIGH);
  Serial.println("/RESET liberado");
}