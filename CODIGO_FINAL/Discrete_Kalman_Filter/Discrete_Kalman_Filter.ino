#define ARM_MATH_CM3
#include <arm_math.h>
#define tam_sig 400

extern float32_t senal[tam_sig];
// Kalman Filter    El filtro de Kalman mantiene los dos primeros momentos de la distribucion del estado
float32_t R = 6e-3, Q = 1e-4; //matrices de covarianzas de ruido
float32_t Xpe0[tam_sig];
float32_t Xe1[tam_sig]; 
float32_t Xe0[tam_sig];
float32_t P1 = 1.0;
float32_t Ppe0[tam_sig];
float32_t P0[tam_sig];
float32_t  K = 0.0;

void setup() {
  Serial.begin(9600);
}

void loop() {

filtro(&senal[0],tam_sig);
  
}

void filtro(float32_t *sig_in , int32_t tam){
  for(int i=0; i<tam;i++){
  Xpe0[i] = Xe1[i];
  Ppe0[i] = P1 + Q;
  K = Ppe0[i] / (Ppe0[i] + R);
  Xe0[i] = Xpe0[i] + K * (sig_in[i] - Xpe0[i]);
  P0[i] = (1 - K) * Ppe0[i];
  Serial.print(sig_in[i]*100);
  Serial.print(",");
  Serial.println(Xe0[i]*100);
  Xe1[i] = Xe0[i];
  P1 = P0[i];
  delay(20);
  }
}
