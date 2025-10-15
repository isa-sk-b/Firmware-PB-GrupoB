// Arquivo com as funcoes parciais para verificar os pinos usados 
//Inclusao das bibliotecas 
#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>
// Pinos dos servos
#define pinDIR          18      // Pino funcionando!
#define pinESQ          19      // Pino funcionando!
#define pin_quadril_DIR 21      // Pino funcionando! 
#define pin_quadril_ESQ 22      // Pino funcionando!
#define pin_garra_DIR   23      // Pino funcionando!
#define pin_garra_ESQ   33      // Pino funcionando! 
// Pinos do ultrassonico 
#define echoPin         27          
#define trigPin         26 
// Pinos para iluminacao 
#define LED_PIN1        13      // Pino funcionando! 
#define LED_PIN2        14      // Pino funcionando! 
#define ldr_sensor      25
// Numero de LEDs WS2812B conectados a ESP  
#define LED_COUNT       8

// Declaracao dos servomotores dos pes
Servo servodir;
Servo servoesq;
Servo servo_quadril_DIR; 
Servo servo_quadril_ESQ;
Servo servo_garra_DIR;
Servo servo_garra_ESQ; 
int tmp = 2000; 

// Declaracao de variaveis para modulo LED
Adafruit_NeoPixel strip1(LED_COUNT, LED_PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(LED_COUNT, LED_PIN2, NEO_GRB + NEO_KHZ800);

int valor_ldr;
float duration,distance; 

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

  strip1.begin();           // Initialize NeoPixel object
  strip1.setBrightness(10); // Set BRIGHTNESS to about 4% (max = 255)
  strip2.begin(); 
  strip2.setBrightness(10);
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
void teste_ultrassonico() {      // A verificar...
  digitalWrite(trigPin, LOW);
  delay(2);
  digitalWrite(trigPin, HIGH);
  delay(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); 
  distance = (duration*.0343)/2; 
  delay(100);                           
  Serial.print("Distancia (cm): "); 
  Serial.println(distance); 
}

// Funcao para teste do modulo Led RGB 
int teste_ldr() {                       // A verificar... 
  valor_ldr = analogRead(ldr_sensor);   // Faixa de valores: 0 a 1023 
  return valor_ldr; 
}

void teste_moduloLED() {
  strip1.clear(); 
  strip2.clear(); 
  
  for(int i=0; i<LED_COUNT; i++) {
    // Set the i-th LED to pure green:
    strip1.setPixelColor(i, 0, 255, 0);
    strip2.setPixelColor(i, 0, 255, 0);
    strip1.show();   
    strip2.show(); 
    delay(200); // Pausa antes do novo loop 
  }
}

// FUNCAO PRINCIPAL
void loop() {
  //teste_ultrassonico();
  teste_servo(); 
  if((teste_ldr())<200) teste_moduloLED(); 
  Serial.println("Distancia (cm): "); 
}


