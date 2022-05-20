/*
* Brian R Taylor
* brian.taylor@bolderflight.com
* 
* Copyright (c) 2022 Bolder Flight Systems
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
* and associated documentation files (the "Software"), to deal in the Software without restriction, 
* including without limitation the rights to use, copy, modify, merge, publish, distribute, 
* sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all copies or 
* substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "ms4525do.h"

/* 
* An MS4525DO object
*/
bfs::Ms4525do pres;

void setup() {
  /* Serial to display data */
  Serial.begin(9600);
  while(!Serial){}
  Wire.begin();
  Wire.setClock(400000);
  /* 
  * I2C address of 0x28, on bus 0, with a -1 to +1 PSI range
  */
  pres.Config(&Wire, 0x28, 1.0f, -1.0f);
  /* Starting communication with the pressure transducer */
  if (!pres.Begin()) {
    Serial.println("Error communicating with sensor");
    while(1){}
  }
}

void loop() {
  /* Read the sensor */
  if (pres.Read()) {
    /* Display the data */
    Serial.print(pres.pres_pa(), 6);
    Serial.print("\t");
    Serial.print(pres.die_temp_c(), 6);
    Serial.print("\n");
  }
  delay(10);
}