#ifndef adns5050_h
#define adns5050_h

#include <avr/pgmspace.h>
#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class ADNS5050
{
	public:
		ADNS5050(int sdio, int sclk, int ncs);
		void	begin();
		void	sync();
		int		read(unsigned char address);
		void	write(unsigned char address, unsigned char data);
	private:
		int		ConvertToSignedNumber(byte twoscomp);
		int		_sdio;
		int		_sclk;
		int		_ncs;
};

/* Register Map for the ADNS5050 Optical Mouse Sensor */
#define PRODUCT_ID          0x00  //R			0x12
#define PRODUCTID2          0x3e  //R			0x26
#define REVISION_ID         0x01  //R			0x01
#define DELTA_X_REG         0x03  //R			any
#define DELTA_Y_REG         0x04  //R			any
#define SQUAL_REG           0x05  //R			any
#define MAXIMUM_PIXEL_REG   0x08  //R			any
#define MINIMUM_PIXEL_REG   0x0a  //R			any
#define PIXEL_SUM_REG       0x09  //R			any
#define PIXEL_DATA_REG      0x0b  //R			any
#define SHUTTER_UPPER_REG   0x06  //R			any
#define SHUTTER_LOWER_REG   0x07  //R			any
#define RESET				0x3a  //W			N/A
#define MOUSE_CONTROL       0x0D  //R/W		0x00
#define CPI500v				0x00
#define CPI1000v			0x01

#define	NUM_PIXELS			361

#endif
