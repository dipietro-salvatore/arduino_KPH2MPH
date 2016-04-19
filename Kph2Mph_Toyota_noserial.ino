/*
  CAN-BUS Shield Bridge Demo
  Requires two CAN Bus Shields

  SCK -> D13
  SI  -> D11
  SO  -> D12

  GND -> GND
  VCC -> 5V
  
  First shield should be left default = CS is on D9 and INT is on D2
  Second shield needs modified so CS is on D10 and INT is on D3
*/

#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];

long unsigned int speedId = 0x0b4;
String output;
unsigned int miles = 0;


MCP_CAN CAN0(9);                                // Set first CAN interface CS to pin 9
MCP_CAN CAN1(10);                               // Set second CAN interface CS to pin 10

void setup()
{    
  while(CAN0.begin(CAN_1000KBPS) != CAN_OK) {
    delay(100);
  }
  
  while(CAN1.begin(CAN_1000KBPS) != CAN_OK) {
    delay(100);
  }
}


void loop()
{  
    
    
    if(CAN_MSGAVAIL == CAN0.checkReceive())                         
    {
      CAN0.readMsgBuf(&len, rxBuf);     
      rxId = CAN0.getCanId();
      
      if(rxId == speedId){           
        unsigned int speedH = (unsigned int) rxBuf[5]<<8;
        unsigned int speedL = (unsigned int) rxBuf[6];
        speedH |= speedL;

        miles = speedH * 0.62;
        rxBuf[5] =  (unsigned char) ((miles >> 8) & 0xff);
        rxBuf[6] =  (unsigned char) miles;
      }
      
      CAN1.sendMsgBuf(rxId, 0, len, rxBuf);     
      
    }
    
    if(CAN_MSGAVAIL == CAN1.checkReceive())   
    {    
      CAN1.readMsgBuf(&len, rxBuf);          
      rxId = CAN1.getCanId();           
      CAN0.sendMsgBuf(rxId, 0, len, rxBuf);  
    } 
}



/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
