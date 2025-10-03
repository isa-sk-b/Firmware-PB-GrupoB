// Arquivo com as funcoes parciais para verificar os pinos usados 
//Inclusao das bibliotecas 
#include <ESP32Servo.h>
// Pinos dos servos
#define pinDIR          18      // Pino funcionando!
#define pinESQ          19      // Pino funcionando!
#define pin_quadril_DIR 21      // Pino funcionando! 
#define pin_quadril_ESQ 33      // Pino funcionando!
#define pin_garra_DIR   32      // Pino funcionando!
#define pin_garra_ESQ   22      // Pino funcionando!
// Pinos do ultrassonico 
#define trigPin         2
#define echoPin         4
// Pinos para iluminacao 
#define ldr_sensor      26
#define redPin1          5
#define greenPin1        6
#define bluePin1         7 

// Declaracao dos servomotores dos pes
Servo servodir;
Servo servoesq;
Servo servo_quadril_DIR; 
Servo servo_quadril_ESQ;
Servo servo_garra_DIR;
Servo servo_garra_ESQ; 
int tmp = 2000; 

int valor_ldr;

void setup()
{
  servodir.attach(pinDIR); 
  servoesq.attach(pinESQ);
  servo_quadril_DIR.attach(pin_quadril_DIR);
  servo_quadril_ESQ.attach(pin_quadril_ESQ);
  servo_garra_DIR.attach(pin_garra_DIR);
  servo_garra_ESQ.attach(pin_garra_ESQ); 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ldr_sensor,INPUT);
  pinMode(ldr_sensor, INPUT);
  Serial.begin(9600);
}

// Funcao para teste de servo -> Gera delay de 3 segundos apos a execucao
void teste_servo() {            // Funcionando!
  // -- perna,quadril e garra
  servodir.write(0);
  servoesq.write(0);
  delay(tmp);
  servo_quadril_DIR.write(0);
  servo_quadril_ESQ.write(0); 
  delay(tmp);
  servo_garra_DIR.write(0);
  servo_garra_ESQ.write(0); 
  delay(tmp); 

  // -- perna, quadril e garra 
  servo_quadril_DIR.write(90); 
  servo_quadril_ESQ.write(90);
  delay(tmp);
  servodir.write(90);
  servoesq.write(90);
  delay(tmp); 
  servo_garra_DIR.write(90);
  servo_garra_ESQ.write(90); 
  delay(tmp); 

  // -- perna, quadril e garra
  servodir.write(180);
  servoesq.write(180);
  delay(tmp);
  servo_quadril_DIR.write(180); 
  servo_quadril_ESQ.write(180);
  delay(tmp);
  servo_garra_DIR.write(180);
  servo_garra_ESQ.write(180); 
  delay(tmp);
}

// Funcao para teste do Sensor Ultrassonico -> Gera delay de ??? segundos
int teste_ultrassonico() {      // A verificar...
  digitalWrite(trigPin, LOW);
  delay(2);
  digitalWrite(trigPin, HIGH);
  delay(10); 
  digitalWrite(trigPin, LOW);
  int duration = pulseIn(echoPin, HIGH); 
  int distance = (duration*.0343)/2; 
  delay(100);                           
  Serial.print("Distancia: "); 
  Serial.println(distance); 
}

// Funcao para teste do modulo Led RGB 
void teste_led_ldr() {         // A verificar... 
  valor_ldr = analogRead(ldr_sensor);
  Serial.print("Valor lido pelo LDR = "); 
  Serial.println(valor_ldr); 
}

// VOID LOOP 
void loop() {
  teste_servo(); 
  //teste_ultrassonico();
  //teste_led_ldr();
}
