#include "Mpu6050.h"

float *ax,*ay,*az;
float *gx,*gy,*gz;
float *temperature;

void initMpu6050()
{

    // I2C鍒濆鍖�
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);  // Enable I2C1 peripheral
    SysCtlDelay(2);                              // Insert a few cycles after enabling the peripheral to allow the clock to be fully activated
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // Enable GPIOA peripheral
    SysCtlDelay(2);                              // Insert a few cycles after enabling the peripheral to allow the clock to be fully activated

    // Use alternate function
    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);

    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6); // Use pin with I2C SCL peripheral
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);    // Use pin with I2C peripheral

    // I2CMasterInitExpClk(I2C1_BASE, 400 * 100000, true); // Enable and set frequency to 400 kHz  100
    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), true);
    SysCtlDelay(2); // Insert a few cycles after enabling the I2C to allow the clock to be fully activated

    // 鍒濆鍖朚pu6050
    printf("%d\n", Single_ReadI2C(MPU_ADDR, WHO_AM_I));
    i2cWrite(MPU_ADDR, MPU_PWR_MGMT1_REG, 0X80);
    delayms(100);
    i2cWrite(MPU_ADDR, MPU_PWR_MGMT1_REG, 0X00);
    i2cWrite(MPU_ADDR, MPU_GYRO_CFG_REG, 0X03);    // 璁剧疆闄�铻轰华婊￠噺绋嬭寖鍥�
    i2cWrite(MPU_ADDR, MPU_ACCEL_CFG_REG, 0X00);   // 璁剧疆闄�铻轰华婊￠噺绋嬭寖鍥�
    i2cWrite(MPU_ADDR, MPU_CONFIG, 0x03);          // 鍐呴儴浣庨�氭护娉㈤鐜囷紝鍔犻�熷害璁�44hz锛岄檧铻轰华42hz
    i2cWrite(MPU_ADDR, MPU_SAMPLE_RATE_REG, 0X13); // shezhi caiyanglv to 50
    i2cWrite(MPU_ADDR, MPU_INT_EN_REG, 0X00);      // Shut down all interrupts
    i2cWrite(MPU_ADDR, MPU_USER_CTRL_REG, 0X00);   // Shut down IIC main module
    i2cWrite(MPU_ADDR, MPU_FIFO_EN_REG, 0X00);     // Shut down FIFO
    i2cWrite(MPU_ADDR, MPU_INTBP_CFG_REG, 0X80);   // INT port 0 valid
    Single_ReadI2C(MPU_ADDR, MPU_INTBP_CFG_REG);
    i2cWrite(MPU_ADDR, MPU_PWR_MGMT1_REG, 0X01);
    i2cWrite(MPU_ADDR, MPU_PWR_MGMT2_REG, 0X00);
}

void readMpu6050(float *ax, float *ay, float *az, float *gx, float *gy, float *gz, float *temperature)
{
    uint8_t buf[14];
    int16_t temp;
    i2cReadData(MPU_ADDR, ACCEL_XOUT_H, buf, 14);
    *ax = (int16_t)((buf[0] << 8) | buf[1]);
    *ay = -(int16_t)((buf[2] << 8) | buf[3]);
    *az = -(int16_t)((buf[4] << 8) | buf[5]);
    temp = (int16_t)((buf[6] << 8) | buf[7]);
    *gx = (int16_t)((buf[8] << 8) | buf[9]);
    *gy = -(int16_t)((buf[10] << 8) | buf[11]);
    *gz = -(int16_t)((buf[12] << 8) | buf[13]);
    *temperature = 36.53f + (double)(temp / 340.0f);
}

void i2cWriteData(uint8_t addr, uint8_t regAddr, uint8_t *data, uint8_t length)
{
    I2CMasterSlaveAddrSet(I2C1_BASE, addr, false); // Set to write mode

    I2CMasterDataPut(I2C1_BASE, regAddr);                         // Place address into data register
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START); // Send start condition
    while (I2CMasterBusy(I2C1_BASE))
        ; // Wait until transfer is done
    uint8_t i;
    for (i = 0; i < length - 1; i++)
    {
        I2CMasterDataPut(I2C1_BASE, data[i]);                        // Place data into data register
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT); // Send continues condition
        while (I2CMasterBusy(I2C1_BASE))
            ; // Wait until transfer is done
    }

    I2CMasterDataPut(I2C1_BASE, data[length - 1]);                 // Place data into data register
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH); // Send finish condition
    while (I2CMasterBusy(I2C1_BASE))
        ; // Wait until transfer is done
}

/***********************************************************
@鍑芥暟鍚嶏細i2cRead
@鍏ュ彛鍙傛暟锛歶int8_t addr, uint8_t regAddr
@鍑哄彛鍙傛暟锛氭棤
鍔熻兘鎻忚堪锛欼2C璇绘暟鎹�
@浣滆�咃細鏃犲悕灏忓摜
@鏃ユ湡锛�2019骞�01鏈�27鏃�
*************************************************************/
uint8_t i2cRead(uint8_t addr, uint8_t regAddr)
{
    I2CMasterSlaveAddrSet(I2C1_BASE, addr, false); // Set to write mode

    I2CMasterDataPut(I2C1_BASE, regAddr);                    // Place address into data register
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND); // Send data
    while (I2CMasterBusy(I2C1_BASE))
        ; // Wait until transfer is done

    I2CMasterSlaveAddrSet(I2C1_BASE, addr, true); // Set to read mode

    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE); // Tell master to read data
    while (I2CMasterBusy(I2C1_BASE))
        ;                               // Wait until transfer is done
    return I2CMasterDataGet(I2C1_BASE); // Read data
}

/***********************************************************
@鍑芥暟鍚嶏細i2cWrite
@鍏ュ彛鍙傛暟锛歶int8_t addr, uint8_t regAddr, uint8_t data
@鍑哄彛鍙傛暟锛氭棤
鍔熻兘鎻忚堪锛欼2C鍐欐暟鎹�
@浣滆�咃細鏃犲悕灏忓摜
@鏃ユ湡锛�2019骞�01鏈�27鏃�
*************************************************************/
void i2cWrite(uint8_t addr, uint8_t regAddr, uint8_t data)
{
    i2cWriteData(addr, regAddr, &data, 1);
}

/***********************************************************
@鍑芥暟鍚嶏細i2cReadData
@鍏ュ彛鍙傛暟锛歶int8_t addr, uint8_t regAddr, uint8_t *data,
uint8_t length
@鍑哄彛鍙傛暟锛氭棤
鍔熻兘鎻忚堪锛欼2C璇绘暟鎹�
@浣滆�咃細鏃犲悕灏忓摜
@鏃ユ湡锛�2019骞�01鏈�27鏃�
*************************************************************/
void i2cReadData(uint8_t addr, uint8_t regAddr, uint8_t *data, uint8_t length)
{
    I2CMasterSlaveAddrSet(I2C1_BASE, addr, false);           // Set to write mode
    I2CMasterDataPut(I2C1_BASE, regAddr);                    // Place address into data register
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND); // Send data
    while (I2CMasterBusy(I2C1_BASE))
        ;                                                            // Wait until transfer is done
    I2CMasterSlaveAddrSet(I2C1_BASE, addr, true);                    // Set to read mode
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START); // Send start condition
    while (I2CMasterBusy(I2C1_BASE))
        ;                                  // Wait until transfer is done
    data[0] = I2CMasterDataGet(I2C1_BASE); // Place data into data register
    uint8_t i;
    for ( i = 1; i < length - 1; i++)
    {
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT); // Send continues condition
        while (I2CMasterBusy(I2C1_BASE))
            ;                                  // Wait until transfer is done
        data[i] = I2CMasterDataGet(I2C1_BASE); // Place data into data register
    }
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH); // Send finish condition
    while (I2CMasterBusy(I2C1_BASE))
        ;                                           // Wait until transfer is done
    data[length - 1] = I2CMasterDataGet(I2C1_BASE); // Place data into data register
}

void Single_WriteI2C(unsigned char SlaveAddress, unsigned char REG_Address, unsigned char REG_data)
{
    i2cWrite(SlaveAddress, REG_Address, REG_data);
}

unsigned char Single_ReadI2C(unsigned char SlaveAddress, unsigned char REG_Address)
{
    return i2cRead(SlaveAddress, REG_Address);
}

short int Double_ReadI2C(unsigned char SlaveAddress, unsigned char REG_Address)
{
    unsigned char msb, lsb;
    msb = i2cRead(SlaveAddress, REG_Address);
    lsb = i2cRead(SlaveAddress, REG_Address + 1);
    return (((short int)msb) << 8 | lsb);
}
