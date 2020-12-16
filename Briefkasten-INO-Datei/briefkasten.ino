// This #include statement was automatically added by the Particle IDE.
#include <HX711ADC.h>

// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_Sensor.h>

// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_LIS3DH.h>

HX711ADC scale(A1, A0);		// parameter "gain" is ommited; the default value 128 is used by the library
Adafruit_LIS3DH lis = Adafruit_LIS3DH();


sensors_event_t event; 

//Thresholds
//Variablen als Geschwindigkeitsindikatoren, t= 120 millisekunden (ab überschreitung a1 bis überschreitung b1 und überschreitung c1). 
// a1, b1, c1 und d1 gehören zusammen für Klappenbetätigung (Brief Einwurf). v1, w1, x1, y1 und z1 für Öffnung (Brief Entnahme).
double a1 = 0.5;
double b1 = 0.2;
double c1 = 1.1;

double v1 = 0.2;
double w1 = -0.2;
double x1 = 0.1;

double to = 10000; // 10 Sek
double tk = 3000; //3 sek

unsigned long startTime;
unsigned long deltaTime;
boolean Oeffnung = false;
boolean Klappe = false;
boolean o = false;
boolean k = false;

double weight = 0;
double totalweight = 0;
double scalertemp;

unsigned long startZeit, deltaZeit;
String Wochentag, Postmenge;
int Tag = 0;


String Datum = Time.format("%d %B %Y");

void setup() {  
    
//um auf variable zugreifen zu können von particle console und zum dann auslesen können auf website
    Particle.variable("a1", a1);
    Particle.variable("b1", b1);
    Particle.variable("c1", c1);

    Particle.variable("v1", v1);
    Particle.variable("w1", w1);
    Particle.variable("x1", x1);

    Particle.variable("to", to);
    Particle.variable("tk", tk);
    
    Particle.variable("weight", weight);
    Particle.variable("Postmenge", Postmenge);
    Particle.variable("Datum", Datum);
    Particle.variable("Wochentag", Wochentag);
    // register the cloud function 
    //Unsere Funktion
    Particle.function("remoteOeffnung", remoteOeffnung);
    Particle.function("remoteKlappe", remoteKlappe);
    
// Serial Setup Geschwindigkeitssensor
    if (! lis.begin(0x18)) {  
        Particle.publish("ev","LIS3DH configuration error");
    }
    
    lis.setRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!
    Particle.publish("ev","LIS3DH configured successfully");

    delay(100);
    
	Serial.begin(9600);
	
	// Serial Setup Waage	
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
    
    //Setzt bei erkannter Bewegung das entsprechende auf true
    //o und k als Hilfsvariablen
    
    Oeffnung = false;
    Klappe = false;
    o = false;
    k = false;
    

    //ruft variable ab 
    lis.getEvent(&event);
    
    //detect Öffnung
    //sobald a1 überschritten wird merken wir uns startzeit. Delta Zeit misst die Zeit. Solang Delta kleiner als t hört er auf die weiteren events
    //sobald zeit t überschritten wurde werden andere events unwichtig
    if(event.acceleration.y >= a1) {
        startTime = millis();
        deltaTime = millis() - startTime;
        while(deltaTime <= to){
            lis.getEvent(&event);
            if(event.acceleration.y <= b1){
               o = true;
            }else if(o && event.acceleration.y >= c1){
                Oeffnung = true;
            }
            
            deltaTime = millis() - startTime;
        }
        //detect Klappe   
    
    }else if (event.acceleration.z >= v1) {
        startTime = millis();
        deltaTime = millis() - startTime;
        while(deltaTime <= tk){
            lis.getEvent(&event);
            if(event.acceleration.z <= w1){
               k = true;
            }else if(k && event.acceleration.z >= x1){
                Klappe = true;
            }
            
            deltaTime = millis() - startTime;
        }
    }

    //send event to particle cloud when signal was detected
    if(Oeffnung){
        Particle.publish("oeffnung","Post Entnahme");
    }
    else if(Klappe){
        Particle.publish("klappe","Post Einwurf");
    }    
    
    //Ab hier Gewicht 
        float oner = scale.get_units(); 
        float scaler = round(oner)*10; 

  //scale.power_down();                    // put the ADC in sleep mode
  delay(3000);                          // Delay, damit nicht bei jeder Berührung der Post ein Event gesendet wird, sondern nur "Endgewicht" der Post
    //scale.power_up();
    
    
  if (Time.weekday() == 1){
      Wochentag = "Sonntag";
  } else if (Time.weekday() == 2){
      Wochentag = "Montag";
  } else if (Time.weekday() == 3){
      Wochentag = "Dienstag";
  } else if (Time.weekday() == 4){
      Wochentag = "Mittwoch";
  } else if (Time.weekday() == 5){
      Wochentag = "Donnerstag";
  } else if (Time.weekday() == 6){
      Wochentag = "Freitag";
  } else if (Time.weekday() == 7){
      Wochentag = "Samstag";
  }



// Gewicht
        if (scaler != weight){
            Particle.publish("Gramm",String(scaler));
            Particle.publish("Wochentag",String(Wochentag));
           // Datum = Time.format("%d %B %Y");
            Particle.publish("Datum",String(Time.format("%d %B %Y")));
                if (scaler != weight){
                    if (scalertemp == 0){
                        scalertemp = scaler;
                        //return scalertemp;
                    } else if (scalertemp != 0 && weight == 0){
                        scalertemp = scalertemp + scaler;
                        //return scalertemp;
                }
                    //Particle.publish("Bisheriges totales Togesgewicht",String(scalertemp));
                    
                }}
                
                   
        weight = scaler;
        scalertemp = 0;
        
         if (weight == 0){
                        Postmenge ="Aktuell keine Post vorhanden!";
                    } else if (weight <= 80 && weight!= 0){
                        Postmenge = "Aktuell wenig Post vorhanden!";
                    } else if (weight <= 300 && weight > 80){
                        Postmenge = "Aktuell mittelmässig Post vorhanden!";
                    } else if (weight >= 300 && weight <= 750){
                        Postmenge = "Aktuell viel Post vorhanden!";
                    } else if (weight > 750){
                        Postmenge = "Sehr viel Post, Briefkasten bitte dringend leeren!";
                    }
            /*
    if (scaler != weight && Oeffnung){
      Particle.publish("Öffnung","Brief Entnahme");
      Particle.publish("Aktuelle Post in Gramm",String(scaler));
    }
    else if (scaler != weight && Klappe){
      Particle.publish("Öffnung","Brief Einwurf");
      Particle.publish("Eingeworfene Post in Gramm",String(scaler));
    } 
      weight = scaler;
  } */


    

}


// Cloud functions must return int and take one String
//Unsere Funktion. Sendet bei erkanntem Öffnungs oder Klappe-Signal an cloud (lässt beides auf Website auslösen zum testen, obs funktiuoniert)
int remoteOeffnung(String arg) {
    Particle.publish("oeffnung", "Remote Post Entnahme");
    return 0;
}

int remoteKlappe(String arg) {
    Particle.publish("klappe", "Remote Post Einwurf");
    return 0;
}
