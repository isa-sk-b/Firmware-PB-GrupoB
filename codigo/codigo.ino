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
#define echoPin         27
#define trigPin         26

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
#define maxDistanciaCor 100   // A distancia maxima que atualiza a cor, em cm. 
#define minDistanceMove 15    // Se a distancia for menor que essa, nao ocorre o movimento. 

Adafruit_NeoPixel ws2812bONE(NUM_PIXELS, PIN_WS2812BONE, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ws2812bTWO(NUM_PIXELS, PIN_WS2812BTWO, NEO_GRB + NEO_KHZ800);

// CONSTANTES GERAIS 
#define SOUND_SPEED 0.034
#define BAUD_RATE 115200
#define ESQ 0
#define DIR 1 

// VARIAVEIS PARA REGULAR OS SERVOS
Servo servo[6]; 
#define wait_pos_leg  90            // Angulo de espera dos servos do quadril -> Posicao "neutra"
#define wait_pos_feet 90            // Angulo de espera dos servos do pe 
#define wait_pos_claw 90            // Angulo de espera dos servos da garra 
// Essas posicoes sao as que os servos devem ser colocados quando forem ser inseridos no robo. Uma vez que e utilizada como posicao 0 (padrao) dos servos. 
// Posicoes maximas e minimas: 
#define ang_closed_garra 110 
#define ang_open_garra 90
#define MovEnable 1       /* Define se pode ocorrer o movimento */

// ESTADOS A SEREM ATUALIZADOS:
float distanceDetectedCm;
int luminosidade; 
int cor = NADA;
volatile bool andando = false;
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
void Task1code( void * pvParameters ){
  while(true){
  int tempo = 50; 
  detectaDistancia(); 
  detectaLuz();  
  vTaskDelay(tempo / portTICK_PERIOD_MS);   // Suspender a task por um intervalo de tempo para 
  /* Isso permite que o EspWatchdog retorne o controle para a Esp32 */
  }
}


//Task2code: Controla o andar do robô
void Task2code( void * pvParameters ) {
  while(true){
    int tempo = 50; 
    vTaskDelay(tempo / portTICK_PERIOD_MS);
  }
}

void loop(){}

// FUNCOES: 

// FUNCOES DO SERVOMOTOR: 
// Coloca as posicoes padroes dos servos (ou qualquer posicao)
void setPosicaoPadrao(){
  int tempo = 100; 
  int print = 1;      // Variavel que indica se printa ou nao no monitor serial (Tomar cuidado para nao haver requisicoes do monitor serial ao mesmo tempo)
  int posicao[6];
  
  posicao[PEDIR] = 90; 
  posicao[PEESQ] = 90; 
  posicao[QUADDIR] = 90; 
  posicao[QUADESQ] = 90;
  posicao[GARRADIR] = 90;
  posicao[GARRAESQ] = 90;
  
  if(print) {
    Serial.print("Setando posicao no servo Pe Direita...");
    Serial.println(posicao[PEDIR]); 
  }
  servo[PEDIR].write(posicao[PEDIR]);
  vTaskDelay(tempo / portTICK_PERIOD_MS);

  if(print) {
    Serial.print("Setando posicao no servo Pe Esquerda..."); 
    Serial.println(posicao[PEESQ]);
  }
  servo[PEESQ].write(posicao[PEESQ]);
  vTaskDelay(tempo / portTICK_PERIOD_MS);

  if(print) {
    Serial.print("Setando posicao no servo Quadril Direita..."); 
    Serial.println(posicao[QUADDIR]); 
  }
  servo[QUADDIR].write(posicao[QUADDIR]);
  vTaskDelay(tempo / portTICK_PERIOD_MS);

  if(print) {
    Serial.print("Setando posicao no servo Quadril Esquerda...");  
    Serial.println(posicao[QUADESQ]); 
  }
  servo[QUADESQ].write(posicao[QUADESQ]);
  vTaskDelay(tempo / portTICK_PERIOD_MS);

  if(print) {
    Serial.print("Setando posicao no servo Garra Esquerda..."); 
    Serial.println(posicao[GARRAESQ]);
  }
  servo[GARRAESQ].write(posicao[GARRAESQ]);
  vTaskDelay(tempo / portTICK_PERIOD_MS);

  if(print) { 
    Serial.print("Setando posicao no servo Garra Direita...");  
    Serial.println(posicao[GARRADIR]);  
  }
  servo[GARRADIR].write(posicao[GARRADIR]);
  vTaskDelay(tempo / portTICK_PERIOD_MS);
}

// Bate as garras por uma certa quantidade de vezes fornecida 
// O angulo maximo a qual as garras devem atingir nao deve gerar o choque das garras, Assim como o angulo minimo nao pode gerar distensao (choque dentro das garras)
void bateGarras(int qtdeVezes) {
  int tempo = 10;       // O tempo entre as iteracoes do loop 
  int pos_garra_esq;
  int pos_garra_dir;
  int incremento = (ang_closed_garra - ang_open_garra)/10;    // O incremento a cada iteracao do loop, mudar o denominador para maior ou para menor para regular 
  // Garante que ap posicao maxima e minima nunca seja ultrapassada   -> TOMAR CUIDADO COM ISSO! 

  // Setar os servos para posicao de espera 
  servo[GARRADIR].write(wait_pos_claw);
  servo[GARRAESQ].write(wait_pos_claw);         // Depois definir se a posicao de espera da garra e a mesma que o angulo minimo ocupado pela garra

  // Movimento de bater as garras
  for(int i=0; i<qtdeVezes ; i++) {
    pos_garra_dir = ang_open_garra;
    pos_garra_esq = ang_open_garra;
    vTaskDelay(tempo / portTICK_PERIOD_MS);
  // Transicao nao tao brusca para fechar a garra
    while((pos_garra_dir<=ang_closed_garra) && (pos_garra_esq<=ang_closed_garra)) {
      servo[GARRADIR].write(pos_garra_dir); 
      servo[GARRAESQ].write(pos_garra_esq);
      pos_garra_dir+= incremento;
      pos_garra_esq+= incremento; 
      vTaskDelay(tempo / portTICK_PERIOD_MS);
    }
    
    vTaskDelay(tempo / portTICK_PERIOD_MS);

    pos_garra_dir = ang_closed_garra; 
    pos_garra_esq = ang_closed_garra; 
    // Transicao nao tao brusca para abrir a garra
    while((pos_garra_dir>=ang_open_garra) && (pos_garra_esq>=ang_open_garra)) {
      servo[GARRADIR].write(pos_garra_dir); 
      servo[GARRAESQ].write(pos_garra_esq);
      pos_garra_dir-= incremento;
      pos_garra_esq-= incremento; 
      vTaskDelay(tempo / portTICK_PERIOD_MS);
    }
    pos_garra_dir = ang_open_garra;
    pos_garra_esq = ang_open_garra;
    vTaskDelay(tempo / portTICK_PERIOD_MS);
  }
}

// Oscila para a direita e para a esquerda uma certa quantidade de vezes 
void oscilaLados(int qtdeOscilacoes) {
    // Setar os servos para a posicao de espera 
    servo[PEESQ].write(wait_pos_feet);
    servo[PEDIR].write(wait_pos_feet);

    // Variaveis para regular a posicao dos servos 
    int pos_maxima =  40;
    int range = pos_maxima - 0;         // A abrangencia do movimento, utilizado para determinar o incremento  
    int pos_cur_esq = wait_pos_leg;     // Permite a regulacao da posicao dos servos 
    int pos_cur_dir = wait_pos_leg;     // Podemos utilizar para atualizar a posicao e como criterio de break do loop
    int incremento = 10; 
    int tempo = 10; 

    // Executa as duas oscilacoes 
    for(int i = 0; i<qtdeOscilacoes; i++) {

      vTaskDelay(tempo / portTICK_PERIOD_MS);

        // Direita para cima: 
        servo[PEESQ].write(wait_pos_feet);   // Seta o pe esquerdo para ficar no chao 
        for(int j = 0; (pos_cur_dir<=pos_maxima) ; j ++) { 
            servo[PEDIR].write(pos_cur_dir); 
            pos_cur_dir+= incremento;
            vTaskDelay(tempo / portTICK_PERIOD_MS);
            if((pos_cur_dir>=pos_maxima)) break; 
        }
    
        // Esquerda para cima: 
        servo[PEDIR].write(wait_pos_feet);  // Seta pe direito para ficar no chao
        for(int j = 0; (pos_cur_esq<=pos_maxima) ; j++) {
            servo[PEESQ].write(pos_cur_esq);
            pos_cur_esq += incremento;
            vTaskDelay(tempo / portTICK_PERIOD_MS);
            if((pos_cur_esq>=pos_maxima)) break; 
        }
    }
}

// Move os servos (funcao teste de movimento)
void moveServos() {
  int tempo = 100; 

  if((distanceDetectedCm>=minDistanceMove) && (MovEnable==1))  {
  servo[QUADDIR].write(0);
  vTaskDelay(tempo / portTICK_PERIOD_MS);
  servo[PEDIR].write(0);
  vTaskDelay(tempo / portTICK_PERIOD_MS);
  servo[QUADESQ].write(0);
  vTaskDelay(tempo / portTICK_PERIOD_MS);
  servo[PEESQ].write(0);
  vTaskDelay(tempo / portTICK_PERIOD_MS);
  servo[GARRADIR].write(0);
  vTaskDelay(tempo / portTICK_PERIOD_MS);
  servo[GARRAESQ].write(0);
  vTaskDelay(tempo / portTICK_PERIOD_MS);
  }

  if((distanceDetectedCm>=minDistanceMove) && (MovEnable==1))  { 
  servo[QUADDIR].write(90);
  vTaskDelay(tempo / portTICK_PERIOD_MS);
  servo[PEDIR].write(90);
  vTaskDelay(tempo / portTICK_PERIOD_MS);
  servo[QUADESQ].write(90);
  vTaskDelay(tempo / portTICK_PERIOD_MS);
  servo[PEESQ].write(90);
  vTaskDelay(tempo / portTICK_PERIOD_MS);
  servo[GARRADIR].write(90);
  vTaskDelay(tempo / portTICK_PERIOD_MS);
  servo[GARRAESQ].write(90); 
  vTaskDelay(tempo / portTICK_PERIOD_MS);
  }

  if((distanceDetectedCm>=minDistanceMove) && (MovEnable==1))  {
  servo[QUADDIR].write(180);
  vTaskDelay(tempo / portTICK_PERIOD_MS);
  servo[PEDIR].write(180);
  vTaskDelay(tempo / portTICK_PERIOD_MS);
  servo[QUADESQ].write(180);
  vTaskDelay(tempo / portTICK_PERIOD_MS);
  servo[PEESQ].write(180);
  vTaskDelay(tempo / portTICK_PERIOD_MS);
  servo[GARRADIR].write(180);
  vTaskDelay(tempo / portTICK_PERIOD_MS);
  servo[GARRAESQ].write(180); 
  vTaskDelay(tempo / portTICK_PERIOD_MS);
  }
}

// FUNCOES DE SENSORES: 
void detectaDistancia() {
  int duration; 
  int print = 1; 
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
  int print = 1; 
  luminosidade = analogRead(ldr);
  if(print) {
    Serial.print("Luminosidade detectada: ");
    Serial.println(luminosidade);
  } 
  vTaskDelay(10 / portTICK_PERIOD_MS);
}

// FUNCOES DO MODULO LED:
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
    default:
      cores[0] = 0; 
      cores[1] = 0;
      cores[2] = 0; 
    break; 
  }

  for(int i=0; i<NUM_PIXELS; i++) {
    ws2812bONE.setPixelColor(i, ws2812bONE.Color(cores[0],cores[1],cores[2]));  
    ws2812bTWO.setPixelColor(i, ws2812bTWO.Color(cores[0],cores[1],cores[2]));  
  }
}

// 2. CODIGOS PRONTOS E TESTADOS: 
// Atualiza os modulos LEDs para a cor que foi detectada pela camera 
/*
void atualizarLEDS(){
  switch (cor){
    case NADA:
      ws2812bONE.clear();
      ws2812bTWO.clear();
      ws2812bONE.show();
      ws2812bTWO.show();
      break;
    case AZUL:
      if(distanceDetectedCm<=maxDistanciaCor) {
        nh.loginfo("Setando LED azul");
        for(int i=0;i<NUM_PIXELS;i++) {
          ws2812bONE.setPixelColor(i, ws2812bONE.Color(0,0,255));  
          ws2812bTWO.setPixelColor(i, ws2812bONE.Color(0,0,255));  
          ws2812bONE.show();
          ws2812bTWO.show();
        }
      }
      break;
    case VERMELHO:
      if(distanceDetectedCm<=maxDistanciaCor) {
      nh.loginfo("Setando LED vermelho");
        for(int i=0;i<NUM_PIXELS;i++) {
          ws2812bONE.setPixelColor(i, ws2812bONE.Color(255,0,0));  
          ws2812bONE.show();
          ws2812bTWO.show();
        }
      }
      break;
    case VERDE:
    if(distanceDetectedCm<=maxDistanciaCor) {
      nh.loginfo("Setando LED verde");
      for(int i=0;i<NUM_PIXELS;i++) {
        ws2812bONE.setPixelColor(i, ws2812bONE.Color(0,255,0));  
        ws2812bONE.show();
        ws2812bTWO.show();
      }
    }
      break;
    default:
      ws2812bTWO.clear();
      ws2812bONE.clear();
      ws2812bONE.show();
      ws2812bTWO.show();
      break;
  }
}*/

// Da uma certa quantidade de passos para o lado 
void moveLado(int side, int qtdePassos) {
    // Setar os servos para posicao de espera 
    servo[PEDIR].write(wait_pos_feet); 
    servo[PEESQ].write(wait_pos_feet); 

    // Variaveis para regular o movimento dos servos
    int tempo = 10; 
    
    // Movimento ocorre com o pe do lado para o qual esta direcionado o movimento se 
    // Para que ele se movimente de lado, 
    //if(side==DIREITA) servo[].write(40)
    //else servo[].write(140); 
    //delay(tempo/2); 

    //servo[].write(); 
    //servo[].write(); 
    vTaskDelay(tempo / portTICK_PERIOD_MS);
}