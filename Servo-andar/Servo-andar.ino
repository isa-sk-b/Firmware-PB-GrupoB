// Funcao para o andar do robo

//Inclusao das bibliotecas 
#include<Arduino.h>
#include<Servo.h>
// Declaracao dos pinos a serem colocados nos servomotores
#define pinDIR 24
#define pinESQ 30
#define pinQUADRIL 32
// Variaveis para definir a posicao do servo;
// Valores inciais atribuidos as posicoes; 
// > > >                             depois testar qual seria essa posição e em qual posição especificamente manter o servo parado

// Declaracao dos servomotores dos pes; 
Servo servodir;
Servo servoesq;
Servo servoquadril; 

// A ser executado uma unica vez;
void setup() {
  Serial.begin(9600);
  // Anexar os servomotores aos pinos
  servodir.attach(pinDIR); 
  servoesq.attach(pinESQ); 
  servoquadril.attach(pinQUADRIL);
  // Verificar quais posicoes dos servomotores correspondem ao 0 (Permite determinar a faixa dos angulos possiveis do servomotor)
  //    > > > > >       Depois setar essa parte para valores 0 para verificar qual o angulo que o servomotor precisa ficar parado para 
  
}

// Funcoes modulares para o andar do robo
void andar_direita() {
  delay(500); 
}

void andar_esquerda() {
  delay(500); // Esperar para que o servo atinja a posicao especificada > > Testar os tempos necessarios para que isso aconteça 
}

// Incrementos das posicoes dos Servos
int posDIR = 0; 
int incrementoDIR = +1; 
int posESQ = 180; 
int incrementoESQ = +1; 
int posQUADRIL = 0;
int incrementoQUADRIL = +1; 

void functeste() { // Funcionando! 
    if(posDIR>180 || posDIR<0) incrementoDIR = -incrementoDIR; 
    if(posQUADRIL>180 || posQUADRIL<0) incrementoQUADRIL = -incrementoQUADRIL; 
    if(posESQ<0 || posESQ>180) incrementoESQ =- incrementoESQ;
    posDIR += incrementoDIR; 
    posESQ += incrementoESQ;
    posQUADRIL += incrementoQUADRIL;
    servoquadril.write(posQUADRIL); 
    servodir.write(posDIR);
    servoesq.write(posESQ); 
    delay(10); // Variacao minima de angulo, logo nao precisa parar o codigo por tanto tempo 
}

void teste_basico() { // Funcionando!
  servodir.write(0);
  servoesq.write(180);
  delay(2000);
  servodir.write(90);
  servoesq.write(90);
  delay(2000); 
  servodir.write(180);
  servoesq.write(0);
  delay(2000);
}

// Codigo a ser repetido em loop: 
void loop() {
 functeste();
}



