#include <Adafruit_NeoPixel.h>
#include <ESP32Servo.h>

// Módulo LED
#define PIN_WS2812B     13
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

// Cores que podem ser detectadas
#define NADA -1
#define AZUL 0
#define VERMELHO 1
#define VERDE 2

// CONSTANTES 
#define SOUND_SPEED 0.034
#define BAUD_RATE 115200
#define ESQ 0
#define DIR 1 

// VARIAVEIS PARA REGULAR OS SERVOS
Servo servo[6]; 
#define wait_pos_leg 90             // Angulo de espera dos servos do quadril -> Posicao "neutra"
#define wait_pos_feet 90            // Angulo de espera dos servos do pe -> Posicao "parada", "neutra"

Adafruit_NeoPixel ws2812b(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

// ESTADOS A SEREM ATUALIZADOS:
long duration;
float distanceCm;
int luz;
int cor = NADA;
volatile bool andar = false;

// TASK HANDLE
TaskHandle_t Task1;
TaskHandle_t Task2;

void setup()
{
  // Inicializa componentes
  ws2812b.begin();
  ws2812b.setBrightness(10);
  pinMode(ldr, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo[0].attach(QUADDIR_PIN);
  servo[1].attach(PEDIR_PIN);
  servo[2].attach(QUADESQ_PIN);
  servo[3].attach(PEESQ_PIN);
  servo[4].attach(GARRADIR_PIN);
  servo[5].attach(GARRAESQ_PIN);

    // Inicializa as task para utilizar o DualCore 
  // Create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 2, &Task1, 0);                           
  delay(500); 

  // Create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(Task2code, "Task2", 10000, NULL, 1, &Task2, 1);          
  delay(500); 

}

// Task1code: Controla os sensores e os leds
// Codigo para as funcoes que usam menos delay 
void Task1code( void * pvParameters ){
  while(true){

  }
}


//Task2code: Controla o andar do robô
void Task2code( void * pvParameters ){
  while(true){

  }
}

void loop(){

}

// FUNCOES A SEREM CHAMADAS: 
// FUNCOES DO SERVOMOTOR: 
void fazDancinha() {
    int move1[] = {60,120,} 
    QUADPE.write(); 



}

// Da um passo para o lado 
void moveLado(int side, int tempo) {
    // Setar os servos para posicao de espera 
    servo[PEDIR].write(wait_pos_feet); 
    servo[PEESQ].write(wait_pos_feet); 
    
    // Movimento ocorre com o pe do lado para o qual esta direcionado o movimento se 
    // Para que ele se movimente de lado, 
    if(side==DIREITA) servo[].write(40)
    else servo[].write(140); 
    delay(tempo/2); 

    servo[].write(); 
    servo[].write(); 
    delay(tempo/2); 
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

    // Executa as duas oscilacoes 
    for(int i = 0; i<qtdeOscilacoes; i++) {

        // Direita para cima: 
        servo[PEESQ].write(wait_pos_feet);   // Seta o pe esquerdo para ficar no chao 
        for(int j = 0; (pos_cur_dir<=pos_maxima) ; j ++) { 
            servo[PEDIR].write(pos_cur_dir); 
            pos_cur_dir+= incremento;
            delay(10); 
            if((pos_cur_dir>=pos_maxima)) break; 
        }
    
        // Esquerda para cima: 
        servo[PEDIR].write(wait_pos_feet);  // Seta pe direito para ficar no chao
        for(int j = 0; (pos_cur_esq<=pos_maxima) ; j++) {
            servo[PEESQ].write(pos_cur_esq);
            pos_cur_esq += incremento;
            delay(10);
            if((pos_cur_esq>=pos_maxima)) break; 
        }
    }
}


// 2. CODIGOS PRONTOS E TESTADOS: 
void atualizarLEDS(){
  switch (cor){
    case NADA:
      ws2812b.clear();
      ws2812b.show();
      break;
    case AZUL:
      nh.loginfo("Setando LED azul");
      for(int i=0;i<NUM_PIXELS;i++) {
        ws2812b.setPixelColor(i, ws2812b.Color(0,0,255));  
        ws2812b.show();
      }
      break;
    case VERMELHO:
      nh.loginfo("Setando LED vermelho");
      for(int i=0;i<NUM_PIXELS;i++) {
        ws2812b.setPixelColor(i, ws2812b.Color(255,0,0));  
        ws2812b.show();
      }
      break;
    case VERDE:
      nh.loginfo("Setando LED verde");
      for(int i=0;i<NUM_PIXELS;i++) {
        ws2812b.setPixelColor(i, ws2812b.Color(0,255,0));  
        ws2812b.show();
      }
      break;
    default:
      ws2812b.clear();
      ws2812b.show();
      break;
  }
}

void DaUmPasso(){

}

void setPosicaoPadrao(){
    servo1.write(45);
    delay(100);
    servo2.write(45);
    delay(100);
    servo3.write(45);
    delay(100);
    servo4.write(45);
    delay(100);
    servo5.write(45);
    delay(100);
    servo6.write(45);
    delay(100);
}