/*
  ProcessShield.h - Library for interfacing with analog process simulator shield for Arduino Uno.
  Created by Torleif Anstensrud and Kristian Strøm, Feburary 25, 2021.
  Released into the public domain.
*/

#include "Arduino.h"
#include "ProcessShield.h"
#include <Wire.h>   // I2C
#include <SPI.h>    // SPI

#define LED_D1_PIN 3
#define LED_D2_PIN 4
#define LED_D3_PIN 5
#define BUTTON_SW1_PIN 2
#define POTMETER_RV1_PIN 2
#define CHIPSELECT_PIN 10
#define OUTPUT_INTEGRATOR_A0_PIN 0
#define OUTPUT_A1_PIN 1

#define MAX_VOLTAGE 5.0
#define MIN_VOLTAGE 0.0
#define N_DAC_LEVELS 4095
#define N_ADC_LEVELS 1023

//I2C
//https://www.analog.com/media/en/technical-documentation/data-sheets/AD5124_5144_5144A.pdf
#define ADDRESS_U5 0b0101010 // The last bit is an implicit write bit when using Wire.beginTransmission()
#define ADDRESS_U6 0b0101011 // The last bit is an implicit write bit when using Wire.beginTransmission()
#define COMMAND_WRITE_RDAC1 0b00010000 //Write the correct command byte here 0bXXXXXXXX
#define COMMAND_ACCESS_EEPROM 0b01110000
#define ADDRESS_ALL_CHANNELS 0b00001000
#define DATA_WRITE_EEPROM 0b00000001
#define DATA_READ_EEPROM 0b00000000
#define RAB_RESISTANCE 100000.0
#define BS_TS_RESISTANCE 60.0
#define RW_RESISTANCE 130.0
#define N_RESISTANCE_LEVELS 255

//SPI
//http://ww1.microchip.com/downloads/en/devicedoc/21897b.pdf
#define ADDRESS_U4 0b0011000000000000
#define SPI_FREQUENCY 1000000 // 1 Mhz

ProcessShield::ProcessShield()
{
	//Digital pins
	pinMode(LED_D1_PIN, OUTPUT);
	pinMode(LED_D2_PIN, OUTPUT);
	pinMode(LED_D3_PIN, OUTPUT);

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW); //Turn off builtin LED on Arduino board

	pinMode(BUTTON_SW1_PIN, INPUT);

	//I2C
	Wire.begin();

	//SPI for DAC
	SPI.begin();
	pinMode(CHIPSELECT_PIN, OUTPUT);
}

void ProcessShield::set_LED_Di(uint8_t pin, uint8_t value)
{
	// LED on -> value=HIGH, LED off -> value=LOW
	digitalWrite(pin, value);
}

void ProcessShield::set_LED_D1(uint8_t value)
{
	set_LED_Di(LED_D1_PIN, value);
}

void ProcessShield::set_LED_D2(uint8_t value)
{
	set_LED_Di(LED_D2_PIN, value);
}

void ProcessShield::set_LED_D3(uint8_t value)
{
	set_LED_Di(LED_D3_PIN, value);
}

uint8_t ProcessShield::get_button_SW1()
{
	// returns HIGH if button pressed, else LOW
	return !digitalRead(BUTTON_SW1_PIN);
}

float ProcessShield::get_potmeter_RV1_percentage()
{
	return ProcessShield::get_output_Ai_percentage(POTMETER_RV1_PIN);
}

float ProcessShield::get_potmeter_RV1_voltage()
{
	return ProcessShield::get_output_Ai_voltage(POTMETER_RV1_PIN);
}

void ProcessShield::set_Ui_channel_BW_level(uint8_t address, uint8_t channel, uint8_t value)
{
	// channel:  1 -> 4
	// value: 0 -> 255 (0 -> 100 kOhm between B and W)
	Wire.beginTransmission(address);

	if(channel >4) //TODO: add warning for illegal values
	{
		channel = 4;
	} else if(channel < 1)
	{
		channel = 1;
	}

	uint8_t channel_address_byte = channel-1;
	Wire.write(COMMAND_WRITE_RDAC1 | channel_address_byte);
	Wire.write(value);
	Wire.endTransmission();
}

void ProcessShield::set_Ui_channel_resistance(uint8_t address, uint8_t channel, float value)
{
	// TODO: add special case for U6 channel 2 and 4
	// TODO: add wiper resistance
	// value: 0 -> 100 kOhm between A and W

	if(value > RAB_RESISTANCE) //TODO: add warning for illegal values
	{
		value = RAB_RESISTANCE;
	} else if(value < BS_TS_RESISTANCE) //Special mode for bottom scale/top scale
	{
		value = 0.0;
	}

	uint8_t level = (int) N_RESISTANCE_LEVELS * (1.0-(value/RAB_RESISTANCE));
	set_Ui_channel_BW_level(address, channel, level);
}

void ProcessShield::set_U5_channel_resistance(uint8_t channel, float value)
{
	set_Ui_channel_resistance(ADDRESS_U5, channel, value);
}

void ProcessShield::set_U6_channel_resistance(uint8_t channel, float value)
{
	set_Ui_channel_resistance(ADDRESS_U6, channel, value);
}

void ProcessShield::store_Ui_eeprom(uint8_t address)
{
	Wire.beginTransmission(address);
	Wire.write(COMMAND_ACCESS_EEPROM | ADDRESS_ALL_CHANNELS);
	Wire.write(DATA_WRITE_EEPROM);
	Wire.endTransmission();
}

void ProcessShield::store_U5_eeprom()
{
	  store_Ui_eeprom(ADDRESS_U5);
}

void ProcessShield::store_U6_eeprom()
{
	  store_Ui_eeprom(ADDRESS_U6);
}

void ProcessShield::load_Ui_eeprom(uint8_t address)
{
	Wire.beginTransmission(address);
	Wire.write(COMMAND_ACCESS_EEPROM | ADDRESS_ALL_CHANNELS);
	Wire.write(DATA_READ_EEPROM);
	Wire.endTransmission();
}

void ProcessShield::load_U5_eeprom()
{
	load_Ui_eeprom(ADDRESS_U5);
}

void ProcessShield::load_U6_eeprom()
{
	load_Ui_eeprom(ADDRESS_U6);
}

void ProcessShield::set_DAC_U4(int value_12bit)
{
	// value: 0 -> 4095 (MIN_VOLTAGE -> MAX_VOLTAGE)
	SPI.beginTransaction(SPISettings(SPI_FREQUENCY, MSBFIRST, SPI_MODE0));
	digitalWrite(CHIPSELECT_PIN, LOW);

	SPI.transfer16(ADDRESS_U4 | value_12bit);

	digitalWrite(CHIPSELECT_PIN, HIGH);
	SPI.endTransaction();
}

void ProcessShield::set_DAC_U4_voltage(float value)
{
	// value: MIN_VOLTAGE -> MAX_VOLTAGE

	if(value > MAX_VOLTAGE) //TODO: add warning for illegal values
	{
		value = MAX_VOLTAGE;
	} else if(value < MIN_VOLTAGE)
	{
		value = MIN_VOLTAGE;
	}

	// Write code to set the 12-bit representation of the parameter value (given in voltage MIN_VOLTAGE -> MAX_VOLTAGE)
	int value_12bit = value * N_DAC_LEVELS/(MAX_VOLTAGE-MIN_VOLTAGE); // 0 ... 4095
	set_DAC_U4(value_12bit);
}

void ProcessShield::set_DAC_U4_percentage(float value)
{
	// value: 0.0 -> 1.0 (MIN_VOLTAGE -> MAX_VOLTAGE)

	if(value > 1) //TODO: add warning for illegal values
	{
		value = 1;
	} else if(value < 0)
	{
		value = 0;
	}

	// Write code to set the 12-bit representation of the parameter value (given in percentage 0.0 -> 1.0)
	int value_12bit = value * N_DAC_LEVELS; // 0 ... 4095
	set_DAC_U4(value_12bit);
}

float ProcessShield::get_output_Ai_voltage(uint8_t pin)
{
	int output_ADC_level = analogRead(pin);
	return output_ADC_level * ((MAX_VOLTAGE-MIN_VOLTAGE) / N_ADC_LEVELS) + MIN_VOLTAGE;
}

float ProcessShield::get_output_integrator_A0_voltage()
{
	return get_output_Ai_voltage(OUTPUT_INTEGRATOR_A0_PIN);
}

float ProcessShield::get_output_A1_voltage()
{
	return get_output_Ai_voltage(OUTPUT_A1_PIN);
}

float ProcessShield::get_output_Ai_percentage(uint8_t pin)
{
	int output_ADC_level = analogRead(pin);
	return output_ADC_level / (float) N_ADC_LEVELS;
}

float ProcessShield::get_output_integrator_A0_percentage()
{
	return get_output_Ai_percentage(OUTPUT_INTEGRATOR_A0_PIN);
}

float ProcessShield::get_output_A1_percentage()
{
	return get_output_Ai_percentage(OUTPUT_A1_PIN);
}
