#include  <avr/pgmspace.h>
#include  "adns5050.h"
#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

//Constructor sets the pins used for the mock 'i2c' communication
ADNS5050::ADNS5050(int sdio, int sclk, int ncs)
{
	_sdio = sdio;
	_sclk = sclk;
	_ncs = ncs;
}

//Configures the communication pins for their initial state
void ADNS5050::begin()
{
	pinMode(_sdio, OUTPUT);
	pinMode(_sclk, OUTPUT);
	pinMode(_ncs,OUTPUT);
}

//Essentially resets communication to the ADNS5050 module
void ADNS5050::sync()
{
	digitalWrite(_ncs, LOW);
  delayMicroseconds(1);
	digitalWrite(_ncs, HIGH);
}

//Reads a register from the ADNS5050 sensor. Returns the result to the calling function.
//Example: value = mouse.read(CONFIGURATION_REG);
int ADNS5050::read(unsigned char addr)
{
  byte temp;
  int n;

  //select the chip
  digitalWrite(_ncs, LOW);	//nADNSCS = 0;
  temp = addr;

  //start clock low
  digitalWrite(_sclk, LOW); //SCK = 0;

	//set data line for output
  pinMode(_sdio, OUTPUT); //DATA_OUT;
  //read 8bit data
  for (n=0; n<8; n++) {
    //delayMicroseconds(2);
    digitalWrite(_sclk, LOW);//SCK = 0;
    //delayMicroseconds(2);
    pinMode(_sdio, OUTPUT); //DATA_OUT;
    if (temp & 0x80) {
      digitalWrite(_sdio, HIGH);//SDOUT = 1;
    }
    else {
      digitalWrite(_sdio, LOW);//SDOUT = 0;
    }
    delayMicroseconds(2);
    temp = (temp << 1);
    digitalWrite(_sclk, HIGH); //SCK = 1;
  }

  // This is a read, switch to input
  temp = 0;
  pinMode(_sdio, INPUT); //DATA_IN;
  //read 8bit data
	for (n=0; n<8; n++) {		// read back the data
    delayMicroseconds(1);
    digitalWrite(_sclk, LOW);
    delayMicroseconds(1);
    if(digitalRead(_sdio)) {
      temp |= 0x1;
    }
    if( n != 7) temp = (temp << 1); // shift left
    digitalWrite(_sclk, HIGH);
  }
  delayMicroseconds(20);
  digitalWrite(_ncs, HIGH);// de-select the chip
  return ConvertToSignedNumber(temp);
}

int ADNS5050::ConvertToSignedNumber(byte twoscomp){
  int value;

	if (bitRead(twoscomp,7)){
    value = -128 + (twoscomp & B01111111 );
  }
  else{
    value = twoscomp;
  }
	return value;
}

//Writes a value to a register on the ADNS2620.
//Example: mouse.write(CONFIGURATION_REG, 0x01);
void ADNS5050::write(unsigned char addr, unsigned char data)
{
  char temp;
  int n;

  //select the chip
  //nADNSCS = 0;
  digitalWrite(_ncs, LOW);

  temp = addr;
  //クロックを開始
  digitalWrite(_sclk, LOW);//SCK = 0;					// start clock low
  //SDIOピンを出力にセット
  pinMode(_sdio, OUTPUT);//DATA_OUT; // set data line for output
  //8ビットコマンドの送信
  for (n=0; n<8; n++) {
    digitalWrite(_sclk, LOW);//SCK = 0;
    pinMode(_sdio, OUTPUT);
    delayMicroseconds(1);
    if (temp & 0x80)  //0x80 = 0101 0000
      digitalWrite(_sdio, HIGH);//SDOUT = 1;
    else
      digitalWrite(_sdio, LOW);//SDOUT = 0;
    temp = (temp << 1);
    digitalWrite(_sdio, HIGH);//SCK = 1;
    delayMicroseconds(1);//delayMicroseconds(1);			// short clock pulse
  }
	temp = data;
  for (n=0; n<8; n++) {
    delayMicroseconds(1);
    digitalWrite(_sclk, LOW);//SCK = 0;
    delayMicroseconds(1);
    if (temp & 0x80)
      digitalWrite(_sdio, HIGH);//SDOUT = 1;
    else
      digitalWrite(_sdio, LOW);//SDOUT = 0;
    temp = (temp << 1);
    digitalWrite(_sclk, HIGH);//SCK = 1;
    delayMicroseconds(1);			// short clock pulse
  }
  delayMicroseconds(20);
  digitalWrite(_ncs, HIGH);//nADNSCS = 1; // de-select the chip

}
