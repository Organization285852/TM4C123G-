#include "includes.h"
#include "i2c.h"

int c;
uint32_t err;
void I2C0Init(void)
{
	// The I2C0 peripheral must be enabled before use.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    //
    // For this example I2C0 is used with PortB[3:2].  The actual port and
    // pins used may be different on your part, consult the data sheet for
    // more information.  GPIO port B needs to be enabled so these pins can
    // be used.
    // TODO: change this to whichever GPIO port you are using.
				//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //
    // Configure the pin muxing for I2C0 functions on port B2 and B3.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
	GPIOPinConfigure(GPIO_PB2_I2C0SCL);
	GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    //
    // Select the I2C function for these pins.  This function will also
    // configure the GPIO pins pins for I2C operation, setting them to
    // open-drain operation with weak pull-ups.  Consult the data sheet
    // to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using.
    //
	GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    //
    // Enable loopback mode.  Loopback mode is a built in feature that is
    // useful for debugging I2C operations.  It internally connects the I2C
    // master and slave terminals, which effectively let's you send data as
    // a master and receive data as a slave.
    // NOTE: For external I2C operation you will need to use external pullups
    // that are stronger than the internal pullups.  Refer to the datasheet for
    // more information.
    //
	 //HWREG(I2C0_BASE + I2C_O_MCR) |= 0x01;

    //
    // Enable and initialize the I2C0 master module.  Use the system clock for
    // the I2C0 module.  The last parameter sets the I2C data transfer rate.
    // If false the data rate is set to 100kbps and if true the data rate will
    // be set to 400kbps.  For this example we will use a data rate of 100kbps.
    //
  
  I2CMasterInitExpClk(I2C0_BASE,SysCtlClockGet(),true);//SysCtlClockGet()
	
	I2CMasterEnable(I2C0_BASE);//主机模式
	
	
	I2CMasterIntEnable(I2C0_BASE);//主模式中断使能
	I2CMasterIntEnableEx(I2C0_BASE,I2C_MASTER_INT_NACK|I2C_MASTER_INT_DATA);
}

uint32_t I2CReadToBuf(uint8_t adress,uint8_t reg,uint8_t *buf,uint8_t len)
{
	int i;

	I2CMasterSlaveAddrSet(I2C0_BASE,adress, false);//写
	I2CMasterDataPut(I2C0_BASE,reg);
	I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_SINGLE_SEND);
	while(I2CMasterBusy(I2C0_BASE));
	err = I2CMasterErr(I2C0_BASE);//Returns the error status, as one of I2C_MASTER_ERR_NONE,I2C_MASTER_ERR_ADDR_ACK, I2C_MASTER_ERR_DATA_ACK, orI2C_MASTER_ERR_ARB_LOST
  if(err)
	{
	//	printf("err1");
		return err;
	}
	I2CMasterSlaveAddrSet(I2C0_BASE,adress, true);//读
	if(len>=2)
	{
		I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_BURST_RECEIVE_START);
		while(I2CMasterBusy(I2C0_BASE));
		err = I2CMasterErr(I2C0_BASE);
		if(err)
		{
		//	printf("err2");
			return err;
		}
		*buf = (uint8_t)I2CMasterDataGet(I2C0_BASE);
		buf++;
		if(len>2)
			for(i=0;i<len-2;i++)
			{
				I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_BURST_RECEIVE_CONT);
				while(I2CMasterBusy(I2C0_BASE));
				err = I2CMasterErr(I2C0_BASE);
				if(err)
				{
//					printf("err3");
					return err;
				}
				*buf = (uint8_t)I2CMasterDataGet(I2C0_BASE);
				buf++;
			}
		I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_BURST_SEND_FINISH);
		while(I2CMasterBusy(I2C0_BASE));
		err = I2CMasterErr(I2C0_BASE);
		if(err)
			return err;
		*buf = (uint8_t)I2CMasterDataGet(I2C0_BASE);
	 }
	else
	{
		I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_SINGLE_RECEIVE);
		while(I2CMasterBusy(I2C0_BASE));
		err = I2CMasterErr(I2C0_BASE);
		if(err)
		{
//			printf("err3");
			return err;
		}
		*buf = (uint8_t)I2CMasterDataGet(I2C0_BASE);
	}
	 return 0;
} 

uint32_t I2CWriteToReg(uint8_t adress,uint8_t reg,uint8_t *buf,uint8_t len)
{
	uint32_t e;
	int i;
	I2CMasterSlaveAddrSet(I2C0_BASE,adress, false);//写
	I2CMasterDataPut(I2C0_BASE,reg);
	I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_BURST_SEND_START);
	while(I2CMasterBusy(I2C0_BASE));
	e = I2CMasterErr(I2C0_BASE);//Returns the error status, as one of I2C_MASTER_ERR_NONE,I2C_MASTER_ERR_ADDR_ACK, I2C_MASTER_ERR_DATA_ACK, orI2C_MASTER_ERR_ARB_LOST
	if(e)
	{
//		printf("err1");
		return e;
	}
//  if(len>1)
		for(i=0;i<len-1;i++)
		{
			I2CMasterDataPut(I2C0_BASE,*buf);
			buf++;
			I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_BURST_SEND_CONT);
			while(I2CMasterBusy(I2C0_BASE));
			e = I2CMasterErr(I2C0_BASE);//Returns the error status, as one of I2C_MASTER_ERR_NONE,I2C_MASTER_ERR_ADDR_ACK, I2C_MASTER_ERR_DATA_ACK, orI2C_MASTER_ERR_ARB_LOST
			if(e)
				return e;
		}
	I2CMasterDataPut(I2C0_BASE,*buf);
	I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_BURST_SEND_FINISH);
	while(I2CMasterBusy(I2C0_BASE));
	e = I2CMasterErr(I2C0_BASE);//Returns the error status, as one of I2C_MASTER_ERR_NONE,I2C_MASTER_ERR_ADDR_ACK, I2C_MASTER_ERR_DATA_ACK, orI2C_MASTER_ERR_ARB_LOST
	if(e)
		return e;
	return 0;
}


void I2C0_Handler(void)
{
	c++;
}
