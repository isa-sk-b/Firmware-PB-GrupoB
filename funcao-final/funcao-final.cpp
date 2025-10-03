// Inclusao de bibliotecas 
#include<Servo.h>

// Definicao dos pinos 
#define pinDIRPE            18      // Pino funcionando!
#define pinESQPE            19      // Pino funcionando!
#define pinDIRQUAD          -1
#define pinESQQUAD          -1   
#define pinDIRGARRA         -1
#define pinESQGARRA         -1
#define ESQUERDA            1
#define DIREITA             0


// OBS: CUIDADO COM PROPAGACAO DOS DELAYS NAS FUNCOES!!      

// PARAMETROS DE CONTROLE PARA REGULAR O ANDAR DOS SERVOMOTORES 
// Definicao de constantes para ambos os servomotores 
int intervalo_servo = 100;          // Intervalo, em milissegundos, entre a chamada de cada servo  
int qtde_variacoes = 3;             // Regula quao brusca e a variacao dos angulos do servomotor 


// Relacionados aos angulos dos servomotores  
// O angulo parado precisa ser, no minimo, 
// QUADRIS  
int angulo_parado_quadril = 22.5;   // O angulo em que o quadril deve ficar em repouso (angulo de referencia) deve estar no meio dos angulos maximos e minimos  
int diferenca_Quad = 45;            // Faixa de angulos varrida (a distancia angular entre as posicoes das extremidades do servomotor)
int angulo_maximo_quadril = angulo_parado_quadril + diferenca_Quad/2; 
int angulo_minimo_quadril = angulo_parado_quadril - diferenca_Quad/2;
int variacao_quadril = diferenca_Quad/qtde_variacoes; 

// PES
int angulo_parado_pe = 22.5; 
int diferenca_Pe = 45;              
int angulo_maximo_pe = angulo_parado_pe + diferenca_Pe/2;
int angulo_minimo_pe = angulo_parado_pe - diferenca_Pe/2; 
int variacao_quadril = diferenca_Pe/qtde_variacoes; 


// DECLARACAO DOS SERVOS 
Servo servo_quadril[2]; 
Servo servo_pe[2]; 


void setup() {
    // Anexacao de pinos: 
    // Anexar os SERVOS aos pinos: 
    servo_quadril[DIREITA].attach(pinDIRQUAD); 
    servo_quadril[ESQUERDA].attach(pinESQQUAD); 
    servo_pe[DIREITA].attach(pinDIRPE);
    servo_pe[ESQUERDA].attach(pinESQPE);

    // Setar os SERVOS para uma posicao de repouso: 
    servo_quadril[ESQUERDA].write(angulo_parado_quadril); 
    servo_quadril[DIREITA].write(angulo_parado_quadril); 
    servo_pe[ESQUERDA].write(angulo_parado_pe);
    servo_pe[DIREITA].write(angulo_parado_pe); 
}


void dar_passo(int passo) {
    int i=0; 
    // Executar um codigo a depender se e passo para a direita ou para a esquerda 
    if(passo==ESQUERDA) {
        while(i<qtdeVariacoes) {
                if()
                servo_quadril[ESQUERDA].write(); 
                servo_pe[ESQUERDA].write();
                i++; 
        }
        
            /*servo_quadril[ESQUERDA].write(); 
            servo_pe[ESQUERDA].write();
            delay(intervalo_servo);
            servo_quadril[ESQUERDA].write(45); 
            servo_pe[ESQUERDA].write(30); 
            delay(intervalo_servo);
            servo_quadril[ESQUERDA].write();
            servo_pe[ESQUERDA].write();
            i++;*/
    } 
    else (passo==DIREITA) {
        servo_quadril[DIREITA].write(90); 
        servo_pe[DIREITA].write(15);
        delay(10);
        servo_quadril[DIREITA].write(); 
        servo_pe[DIREITA].write(30); 
    }

    // Terminar o passo com os estados de menor corrente (repouso)

}


// Deteccao da distancia em relacao a um obstaculo do robo
int distancia() {

}


void luminosidade() {

    
}


void loop() {

}