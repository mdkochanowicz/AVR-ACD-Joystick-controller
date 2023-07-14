#define F_CPU 16000000UL
#include <avr/io.h>
#include <u=l/delay.h>
#include <stdlib.h>
#include <stdio.h>
#define USART_BAUDRATE_ 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE_ * 16UL))) - 1)
uint16_t ADC_odczyt10bit(void);
uint8_t ADC_odczyt8bit(void);
void Joys=ckX(uint16_t odczyt_10bit, uint8_t odczyt_8bit);
void Joys=ckY(uint16_t odczyt_10bit, uint8_t odczyt_8bit);
//uint16_t Joys=ckO(uint16_t odczyt_10bit, uint8_t odczyt_8bit);
void UART_init(long USART_BAUDRATE);
unsigned char UART_RxChar();
void UART_TxChar(char ch);
void UART_SendString(char *str);
int main(void)
{
char c;
UART_init(9600);
DDRB |= (1<<PINB1) | (1<<PINB2) | (1<<PINB3);
DDRD |= (1<<PIND6) | (1<<PIND5) | (1<<PIND3);
TCCR1A = (1<<WGM11) | (1<<COM1A1) | (1<<COM1B1); 
TCCR1B = (1<<WGM13) | (1<<WGM12)| (1<<CS12);
TCCR2A = (1<<COM2A1) | (1<<COM2B1) | (1<<WGM21) | (1<<WGM20);
TCCR2B = (1<<CS22) | (1<<CS21);
TCCR0A = (1<<COM0A1) | (1<<COM0B1) | (1<<WGM01) | (1<<WGM00);
TCCR0B = (1<<CS02);
OCR1A = 0;
OCR1B = 0;
ICR1 = 1023;
OCR2A = 0;
OCR2B = 0;
OCR0A = 0;
OCR0B = 0;
ADMUX |= (1<<REFS0) | (1<<ADLAR);
 
ADCSRA |= (1<<ADPS1) | (1<<ADPS2) | (1<<ADEN); 
 while (1) 
 {
for(int i=0; i >= 0; i++)
{
ADMUX &= ~(1<<MUX1);
ADMUX &= ~(1<<MUX0);
ADMUX &= ~(1<<MUX2);
uint16_t odczyt_10bit;
uint8_t odczyt_8bit;
odczyt_10bit = ADC_odczyt10bit();
odczyt_8bit = ADC_odczyt8bit();
if(i%10000==0)
{
char b[20];
itoa(odczyt_8bit, b, 10);
UART_SendString(b);
c=UART_RxChar();
UART_TxChar(c);
}
_delay_us(50);
ADMUX |= (1<<MUX1);
Joys=ckX(odczyt_10bit, odczyt_8bit);
_delay_us(50);
ADMUX |= (1<<MUX1) | (1<<MUX0);
Joys=ckY(odczyt_10bit, odczyt_8bit);
_delay_us(50);
}
}
}
uint16_t ADC_odczyt10bit(void)
{
ADCSRA |=(1<<ADSC);
while(ADCSRA & (1<<ADSC))
{
}
return (ADCL >> 6 | ADCH << 2);
}
uint8_t ADC_odczyt8bit(void)
{
ADCSRA |=(1<<ADSC);
while(ADCSRA & (1<<ADSC))
{
}
return (ADCH);
}
void Joys=ckX(uint16_t odczyt_10bit, uint8_t odczyt_8bit)
{
ADCSRA |=(1<<ADSC);
while(ADCSRA & (1<<ADSC))
{
//
}
if(ADCH > 200)
{
OCR2A = odczyt_8bit;
}
else if(ADCH < 50)
{
OCR2B = odczyt_8bit;
}
else
{
OCR2A = 0;
OCR2B = 0;
}
}
void Joys=ckY(uint16_t odczyt_10bit, uint8_t odczyt_8bit)
{
ADCSRA |=(1<<ADSC);
while(ADCSRA & (1<<ADSC))
{
//
}
if(ADCH > 200)
{
OCR0A = odczyt_8bit;
}
else if(ADCH < 50)
{
OCR0B = odczyt_8bit;
}
else
{
OCR0A = 0;
OCR0B = 0;
}
}
void UART_init(long USART_BAUDRATE)
{
UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
UBRR0L = BAUD_PRESCALE;
UBRR0H = (BAUD_PRESCALE >> 8);
}
unsigned char UART_RxChar()
{
while ((UCSR0A & (1 << RXC0)));
return(UDR0);
}
void UART_TxChar(char ch)
{
while (! (UCSR0A & (1<<UDRE0)));
UDR0 = ch;
}
void UART_SendString(char *str)
{
unsigned char j=0;
while (str[j]!=0)
{
UART_TxChar(str[j]);
j++;
}
}