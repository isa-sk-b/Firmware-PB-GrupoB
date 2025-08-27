// FUNCAO PARA O ANDAR DO ROBO

//Inclusao das bibliotecas 
#include<Arduino.h>
#include<Servo.h>

// Declaracao dos pinos a serem colocados nos servomotores
//     > > >  >                        verificar qual seria o melhor posicionamento dos pinos, se seria melhor que eles fossem proximos ou separados...
#define pinDIR 24
#define pinESQ 30 
#define pinQUADRIL_DIR 32
#define pinQUADRIL_ESQ 34
// Variaveis para definir a posicao do servo;
// Valores inciais atribuidos as posicoes; 
// > > > >                            depois testar qual seria essa posição e em qual posição especificamente manter o servo parado


// Declaracao dos servomotores (objeto); 
Servo pe_dir;
Servo pe_esq;
Servo quadril_dir; // VERIFICAR SE VAI USAR MESMO TRES SERVOS PARA O QUADRIL OU SE SÓ UM JÁ BASTA
Servo quadril_esq; 

// Variaveis de controle para o movimento do quadril 
int pos_quadril = 90; 
int incremento_quadril = +1; 
int min_quadril = 60;
int max_quadril = 120; // Variacao de 30 graus no angulo de movimentacao do quadril

// A ser executado uma unica vez;
void setup() {
  // Anexar os servomotores aos pinos
  pe_dir.attach(pinDIR); 
  pe_esq.attach(pinESQ); 
  quadril_dir.attach(pinQUADRIL_DIR); // Verificar se precisa de dois servomotores ou se daria para fazer com apenas um 
  quadril_esq.attach(pinQUADRIL_ESQ); 
  // Verificar quais posicoes dos servomotores correspondem ao 0 (Permite determinar a faixa dos angulos possiveis do servomotor)
  //    > > > > >                     depois setar essa parte para valores 0 para verificar qual o angulo que o servomotor precisa ficar parado para atingir os angulos que precisamos
  // SUGESTÃO: Talvez escrever todos os servos começando em 90° para estabelecer os valores de referencia e ele poder tanto assumir angulos negativos em relacao ao referencial quanto positivos?
  /*pe_dir.write(90);
  pe_esq.write(90); 
  quadril_dir.write(90); 
  quadril_esq.write(90);
  */
}

void movimento_quadril() {


}

// Funcoes modulares para o andar do robo
void andar_frente() {
  // Para cada etapa do andar, definir um loop diferente
  //Loop: pe direito avança 
  while(1) {
    if() break; 
  }





  //Loop: pe esquerdo avança
  while(1) {

    if() break; 
  }

}

void girar_esquerda() {


}

void girar_direita() {


}

// Codigo a ser repetido em loop: 
void loop() {

  // Criar alguma rotina de interrupcao no meio do codigo

}



// usar a mesma logica do incremento para definir o andar do robo!! 
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

