const byte motorPower = 4;          // A motor is used to pump the water. Could be done using a valve too.

int setMoistValue=350;              // Value below which the soil is moist.

bool moist= false;                  // Checks if the plant is moist.
bool plantCheck=true;               // Checks if the sensor is placed where the watering is being done.

class MoistureSensor                // To create sensors.
{
  int reading = 0;
  int analogPin=0;
  public:
    MoistureSensor(int ap)
   {
    pinMode(ap, INPUT);
    analogPin= ap;
   }
  
  void read_sensor()                // Method to read the sensor value.
    {
       reading = analogRead(analogPin);
       
       if (reading<=0)
       {
         Serial.println("Please connect the moisture sensor properly.");
       }

    }
};

MoistureSensor sensor(A0);           //Creating a sensor.




//initial - 642
//water - >350



void setup() {
  
  pinMode(motorPower, OUTPUT);
  Serial.begin(9600);
  delay(500);

}

void loop() {
  sensor.read_sensor();
  water(10000);                      //Run the water pump for 10 seconds and checks for change.
  delay(30000);                      //Checks for moisture level every 30 seconds.
}



 void water (int t)
{
  int initialMoistureReading= sensor.reading;
  const int valueOffset=30;          //Accounts for the inconsistency in the analog value.

  Serial.print("Water function initiated with initial moiture reading as: ");
  Serial.println(initialMoistureReading);
  
  while (plantCheck)
  { 
    
    if (sensor.reading <= setMoistValue) 
    {
      moist = true;
      break;
    }
    
    else
    { 
      moist = false;
    }
  
    digitalWrite(motorPower, HIGH);
    delay(t);
    digitalWrite(motorPower, LOW);
    delay(15000);
    sensor.read_sensor();
    
    if (((initialMoistureReading + valueOffset) <= sensor.reading)|| ((initialMoistureReading - valueOffset) <= sensor.reading)) //If the mositure value doesn't change after 10 seconds of continuous watering it's safe to assume that the sensor isn't placed properly.
    {
      plantCheck=false;
      Serial.println("Please check the water level and if the moisture sensor is properly placed in the soil bed");
    }
  }

  if(moist)
  {
    Serial.print("Water function worked properly and the moisture reading of the soil now is: ");
    Serial.println(sensor.reading);
  }
  else
  {
    Serial.print("Water function did not work properly and the moisture reading of the soil now is: ");
    Serial.println(sensor.reading);
  }
  
}
