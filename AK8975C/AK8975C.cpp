/*
 * @Author: Plla 1183136570@qq.com
 * @Date: 2022-06-28 14:57:24
 * @LastEditors: Plla 1183136570@qq.com
 * @LastEditTime: 2022-07-14 10:54:52
 * @FilePath: \undefinedi:\plla\我的工作\【新项目】42传感器编程盒\Kidsbits STEM电子积木 磁场计电子罗盘 黑色环保\测试与参数文件\代码\AK8975C\AK8975C.cpp
 * @Description: 
 * 
 * 摇杆模块 I2C设备
 * 适用于Arduino：UNO | ESP32 | PICO
 * 
 * @HTTP:https://github.com/plla1981
 * 
 *                     license The MIT License (MIT)
 * 
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the 'Software'), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 * 
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 * 
 *  THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 * 
 * 
 * Version:
 * 
 * v0.0.1: 2022.7.9, Initial version.
 * 
 * Copyright (c) 2022 by Plla 1183136570@qq.com.
 */

#include "AK8975C.h"


/**
 * @description: 初始化 bool
 * @param {uint8_t} address
 * @param {TwoWire} &wirePort
 * @return {0 & 1}
 */
bool AK8975C::begin(uint8_t address,TwoWire &wirePort)
{
    delay(5);
    _address = address;
    _i2cPort = &wirePort;
    _i2cPort->begin();
    if(!IsExist())
        return false;
    return true;
}

/**
 * @description: 检测是否读取到设备ID
 * @return {*}
 */
bool AK8975C::IsExist(void)
{
    uint8_t config_ID;
    IIC_Read_Byte(WIA_AKM_ID,&config_ID,1);
    Serial.println(config_ID);
    if(config_ID == CHIP_ID)
        return true;
    else
        return false;
}

/**
 * @description: IIC写数据
 * @param {uint8_t} reg
 * @param {uint8_t} data
 * @return {*}
 */
void AK8975C::IIC_Write_Byte(uint8_t reg,uint8_t data)
{
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(reg);
    _i2cPort->write(data);
    _i2cPort->endTransmission();
}

/**
 * @description: IIC读数据
 * @param {uint8_t} reg
 * @param {uint8_t*} buf
 * @param {int} lenght
 * @return {*}
 */
void AK8975C::IIC_Read_Byte(uint8_t reg,uint8_t* buf,uint8_t lenght)
{
    uint8_t i = 0;
    _i2cPort->beginTransmission(_address);
    //reg |= 0x80; //turn auto-increment bit on
    _i2cPort->write(reg);
    _i2cPort->endTransmission(false);
    _i2cPort->requestFrom(_address,lenght);

    while (_i2cPort->available() && i<lenght)
    {
        *buf = _i2cPort->read();
        buf++;
        i++;
    }
}

/**
 * @description: 模式设置
 * @param {uint8_t} mode
 * @return {*}
 */
bool AK8975C::AK8975_MODE_SET(uint8_t mode)
{
    uint8_t _mode = MODE_PWR_DOWM;   /* 默认掉电模式 */
    if(mode != MODE_PWR_DOWM && mode != MODE_SING_MEA && mode != MODE_SELF_TEST && mode !=MODE_FUSE_ROM)
    {
        return false; /* set mode error */
    }
    else
    {
        _mode = mode;
    }
    IIC_Write_Byte(CNTL,_mode);
    return true;

}

/**
 * @description: 检查数据是否准备就绪
 * @return {*}
 */
bool AK8975C::AK8975_CHK_DATA_RDY(void)
{
    uint8_t ready[1],error[1];
    /* 1.读取Status1 */
    IIC_Read_Byte(Status_1,ready,1);
    if(ready[0] != 1)
    {
        return false;
    }
    delayMicroseconds(100);
    IIC_Read_Byte(Status_2,error,1);
    if(error[0] != 1)
    {
        return false;
    }
    if((ready[0] & 0x01 == 0x01) && (error[0] & 0x0C))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @description: 获取角度数据
 * @return {*}
 */
float AK8975C::AK8975_GET_AZIMUTH(int8_t x,int8_t y)
{
  float x_data,y_data,result_data,cal;

  /* calculate the azimuth */
  result_data =atan2((float)x,(float)y)*180.0/3.14159 +180.0;
  return result_data + 30; //补上30度的偏差
}

/**
 * @description: 获取数据
 * @return {*}
 */
void AK8975C::measure(void)
{
    uint8_t buff_data[8];
    uint8_t buff2[8];
    uint8_t status = 0;
    /* 1.设置熔断模式，读取灵敏度调整值 */
    delayMicroseconds(101);
    AK8975_MODE_SET(MODE_FUSE_ROM);
    IIC_Read_Byte(ASAX,buff_data,3);
    _ASAX = buff_data[0];
    _ASAY = buff_data[1];
    _ASAZ = buff_data[2];
    /* 2.延时100us后 设置为断电模式 */
    delayMicroseconds(101);
    AK8975_MODE_SET(MODE_PWR_DOWM);
    delayMicroseconds(101);
    /* 3.判断当前模式 */
    IIC_Read_Byte(CNTL,buff2,1);
    if(buff2[0] == MODE_PWR_DOWM)
    {
        AK8975_MODE_SET(MODE_SING_MEA);
        for (size_t i = 0; i < 4; i++)
        {
            if(AK8975_CHK_DATA_RDY())
            {
                status = 0;
            }
            else
            {
                status = 1;
            }
        }
        delayMicroseconds(50);
        if(status)
        {
          
            IIC_Read_Byte(HXL,buff_data,1);
            buff2[0] = buff_data[0];
            delayMicroseconds(50);
            IIC_Read_Byte(HXH,buff_data,1);
            buff2[1] = buff_data[0];
            delayMicroseconds(50);
            IIC_Read_Byte(HYL,buff_data,1);
            buff2[2] = buff_data[0];
            delayMicroseconds(50);
            IIC_Read_Byte(HYH,buff_data,1);
            buff2[3] = buff_data[0];
            delayMicroseconds(50);
            IIC_Read_Byte(HZL,buff_data,1);
            buff2[4] = buff_data[0];
            delayMicroseconds(50);
            IIC_Read_Byte(HZH,buff_data,1);
            buff2[5] = buff_data[0];
            
            X = (buff2[1] << 8) | buff2[0];
            Y = (buff2[3] << 8) | buff2[2];
            Z = (buff2[5] << 8) | buff2[4];
        }
        //进行调整
        X = (X * (((_ASAX - 128) * 0.5) / 128 + 1)) * 0.3;
        Y = (Y * (((_ASAY - 128) * 0.5) / 128 + 1)) * 0.3;
        Z = (Z * (((_ASAZ - 128) * 0.5) / 128 + 1)) * 0.3;
        degree = AK8975_GET_AZIMUTH(Y,X);
    }
    else
    {
       X = 0;
       Y = 0;
       Z = 0;  
    }

}
