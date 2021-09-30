#include <Arduino.h>

#define TRIGGER_ABOVE 2
#define ECHO_ABOVE 3

#define ECHO_BELOW 4
#define TRIGGER_BELOW 5

#define MAX_ARRAY_SIZE 200

#define SENSITIVITY 1.5

#define SENSOR_TOP true
#define SENSOR_BOTTOM false

float distances_top[MAX_ARRAY_SIZE];
float distances_bottom[MAX_ARRAY_SIZE];
int last_index_t = 0;
int last_index_b = 0;
long time_start = 0;
long time_end = 0;

float get_distance(bool sensor)
{
  if (sensor == SENSOR_TOP)
  {
    digitalWrite(TRIGGER_ABOVE, LOW);
    delay(5);
    digitalWrite(TRIGGER_ABOVE, HIGH);
    delay(10);
    digitalWrite(TRIGGER_ABOVE, LOW);
    long dauer = pulseIn(ECHO_ABOVE, HIGH);
    float distance = (dauer / 2) * 0.03432;
    return distance;
  }
  else
  {
    digitalWrite(TRIGGER_BELOW, LOW);
    delay(5);
    digitalWrite(TRIGGER_BELOW, HIGH);
    delay(10);
    digitalWrite(TRIGGER_BELOW, LOW);
    long dauer = pulseIn(ECHO_BELOW, HIGH);
    float distance_b = (dauer / 2) * 0.03432;
    return distance_b;
  }
}

boolean pass_detection(bool sensor)
{
  // TODO: Different distances arrays for different sensors
  //Serial.println("Checking for pass");
  float(&distances)[MAX_ARRAY_SIZE] = (sensor == SENSOR_TOP) ? distances_top : distances_bottom;
  int &last_index = (sensor == SENSOR_TOP) ? last_index_t : last_index_b;

  if (last_index == MAX_ARRAY_SIZE)
  {
    last_index = 0;
  }

  /*
  Serial.print("Last index: ");
  Serial.println(last_index);

  Serial.print("Distance: ");
  Serial.println(get_distance(sensor));
  */

  distances[last_index++] = get_distance(sensor);

  double sum = 0;
  short index = MAX_ARRAY_SIZE;
  for (int i = 0; i < MAX_ARRAY_SIZE; i++)
  {
    sum += distances[i];
    if (distances[i] == 0 && distances[i + 1] == 0)
    {
      index = i;
      break;
    }
  }

  /*
  Serial.print("Sum: ");
  Serial.println(sum);
  Serial.print("Index: ");
  Serial.println(index);
  */

  float average = sum / index;

  /*
  Serial.print("Average: ");
  Serial.println(average);
  */

  if (average + SENSITIVITY < distances[index - 1] || average - SENSITIVITY > distances[index - 1])
  {
    return true;
  }

  return false;
}

long get_time_difference(long timestamp_1, long timestamp_2)
{
  return timestamp_1 - timestamp_2;
}

void setup()
{
  // clears the array
  for (int i = 0; i < MAX_ARRAY_SIZE; i++)
  {
    distances_top[i] = 0;
    distances_bottom[i] = 0;
  }

  Serial.begin(9600);
  pinMode(TRIGGER_ABOVE, OUTPUT); // Trigger-Pin ist ein Ausgang
  pinMode(ECHO_ABOVE, INPUT);     // Echo-Pin ist ein Eingang
  pinMode(TRIGGER_BELOW, OUTPUT); // Trigger-Pin ist ein Ausgang
  pinMode(ECHO_BELOW, INPUT);     // Echo-Pin ist ein Eingang
}

void measure()
{
  if (pass_detection(SENSOR_TOP) && time_start == 0)
  {
    time_start = millis();
    Serial.println("Pass detected 1");
  }
  else if (pass_detection(SENSOR_BOTTOM) && time_start != 0)
  {
    time_end = millis();
    Serial.println("Pass detected 2");
    Serial.print("Time difference: ");
    Serial.println(get_time_difference(time_end, time_start));
    delay(5000);
    time_start = 0;
    time_end = 0;
  }
}

void debug()
{
  Serial.println("Sensor 1:");
  Serial.println(pass_detection(SENSOR_TOP));
  Serial.println("Sensor 2:");
  Serial.println(pass_detection(SENSOR_BOTTOM));
  delay(2500);
}

void loop()
{
  measure();
}