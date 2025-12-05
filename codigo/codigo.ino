#include <Adafruit_NeoPixel.h>
#include <ESP32Servo.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "stdio.h"

// PINOS 
// Módulo LED
#define PIN_WS2812BONE  13
#define PIN_WS2812BTWO  14
#define NUM_PIXELS      8          //The number of LEDs (pixels) on WS2812B LED strip

// LDR e Ultrassonico
#define ldr             25
#define echoPin         26
#define trigPin         27

// SERVOS
// Pinos 
#define QUADDIR_PIN     18
#define PEDIR_PIN       19
#define QUADESQ_PIN     21
#define PEESQ_PIN       22
#define GARRADIR_PIN    23
#define GARRAESQ_PIN    33

// Indices no vetor de servo 
#define QUADDIR     0 
#define PEDIR       1
#define QUADESQ     2
#define PEESQ       3
#define GARRADIR    4
#define GARRAESQ    5
 
// VARIAVEIS 
// MODULO LED 
#define NADA -1
#define AZUL 0
#define VERMELHO 1
#define VERDE 2
#define BRANCO 4 
#define maxDistanciaCor 100   // A distancia maxima que atualiza a cor, em cm. 
#define minDistanceMove 15    // Se a distancia for menor que essa, nao ocorre o movimento. 

Adafruit_NeoPixel ws2812bONE(NUM_PIXELS, PIN_WS2812BONE, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ws2812bTWO(NUM_PIXELS, PIN_WS2812BTWO, NEO_GRB + NEO_KHZ800);

// CONSTANTES GERAIS 
#define SOUND_SPEED 0.034
#define BAUD_RATE 115200
#define ESQUERDA 0
#define DIREITA 1 

// VARIAVEIS PARA REGULAR OS SERVOS
Servo servo[6]; 
// ANGULOS DE ESPERA DOS SERVOMOTORES 
// Posicao padrao para os pes 
#define pos_padrao_esquerdo 90 
#define pos_padrao_direito 90                 
#define pos_wait_quadrildireito 90              // Angulo de espera dos servos do pe 
#define pos_wait_quadrilesquerdo 90
#define pos_wait_garra_direita    90            // Angulo de espera dos servos da garra 
#define pos_wait_garra_esquerda 90
// Essas posicoes sao as que os servos devem ser colocados quando forem ser inseridos no robo. Uma vez que e utilizada como posicao 0 (padrao) dos servos. 
// Posicoes maximas e minimas: 
#define ang_closed_garra 130
#define ang_open_garra 70
#define MovEnable 1       /* Define se pode ocorrer o movimento */

// ESTADOS A SEREM ATUALIZADOS:
float distanceDetectedCm;
int luminosidade; 
int cor = NADA;
volatile bool andando = false;
int pos_cur_pedir;  
int pos_cur_quaddir; 
int pos_cur_peesq;
int pos_cur_quadesq; 
int pos_garra_esq;
int pos_garra_dir;
int print = 1;                // Variavel que indica se printa ou nao no monitor serial (Tomar cuidado para nao haver requisicoes do monitor serial ao mesmo tempo)    
/*
Logica para andando: 
- Quando ja estiver andando e for dado o comando de andar, nao reseta o andar. 
- Impede que ande quando a distancia entre obstaculo for menor que a distancia minima realize o andar (poderia ser so um AND dentro de um if, por isso talvez nem seja necessario)
*/

// TASK HANDLE
TaskHandle_t Task1;
TaskHandle_t Task2;

void setup()
{   
  Serial.begin(9600); 
  // Inicializa componentes
  ws2812bONE.begin();
  ws2812bONE.setBrightness(10);
  ws2812bTWO.begin();
  ws2812bTWO.setBrightness(10);
  pinMode(ldr, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo[QUADDIR].attach(QUADDIR_PIN);
  servo[PEDIR].attach(PEDIR_PIN);
  servo[QUADESQ].attach(QUADESQ_PIN);
  servo[PEESQ].attach(PEESQ_PIN);
  servo[GARRADIR].attach(GARRADIR_PIN);
  servo[GARRAESQ].attach(GARRAESQ_PIN);

  // Inicializa as task para utilizar o DualCore 
  // Create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(Task1code, "Task1", 8192, NULL, 2, &Task1, 0);                           
  delay(500); 

  // Create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(Task2code, "Task2", 8192, NULL, 1, &Task2, 1);          
  delay(500); 
}

// Task1code: Controla os sensores e os leds
// Codigo para as funcoes que usam menos delay 
/* A separacao usada para esse codigo nao e a adotada no projeto. */
void Task1code( void * pvParameters ){
  while(true){
  int tempo = 50; 
  /* Deteccao da leitura dos sensores */ 
  detectaDistancia(); 
  detectaLuz();  

  vTaskDelay(100);
  vTaskDelay(tempo / portTICK_PERIOD_MS);   // Suspender a task por um intervalo de tempo para 
  /* Isso permite que o EspWatchdog retorne o controle para a Esp32 */
  }
}


//Task2code: Controla o andar do robô
/* Proposta de solucao: Separar o processamento dos servos maiores do dos servos menores */
void Task2code( void * pvParameters ) {
  while(true){
  int tempo = 50;
  bateGarras(1); 
  modoBalada(); 

  vTaskDelay(tempo / portTICK_PERIOD_MS);
  }
}
//  setPosicaoPadrao(); 

void loop(){}


// --------------------------------------------------- ################# FUNCOES DO SERVOMOTOR: #################### ----------------------------------------------------------------
// Coloca as posicoes padroes dos servos (ou qualquer posicao)
void setPosicaoPadrao(){
  int tempo = 100; 
  
  if(print) {
    Serial.print("Setando posicao no servo Pe Direita...");
    Serial.println(pos_padrao_direito); 
  }
  servo[PEDIR].write(pos_padrao_direito);
  vTaskDelay(tempo);

  if(print) {
    Serial.print("Setando posicao no servo Pe Esquerda..."); 
    Serial.println(pos_padrao_esquerdo);
  }
  servo[PEESQ].write(pos_padrao_esquerdo);
  vTaskDelay(tempo);

  if(print) {
    Serial.print("Setando posicao no servo Quadril Direita..."); 
    Serial.println(pos_wait_quadrildireito); 
  }
  servo[QUADDIR].write(pos_wait_quadrildireito);
  vTaskDelay(tempo);

  if(print) {
    Serial.print("Setando posicao no servo Quadril Esquerda...");  
    Serial.println(pos_wait_quadrilesquerdo); 
  }
  servo[QUADESQ].write(pos_wait_quadrilesquerdo);
  vTaskDelay(tempo / portTICK_PERIOD_MS);

  if(print) {
    Serial.print("Setando posicao no servo Garra Esquerda..."); 
    Serial.println(pos_wait_garra_esquerda);
  }
  servo[GARRAESQ].write(pos_wait_garra_esquerda);
  vTaskDelay(tempo / portTICK_PERIOD_MS);

  if(print) { 
    Serial.print("Setando posicao no servo Garra Direita...");  
    Serial.println(pos_wait_garra_direita);  
  }
  servo[GARRADIR].write(pos_wait_garra_direita);
  vTaskDelay(tempo / portTICK_PERIOD_MS);
}

// Executar essa funcao toda vez que um loop de andar for terminado, para retornar os quadris a sua posicao neutra
void retornaPosicaoPadrao() {
  int qtde_iteracoes = 10; 

  moveUmServoSuavemente(QUADDIR,&pos_cur_quaddir,pos_wait_quadrildireito,qtde_iteracoes); 
  moveUmServoSuavemente(QUADESQ,&pos_cur_quadesq,pos_wait_quadrildireito,qtde_iteracoes);

}

// Bate as garras por uma certa quantidade de vezes fornecida 
// O angulo maximo a qual as garras devem atingir nao deve gerar o choque das garras, Assim como o angulo minimo nao pode gerar distensao (choque dentro das garras)
void bateGarras(int qtdeVezes) {
  int intervalo = 100;
  int qtde_iteracoes = 10; 

  servo[GARRAESQ].write(pos_wait_garra_direita);
  servo[GARRADIR].write(pos_wait_garra_esquerda); 

  for(int j=0; j<qtdeVezes; j++) {
    MoveDoisServosSuavemente(GARRAESQ,GARRADIR,pos_garra_dir,ang_open_garra,qtde_iteracoes,intervalo); 
    pos_garra_dir = ang_open_garra;
    pos_garra_esq = ang_open_garra;
    MoveDoisServosSuavemente(GARRAESQ,GARRADIR,pos_garra_dir,ang_closed_garra,qtde_iteracoes,intervalo); 
    pos_garra_dir = ang_closed_garra;
    pos_garra_esq = ang_closed_garra;
    MoveDoisServosSuavemente(GARRAESQ,GARRADIR,pos_garra_dir,pos_wait_garra_direita,qtde_iteracoes,intervalo); 
  }
}

// Oscila para a direita e para a esquerda uma certa quantidade de vezes 
void oscilaLados(int qtdeOscilacoes) {

    // Variaveis para regular a posicao dos servos 
    int pos_maxima =  40;
    int range = pos_maxima - 0;             // A abrangencia do movimento, utilizado para determinar o incremento  
    int pos_cur_esq = pos_wait_quadrilesquerdo;     // Permite a regulacao da posicao dos servos 
    int pos_cur_dir = pos_wait_quadrildireito;     // Podemos utilizar para atualizar a posicao e como criterio de break do loop
    int incremento = 10; 
    int tempo = 10; 

    // Executa as duas oscilacoes 
    for(int i = 0; i<qtdeOscilacoes; i++) {

      vTaskDelay(tempo / portTICK_PERIOD_MS);

        // Direita para cima: 
        servo[PEESQ].write(pos_padrao_esquerdo);   // Seta o pe esquerdo para ficar no chao 
        for(int j = 0; (pos_cur_dir<=pos_maxima) ; j ++) { 
            servo[PEDIR].write(pos_cur_dir); 
            pos_cur_dir+= incremento;
            vTaskDelay(tempo / portTICK_PERIOD_MS);
            if((pos_cur_dir>=pos_maxima)) break; 
        }
    
        // Esquerda para cima: 
        servo[PEDIR].write(pos_padrao_direito);  // Seta pe direito para ficar no chao
        for(int j = 0; (pos_cur_esq<=pos_maxima) ; j++) {
            servo[PEESQ].write(pos_cur_esq);
            pos_cur_esq += incremento;
            vTaskDelay(tempo / portTICK_PERIOD_MS);
            if((pos_cur_esq>=pos_maxima)) break; 
        }
    }
}


// ESSA FUNCAO TEM COMO FONTE O MODO "MOONWALK" DO VIDEO https://www.youtube.com/watch?v=VD6sgTo6NOY
void UmSwing() {
  /* Valores ja definidos */ 
  int intervalo = 100;          /* Intervalo de tempo entre os passos */
  int qtde_iteracoes = 5;
  int pos_max_pedir = 120; 
  int pos_max_peesq = 120; 

  // Primeira parte: Movimento comeca pela parte direita 
  /* Movimento ascendente do pe direito */
  moveUmServoSuavemente(PEDIR,&pos_cur_pedir,pos_max_pedir,qtde_iteracoes); 

  /* Movimento ascendente do pe esquerdo */
  moveUmServoSuavemente(PEESQ,&pos_cur_peesq,pos_max_peesq,qtde_iteracoes); 

  /* Movimento descendente do pe esquerdo */
  moveUmServoSuavemente(PEESQ,&pos_cur_peesq,pos_padrao_esquerdo,qtde_iteracoes);

  /* Movimento descendente do pe direito */
  moveUmServoSuavemente(PEDIR,&pos_cur_pedir,pos_padrao_direito,qtde_iteracoes);
  
  // Segunda parte: Movimento comeca pela parte esquerda 
  /* Movimento ascendente do pe esquerdo */
  moveUmServoSuavemente(PEESQ,&pos_cur_peesq,pos_max_peesq,qtde_iteracoes); 

  /* Movimento ascendente do pe direito */
  moveUmServoSuavemente(PEDIR,&pos_cur_pedir,pos_max_pedir,qtde_iteracoes); 

  /* Movimento descendente do pe direito */
  moveUmServoSuavemente(PEDIR,&pos_cur_pedir,pos_padrao_direito,qtde_iteracoes);

  /* Movimento descendente do pe esquerdo */
  moveUmServoSuavemente(PEESQ,&pos_cur_peesq,pos_padrao_esquerdo,qtde_iteracoes);
}

// ESSA FUNCAO TEM COMO FONTE O MODO "TIPTOE SWING" DO VIDEO https://www.youtube.com/watch?v=VD6sgTo6NOY
void UmSwingQuadris() {
}

/* ----------------------------------- ######### FUNCAO PRINCIPAL DE ANDAR ########### --------------------------- 
--------- ###### OBSERVACOES SOBRE ESSA FUNCAO ######### ------------- 
- O movimento dos pes esta sincronizado 
- Os quadris se movem em direcoes opostas de varredura de angulo
- Mas os quadris estao "alinhados" pelo diametro, apresentam a mesma diferenca de angulo em relacao a posicao em que o servo esta parado. 
OBS: Essa funcao pode usar apenas dois servomotores simultaneamente, no maximo. 
NAO FAZER NENHUM MOVIMENTO BRUSCO! 
*/
// O QUE PODE ESTAR ACONTECENDO COM O ANDAR ESQUERDO: A DIRECAO DOS ANGULOS E DIFERENTE 
void DaUmPassoFrente() {
  /* Valores ja definidos */ 
  int intervalo = 100;          /* Intervalo de tempo entre os passos */
  int pos_max_quadril_direita = 90;    /* A posicao minima deve ser a posicao padrao */
  int pos_max_quadril_esquerda = 90; 
  int pos_max_pe_direito = 90;
  int pos_max_pe_esquerdo = 90;  
  /* Espera-se que as posicoes minimas para ambos os pes sejam a mesma, que vai ser a posicao padrao. Sendo minimas as posicoes de menor diferenca em relacao ao solo.*/
  int pos_min_quadrildireito = pos_wait_quadrildireito; // DEPOIS REVER  SE PRECISA DESSA PARTE -> TALVEZ SO ALTERNE ENTRE AS POSICOES MAXIMAS E AS PADROES 
  int pos_min_quadrilesquerdo = pos_wait_quadrilesquerdo; 
  int qtde_iteracoes = 10;

  /* ----------------------------------------- MOVIMENTO DA PARTE DIREITA ------------------------------------------ */
  /* Retornar o quadril direito a posicao padrao */
  moveUmServoSuavemente(QUADDIR,&pos_cur_quaddir,pos_wait_quadrildireito,qtde_iteracoes); 

  /* 
  // Movimento descedente do pe direito para a posicao original 
  moveUmServoSuavemente(PEDIR,&pos_cur_pedir,pos_wait_pe_direito,qtde_iteracoes);
  pos_cur_pedir = pos_wait_pe; */
  vTaskDelay(500);


  /* ----------------------------------------------- MOVIMENTO DA PARTE ESQUERDA ------------------------------------------------- */
  /* Retornar o quadril esquerdo a posicao padrao */
  moveUmServoSuavemente(QUADESQ,&pos_cur_quadesq,pos_wait_quadrilesquerdo,qtde_iteracoes);

  /*
  //Movimento descendente do pe esquerdo para a posicao original 
  moveUmServoSuavemente(PEESQ,&pos_cur_peesq,pos_wait_pe_esquerdo,qtde_iteracoes); 
  pos_cur_peesq = pos_wait_pe; 
  */
}


/* ---------------------------------- #################### FUNCAO DE ANDAR PARA TRAS #################### ---------------------------------------- */
/*
IDEIA: Basicamente a funcao de dar passo para a frente, porem os limites sao invertidos. Varia por uma faixa de angulos "inversa" em relacao ao andar para frente. 
Hipotese: Apenas os angulos do quadril devem se alterar, os angulos dos pes continuam os mesmos. 
*/

void DaUmPassoTras() {
  // PENSAR EM SETAR AS POSICOES MINIMAS COMO A POSICAO PADRAO -> PRECISA FAZER ISSO 
}

/* ------------------ FUNCAO DE DESVIAR PARA UM LADO ---------------------- */
/* 
Desvia para o lado especificado em 90 graus. Apos isso precisaria realizar a verificacao antes de iniciar o andar. 
Video de referencia: https://www.youtube.com/watch?v=VD6sgTo6NOY      - Minuto a

*/
void DesviaUmLado(int side) {
  switch(side) {
    case DIREITA: 



    break; 
    case ESQUERDA: 
    break; 
  }
}

/* ------------------------------- ####################### FUNCOES MODULARES PARA MEXER OS SERVOS ##################### --------------------------------------- */

// Move o servo suavemente de uma posicao para outra. 
// OBS: NAO ATUALIZA A POSICAO, ELA PRECISA SER ATUALIZADA NA FUNCAO EM QUE E CHAMADA. 
void moveUmServoSuavemente(int s, int *pos_inicial, int pos_final, int qtde_iteracoes) {
  int incremento = (pos_final - *pos_inicial)/qtde_iteracoes; 
  int pos_cur = *pos_inicial; 
  int intervalo = 100;

  for(int i=0; i<qtde_iteracoes; i++) {
    if(incremento<0 && pos_cur<=pos_final) break; 
    if(incremento>=0 && pos_cur>=pos_final) break; 
    servo[s].write(pos_cur);
    pos_cur += incremento; 
    vTaskDelay(intervalo); 
  }
  servo[s].write(pos_final);
  *pos_inicial = pos_final; 
}

// OBS: A POSICAO PRECISA SER ATUALIZADA NA FUNCAO QUE E CHAMADA
// Dois servos movem nas mesmas posicoes e taxas 
void MoveDoisServosSuavemente(int s1, int s2, int pos_inicial, int pos_final, int qtde_iteracoes, int intervalo) {
  int incremento = (pos_final - pos_inicial)/qtde_iteracoes; 
  int pos_cur = pos_inicial; 

  for(int i=0; i<qtde_iteracoes; i++) {
    if(incremento<=0 && pos_cur<=pos_final) break; 
    if(incremento>=0 && pos_cur>=pos_final) break; 
    servo[s1].write(pos_cur);
    servo[s2].write(pos_cur);
    pos_cur += incremento; 
    vTaskDelay(intervalo); 
  }
  servo[s1].write(pos_final);
  servo[s2].write(pos_final); 
}


// -------------------------------------- ################## FUNCOES DE SENSORES ################### ------------------------------------------
void detectaDistancia() {
  int duration; 
  digitalWrite(trigPin, LOW);
  vTaskDelay(2);
  digitalWrite(trigPin, HIGH);
  vTaskDelay(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceDetectedCm = duration * SOUND_SPEED/2;
  if(print) {
    Serial.print("Distancia detectada no ultrassonico: "); 
    Serial.println(distanceDetectedCm); 
  }
  vTaskDelay(10 / portTICK_PERIOD_MS);
}

void detectaLuz() {
  luminosidade = analogRead(ldr);
  if(print) {
    Serial.print("Luminosidade detectada: ");
    Serial.println(luminosidade);
  } 
  vTaskDelay(10);
}

// -------------------------------------- ################## FUNCOES DO MODULO LED ################### ------------------------------------------
void modoBalada() {
  ws2812bONE.clear(); 
  ws2812bTWO.clear(); 
  int tempo = 10;

  // Cor Vermelha
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {         
    ws2812bONE.setPixelColor(pixel, ws2812bONE.Color(255, 0, 0));  
    ws2812bTWO.setPixelColor(pixel, ws2812bTWO.Color(255, 0, 0));  
    ws2812bONE.show();                                          
    ws2812bTWO.show(); 
    vTaskDelay(tempo / portTICK_PERIOD_MS);
  }
  // Cor Verde 
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {         
    ws2812bONE.setPixelColor(pixel, ws2812bONE.Color(0, 255, 0));  
    ws2812bTWO.setPixelColor(pixel, ws2812bTWO.Color(0, 255, 0));
    ws2812bONE.show();                                     
    ws2812bTWO.show();
    vTaskDelay(tempo / portTICK_PERIOD_MS); 
  }
  // Cor Azul 
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {        
    ws2812bONE.setPixelColor(pixel, ws2812bONE.Color(0, 0, 255));  
    ws2812bTWO.setPixelColor(pixel, ws2812bTWO.Color(0, 0, 255)); 
    ws2812bONE.show();                                          
    ws2812bTWO.show(); 
    vTaskDelay(tempo / portTICK_PERIOD_MS);
  }
  // Cor Branca 
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {        
    ws2812bONE.setPixelColor(pixel, ws2812bONE.Color(255, 255, 255));  
    ws2812bTWO.setPixelColor(pixel, ws2812bTWO.Color(255, 255, 255)); 
    ws2812bONE.show();                                          
    ws2812bTWO.show(); 
    vTaskDelay(tempo / portTICK_PERIOD_MS);
  }
}

// Setar uma cor para os Leds (usado para evitar repeticao de codigo)
void setCor(int corLigar) {
  int cores[3];
  switch (corLigar) {
    case VERMELHO: 
      cores[0] = 255;
      cores[1] = 0;
      cores[2] = 0;
    break; 
    case VERDE: 
      cores[0] = 0;
      cores[1] = 255;
      cores[2] = 0;
    break; 
    case AZUL: 
      cores[0] = 0;
      cores[1] = 0;
      cores[2] = 255;
    break; 
    case BRANCO: 
      cores[0] = 255;
      cores[1] = 255;
      cores[2] = 255;
    break; 
    default:
      cores[0] = 0; 
      cores[1] = 0;
      cores[2] = 0; 
    break; 
  }

  for(int i=0; i<NUM_PIXELS; i++) {
    ws2812bONE.setPixelColor(i, ws2812bONE.Color(cores[0],cores[1],cores[2]));  
    ws2812bTWO.setPixelColor(i, ws2812bTWO.Color(cores[0],cores[1],cores[2]));  
    ws2812bONE.show(); 
    ws2812bTWO.show(); 
  }
}
