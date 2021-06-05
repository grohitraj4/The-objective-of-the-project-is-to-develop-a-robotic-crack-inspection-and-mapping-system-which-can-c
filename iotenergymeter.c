#include<reg51.h>
#define lcd_data P2

sbit  lcd_rs   =  P2^0;  
sbit  lcd_en   =  P2^1;


unsigned char rcg,pastnumber[11],i,count=0,chr='x',chr1='x';
unsigned int temp=0,hbt=0;

unsigned char q,ql,rcv,rcv1,rcv2,rcv3,rcv4,rcv5,msg1[15],msg2[15],r;



sbit ir       = P1^1; 
sbit relay    = P1^0; 

sbit buzzer = P3^7;

unsigned convert(unsigned int);
unsigned convert1(unsigned int);

void gpssignalgain();


void delay(unsigned int t)
 {
  unsigned int i,j;
  for(i=0;i<t;i++)
  for(j=0;j<200;j++);
  }

  void lcdcmd(unsigned char value)		   // LCD COMMAND
  {
      
   
	 lcd_data=value&(0xf0); //send msb 4 bits
      lcd_rs=0;	  //select command register
      lcd_en=1;	  //enable the lcd to execute command
	  delay(3);
	  lcd_en=0;  
      lcd_data=((value<<4)&(0xf0));	 //send lsb 4 bits
      lcd_rs=0;	 //select command register
      lcd_en=1;	 //enable the lcd to execute command
	  delay(3);
	  lcd_en=0;  
      
  }  
void lcd_init(void)
{
 lcdcmd(0x02);	
 lcdcmd(0x02);
 lcdcmd(0x28);  //intialise the lcd in 4 bit mode*/
 lcdcmd(0x28);  //intialise the lcd in 4 bit mode*/
 
 lcdcmd(0x0e);	//cursor blinking
 lcdcmd(0x06);	//move the cursor to right side
 lcdcmd(0x01);	//clear the lcd

}	
void lcddata(unsigned char value)

  {
       
      lcd_data=value&(0xf0); //send msb 4 bits
      lcd_rs=1;	  //select data register
      lcd_en=1;	  //enable the lcd to execute data
	  delay(3);
	  lcd_en=0;  
      lcd_data=((value<<4)&(0xf0));	 //send lsb 4 bits
      lcd_rs=1;	  //select data register
      lcd_en=1;	  //enable the lcd to execute data
	  delay(3);
	  lcd_en=0;  
      
     delay(3); 
  }  



void msgdisplay(unsigned char b[]) // send string to lcd
  {
unsigned char s,count=0;
for(s=0;b[s]!='\0';s++)
 {
  count++;	 
  if(s==16)
   lcdcmd(0xc0);
   if(s==32)
   {
   lcdcmd(1);
   count=0;
   }
  lcddata(b[s]);
 }
}	



void serinit()
  {
    TMOD=0x20;
    TH1=0xFD;	  //9600
    SCON=0x50;
    TR1=1;
  }
  
 unsigned char receive()
 {
  unsigned char rx; 
   while(RI == 0);
    rx=SBUF;
	RI=0;
	return rx;
 }

void sertxs(unsigned char *tx)
 {
    //unsigned char v;
   for(;*tx != '\0';tx++)
     {
	   SBUF=*tx;
	  while(TI == 0);
	   TI=0;
	  // v= receive();
	   //delay(2);
	   
	 }
 }

void sertx(unsigned char tx)
 {
  	   ///unsigned char v;
	   SBUF=tx;
	  while(TI == 0);
	   TI=0;
	     //v= receive();
		 //delay(2);
 }


void sie()
 {
 //ET0 = 0;
 ES   = 1;                /* allow serial interrupts */
 EA   = 1;
 }

 void sid()
 {
 //ET0 = 1;
 ES   = 0;                /* allow serial interrupts */
 EA   = 0;
 }    
void okcheck()
  {
  	do{
	
	rcg=receive();
	}	while(rcg!='K');
  
  }
void lcdbasic()
{
   lcdcmd(0x01);
   msgdisplay("H.B:"); //0x84,5,6
   lcdcmd(0x88);
   msgdisplay("T:");   //0x8a,b,c
}


void main()
{
 unsigned char rtr=0;

  P2=0xff;
  ir=1;
  relay=0;
  buzzer=1;
  						//THE DESIGN OF ADVANCED VEHICLE SAFETY AND           SECURITY SYSTEM
   serinit(); 

   lcd_init();
   msgdisplay("Avoidance of Accident");
  
  
		delay(400);

  	   lcdcmd(1);
	   msgdisplay("IR:"); //0x87
	  // sie();

  while(1)
   {
    if(ir == 0)  
	  {	  
	   lcdcmd(0x88);msgdisplay("ON  ");		buzzer=0;  relay=0;
	  }
	
    if(ir == 1)  
	  {	  
	   lcdcmd(0x88);msgdisplay("OFF ");		  buzzer=1;	  relay=1;
	  }

	  }     
}


unsigned convert1(unsigned int value)
{
   unsigned int i,j,x,k,l;
 i=value/100;
 x=i|0x30;
 sertx(x);	  // converts the data to digital as per the address selection
 
 j=value%100;
 k=j/10;
 x=k|0x30;
 sertx(x);

 l=j%10;
 x=l|0x30;
 sertx(x);
 	return 1;
}



unsigned convert(unsigned int value)
{
   unsigned int i,j,x,k,l;
 i=value/100;
 x=i|0x30;
 lcddata(x);	  // converts the data to digital as per the address selection
 
 j=value%100;
 k=j/10;
 x=k|0x30;
 lcddata(x);

 l=j%10;
 x=l|0x30;
 lcddata(x);
 	return 1;
}


void gpssignalgain()
{
  ag:
    for(q=0;q<10;q++) 
	    msg1[q] = '\0';

	for(q=0;q<11;q++) 
	    msg2[q] = '\0';
		
			do{
    	       rcv=receive();
              }while(rcv != '$'); 

	  	
	     rcv1=receive();
		 rcv2=receive();
		 rcv3=receive();
		 rcv4=receive();
		 rcv5=receive(); 

        if((rcv1 == 'G') && (rcv2 == 'P') && (rcv3 == 'R') && (rcv4 == 'M') && (rcv5 == 'C'))
	      {
			
			  for(r=0;r<12;r++)	 //14
			    {
				  rcv=receive();
				}
		

              for(r=0;r<9;r++)
			    {
				  msg1[r]=receive();  //lat
				}
				 rcv=receive();	rcv=receive(); rcv=receive();
			  for(r=0;r<10;r++)
			    {
				  msg2[r]=receive();  //long
				}
				 rcv=receive();	rcv=receive(); rcv=receive();
				 ql=receive();	

			  for(r=0;r<8;r++)
  			     {
				  rcv=receive();  
				 }

	      }
	else
	{
	 goto ag;
	}
 
}  

				  /*
void serial_int (void) interrupt 4
{
	 
       if (RI == 1)        
        {
               chr = SBUF;        
               RI = 0;        
               if(chr == '*')
			     {	
					    chr1 = receive();
					  
				 }
			//	 sid();
			//	 lcdcmd(0XC0);msgdisplay(str1); // delay(2000);//sie();
				 //lcdbasic();
		}
}	  
					*/


