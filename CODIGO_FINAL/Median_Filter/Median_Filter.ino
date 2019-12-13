const int windowSize = 3;
int buffer[windowSize];
int medianBuffer[windowSize];
int* medianBufferAccessor = medianBuffer;
#define MEDIAN(a, n) a[(((n)&1)?((n)/2):(((n)/2)-1))];
#include "muestra.h"
int valuesLength = sizeof(values) / sizeof(int);
int getMeasure()
{
  int static index = 0;
  index++;
  return values[index - 1];
}
int appendToBuffer(int value)
{
  *medianBufferAccessor = value;
  medianBufferAccessor++;
  if (medianBufferAccessor >= medianBuffer + windowSize)
    medianBufferAccessor = medianBuffer;
}
int elementCount;
float AddValue(int value)
{
  appendToBuffer(value);

  if (elementCount < windowSize)
    ++elementCount;
}
void setup()
{
  Serial.begin(115200);

  float timeMean = 0;
  for (int iCount = 0; iCount < valuesLength; iCount++)
  {
    float value = getMeasure();
    long timeCount = micros();

    AddValue(value);
    memcpy(buffer, medianBuffer, sizeof(medianBuffer));
    QuickSortAsc(buffer, 0, elementCount - 1);
    float med = MEDIAN(medianBuffer, windowSize);

    timeCount = micros() - timeCount;
    timeMean += timeCount;
    //Serial.print(value);
    //Serial.print(" , ");
    Serial.println(med);
    //delay(500);
  }
  //Serial.println(timeMean / valuesLength);
}
void loop() {
}
void QuickSortAsc(int* arr, const int left, const int right)
{
  int i = left, j = right;
  int tmp;

  int pivot = arr[(left + right) / 2];
  while (i <= j)
  {
    while (arr[i] < pivot) i++;
    while (arr[j] > pivot) j--;
    if (i <= j)
    {
      tmp = arr[i];
      arr[i] = arr[j];
      arr[j] = tmp;
      i++;
      j--;
    }
  };

  if (left < j)
    QuickSortAsc(arr, left, j);
  if (i < right)
    QuickSortAsc(arr, i, right);
}
