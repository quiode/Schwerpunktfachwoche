#include <Arduino.h>

// declaring constants
#define TRIGGER_ABOVE 2
#define ECHO_ABOVE 3

#define ECHO_BELOW 4
#define TRIGGER_BELOW 5

#define MAX_ARRAY_SIZE 200

#define SENSITIVITY 1

#define SENSOR_TOP true
#define SENSOR_BOTTOM false

// declaring global variables
float distances_top[MAX_ARRAY_SIZE];
float distances_bottom[MAX_ARRAY_SIZE];
int last_index_t = 0;
int last_index_b = 0;
long time_start = 0;
long time_end = 0;

// get's the distance from the sensor
float get_distance(bool sensor)
{
  long duration;
  // targets different output pins depending on chosen sensor
  if (sensor == SENSOR_TOP)
  {
    // sets the sensor to low
    digitalWrite(TRIGGER_ABOVE, LOW);
    // waits a few seconds to make sure sensor is low
    delay(5);
    // send a ping signal
    digitalWrite(TRIGGER_ABOVE, HIGH);
    // send the singal 10 seconds long
    delay(10);
    // end transmittion
    digitalWrite(TRIGGER_ABOVE, LOW);
    // gets the time until the pulse reaches the sensor
    duration = pulseIn(ECHO_ABOVE, HIGH);
  }
  else
  {
    // sets the sensor to low
    digitalWrite(TRIGGER_BELOW, LOW);
    // waits a few seconds to make sure sensor is low
    delay(5);
    // send a ping signal
    digitalWrite(TRIGGER_BELOW, HIGH);
    // send the singal 10 seconds long
    delay(10);
    // end transmittion
    digitalWrite(TRIGGER_BELOW, LOW);
    // gets the time until the pulse reaches the sensor
    duration = pulseIn(ECHO_BELOW, HIGH);
  }
  // calculates the distance pased on the ping duratio
  float distance = (duration / 2) * 0.03432;
  return distance;
}

// functions that return true when an object has passed the sensor
boolean pass_detection(bool sensor)
{
  // makes a reference to the data of the last distances the selected sensor measured
  float(&distances)[MAX_ARRAY_SIZE] = (sensor == SENSOR_TOP) ? distances_top : distances_bottom;
  // makes a reference to the last written to index (if it is above the array size, it begins to overrite the oldest measurements)
  int &last_index = (sensor == SENSOR_TOP) ? last_index_t : last_index_b;

  // checks if the index has reached the highest level, if so, reset it
  // because of that the array will be rewritten every MAX_ARRAY_SIZE measurements
  if (last_index == MAX_ARRAY_SIZE)
  {
    last_index = 0;
  }

  // get the new distance
  float current_distance = get_distance(sensor);

  // calculates the average, and therefore the sum
  double sum = 0;
  short index = MAX_ARRAY_SIZE;
  for (int i = 0; i < MAX_ARRAY_SIZE; i++)
  {
    sum += distances[i];
    // if the next data is zero (which means that there is no more data above it, stop adding)
    if (distances[i] == 0 && distances[i + 1] == 0)
    {
      index = i;
      break;
    }
  }

  // calculate the average based on the sum and the amount of data
  float average = sum / index;

  // write the current distance to the data
  distances[last_index++] = current_distance;

  // check if the current distance is greater or smaller than the allowed amount, if so, the object has passed, if not, no object has passed
  if (average + SENSITIVITY < current_distance || average - SENSITIVITY > current_distance)
  {
    return true;
  }

  return false;
}

// function to get the time difference between to timestamps
long get_time_difference(long timestamp_1, long timestamp_2)
{
  return timestamp_1 - timestamp_2;
}

// function that starts the measures
void measure()
{
  // if an object has passed the top sensor and the sensor wasn't already reached...
  if (pass_detection(SENSOR_TOP) && time_start == 0)
  {
    // record time of passing
    time_start = millis();
    // send message to user
    Serial.println("Pass detected 1");
  }
  // if the second sensor was reached and also the first...
  else if (pass_detection(SENSOR_BOTTOM) && time_start != 0)
  {
    // record time of passing
    time_end = millis();
    // send message to user
    Serial.println("Pass detected 2");
    // output the time it needed to pass both
    Serial.print("Time difference: ");
    Serial.println(get_time_difference(time_end, time_start));
    // reset after specified amount
    delay(5000);
    time_start = 0;
    time_end = 0;
  }
}

// function for debugging, if there is a problem
void debug()
{
  Serial.println("Sensor 1:");
  Serial.println(pass_detection(SENSOR_TOP));
  Serial.println("Sensor 2:");
  Serial.println(pass_detection(SENSOR_BOTTOM));
  delay(2500);
}

// function that gets called once at the beginning
void setup()
{
  // clears the arrays
  for (int i = 0; i < MAX_ARRAY_SIZE; i++)
  {
    distances_top[i] = 0;
    distances_bottom[i] = 0;
  }

  // begins transmitting data via usb
  Serial.begin(9600);
  // registers the pins
  pinMode(TRIGGER_ABOVE, OUTPUT); // Trigger-Pin ist ein Ausgang
  pinMode(ECHO_ABOVE, INPUT);     // Echo-Pin ist ein Eingang
  pinMode(TRIGGER_BELOW, OUTPUT); // Trigger-Pin ist ein Ausgang
  pinMode(ECHO_BELOW, INPUT);     // Echo-Pin ist ein Eingang

  // let's the user know the setup has finished
  Serial.println("First round");
}

// function that gets called indefinetly
void loop()
{
  // measure
  measure();
}