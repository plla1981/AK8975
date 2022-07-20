/*
 * @Author: Plla 1183136570@qq.com
 * @Date: 2022-06-28 14:57:24
 * @LastEditors: Plla 1183136570@qq.com
 * @LastEditTime: 2022-07-14 08:07:10
 * @FilePath: \undefinedi:\plla\我的工作\【新项目】42传感器编程盒\Kidsbits STEM电子积木 磁场计电子罗盘 黑色环保\测试与参数文件\代码\AK8975C\AK8975C.ino
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

// Include the correct display library

// For a connection via I2C using the Arduino Wire include:
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "AK8975C.h"
//#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"

AK8975C ak8975c;
//SSD1306Wire display(0x3c, SDA, SCL);

void setup() {
  Wire.begin();
  Serial.begin(9600);
  if(!ak8975c.begin())
  {
    Serial.println("error!");
    while(1);  
  }
  //display.init();
}
void loop() {

  ak8975c.measure();
  float degree1 = ak8975c.degree;
  
  Serial.println("----------------------");
  Serial.print("X：");
  Serial.println(ak8975c.X);

  Serial.print("Y：");
  Serial.println(ak8975c.Y);

  Serial.print("Z：");
  Serial.println(ak8975c.Z);
  Serial.println("----------------------");
  
  //display.clear();
  
  //display.setTextAlignment(TEXT_ALIGN_LEFT);
  //display.setFont(ArialMT_Plain_10);
  //display.drawString(0, 0, "X:");
  //display.drawString(20, 0, String(ak8975c.X));
  //display.drawString(0, 11, "Y:");
  //display.drawString(20, 11, String(ak8975c.Y));
  //display.drawString(0, 22,"Z:");
  //display.drawString(20, 22, String(ak8975c.Z));
  if((degree1 < 22.5) && (degree1 >= 337.5 ))
  {
    Serial.print(degree1);
    Serial.print("°  ");
    Serial.println("nNorth 北");
    //Serial.println("nSouth 南");
    //display.drawString(0, 33,"nNorth 北");
  }
  else if((degree1 > 22.5) && (degree1 < 67.5 ))
  {
    Serial.print(degree1);
    Serial.print("°  ");
    //Serial.println("South-West 西南");
    Serial.println("nNorthEast 东北");
    //display.drawString(0, 33,"nNorthEast 东北");
  }
  else if((degree1 > 67.5) && (degree1 < 112.5 ))
  {
    Serial.print(degree1);
    Serial.print("°  ");
    //Serial.println("nWest 西");
    Serial.println("East 东");
    //display.drawString(0, 33,"East 东");
  }
  else if((degree1 > 112.5) && (degree1 < 157.5 ))
  {
    Serial.print(degree1);
    Serial.print("°  ");
    //Serial.println("nNorth-West 西北");
    Serial.println("nSouthEast 东南");
    //display.drawString(0, 33,"nSouthEast 东南");
  }
  else if((degree1 > 157.5) && (degree1< 202.5 ))
  {
    Serial.print(degree1);
    Serial.print("°  ");
    //Serial.println("nNorth 北");
    Serial.println("nSouth 南");
    //display.drawString(0, 33,"nSouth 南");
  }
  else if((degree1 > 202.5) && (degree1 < 247.5 ))
  {
    Serial.print(degree1);
    Serial.print("°  ");
    //Serial.println("nNorthEast 东北");
    Serial.println("South-West 西南");
    //display.drawString(0, 33,"South-West 西南");
  }
  else if((degree1 > 247.5) && (degree1 < 292.5 ))
  {
    Serial.print(degree1);
    Serial.print("°  ");
//    Serial.println("East 东");
    Serial.println("nWest 西");
    //display.drawString(0, 33,"nWest 西");
  }
  else if((degree1 > 292.5) && (degree1 < 337.5 ))
  {
    Serial.print(degree1);
    Serial.print("°  ");
//    Serial.println("nSouthEast 东南");
    Serial.println("nNorth-West 西北");
    //display.drawString(0, 33,"nNorth-West 西北");
  }
  //display.display();
  delay(200);
}
