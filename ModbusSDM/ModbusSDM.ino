#include <ModbusMaster.h>
#define RXD2 16
#define TXD2 17

#define MAX485_DE      3 // I do not use this port since I only receive data
#define MAX485_RE_NEG  2

float voltage, current, power, energy;

// instantiate ModbusMaster object
ModbusMaster node;

void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

void setup()
{
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  // Modbus communication runs at 9600 baud
  Serial.begin(19200);
  Serial2.begin(2400, SERIAL_8N1, RXD2, TXD2);
  // Callbacks allow us to configure the RS485 transceiver correctly
//  node.preTransmission(preTransmission);
//  node.postTransmission(postTransmission);
}

bool state = true;
void loop()
{
  voltage = get_data(1, 0x0000, 2); // get_data(slave_id, register, number of register)
  current = get_data(1, 0x0006, 2);
  power   = get_data(1, 0x000C, 2);
  energy  = get_data(1, 0x0156, 2);
  
  Serial.println(voltage, 2); // 1 decimal   (x, 2)= x with 2 decimal
  Serial.println(current, 2);
  Serial.println(power  , 2);
  Serial.println(energy , 2);
  Serial.println("-----------------");
}

float get_data(int id, int reg, int num_reg)
{
    // Modbus slave ID
  node.begin(id, Serial2);
  float x;  
  uint16_t result;
  result = node.readInputRegisters(reg, num_reg);
  if (result == node.ku8MBSuccess)
  {
    unsigned long *p;
    p = (unsigned long*)&x;
    *p = (unsigned long)node.getResponseBuffer(0x00) <<16 | node.getResponseBuffer(0x01);
  } 
  node.clearResponseBuffer();
  delay(1000);
  return(x);
}
