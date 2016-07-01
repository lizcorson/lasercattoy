// Example of using the 74HC4051 multiplexer/demultiplexer

// Author: Nick Gammon
// Date:   14 March 2013
// Source: http://www.gammon.com.au/forum/?id=11976
// Modified by Liz Corson, June 2016

const byte sensor = A0;  // where the multiplexer in/out port is connected

// the multiplexer address select lines (A/B/C)
const byte addressA = 6; // low-order bit
const byte addressB = 5;
const byte addressC = 4; // high-order bit

void setup ()
  {
  Serial.begin (115200);
  Serial.println ("Starting multiplexer test ...");
  pinMode (addressA, OUTPUT); 
  pinMode (addressB, OUTPUT); 
  pinMode (addressC, OUTPUT); 
  }  // end of setup

int readSensor (const byte which)
  {
  // select correct MUX channel
  digitalWrite (addressA, (which & 1) ? HIGH : LOW);  // low-order bit
  digitalWrite (addressB, (which & 2) ? HIGH : LOW);
  digitalWrite (addressC, (which & 4) ? HIGH : LOW);  // high-order bit
  // now read the sensor
  return analogRead (sensor);
  }  // end of readSensor
  
void loop ()
  {
  // show all 8 sensor readings
  for (byte i = 0; i < 2; i++)
    {
    Serial.print ("Multiplexed sensor ");
    Serial.print (i);
    Serial.print (" reads: ");
    Serial.println (readSensor (i));
    Serial.print("Analog Read: ");
    if (i == 0) {
      Serial.println(analogRead(A1));
    } else if (i == 1) {
      Serial.println(analogRead(A2));
    }
    
    }
  delay (3000);
  }  // end of loop
