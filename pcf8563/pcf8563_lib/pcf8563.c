#include "i2c.h"

#include "pcf8563.h"

void I2C_Scan(void) 
{
    printf("Scanning I2C bus...\r\n");

    HAL_StatusTypeDef res;
    for(uint16_t i = 0; i < 128; i++) 
		{
        res = HAL_I2C_IsDeviceReady(&hi2c1, i << 1, 1, 10);
        if(res == HAL_OK) 
				{
            printf("0x%02X", i);
        } 
				else 
				{
            printf(".");
        }
    }
    printf("\r\n");
}

void readyI2C(void)
{
    HAL_StatusTypeDef res;

    for(;;) 
		{
        res = HAL_I2C_IsDeviceReady(&hi2c1, RTC_ADDR, 1, HAL_MAX_DELAY);
        if(res == HAL_OK)
            break;
    }
}

void setI2C(uint8_t reg, uint8_t data)
{
		uint8_t txData[2];
    HAL_StatusTypeDef res;
		
		readyI2C();
	
		txData[0] = reg;
		txData[1] = data;		
    res = HAL_I2C_Master_Transmit(&hi2c1, RTC_ADDR, txData, sizeof(txData), HAL_MAX_DELAY);
		
		if (res != HAL_OK)
		{
				printf("setI2C: Not Okay[0x%x]\r\n", reg);
		}		
}

uint8_t getI2C(uint8_t reg)
{
		uint8_t txData[1];
		uint8_t rxData[1];
    HAL_StatusTypeDef res;

		readyI2C();
		
		txData[0] = reg;
    res = HAL_I2C_Master_Transmit(&hi2c1, RTC_ADDR, txData, sizeof(txData), HAL_MAX_DELAY);
		
		if (res != HAL_OK)
		{
				printf("getI2C: Not Okay[0x%x]\r\n", reg);
		}		
		
    readyI2C();

    res = HAL_I2C_Master_Receive(&hi2c1, RTC_ADDR, rxData, sizeof(rxData), HAL_MAX_DELAY);
		if (res != HAL_OK)
		{
				printf("getI2C: Not Okay[0x%x]\r\n", reg);
		}
		
		return rxData[0];
}

void RTC_Init(void)
{
		setI2C(REG_CTRL_STATUS_1, 0x00);
		setI2C(REG_CTRL_STATUS_2, 0x00);
}

void PCF8563_setDate(uint16_t year, uint8_t mon, uint8_t day)
{
		year	= changeIntToHex(year % 100);
		mon 	= changeIntToHex(mon) | ((year > 1999) ? 0x00 : 0x80);
		day 	= changeIntToHex(day);
		
		setI2C(REG_YEAR, year);
		setI2C(REG_MON, mon);
		setI2C(REG_DAY, day);
}

void PCF8563_getDate(void)
{
		uint8_t buf[3];
		uint8_t century = 0;
		uint16_t year;
		uint8_t mon, day;
	
		buf[0] = getI2C(REG_YEAR);
		buf[1] = getI2C(REG_MON);
		buf[2] = getI2C(REG_DAY);

		if (buf[1] & 0x80)
				century = 1;
		
		buf[0] = buf[0]&0xff;
		buf[1] = buf[1]&0x1f;
		buf[2] = buf[2]&0x3f;
		
		year = (century == 1) ? (1900 + changeHexToInt(buf[0])) : (2000 + changeHexToInt(buf[0]));
		mon = changeHexToInt(buf[1]);
		day = changeHexToInt(buf[2]);

		printf("%d-%d-%d ", year, mon, day);
}

void PCF8563_setTime(uint8_t hour, uint8_t min, uint8_t sec)
{
		setI2C(REG_HOUR, changeIntToHex(hour));
		setI2C(REG_MIN, changeIntToHex(min));
		setI2C(REG_SEC, changeIntToHex(sec));
}

void PCF8563_getTime(void)
{
		uint8_t hour, min, sec;

		hour = changeHexToInt(getI2C(REG_HOUR) & 0x3f);
		min = changeHexToInt(getI2C(REG_MIN) & 0x7f);
		sec = changeHexToInt(getI2C(REG_SEC) & 0x7f);

		printf("%d:%d:%d\r\n", hour, min, sec);
}
