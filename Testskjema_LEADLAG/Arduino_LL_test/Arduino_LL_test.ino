#include <ProcessShield.h>
#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

// Include specific FreeRTOS function
#define INCLUDE_vTaskDelay 1

ProcessShield ps = ProcessShield();

// Task prototypes
void task_testing( void *pvParameters);

void setup()
{
  Serial.begin(9600);

  // Define charging system
  uint8_t channel = 3;
  float value = 20000; //Ohm
  ps.set_U6_channel_resistance(channel, value);

  ps.set_DAC_U4_percentage(0.0);

  xTaskCreate(
    task_testing
    ,  "Charging"
    ,  128      // Stack size
    ,  NULL     // Parameters
    ,  1        // priority
    ,  NULL );  // Task handle
}


void loop()
{

}

void task_testing(void *pvParameters)
{
  float local_process_value = 0.0;

  while(true)
  {
    local_process_value = ps.get_output_A1_percentage()*255; //Getting the processvalue with a number between 0 and 255.
    Serial.print("Prosessverdi:");
    Serial.print(local_process_value);
    Serial.print(",");
    Serial.print(50); //plotting lines to hold the axes constant.
    Serial.print(",");
    Serial.print(100);
    Serial.print(",");
    Serial.println(150);
    
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
