/* La configuración de conexiones es la siguiente:

Segmentos:
A         PD4
B         PD1
C         PB0
D         PB5
E         PB6
F         PD5
G         PB1
PUNTOS    PD6 NO NECESITA ACTIVAR GND PARA DRENAGE. DIRECTAMENTE CONECTADO A GND)

PINES DE DRENAJE:
DIG1     PB7
DIG2     PB6
DIG3     PB4
DIG4     PB2

INTERRUPTORES DE CAMBIO DE HORA
MINUTO        PD3 (INT1)
HORA          PD2 (INT0)

PIN DHT       PD0
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// CONEXIONADO DHT11
//sbit DDHT at DDD0_bit;
//sbit PDHT at PORTD0_bit;
//sbit Pin_DHT at PIND0_bit;// Para leer el estado el pin

// CONEXIONADO DISPLAY 3492AS
//direcciones
sbit DA at DDD4_bit;
sbit DB at DDD1_bit;
sbit DC at DDB0_bit;
sbit DD at DDB5_bit;
sbit DE at DDB4_bit;
sbit DF at DDD5_bit;
sbit DG at DDB1_bit;
sbit DPUNTO at DDD6_bit;

sbit DDIG1  at DDB7_bit;
sbit DDIG2  at DDB6_bit;
sbit DDIG3  at DDB3_bit;
sbit DDIG4  at DDB2_bit;

//puertos
sbit A at PORTD4_bit;
sbit B at PORTD1_bit;
sbit C at PORTB0_bit;
sbit D at PORTB5_bit;
sbit E at PORTB4_bit;
sbit F at PORTD5_bit;
sbit G at PORTB1_bit;
sbit PUNTO at PORTD6_bit;

sbit DIG1  at PORTB7_bit;
sbit DIG2  at PORTB6_bit;
sbit DIG3  at PORTB3_bit;
sbit DIG4  at PORTB2_bit;

//Variables necesarias para almacenar datos sensor DHT
unsigned char I_RH,D_RH,I_Temp,D_Temp,CheckSum;
unsigned char buffer=0;
unsigned long i=0;
char txt[8];

// para establecer un tiempo límite si no se recibe respuesta del sensor
//unsigned long int contador = 1;
//unsigned long int limite = 20;

//variables necesarias para el reloj
unsigned short int segundos = 0;
unsigned short int minutos = 0;
unsigned short int horas = 0;

void InicializarPines();
void LimpiarDisplay();
void Escribir(unsigned short int posicion, unsigned short int digito);
void ComprobarDisplay(); //se puede llamar en main para comprobar el pineado
void Inicializa_Timer0();//NO USADO EN ESTE PROGRAMA
void Inicializa_Timer1();

void DHT11(); // Engloba las tres siguientes
void Peticion();  //Envio señal al sensor DHT
void Respuesta(); //Leo la Respuesta del del sensor
void Recepcion(); //Si hay Respuesta se espera la recepción los datos

void Request()                                /* Microcontroller send start pulse/request */
{
  DDD0_bit = 1;         // Configuro el pin como salida
  PORTD0_bit = 0;       // Establezco la salida en LOW
  Delay_ms(20);
  PORTD0_bit = 1;       // Establezco la salida en HIGH
}

void Response()                                /* receive response from DHT11 */
{
  DDD0_bit = 0;   // Establezco el pin como entrada
  //DDRD &= ~(1<<DHT11_PIN);    // estable el bit dht11 en bajo
  while(PIND0_bit);
  while(PIND0_bit == 0);
  while(PIND0_bit);
}

unsigned char Receive_data()
{
  unsigned char q;
  for (q=0; q<8; q++)
  {
    while(PIND0_bit == 0);
    Delay_us(35);//_delay_us(30);
    if(PIND0_bit)
    {
      buffer = (buffer<<1)|(0x01);
    }
    else
    {
      buffer = (buffer<<1);
    }
    while(PIND0_bit);
  }
  return buffer;
}

void Muestra_Hora(){

  //LimpiarDisplay();
  PUNTO = PUNTO ^ 1;
  Escribir(1,horas/10);
  //LimpiarDisplay();
  //PUNTO = PUNTO ^ 1;
  Escribir(2,horas%10);
  //PUNTO = PUNTO ^ 1;
  //LimpiarDisplay();
  //PUNTO = PUNTO ^ 1;
  Escribir(3,minutos/10);
  //PUNTO = PUNTO ^ 1;
  //LimpiarDisplay();
  //PUNTO = PUNTO ^ 1;
  Escribir(4,minutos%10);
  //PUNTO = PUNTO ^ 1;
//
}

void Incrementa_Hora(){
  if(segundos == 60){
    segundos =0;
    minutos++;
    if (minutos == 60){
      minutos = 0;
      horas++;
      if (horas == 24){
        horas=0;
      }
    }
  }
//
}

void Cambia_Hora() iv IVT_ADDR_INT0 ics ICS_AUTO {
  unsigned char i;
  horas++;
  Incrementa_Hora();
  for(i=0; i<150; i++){
    Muestra_Hora();
  }
}

void Cambia_Minuto() iv IVT_ADDR_INT1 ics ICS_AUTO {
  unsigned char i;
  minutos++;
  Incrementa_Hora();
  for(i=0; i<150; i++){
     Muestra_Hora();
  }
}




void main() {
  /*
   Request();
   Response();
    I_RH=Receive_data();
    D_RH=Receive_data();
    I_Temp=Receive_data();
    D_Temp=Receive_data();
    CheckSum=Receive_data();
  Delay_ms(3000);
    
  UART1_Init(9600);
  UART1_Write_Text("HOLA");
  UART1_Write(13);UART1_Write(10);
  Delay_ms(2000);

  UART1_Write(I_RH/10);
  Delay_ms(500);
  UART1_Write(I_RH%10);
  Delay_ms(500);
  UART1_Write(I_Temp/10);
  Delay_ms(500);
  UART1_Write(I_Temp%10);*/

  Delay_ms(2000);
  InicializarPines();
  // Registros para habilitar las interupciones internas (pulsadores para modificar hora y minutos) // INT 0; INT 1;
  MCUCR = 0b00000000;
  GIMSK = 0b11000000;
  Inicializa_Timer1();
  SREG_I_bit = 1; // Interrupt enable

  while(1){
    Muestra_Hora();
    //
  }


}



// NO NECESARIO EN ESTE PROGRAMA
//void Timer0Comp0A() iv IVT_ADDR_TIMER0_COMPA ics ICS_AUTO {
     //InicializarPines();
     //Delay_us(10);
     //Escribir(1,1);
     //PUNTO = PUNTO ^ 1;
//}

//void Timer0() iv IVT_ADDR_TIMER0_OVF ics ICS_AUTO {
    
    //PUNTO = PUNTO ^ 1;
    //Se ejecuta cada 0.189 segundos
    //if(contador == limite){
      //contador = 0;
      //TOIE0_bit = 0;
    //}
    //contador++;
//
//}

//void Timer1() iv IVT_ADDR_TIMER1_OVF ics ICS_AUTO {
void Timer1() iv IVT_ADDR_TIMER1_COMPA ics ICS_AUTO {
//void Timer1() iv IVT_ADDR_TIMER1_COMPB ics ICS_AUTO {
  segundos = segundos + 30;
  LimpiarDisplay();
  Incrementa_Hora();
  DHT11();
}

void DHT11(){

  Request();
  Response();
    I_RH=Receive_data();
    D_RH=Receive_data();
    I_Temp=Receive_data();
    D_Temp=Receive_data();
    CheckSum=Receive_data();
  Delay_ms(3000);
    //if (i != 0){  //Si se han recibido todos los datos continua
      if ( I_RH + I_Temp == CheckSum){ // So la Lectura es Correcta  continua
        for (i=0; i<1500; i++){  // Escribo Humedad
          //LimpiarDisplay();
          Escribir(1,11); //Simbolo H en digito 1
          //LimpiarDisplay();
          Escribir(3,I_RH/10); //PROBAR A COMPARARLO CON BINARIO
          //LimpiarDisplay();
          Escribir(4,I_RH%10);
        //
        }
        for (i=0; i<1500; i++){  // Escribo Temperatura
          //LimpiarDisplay();
          Escribir(1,I_Temp/10); //Simbolo H en digito 1
          //LimpiarDisplay();
          Escribir(2,I_Temp%10);
          //LimpiarDisplay();
          Escribir(3,12);
          //LimpiarDisplay();
          Escribir(4,13);
         //
         }
      //
      }
    //
}


void Inicializa_Timer0(){
//
  ////////////////////////
  //CONFIGURACIÓN TIMER 0
  ////////////////////////
  //TCCR0A Por defecto todo en cero
  //TCCR0B
    //clock/1
      //  CS00_bit = 1;  // el display consume menos con este
    //clock/8
      //CS01_bit = 1;
    //clock/64
      //CS00_bit = 1;
      //CS01_bit = 1;
    //clock/256
      //CS02_bit = 1;
    //clock/1024
      CS00_bit = 1;
      CS02_bit = 1;
    //OCR0A – OutputCompare Register A
    //The Output Compare Register A contains an 8-bit value that is continuously compared with the counter value (TCNT0)
    //OCR0A = 0b01111111;// EN BINARIO HASTA 255 (0b00000000)

          // 1 382 400MHz/1024 = 1350MHz
          // 1350/255 = 5.29Hz = 0.18888888 (periodico) seg
    
  //TIMSK0
    TOIE0_bit = 1;//TOIE: Timer/Counter0, Overflow Interrupt Enable
    //OCIE0A_bit = 1;//OCIE0A: Timer/Counter0 Output Compare Match A Interrupt Enable
    
    // 1 382 400MHz/1024 = 1350MHz
    // 1350/255 = 5.29Hz = 0.18888888 (periodico) seg
    
  ///////////////////////////////
  // FINAL CONFIGURACIÓN TIMER 0
  ///////////////////////////////


}

// SOLO SE ESTÁ USANDO EL TIMER 1
void Inicializa_Timer1(){
//
   ////////////////////////
   //Configuración Timer 1
   ////////////////////////
   TCCR1A = 0b00000000; //Todo a cero. Operación normal
   // TCCR1B

   //Para que el timer se reinicie cada vez que cuente hasta OCR1A hay que
   // configurar el modo CTC
     WGM12_bit = 1;
   //clock/256
     //CS12_bit = 1;
   //clock/1024
     CS10_bit = 1;
     CS12_bit = 1;
   //1 382 400MHz/1024=1350MHz
   // OCR1AH and OCR1AL – Output Compare Register 1 A
     OCR1AH =  0b10011110;
     OCR1AL =  0b00110100;
     // 1350MHz/40500 = 0.0333Hz = 30seg

   //TIMSK – Timer/Counter Interrupt Mask Register
     
     //OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
       OCIE1A_bit = 1; //para habilitar la interrupción cuando alzance OCIEA1A
      
     //TOIE1_bit = 1;

   // FINAL CONFIGURACIÓN TIMER 1
   ////////////////////////////
}

void LimpiarDisplay(){
//
    Delay_us(500);
    // Salidas:
    A = 0;
    B = 0;
    C = 0;
    D = 0;
    E = 0;
    F = 0;
    G = 0;
    PUNTO = 0;

    // Pines drenaje corriente:
    DIG1 = 1;
    DIG2 = 1;
    DIG3 = 1;
    DIG4 = 1;
    Delay_us(500);
}

void InicializarPines(){
    // CONFIGURO PINES COMO SALIDAS
    // Salidas:
    DA = 1;    // 1 Configuro el pin como salida
    DB = 1;
    DC = 1;
    DD = 1;
    DE = 1;
    DF = 1;
    DG = 1;
    DPUNTO = 1;

    // Pines drenaje corriente:
    DDIG1 = 1;
    DDIG2 = 1;
    DDIG3 = 1;
    DDIG4 = 1;

    // INICIALIZO TODOS LOS PINES EN LOW
        // Salidas:
    A = 0;
    B = 0;
    C = 0;
    D = 0;
    E = 0;
    F = 0;
    G = 0;
    //PUNTO = 0;

    // Pines drenaje corriente:
    DIG1 = 1;
    DIG2 = 1;
    DIG3 = 1;
    DIG4 = 1;
//
}

void Escribir(uint8_t posicion, uint8_t digito){
//



  switch (posicion){
  case 1:
       DIG1=0;  //Establezco la salida en LOW para que drene corriente
       DIG2=1;
       DIG3=1;
       DIG4=1;
       break;
  case 2:
       DIG2=0;
       DIG1=1;
       DIG3=1;
       DIG4=1;
       break;
  case 3:
       DIG3=0;
       DIG2=1;
       DIG1=1;
       DIG4=1;
       break;
  case 4:
       DIG4=0;
       DIG2=1;
       DIG3=1;
       DIG1=1;
       break;
  //
  }
  switch (digito){
  case 1:
       B=1;
       C=1;
       break;
  case 2:
       A=1;
       B=1;
       G=1;
       E=1;
       D=1;
       break;
  case 3:
       A=1;
       B=1;
       G=1;
       D=1;
       C=1;
       break;
  case 4:
       F=1;
       G=1;
       B=1;
       C=1;
       break;
  case 5:
       A=1;
       F=1;
       G=1;
       C=1;
       D=1;
       break;
  case 6:
       A=1;
       F=1;
       G=1;
       C=1;
       D=1;
       E=1;
       break;
  case 7:
       A=1;
       B=1;
       C=1;
       break;
  case 8:
       A=1;
       B=1;
       C=1;
       D=1;
       E=1;
       F=1;
       G=1;
       break;
  case 9:
       A=1;
       B=1;
       C=1;
       F=1;
       G=1;
       break;
  case 0:
       A=1;
       B=1;
       C=1;
       D=1;
       E=1;
       F=1;
       break;
  case 11: //H DE HUMEDAD
       B=1;
       C=1;
       E=1;
       F=1;
       G=1;
       break;
  case 12: // simbolo DE grado
       A=1;
       B=1;
       F=1;
       G=1;
       break;
  case 13: // C DE grado
       A=1;
       D=1;
       E=1;
       F=1;
       break;

  //
  }
  LimpiarDisplay();
}

void ComprobarDisplay(){
     unsigned short int i=0;
     unsigned short int x=0;
     while(1){
              for(i=1;i<5;i++){
                for(x=1;x<14;x++){
                  Escribir(i,x);
                  Delay_ms(1000);
                  InicializarPines();
                //
                }
              //
              }
     //
     }
//
}