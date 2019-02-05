// demo: CAN-BUS Shield, receive data with interrupt mode
// when in interrupt mode, the data coming can't be too fast, must >20ms, or else you can use check mode
// loovee, 2014-6-13

#include <SPI.h>
#include "mcp_can.h"
#include "LinkedListLib.h"

#define CANDIR 0x1FFFFFFE
// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;
volatile bool f_LastFrame = false;
unsigned char buf[8]; 
unsigned char len = 0;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

typedef struct _CAN_Packet
{
  unsigned char len;
  unsigned char Data[8];
  unsigned long canId;
}CAN_Packet;

LinkedList<CAN_Packet> linkedlist_CAN_RX;
LinkedList<CAN_Packet> linkedlist_CAN_TX;

unsigned char flagRecv = 0;

char str[20];

void setup()
{
    Serial.begin(230400);

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
  CAN_Packet Temp_RX_PKT;
  while (CAN_MSGAVAIL == CAN.checkReceive()) 
  { 
    CAN.readMsgBuf(&Temp_RX_PKT.len, Temp_RX_PKT.Data);
    Temp_RX_PKT.canId = CAN.getCanId();          
  }

  linkedlist_CAN_RX.InsertHead(Temp_RX_PKT);
  
  if(Temp_RX_PKT.Data[0] && 0x80 == true)
  {
    f_LastFrame = true;
  }
  

}

void loop()
{
      if(f_LastFrame == true)
      {
        while(linkedlist_CAN_RX.GetSize() > 0)
        {
          CAN_Packet TmpData;
          TmpData = linkedlist_CAN_RX.RemoveTail();
          for(int i = 0; i < TmpData.len; i++)
            Serial.println(TmpData.Data[i],HEX);
        }
      }
      else
      {
        unsigned char Packet0[8] = {0x80, 0x03, 0x03, 0x04, 0xF9, 0x00, 0x00, 0x00};
        //unsigned char Packet1[8] = {0x01, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        //unsigned char Packet2[8] = {0x02, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        //unsigned char Packet3[8] = {0x83, 0x05, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00};
        
        CAN.sendMsgBuf(CANDIR, 1, 8, Packet0); 
        //delay(10);
        //CAN.sendMsgBuf(CANDIR, 1, 8, Packet1); 
        //delay(10);
        //CAN.sendMsgBuf(CANDIR, 1, 8, Packet2); 
        //delay(10);
        //CAN.sendMsgBuf(CANDIR, 1, 8, Packet3); 
        delay(1000);
      }
       
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
