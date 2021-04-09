/*
  ProcessShield.h - Library for interfacing with analog process simulator shield for Arduino Uno.
  Created by Torleif Anstensrud and Kristian Strøm, Feburary 25, 2021.
  Released into the public domain.
*/
#ifndef ProcessShield_h
#define ProcessShield_h

#include "Arduino.h"

class ProcessShield
{
	public:
		ProcessShield();
		void set_LED_D1(uint8_t value); // value: LOW or HIGH
		void set_LED_D2(uint8_t value); // value: LOW or HIGH
		void set_LED_D3(uint8_t value); // value: LOW or HIGH
		
		uint8_t get_button_SW1(); // returns HIGH if button pressed, else LOW
		
		float get_potmeter_RV1_percentage();
		float get_potmeter_RV1_voltage();
		
		void set_U5_channel_resistance(uint8_t channel, float value); // channel: 1 -> 4, value: 0 -> 100 kOhm between A and W
		void store_U5_eeprom();
		void load_U5_eeprom();
		
		void set_U6_channel_resistance(uint8_t channel, float value); // channel: 1 -> 4, value: 0 -> 100 kOhm between A and W
		void store_U6_eeprom();
		void load_U6_eeprom();
		
		void set_DAC_U4_voltage(float value); 		// value: MIN_VOLTAGE -> MAX_VOLTAGE
		void set_DAC_U4_percentage(float value); 	// value: 0.0 -> 1.0 (MIN_VOLTAGE -> MAX_VOLTAGE)
		
		float get_output_integrator_A0_voltage();
		float get_output_A1_voltage();
		float get_output_integrator_A0_percentage();
		float get_output_A1_percentage();
	
	private: // Not accessible outside library
		void set_LED_Di(uint8_t pin, uint8_t value);
		void set_Ui_channel_BW_level(uint8_t address, uint8_t channel, uint8_t value);
		void set_Ui_channel_resistance(uint8_t address, uint8_t channel, float value);
		void store_Ui_eeprom(uint8_t address);
		void load_Ui_eeprom(uint8_t address);
		void set_DAC_U4(int value_12bit);
		float get_output_Ai_voltage(uint8_t pin);
		float get_output_Ai_percentage(uint8_t pin);
};

#endif


