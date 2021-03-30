// This #include statement was automatically added by the Particle IDE.
#include <BH1750Lib.h>

// Initialise the light sensor.
BH1750Lib lightSensor;

// The value that we will consider full sunlight
int sunlight =  10000;
int lastThreeValues[3];
int nextIndex = 0;
bool wasDark = true;

// IFTTT event name
String event = "sunlight";

// IFTTT recipe data
String eventDataSunlight = "light";
String eventDataDarkness = "dark";

// A reading is taken every 5 s. Remember it's in milliseconds.
int readingTime = 5000;

void setup(){
  Serial.begin(9600);
  lightSensor.begin(BH1750LIB_MODE_CONTINUOUSHIGHRES);
}

void loop() {
    // Take the reading
    int lux = lightSensor.lightLevel();
    // Print to serial so you can keep track
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");
    Serial.print(Time.hourFormat12());
    Serial.print(":");
    Serial.print(Time.minute());
    Serial.print(":");
    Serial.print(Time.second());
    
    // The last three values are averaged so a message isn't sent due to a flash of light
    lastThreeValues[nextIndex] = lux;
    nextIndex++;
    
    if (nextIndex > 2) {
        nextIndex = 0;
        if (isSunlight(lux) && wasDark) {
            wasDark = false;
            // Send notification that sunlight is on
            Particle.publish(event, eventDataSunlight);
        } else if (!isSunlight(lux) && !wasDark) {
            wasDark = true;
            // Send notification that sunlight is off
            Particle.publish(event, eventDataDarkness);
        }
    }
        
    //Take a reading every 5 seconds
    delay(readingTime);
}

// Function to return true if the sensor is sensing light above 5000 lx
bool isSunlight(int lux) {
    return lux >= sunlight;
}
