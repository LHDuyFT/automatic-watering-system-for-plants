#include <16F887.H>
#DEVICE  ADC = 10
#fuses   NOWDT,PUT,HS,NOPROTECT,NOLVP
#use     delay(clock=20M)
#DEFINE  RELAY  PIN_B0

const unsigned char MA7DOAN[10]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};

unsigned int8 I;
unsigned int32 KQ,temp;
unsigned int16 H;
int1  status = 0; //trang thai motor "0" la off, "1" la on


void main()
{
   //config GPIO
   SET_TRIS_C(0x00);
   SET_TRIS_D(0x00);
   SET_TRIS_A(0x01);
   SET_TRIS_B(0b0);
   
   
   //config ADC
   SETUP_ADC(ADC_CLOCK_DIV_32);
   SETUP_ADC_PORTS(SAN0);
   SET_ADC_CHANNEL(0);
   
   OUTPUT_C(MA7DOAN[0]);
   OUTPUT_D(MA7DOAN[0]);
   OUTPUT_LOW(RELAY);
   
   while(TRUE)
   {
      KQ = 0;
      FOR(I = 0;I<100;I++)
      {
         KQ = KQ + READ_ADC(); //Lay gia tri lien tuc 100 lan
         DELAY_MS(1);
      }
      
      KQ = KQ/100; //Gia tri trung binh cua 100lan do
      temp = (KQ*100)/1023; //chuyen doi ADC sang %
      temp  = 100 - temp; //chuyen tu % khô sang % do am
      H =(int16)temp;//do am dat
      
      //Hien thi tren LED
      output_C(MA7DOAN[H/10]);
      output_D(MA7DOAN[H%10]);
      
      //Do am duoi 65% thi bat motor tren 65% thi tat
      IF((H < 65)&&(status == 0))
      {
         status = 1;
         OUTPUT_HIGH(RELAY);
      }
      ELSE
      {
         IF((H > 65)&&(status == 1))
         {
            status =0;
            OUTPUT_LOW(RELAY);   
         }
      }
   }

}
