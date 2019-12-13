//***********************LIBRERIAS*********************
#include <OneWire.h>
#include <DallasTemperature.h>
//******************************************************

OneWire ourWire(4);
DallasTemperature sensors(&ourWire);
int ph = A0;
float vol;
float NTU;
#define VREF 3.3 // analog reference voltage(Volt) of the ADC
#define SCOUNT 30 // sum of sample point
int analogBuffer[SCOUNT]; // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage = 0, tdsValue = 0, temperature = 25;
/////////////Multiplexor////////
const int selectPins[3] = {13, 12, 14}; // S0~2, S1~3, S2~4
const int zInput = A0; // Connect common (Z) to A0 (analog input)

void setup() {
  Serial.begin(115200);
    for (int i=0; i<3; i++){
    pinMode(selectPins[i], OUTPUT);
    digitalWrite(selectPins[i], HIGH);
  }
  pinMode(zInput, INPUT); // Set up Z as an input
  sensors.begin();
  Serial.println("Y0\tY1\tY2\tY3\tY4\tY5\tY6\tY7");
  Serial.println("---\t---\t---\t---\t---\t---\t---\t---");
 
}
//*********************SENSOR DE PH***************
void loop() {
    int medida = analogRead(ph);
    double voltaje = 3 / 1023.0 * medida;
 //   Serial.print("Voltaje: ");
  //  Serial.print(voltaje, 3);
    float Po = 7 + ((2.5 - voltaje) / 0.18);
 //   Serial.print("\tPH: ");
  //  Serial.println(Po, 3);

  //  //******************SENSOR DE TEMPERATURA***************
  sensors.requestTemperatures();   //Se envía el comando para leer la temperatura
  float temp = sensors.getTempCByIndex(0); //Se obtiene la temperatura y convierte en ºC
  Serial.print("Temperatura= ");
  Serial.print(temp);
  Serial.println("\t ºC ");
  //**************************SENSOR DE TURBIDEZ*******************
    float sensorValue = analogRead(A0);// read the input on analog pin 0:
    vol = sensorValue * (5 / 1024.0); // Convert the analog reading (which goes from 0 ‐ 1023) to a voltag e (0 ‐ 5V):
    NTU = (float)(164.16 * pow(vol, 2)) - (float)1269.3 * vol + (float)2453.1;
   // Serial.print("NTU "); // print out the value you read:
  //  Serial.print(NTU);
   // Serial.print("\tVOL: ");
   // Serial.println(vol);
  //  Serial.println(' ');
  //  //*************************SENSOR DE CALIDAD**********************
static unsigned long analogSampleTimepoint = millis();
  if (millis() - analogSampleTimepoint > 40U) //every 40 milliseconds,read the analog value from the ADC
  {
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(A0); //read the analog value and store into the buffer
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
    tdsValue = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5; //convert voltage value to tds value
  //  Serial.print("voltaje: ");
  //  Serial.print(averageVoltage, 2);
  //  Serial.print("V");
  //  Serial.print("\tTDS: ");
  //  Serial.print(tdsValue, 0);
  //  Serial.println("ppm");
  }
  delay(500);
 // Serial.println(' ');
  //Serial.println(' ');

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
  
    for (byte pin=0; pin<=7; pin++)
  {
    selectMuxPin(pin); // Select one at a time
    int inputValue = analogRead(A0); // and read Z
    Serial.print(String(inputValue) + "\t");
  }
  Serial.println();
  delay(1000);
}

void selectMuxPin(byte pin)
{
  for (int i=0; i<3; i++)
  {
    if (pin & (1<<i))
      digitalWrite(selectPins[i], HIGH);
    else
      digitalWrite(selectPins[i], LOW);
  }
}
