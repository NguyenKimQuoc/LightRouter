#include <stdio.h>
#include "onboard.h"
#include "bitmasks.h"
#include "delay.h"
#include "dht22.h"
#include "uart.h"
//#include "global.h"
#define DHT11 P0_5

int Receive_data(void)  /* Receive data */
{
 int q,c=0; 
 for (q=0; q<8; q++)
 {
   
  while(DHT11==0);/* check received bit 0 or 1 */
  _delay_us(40);
  if(DHT11 == 1) /* If high pulse is greater than 30ms */
  c = (c<<1)|(0x01);/* Then its logic HIGH */
  else  /* otherwise its logic LOW */
  c = (c<<1);
  while(DHT11==1);
//  UART_String("5");
 }
 return c;
}

int DHT22_Measure(void){
  int I_RH,D_RH,I_Temp,D_Temp,CheckSum;
  char dat[20];
  P0DIR |= (1<<5); 
  DHT11 = 1;
  _delay_ms(250);
  DHT11 = 0; 
  _delay_ms(20); 
  DHT11 = 1;  
  _delay_us(30);
  P0DIR &= ~(1<<5);
//  UART_String("toi day roi");
  while(DHT11==1);
  while(DHT11==0);
  while(DHT11==1);
  I_RH=Receive_data();
  D_RH=Receive_data();
  I_Temp=Receive_data();
  D_Temp=Receive_data(); 
  CheckSum=Receive_data();
  if(CheckSum == ((I_RH + D_RH + I_Temp + D_Temp)& 0xFF)){
  sprintf(dat,"Hum = %d.%d",I_RH,D_RH);
  UART_String(dat);
  
  sprintf(dat,"Tem = %d.%d",I_Temp,D_Temp);
  UART_String(dat);
  UART_String("--------------");
  return 1;
  }else{
    UART_String("ERROR");
    return 0;
  }
}