// FUNCAO PARA O ANDAR DO ROBO

//Inclusao das bibliotecas 
#include<Arduino.h>
#include<Servo.h>

// Declaracao dos pinos a serem colocados nos servomotores
//     > > >  >                        verificar qual seria o melhor posicionamento dos pinos, se seria melhor que eles fossem proximos ou separados...
#define pinDIR 24
#define pinESQ 30 
#define pinQUADRIL_DIR 32; 
#define pinQUADRIL_ESQ 34;
#define pinQUADRIL 35;

// Variaveis para definir a posicao do servo;
// Valores inciais atribuidos as posicoes; 
// > > > >                            depois testar qual seria essa posicao e em qual posicao especificamente manter o servo parado


// Declaracao dos servomotores (objeto); 
Servo pe_dir;
Servo pe_esq;
Servo quadril_dir; // VERIFICAR SE VAI USAR MESMO DOIS SERVOS PARA O QUADRIL OU SE SO UM JA BASTA 
Servo quadril_esq;
Servo quadril; 

// Variaveis de controle para o movimento do quadril -> ir testando valores diferentes
int incremento_quadril = +1; 
int pos_quadril = 90; // Posicao inicial do quadril
// Variaveis de controle que podem ser utilizadas nos dois casos:
int min_quadril = 60;
int max_quadril = 120; // Variacao de 30 graus no angulo de movimentacao do quadril

// Caso tenham dois motores: 
int pos_quadril_dir = 90;
int pos_quadril_esq = 90;
int incremento_quad_dir = +1; 
int incremento_quad_esq = -1;


// A ser executado uma unica vez;
void setup() {
  // Anexar os servomotores aos pinos
  pe_dir.attach(pinDIR); 
  pe_esq.attach(pinESQ); 

  //Dois servomotores 
  quadril_dir.attach(pinQUADRIL_DIR);
  quadril_esq.attach(pinQUADRIL_ESQ); 

  // Um servomotor
  quadril.attach(pinQUADRIL); 
  // Verificar quais posicoes dos servomotores correspondem ao 0 (Permite determinar a faixa dos angulos possiveis do servomotor)
  //    > > > > >                     depois setar essa parte para valores 0 para verificar qual o angulo que o servomotor precisa ficar parado para atingir os angulos que precisamos
  // SUGESTAO: Talvez escrever todos os servos comecando com 90 graus para estabelecer os valores de referencia e ele poder tanto assumir angulos negativos em relacao ao referencial quanto positivos?
  // Setando as posicoes:
  pe_dir.write(90);
  pe_esq.write(90); 

  // Caso tenha apenas um servomotor no quadril:
  quadril.write(90);

  // Caso tenham dois servomotores no quadril: 
  quadril_dir.write(90); 
  quadril_esq.write(90);
}

// Movimento do pe do servo 
void movimento_pe(Servo servo) {


}


// Para dois servomotores distintos no quadril: basta colocar os incrementos opostos 
//                > > > > >                 precisa ter um jeito de salvar dois servomotores a
void movimento_quadril() {
  // Codigo para um unico servomotor para o quadril 
  if(pos_quadril<min_quadril || pos_quadril>max_quadril) incremento_quadril = - incremento_quadril;
  pos_quadril += incremento_quadril; 
  quadril.write(pos_quadril);

  // Codigo para dois servomotores para o quadril 
  if(pos_quadril_dir<min_quadril || pos_quadril_dir>max_quadril) incremento_quad_dir = - incremento_quad_dir;
  if(pos_quadril_esq<min_quadril || pos_quadril_esq>max_quadril) incremento_quad_esq = - incremento_quad_esq; //Inversao do eixo quando atinge os limites do angulo 
  pos_quadril_dir += incremento_quad_dir;  
  pos_quadril_esq += incremento_quad_esq;
  quadril_dir.write(pos_quadril_dir);
  quadril_esq.write(pos_quadril_esq); 
}


void andar_frente() {
  
  delay(10); // Espera o servo atingir a posicao setada 

}

void girar_esquerda() {


}

void girar_direita() {


}

// Codigo a ser repetido em loop: 
void loop() {

  // Criar alguma rotina de interrupcao no meio do codigo

}
