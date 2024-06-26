// Motores
#define M1 11  // Pino Motor Esquerda/Direita Porta IN2 ponte H
#define M2 8   // Pino Motor Esquerda/Direita Porta IN4 ponte H
#define dir1 9 // Pino Motor Esquerda/Direita Porta IN1 ponte H
#define dir2 10 // Pino Motor Esquerda/Direita Porta IN3 ponte H

// Infravermelho
#define pin_S1 A2 // Mid
#define pin_S2 A3 // Dir
#define pin_S3 A4 // Esq



int SensorEsq = 0;
int SensorDir = 0;
int SensorMid = 0;

// Velocidade padrão dos motores
const int v_Padrao = 100;

// Velocidade padrão para curvas
const int v_CurvaF = 100; // Frente
const int v_CurvaT = 160; // Tras

int status = 0; // 0 - Parado; 1 - Frente; 2 - Esquerda; 3 - Direita

unsigned long start = 0;

const int maxRecords = 100; // Tamanho máximo do vetor de tempo

struct time {
  unsigned long time;
  int state; // 1 - Frente, 2 - Esquerda, 3 - Direita
};

time tempo[100];
int state = 0;
bool replay_mode = false;
int replay = 0;
unsigned long ultimo_tempo = 0;

void setup() {
  Serial.begin(9600);
  // Pinos sensores infravermelhos
  pinMode(pin_S1, INPUT);
  pinMode(pin_S2, INPUT);
  pinMode(pin_S3, INPUT);
  


  // Setamos os pinos de controle dos motores como saída
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);

  start = millis();
}

void loop() {
  

  
    LerInfras();

    unsigned long  temp = millis();
    unsigned long perc = temp - start;

    if (state < 100) {
      tempo[state].time = perc;
      tempo[state].state = status;
      state++;
    }

    start = temp;

    if (SensorDir < 100) {
      Direita();
    } else if (SensorEsq < 100) {
      Esquerda();
    } else if (SensorMid < 100) {
      Parar();
    } else {
      Frente();
    }

    delay(100); // Pequeno atraso para evitar leituras muito rápidas
  
}

/* --- Funções dos sensores Infravermelho ---*/
void LerInfras() {
  SensorEsq = analogRead(pin_S3);
  SensorDir = analogRead(pin_S2);
  SensorMid = analogRead(pin_S1);
}

void Parar() { // Parar ambos os motores
  if (status != 0) {
    digitalWrite(M1, LOW);
    digitalWrite(dir1, LOW);
    digitalWrite(M2, LOW);
    digitalWrite(dir2, LOW);
    status = 0;
  }
}

void Esquerda() {
  if (status != 2) {
    digitalWrite(M1, HIGH);
    analogWrite(dir1, v_CurvaT);
    digitalWrite(M2, HIGH);
    analogWrite(dir2, v_Padrao);
    status = 2;
  }
}

void Direita() {
  if (status != 3) {
    digitalWrite(M2, HIGH);
    analogWrite(dir2, v_CurvaT);
    digitalWrite(M1, HIGH);
    analogWrite(dir1, v_Padrao);
    status = 3;
  }
}

void Frente() {
  if (status != 1) { // Se não já estava indo pra frente
    digitalWrite(M1, HIGH);
    analogWrite(dir1, v_Padrao);
    digitalWrite(M2, HIGH);
    analogWrite(dir2, v_Padrao);
    status = 1;
  }
}


