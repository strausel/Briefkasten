#include "HX711ADC.h"

// HX711.DOUT	- pin #A1
// HX711.PD_SCK	- pin #A0

HX711ADC scale(A1, A0);		// parameter "gain" is ommited; the default value 128 is used by the library

float weight = 0;

void setup () {
  Serial.begin(9600);
  Serial.println("HX711 Demo");
  scale.begin();

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());			// print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));  	// print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);	// print the average of 5 readings from the ADC minus tare weight (not set) divided 
						// by the SCALE parameter (not set yet)  

  scale.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();				        // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided 
						// by the SCALE parameter set with set_scale

  Serial.println("Readings:");
  
  
}

int round(float x){ 
    return (x*100+0.5)/100.0; 
}


void loop() {
  
  float oner = scale.get_units(); 
  float scaler = round(oner)*10; 
  //Particle.publish("Gramm",String(scaler));
  

 

  //scale.power_down();                    // put the ADC in sleep mode
  delay(3000);
  //scale.power_up();
  
  if (scaler != weight){
      Particle.publish("Gramm",String(scaler));
      weight = scaler;
      
  }
}