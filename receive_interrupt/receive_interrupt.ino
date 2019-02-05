// demo: CAN-BUS Shield, receive data with interrupt mode
// when in interrupt mode, the data coming can't be too fast, must >20ms, or else you can use check mode
// loovee, 2014-6-13

#include <SPI.h>
#include "mcp_can.h"

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin


unsigned char flagRecv = 0;
unsigned char flagSent = 0;
unsigned char len = 0;
unsigned char buf[8];
char str[20];

void setup()
{
    Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");

    attachInterrupt(digitalPinToInterrupt(2), MCP2515_ISR, FALLING); // start interrupt
}

void MCP2515_ISR()
{

  unsigned long canId;
  while (CAN_MSGAVAIL == CAN.checkReceive()) 
  { 
    CAN.readMsgBuf(&len, buf);
    canId = CAN.getCanId();          
  }
  Serial.print(canId, HEX);
  Serial.print(":\t");
  
  for(int i = 0; i<len; i++)  
  {
    Serial.print(buf[i], HEX);
    Serial.print("\t");
  }
  
  Serial.println(); 
  
}

void loop()
{
   
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
