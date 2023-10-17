// - - - - -
// ESPDMX - A Arduino library for sending and receiving DMX using the builtin serial hardware port.
// ESPDMX.cpp: Library implementation file
//
// Copyright (C) 2015  Rick <ricardogg95@gmail.com>
// This work is licensed under a GNU style license.
//
// Last change: Marcel Seerig <https://github.com/mseerig>
//
// Documentation and samples are available at https://github.com/Rickgg/ESP-Dmx
// - - - - -

/* ----- LIBRARIES ----- */
#ifdef ESP8266

#include <Arduino.h>

#include "ESPDMX.h"
  
#define dmxMaxChannel 512
#define defaultMax 32

#define DMXSPEED 250000
#define DMXFORMAT SERIAL_8N2
#define BREAKSPEED 83333
#define BREAKFORMAT SERIAL_8N1

bool dmxStarted = false;
int sendPin = 2; // dafault on ESP8266

// DMX value array and size. Entry 0 will hold startbyte
uint8_t dmxDataStore[dmxMaxChannel] = {};
int channelSize;

void DMXESPSerial::init()
{
  channelSize = defaultMax;
#ifdef Versions6_1
  Serial.begin(DMXSPEED);
#else
  Serial1.begin(DMXSPEED);
#endif
  pinMode(sendPin, OUTPUT);
  dmxStarted = true;
}

// Set up the DMX-Protocol
void DMXESPSerial::init(int chanQuant)
{

  if (chanQuant > dmxMaxChannel || chanQuant <= 0)
  {
    chanQuant = defaultMax;
  }

  channelSize = chanQuant;

#ifdef Versions6_1
  Serial.begin(DMXSPEED);
#else
  Serial1.begin(DMXSPEED);
#endif
  pinMode(sendPin, OUTPUT);
  dmxStarted = true;
}

// Function to read DMX data
uint8_t DMXESPSerial::read(int Channel)
{
  if (dmxStarted == false)
    init();

  if (Channel < 1)
    Channel = 1;
  if (Channel > dmxMaxChannel)
    Channel = dmxMaxChannel;
  return (dmxDataStore[Channel]);
}

// Function to send DMX data
void DMXESPSerial::write(int Channel, uint8_t value)
{
  if (dmxStarted == false)
    init();

  if (Channel < 1)
    Channel = 1;
  if (Channel > channelSize)
    Channel = channelSize;
  if (value < 0)
    value = 0;
  if (value > 255)
    value = 255;

  dmxDataStore[Channel] = value;
}

void DMXESPSerial::end()
{
  channelSize = 0;
#ifdef Versions6_1
  Serial.end();
#else
  Serial1.end();
#endif
  dmxStarted = false;
}

void DMXESPSerial::update()
{
  if (dmxStarted == false)
    init();

#ifdef Versions6_1
  // Send break
  digitalWrite(sendPin, HIGH);
  Serial.begin(BREAKSPEED, BREAKFORMAT);
  Serial.write(0);
  Serial.flush();
  delay(1);
  Serial.end();

  // send data
  Serial.begin(DMXSPEED, DMXFORMAT);
  digitalWrite(sendPin, LOW);
  Serial.write(dmxDataStore, channelSize);
  Serial.flush();
  delay(1);
  Serial.end();

#else
  // Send break
  digitalWrite(sendPin, HIGH);
  Serial1.begin(BREAKSPEED, BREAKFORMAT);
  Serial1.write(0);
  Serial1.flush();
  delay(1);
  Serial1.end();

  // send data
  Serial1.begin(DMXSPEED, DMXFORMAT);
  digitalWrite(sendPin, LOW);
  Serial1.write(dmxDataStore, channelSize);
  Serial1.flush();
  delay(1);
  Serial1.end();
#endif
}

// Function to update the DMX bus

#endif