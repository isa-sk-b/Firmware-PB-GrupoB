// Arquivo com as funcoes parciais para definir o andar do robo com os Servomotores
//Inclusao das bibliotecas 
#include<Arduino.h>
#include<Servo.h>
// Declaracao dos pinos a serem colocados nos servomotores
//     > > >                         verificar qual seria o melhor posicionamento dos pinos, se seria melhor que eles fossem proximos ou separados...
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


