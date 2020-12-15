// This #include statement was automatically added by the Particle IDE.
#include <HX711ADC.h>

// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_Sensor.h>

// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_LIS3DH.h>



/*********************************************************
* read analog input, write data to array, send array to serial
**********************************************************/

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

sensors_event_t event; 


double currentTime = 0;


void setup() {  
    
    if (! lis.begin(0x18)) {  
        Particle.publish("ev","LIS3DH configuration error");
    }
    
    lis.setRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!
    Particle.publish("ev","LIS3DH configured successfully");

    delay(100);
    
	// A0: INPUT is the default pin mode. No config required
	Serial.begin(9600);
}

void loop() {
    
    
    lis.getEvent(&event);
  
    Serial.print(currentTime);
    Serial.print("; ");
  
    Serial.print(event.acceleration.x);
    Serial.print("; ");
    Serial.print(event.acceleration.y);
    Serial.print("; ");
    Serial.println(event.acceleration.z);

    delay(10); 
    currentTime = currentTime + 0.01;
    



}
