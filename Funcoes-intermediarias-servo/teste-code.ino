// Arquivo com as funcoes parciais para verificar os pinos usados 
//Inclusao das bibliotecas 
#include<Servo.h>
// Pinos dos servos
#define pinDIR          18      // Pino funcionando!
#define pinESQ          19      // Pino funcionando!
#define pin_quadril_DIR 33      // 
#define pin_quadril_ESQ 22
#define pin_garra_DIR   23 
#define pin_garra_ESQ   25
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

void setup()
{
  servodir.attach(pinDIR); 
  servoesq.attach(pinESQ);
  servo_quadril_DIR.attach(pin_garra_DIR);
  servo_quadril_ESQ.attach(pin_garra_ESQ); 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledRGB1, OUTPUT);
  pinMode(ledRGB2, OUTPUT); 
  //pinMode(ldr_sensor, INPUT);
}

// Funcao para teste de servo -> Gera delay de 3 segundos apos a execucao
void teste_servo() {      // Funcionando!
  servodir.write(0);
  servoesq.write(0);
  servo_quadril_DIR.write(0);
  delay(1000);
  servo_quadril_DIR.write(90); 
  servodir.write(90);
  servoesq.write(90);
  delay(1000); 
  servo_quadril_DIR.write(90);
  servodir.write(180);
  servoesq.write(180);
  delay(1000);
}
// Poderia representar uma passada, e logo depois disso executar o codigo para verificacao da distancia com o ultrassonico! 


// Funcao para teste do Sensor Ultrassonico -> Gera delay de ??? segundos
int ultrassonico() {      // A verificar...
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); 
  distance = (duration*.0343)/2; 
  //delay(100);                           -> Seria necessario colocar um delay para o codigo? 
  //                                        Isa: Creio que nao, porque os delays se propagam pelas funcoes, por isso nao teria problema de intervalo muito pequeno.  
  return distance;                                      // Retorna a distancia 
}
// Deve interromper a execucao do andar caso haja uma condicao E/OU ir para codigo de desvio de objetos, que chama void ultrassonico() para cada direcao (de 90 graus). 
// Pode fazer isso no main tambem (mais facil)


// Funcao para teste do Led RGB 
void ledRGB() {
    int  ldrStatus = analogRead(ldr_sensor);  
    if (ldrStatus <=80) {
      digitalWrite(ledRGB1,  HIGH);         
  }
    else {
digitalWrite(ledRGB1,  LOW);          
  }
}
// 


void loop() {
  if(teste_ultrassonico()>10)  { teste_servo(); }       // Caso a distancia seja maior que n centimetros, movimenta o servomotor. 
  //else { desvio_objetos(); }                     
  ledRGB();
  if(true) {teste_basico();} // Se a distancia for maior que 8cm, andar com o servo. 
}



void setup() {
  //Defining the pins as OUTPUT
  pinMode(redPin,  OUTPUT);              
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}
void  loop() {
  setColor(255, 0, 0); // Red Color
  delay(1000);
  setColor(0,  255, 0); // Green Color
  delay(1000);
  setColor(0, 0, 255); // Blue Color
  delay(1000);
  setColor(255, 255, 255); // White Color
  delay(1000);
  setColor(170, 0, 255); // Purple Color
  delay(1000);
  setColor(127, 127,  127); // Light Blue
  delay(1000);
}
void setColor(int redValue, int greenValue,  int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin,  greenValue);
  analogWrite(bluePin, blueValue);
}
