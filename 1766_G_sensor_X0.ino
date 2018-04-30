//-------------------------------------------------------------------
// Example usage for 1766_FPC_G_sensor_ST_LIS2DH12_ST
// Written by Aero Yao - 2018/04/20
// The follow code assumes a LIS2DH12 connected to an Arduino Mega 2560
// No-Go indicator Pin 4
// Go indicator Pin 5
//-------------------------------------------------------------------

#include<Wire.h>
#define GINT2 1
#define NG_LED_pin 4 // No-Go indicator
#define OK_LED_pin 5 // Go indicator

int G1_address = 0x19; //00011001b
int8_t I2C_det ;
unsigned int temp[6];
char *out_title[]={"X:","Y:","Z:"};
double acc_value[3]  = {0,0,0};

void setup() {
   pinMode(NG_LED_pin, OUTPUT);  
   pinMode(OK_LED_pin, OUTPUT);
   Serial.begin(115200);
   pinMode(GINT2,OUTPUT);
   digitalWrite(GINT2,LOW); //default: push-pull output to GND
   Wire.begin();
   Serial.println("\nBegin G_sensor test");
   LIS2DH12_Init(); 
}

void loop() {
     LIS2DH12_Init();
     if(I2C_det != -1){
     LIS2DH12_read_out();   
     }
}

void LIS2DH12_Init(){ 
  Wire.beginTransmission(G1_address);
  I2C_det = Wire.endTransmission();
    if(I2C_det != 0){
       I2C_det = -1; 
       Serial.println("No I2C devices found");
       digitalWrite(NG_LED_pin,HIGH);
       delay(1000);
       digitalWrite(NG_LED_pin,LOW);     
       delay(1000);   
    }else{
      G_sensor_init();
      I2C_det = 1;         
    }
        return I2C_det;
}
void LIS2DH12_read_out(){
digitalWrite(OK_LED_pin,HIGH);
delay(1000);
digitalWrite(OK_LED_pin,LOW); 
delay(1000);     
Wire.beginTransmission(G1_address);
Wire.write(0x28);
Wire.endTransmission();
Wire.requestFrom(G1_address, 6);
int count = 0;
while(Wire.available() && count <6){
  temp[count] = Wire.read();
  count++;
}
  int temp_value = 0;
  for(int i=0; i<3; i++){
    if((temp[i*2+1] & 0x80)!=0){
     temp_value = (((~0)>>16)<<16)|((temp[i*2+1]<<8)+temp[i*2]); 
    }
    else{
      temp_value = (temp[i*2+1]<<8)+ temp[i*2];
    }
    acc_value[i] = (double)temp_value * 4 /1000;
  }
     for(int i=0;i<3; i++){
      Serial.print(out_title[i]);
      Serial.print(acc_value[i]);
      Serial.print(",\t");
     }
      Serial.println();
      //delay(1000);  
}  

void G_sensor_init(){
  Wire.beginTransmission(G1_address);
  Wire.write(0x1E); //CTRL REG0
  Wire.write(0x10);//90 SD0/SA0 pin: 0x90: connected; 0x10: disconnected
  Wire.endTransmission();
  delayMicroseconds(100); 
 
  Wire.beginTransmission(G1_address);
  Wire.write(0x20); //CTRL_REG1
  Wire.write(0x27);//0x07 off; 0x17:1Hz; 0x27; 10Hz; 0x37:25Hz; 0x47:50Hz; 0x57:100Hz; 0x67:200Hz; 0x77:400Hz; 0x87:1.62K; 0x97:5.4Khz
  Wire.endTransmission();
  delayMicroseconds(100);  

  Wire.beginTransmission(G1_address);
  Wire.write(0x21); //CTRL_REG2
  Wire.write(0x00); //normal operation
  Wire.endTransmission();
  delayMicroseconds(100); 
  
  Wire.beginTransmission(G1_address);
  Wire.write(0x22); //CTRL_REG3
  Wire.write(0x01); //normal operation
  Wire.endTransmission();
  delayMicroseconds(100);
    
  Wire.beginTransmission(G1_address);
  Wire.write(0x23); // CTRL_REG4
  Wire.write(0x00); // 0x00: normal operation; 0x08: High-resolution
  Wire.endTransmission();
  delayMicroseconds(100); 
  
  Wire.beginTransmission(G1_address);
  Wire.write(0x24); // CTRL_REG5
  Wire.write(0x00); // normal operation
  Wire.endTransmission();
  delayMicroseconds(100);  

  Wire.beginTransmission(G1_address);
  Wire.write(0x25); //CTRL_REG6
  Wire.write(0x00); // normal operation
  Wire.endTransmission();
  delayMicroseconds(100);
}

