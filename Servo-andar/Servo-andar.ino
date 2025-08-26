// Funcao para o andar do robo

//Inclusao das bibliotecas 
#include<Arduino.h>
#include<Servo.h>
// Declaracao dos pinos a serem colocados nos servomotores
#define pinDIR 22;
#define pinESQ 30;
// Variaveis para definir a posicao do servo;
// Valores inciais atribuidos as posicoes; 
// > > >                             depois testar qual seria essa posição e em qual posição especificamente manter o servo parado
int posDIR = 0; 
int incrementoDIR = +1; 
int posESQ = 180; 
int incrementoESQ = -1; 

// Declaracao dos servomotores dos pes; 
Servo servodir;
Servo servoesq;

// A ser executado uma unica vez;
void setup() {
  // Anexar os servomotores aos pinos
  servodir.attach(pinDIR); 
  servoesq.attach(pinESQ); 
  // Verificar quais posicoes dos servomotores correspondem ao 0 (Permite determinar a faixa dos angulos possiveis do servomotor)
  //    > > > > >       Depois setar essa parte para valores 0 para verificar qual o angulo que o servomotor precisa ficar parado para 
  servodir.write(posDIR); 
  servoesq.write(posESQ);
}

// Funcoes modulares para o andar do robo
void andar_direita() {
  delay(500); 
}

void andar_esquerda() {
  delay(500); // Esperar para que o servo atinja a posicao especificada > > Testar os tempos necessarios para que isso aconteça 
}

void func-teste() {
    if(posDIR>180 || posDIR<0) incrementoDIR = -incrementoDIR; 
    if(posESQ<0 || posESQ>180) incrementoESQ =- incrementoESQ;
    posDIR += incrementoDIR; 
    posESQ += incrementoESQ;
    servodir.write(posDIR);
    servoesq.write(posESQ); 
    delay(20); // Variacao minima de angulo, logo nao precisa parar o codigo por tanto tempo 
}

// Codigo a ser repetido em loop: 
void loop() {
  func-teste();
}



