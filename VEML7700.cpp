#include "mbed.h"
#include "VEML7700.h"

/* COMMAND REGISTER FORMAT */
#define CMD_ALS_CONF      0x00
#define CMD_ALS_WH        0x01
#define CMD_ALS_WL        0x02
#define CMD_PWR_SAVINGS   0x03
#define CMD_ALS           0x04
#define CMD_WHITE         0x05
#define CMD_ASL_INT       0x06

/* Configuration Register #0 (REG_ALS_CONF) 
bit[15:13] (reserved)
Set 000b

bit[12:11] ALS_GAIN Gain Selection
00 = ALS gain x 1
01 = ALS gain x 2
10 = ALS gain x (1/8)
11 = ALS gain x (1/4)

bit[10]    (reserved)
set 00b

bit[9:6]   ALS_IT ALS integration time settings
1100 =  25 ms
1000 =  50 ms
0000 = 100 ms
0001 = 200 ms
0010 = 400 ms
0011 = 800 ms

bit[5:4]   ALS_PERS ALS Persistence protect number setting
00 = 1
01 = 2
10 = 4
11 = 8

bit[3:2]   (reserved)
Set 00b

bit[1]     ALS_INT_EN ALS interrupt enable setting
0 = ALS INT disable
1 = ALS INT enable

bit[0]     ALS_SD ALS shut down setting
0 = ALS power on
1 = ALS shut down

0000000110000001
*/



/* High Threshold Windows Setting #1 
bit[15:8] ALS high threshold window setting (15:8 MSB 8 bits of whole 16 bits)
bit[7:0]  ALS high threshold window setting (7:0 LSB 8 bits of whole 16 bits)
*/

/* Low Threshold WIndows Setting #2
bit[15:8] ALS Low threshold window setting (15:8 MSB 8 bits of whole 16 bits)
bit[7:0]  ALS Low threshold window setting (7:0 LSB 8 bits of whole 16 bits)
*/

/* Power Saving Mode 
bit[15:3] (reserved)
bit[2:1] PSM Power saving mode; see table "Refresh time"
*/

/* ALS High Resolution Output data #4
bit[15:8] ALS high resolution output data (15:8 MSB 8 bits of whole 16 bits)
bit[7:0]  ALS high resolution output data (7:0 LSB 8 bits of whole 16 bits)
*/

/* WHITE Channel Output data #5
bit[15:8] WHITE output data (15:8 MSB 8 bits of whole 16 bits)
bit[7:0] WHITE output data (7:0 MSB 8 bits of whole 16 bits)
*/

/* Interrupt status #6
bit[15] int_th_low Read bit. Indicate a low threshold exceed.
bit[14] int_th_high Read bit. Indicate a high threshold exceed.
bit[13:0] (reserved)
*/

int VEML7700::setALSConf(uint16_t conf) 
{
    int res;
    uint8_t data[3] ;
    /*
    data[0] = CMD_ALS_CONF ;
    data[1] =  conf  & 0xFF ;
    data[2] = (conf >> 8) & 0xFF ;
    */
    data[0] = CMD_ALS_CONF ;
    data[1] = 0x0000;
    data[2] = 0x0000;
   res += writeRegs(data, 3) ;
   return res;
}

int VEML7700::setALS_WH(uint16_t wh) 
{
    int res; 
    uint8_t data[3] ;
    data[0] = CMD_ALS_WH ;
    data[1] =  wh & 0xFF ;
    data[2] = (wh >> 8) & 0xFF ;
    res += writeRegs(data, 3) ;
    return res;
}

int VEML7700::setPowerSaving(uint16_t ps) 
{
    int res;
    uint8_t data[3] ;
    data[0] = CMD_PWR_SAVINGS ;
    /*
    data[1] = ps & 0xFF ;
    data[2] = (ps >> 8) & 0xFF ;
    */
    if(ps == 1)
    {
    data[1] = 0x00 ;
    data[2] = 0x07 ;
    }
    else 
    {
        data[1] = 0x00 ;
        data[2] = 0x06 ;
    }
    res += writeRegs(data, 3) ;
    return res;
}

int VEML7700::getALS(uint16_t & temp) 
{
    int res;
    uint16_t als = 0 ;
    uint8_t cmd = CMD_ALS ;
    uint8_t data[2] ;
    res += readRegs(cmd, data, 2) ;
    als = (data[1] << 8) | data[0] ;
    temp = als;
    return res ;
}

int VEML7700::getWHITE(uint16_t & temp) 
{
    int res;
    uint16_t white = 0 ;
    uint8_t cmd = CMD_WHITE ;
    uint8_t data[2] ;
    res += readRegs(cmd, data, 2) ;
    white = (data[1] << 8) | data[0] ;
    temp = white;
    return res ;
}

int VEML7700::getALS_INT(uint16_t & temp) 
{
    int res;
    uint16_t als_int = 0 ;
    uint8_t cmd = CMD_ASL_INT ;
    uint8_t data[2] ;
    res += readRegs(cmd, data, 2) ;
    als_int = (data[1] << 8) | data[0] ;
    temp = als_int;
    return res;
    
}
  
VEML7700::VEML7700( int addr) :
    m_addr(addr<<1) 
{
    // activate the peripheral
}


VEML7700::~VEML7700()
{
}

int VEML7700::readRegs(int addr, uint8_t * data, int len) {
    int res;
    char t[1] = {static_cast<char>(addr)};
    res += i2c.write(m_addr, t, 1, true);
    res += i2c.read(m_addr, (char *)data, len);
    return res;
}

int VEML7700::writeRegs(uint8_t * data, int len) {
    int res;
    res += i2c.write(m_addr, (char *)data, len);
    return res;
}

VEML7700* VEML7700::Instance = nullptr;

VEML7700* VEML7700::getInstance(){
    if (Instance == nullptr){
        Instance = new VEML7700(0x10);
    }
    
    return Instance;
}

