/*
 * @Author: Plla 1183136570@qq.com
 * @Date: 2022-06-28 14:57:49
 * @LastEditors: Plla 1183136570@qq.com
 * @LastEditTime: 2022-07-14 09:58:33
 * @FilePath: \undefinedi:\plla\我的工作\【新项目】42传感器编程盒\Kidsbits STEM电子积木 磁场计电子罗盘 黑色环保\测试与参数文件\代码\AK8975C\AK8975C.h
 * @Description: 
 * 
 * Headers
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

#ifndef _AK8975C_H
#define _AK8975C_H
#include "Wire.h"
#include "Arduino.h"

#define I2C_ADDR                0x0E
#define CHIP_ID                 0x48

#define WIA_AKM_ID              0x00        //设备ID
#define INFO_AKM                0x01        //设备信息
#define Status_1                0x02        //数据状态寄存器 0：正常 1：数据已准备
#define HXL                     0x03        //X轴L数据
#define HXH                     0x04        //X轴H数据
#define HYL                     0x05        //Y轴L数据
#define HYH                     0x06        //Y轴H数据
#define HZL                     0x07        //Z轴L数据
#define HZH                     0x08        //Z轴H数据
#define Status_2                0x09        //数据状态寄存器
#define Stu_DERR                0x04        //读取STU2寄存器，进行数据判断 0：正常 1：数据读取错误
#define Stu_HOFL                0x08        //读取STU2寄存器，进行数据判断 0：正常 1：传感器发生溢出
#define CNTL                    0x0A        //操作模式设置
#define MODE_PWR_DOWM           0x00        //[3:0]:"0000"：掉电模式
#define MODE_SING_MEA           0x01        //      "0001"：单一测量模式
#define MODE_SELF_TEST          0x08        //      "1000"：自检模式
#define MODE_FUSE_ROM           0x0F        //      "1111"：保险丝ROM访问模式
                                            //当寄存器被写入时，从02H到09H的寄存器被初始化
#define ASTC                    0x0C        //自我检测控制
#define SELF                    0x40
#define ASAX                    0x10        //灵敏度调整值X
#define ASAY                    0x11        //灵敏度调整值X
#define ASAZ                    0x12        //灵敏度调整值X
                                            //灵敏度调整公式
                                            //Hadj = H * (((ASA - 128) * 0.5)/128 + 1)
                                            //其中H为读出的测量值，ASA为灵敏度调整值，Hadj为调整后的测量数据




class AK8975C
{
private:
    int16_t _ASAX;
    int16_t _ASAY;
    int16_t _ASAZ;
    uint8_t _address;
    TwoWire *_i2cPort;
    void IIC_Write_Byte(uint8_t reg, uint8_t data);
    void IIC_Read_Byte(uint8_t reg, uint8_t* buf, uint8_t lenght);
    void _dataConversion(uint8_t DATA_H,uint8_t DATA_L,uint16_t *_RESULT);
    bool IsExist(void);
    bool AK8975_MODE_SET(uint8_t mode);
    bool AK8975_CHK_DATA_RDY(void);
    float AK8975_GET_AZIMUTH(int8_t x,int8_t y);

public:
    int16_t X;
    int16_t Y;
    int16_t Z;
    float degree;
    bool begin(uint8_t address = I2C_ADDR, TwoWire &wirePort = Wire);
    //void begin(uint8_t address = I2C_ADDR, TwoWire &wirePort = Wire);
    void measure(void);
};

#endif
