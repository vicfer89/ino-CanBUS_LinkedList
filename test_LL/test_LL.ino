#include "LinkedListLib.h"

// Funcion auxiliar para debug
void printArray(int* x, int length)
{
  for (int iCount = 0; iCount < length; iCount++)
  {
    Serial.print(x[iCount]);
    Serial.print(',');
  }
}

typedef struct _LL_Data
{
  unsigned char Data[8];
  int           Number;
}LL_Data;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  // Crear LinkedList
  LinkedList<LL_Data> linkedlist;
  
  // Ejemplos insert
  for(int i = 0; i < 25; i++)
  {
    LL_Data TmpData;
    TmpData.Number = i;
    TmpData.Data[3] += i;    
    linkedlist.InsertHead(TmpData);
  }

  // Ejemplos get
  while(linkedlist.GetSize() > 0)
  {
    LL_Data TmpData;
    TmpData = linkedlist.RemoveTail();
    //Serial.println(TmpData.Number);
    Serial.println(TmpData.Data, HEX);
  }
    
 }

void loop() {
  // put your main code here, to run repeatedly:

}
