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
void teste_servo() {            // Funcionando!
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
int teste_ultrassonico() {      // A verificar...
  digitalWrite(trigPin, LOW);
  delay(2);
  digitalWrite(trigPin, HIGH);
  delay(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); 
  distance = (duration*.0343)/2; 
  //delay(100);                           -> Seria necessario colocar um delay para o codigo? 
  //                                        Isa: Creio que nao, porque os delays se propagam pelas funcoes, por isso nao teria problema de intervalo muito pequeno.  
  return distance;                                      // Retorna a distancia 
}
// Deve interromper a execucao do andar caso haja uma condicao E/OU ir para codigo de desvio de objetos, que chama void ultrassonico() para cada direcao (de 90 graus). 
// Pode fazer isso no main tambem (mais facil)


// Funcao para teste do modulo Led RGB 
void teste_led() {



    // Programacao do led em cascata -> possui dois modulos de led          
}


// VOID LOOP 
void loop() {
  if(teste_ultrassonico()>10)  { teste_servo(); }       // Caso a distancia seja maior que n centimetros, movimenta o servomotor. 
  //else { desvio_objetos(); }                          // Caso a distancia seja menor (identificacao de obstaculo), verificar qual distancia o motor pode andar
  teste_led();
}
