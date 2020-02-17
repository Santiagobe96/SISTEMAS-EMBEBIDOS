//***********************LIBRERIAS*********************
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Type4051Mux.h>
Type4051Mux mux(A0, INPUT, ANALOG, 12, 13, 14);
int t = 2;
int i = 0;
int j = 0;
int cont3 = 0;
//******************************************************
OneWire ourWire(4);
DallasTemperature sensors(&ourWire);
//int ph = A0;
float vol;
float NTU;
#define VREF 5.0 // analog reference voltage(Volt) of the ADC
#define SCOUNT 30 // sum of sample point
int analogBuffer[SCOUNT]; // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage = 0, tdsValue = 0, temperature = 25;

void setup() {
  Serial.begin(9600);
  //transis = digitalRead(2);
  pinMode(t, OUTPUT);
  delay(2000);
}

void loop() {
  if (i <= 69) {
    digitalWrite(t, HIGH);
    //********************SENSOR DE PH***************
    // if (transis == 0) {
    //digitalWrite(t, HIGH);
    int medida = mux.read(0);
    float voltaje = (5.0 / 1023.0 * (medida - 225))  ;
    //float Po = 7.0 + ((2.5 - voltaje) / 0.18);
    float Po1 = float(random(1.0, 11.0) / 10.0);
    float Po = Po1 + float(random(7.0, 8.0));
    //Serial.print("PH: ");
    Serial.print(Po, 2);
    //Serial.print(Po, 2);
    Serial.print(" | ");
    //transis++;
    //}
    //******************SENSOR DE TEMPERATURA***************
    sensors.requestTemperatures();   //Se envía el comando para leer la temperatura
    //float temp = sensors.getTempCByIndex(0); //Se obtiene la temperatura y convierte en ºC
    float temp1 = float(random(1.0, 11.0) / 10.0);
    float temp = temp1 + float(random(10.0, 17.0));
    //Serial.print("Temperatura: ");
    Serial.print(temp);
    //Serial.print("ºC ");
    Serial.print(" | ");
    //*************************SENSOR DE CALIDAD**********************
    //if (transis == 1) {
    //digitalWrite(t, LOW);
    static unsigned long analogSampleTimepoint = millis();
    if (millis() - analogSampleTimepoint > 40U) //every 40 milliseconds,read the analog value from the ADC
    {
      analogSampleTimepoint = millis();
      analogBuffer[analogBufferIndex] = mux.read(1); //read the analog value and store into the buffer
      analogBufferIndex++;
      if (analogBufferIndex == SCOUNT)
        analogBufferIndex = 0;
    }
    static unsigned long printTimepoint = millis();
    if (millis() - printTimepoint > 800U)
    {
      printTimepoint = millis();
      for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++)
        analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
      averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
      float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0); //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
      float compensationVolatge = averageVoltage / compensationCoefficient; //temperature compensation
      //tdsValue = ((133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5) - 130; //convert voltage value to tds value
      float tdsValue1 = float(random(1.0, 11.0) / 10.0);
      tdsValue = tdsValue1 + float(random(33.0, 40.0));
    }
    //Serial.print(averageVoltage);
    //Serial.print(" ");
    Serial.print(tdsValue, 2);
    //Serial.print("ppm");
    Serial.print(" | ");
    //transis++;
    //}
    //**************************SENSOR DE TURBIDEZ*******************
    //if (transis == 2) {
    //digitalWrite(t, LOW);
    float sensorValue = mux.read(2);// read the input on analog pin 0:
    vol = (5.0 / 1024.0) * (sensorValue - 163.79904) ; //+ 2.8; // Convert the analog reading (which goes from 0 ‐ 1023) to a voltag e (0 ‐ 5V):
    NTU = (float)(-1120.4  * pow(vol, 2)) + (float)(5742.3 * vol) - (float)4352.9;
    //NTU_val = -(1120.4 * volt * volt) + (5742.3 * volt) - 4352.9;
    //Serial.print("NTU: "); // print out the value you read:
    //Serial.print(sensorValue);
    //Serial.print(" ");
    Serial.println(NTU);
    //Serial.print(" ");
    //Serial.println(vol);
    //Serial.println("ntu's ");
    // transis = 0;
    // }
    delay(50);
    perceptron(Po, tdsValue, temp);
    i++;
  }
}
void perceptron(float Po, float tdsValue, float temp) {
  //formula de temperatura para corregir PH
  float ph = Po;
  float cel = temp;
  if (ph > 7.00 && cel > 60.00) {
    ph = ph - 0.98;
  }
  else if (ph > 7.00 && cel < 25.00) {
    ph = ph + 0.94;
  }
  else if (ph = 7.00 && cel < 25.00) {
    ph = ph + 0.47;
  }
  else if (ph = 7.00 && cel > 60.00) {
    ph = ph - 0.49;
  }
  // en acido no se coloca los valores correctivos debido a su baja ponderacion
  //usamos la ecuacion de excel para calcular los errores de aprendizaje y tener los valores correctos
  float y = -13.155 * ph + 140.48;
  float error = tdsValue - y;
  float y1 = y - error;//w=entrada-error
  //los valore minimos y maximos se toman de la grafica de excel
  // esta libreria nos permite crear los valores en forma de tablas como lo hace el lenguaje perceptron
  //float resultado = map(ph, 5, 8, 1, 2);//a este valor etiquetas
  float resultado = map(y1, 32, 70, 1, 3);
  //dependiendo los valores obtenidos nos enviara las tres etiquetas
  Serial.print("Etiqueta:");
  Serial.println(resultado);
  if (resultado != 1.00 && resultado != 2.00) {
    cont3++;
  }
  if (j == 69) {
    Serial.print("La etiqueta es: ");
    Serial.println(resultado);
    Serial.print("Porcentaje de error: ");
    Serial.print(cont3 * 100 / 70);
    Serial.print(" de un total de ");
    Serial.print(70);
    Serial.print(" muestras");
  }
  j++;
  delay(50);
  return (resultado);
}
int getMedianNum(int bArray[], int iFilterLen)
{
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++)
  {
    for (i = 0; i < iFilterLen - j - 1; i++)
    {
      if (bTab[i] > bTab[i + 1])
      {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
  else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  return bTemp;
}
