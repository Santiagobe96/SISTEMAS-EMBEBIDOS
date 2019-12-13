const int windowSize = 8;
int circularBuffer[windowSize];
int* circularBufferAccessor = circularBuffer;
int iCount;
float med;
#include "muestra.h"
int valuesLength = sizeof(values) / sizeof(int);
int getMeasure()
{
  int static index = 0;
  index++;
  return values[index - 1];
}
int appendToBuffer(float value)
{
  *circularBufferAccessor = value;
  circularBufferAccessor++;
  if (circularBufferAccessor >= circularBuffer + windowSize)
    circularBufferAccessor = circularBuffer;
}
long sum;
float elementCount;
float mean;
float AddValue(float value)
{
  sum -= *circularBufferAccessor;
  sum += value;
  appendToBuffer(value);

  if (elementCount < windowSize)
    ++elementCount;
  return (float) sum / elementCount;
}
void setup()
{
  Serial.begin(9600);
  for (iCount = 0; iCount < valuesLength; iCount++)
  {
    med = AddValue(getMeasure());
    Serial.print(values[iCount]);
    Serial.print(" , ");
    Serial.println(med);
  }
}
void loop(){
}
