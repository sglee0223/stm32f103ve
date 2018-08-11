#include "gpio.h"

#include "dht22.h"
#include "dwt_stm32_delay.h"

void read_dht_data(void)
{
	uint8_t i =0, j = 0;
	int data[5] = { 0, 0, 0, 0, 0 };

#if 1
#define DHT_TIMEOUT 200
	
	//reset port
	DHT22_OUTPUT_MODE();

	DHT22_SET(GPIO_PIN_SET);
	HAL_Delay(100);

	//send request
	DHT22_SET(GPIO_PIN_RESET);
	DWT_Delay_us(500);	

	DHT22_SET(GPIO_PIN_SET);

	DHT22_INPUT_MODE();
	DWT_Delay_us(40);

	//check start condition 1
	if(DHT22_GET()) {
		printf( "check start condition 1 error\r\n");
		return;
	}
	DWT_Delay_us(80);

	//check start condition 2
	if(!DHT22_GET()) {
		printf( "check start condition 2 error\r\n");
		return;
	}
	DWT_Delay_us(80);

	//read the data
	uint16_t timeoutcounter = 0;
		
	for (j=0; j<5; j++) { //read 5 byte
		uint8_t result=0;
		
		for(i=0; i<8; i++) {//read every bit
			timeoutcounter = 0;
			while(!DHT22_GET()) { //wait for an high input (non blocking)
				timeoutcounter++;
				if(timeoutcounter > DHT_TIMEOUT) {
					printf( "timeout-1\r\n");
					return;
				}
			}
			
			DWT_Delay_us(30);
			if(DHT22_GET()) //if input is high after 30 us, get result
				result |= (1<<(7-i));
			
			timeoutcounter = 0;
			while(DHT22_GET()) { //wait until input get low (non blocking)
				timeoutcounter++;
				if(timeoutcounter > DHT_TIMEOUT) {
					printf( "timeout-2\r\n");
					return;
				}
			}
		}
		data[j] = result;
	}

	//reset port
	DHT22_OUTPUT_MODE();
	DHT22_SET(GPIO_PIN_RESET);
	HAL_Delay(100);

	//check checksum
	if ((uint8_t)(data[0] + data[1] + data[2] + data[3]) == data[4]) 
	{
		float temperature, humidity;
		
		uint16_t rawhumidity = data[0]<<8 | data[1];
		uint16_t rawtemperature = data[2]<<8 | data[3];
		if(rawtemperature & 0x8000) 
		{
			temperature = (float)((rawtemperature & 0x7FFF) / 10.0) * -1.0;
		} else 
		{
			temperature = (float)(rawtemperature)/10.0;
		}
		humidity = (float)(rawhumidity)/10.0;
		
		printf( "Humidity = %.1f %% Temperature = %.1f *C (%.1f *F)\r\n", humidity, temperature, temperature * 1.8f + 32 );
	}		
#endif	
	
#if 0

#define MAX_TIMINGS    85
		
	uint8_t laststate    = 1;
	uint8_t counter      = 0;
	
	/* pull pin down for 18 milliseconds */
	DHT22_OUTPUT_MODE();

	DHT22_SET(GPIO_PIN_RESET);
	//HAL_Delay(18);
	HAL_Delay(20);

	DHT22_SET(GPIO_PIN_SET);
	DWT_Delay_us(40);

	/* prepare to read the pin */
	DHT22_INPUT_MODE();

	/* detect change and read data */
	for ( i = 0; i < MAX_TIMINGS; i++ )
	{
		counter = 0;
		while ( DHT22_GET() == laststate )
		{
			counter++;
			//DWT_Delay_us(40);
			DWT_Delay_us(1);
			if ( counter == 255 )
			{
				printf( "overtime-1!\r\n");
				break;
			}
		}
		laststate = DHT22_GET();

		if ( counter == 255 )
		{
			printf( "overtime-2!\r\n");
			break;
		}
			
		/* ignore first 3 transitions */
		if ( (i >= 4) && (i % 2 == 0) )
		{
			/* shove each bit into the storage bytes */
			data[j / 8] <<= 1;
			if ( counter > 100 )
				data[j / 8] |= 1;
			j++;
			
			printf( "good data[%d]=%d\r\n", j, data[j-1]);
		}
	}
 
	/*
	 * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	 * print it out if data is good
	 */
	if ( (j >= 40) /*&&
		 (data[4] == ( (data[0] + data[1] + data[2] + data[3]) & 0xFF) )*/ )
	{
		float h = (float)((data[0] << 8) + data[1]) / 10;
		if ( h > 100 )
		{
			h = data[0];    // for DHT11
		}
		float c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
		if ( c > 125 )
		{
			c = data[2];    // for DHT11
		}
		if ( data[2] & 0x80 )
		{
			c = -c;
		}
		float f = c * 1.8f + 32;
		printf( "Humidity = %.1f %% Temperature = %.1f *C (%.1f *F)\r\n", h, c, f );
	}else  {
		printf( "Data not good, skip\r\n" );
	}
#endif		
}
