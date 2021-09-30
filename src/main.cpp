#include <Arduino.h>

#define TRIGGER_ABOVE 2
#define ECHO_ABOVE 3

#define ECHO_BELOW 4
#define TRIGGER_BELOW 5

#define MAX_ARRAY_SIZE 200

#define SENSITIVITY 1

const String SENSOR_NAMES[2] = {"ABOVE", "BELOW"};
float distances_top[MAX_ARRAY_SIZE];
float distances_bottom[MAX_ARRAY_SIZE];
int last_index_t = 0;
int last_index_b = 0;
long time_start = 0;
long time_end = 0;

float get_distance(String sensor_name)
{
  if (sensor_name == SENSOR_NAMES[0])
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

boolean pass_detection(String sensor_name)
{
  // TODO: Different distances arrays for different sensors
  //Serial.println("Checking for pass");

  if (sensor_name == SENSOR_NAMES[0])
  {
    if (last_index_t == MAX_ARRAY_SIZE)
    {
      last_index_t = 0;
    }
    distances_top[last_index_t++] = get_distance(sensor_name);

    double sum = 0;
    short index = MAX_ARRAY_SIZE;
    for (int i = 0; i < MAX_ARRAY_SIZE; i++)
    {
      sum += distances_top[i];
      if (distances_top[i] == 0 && distances_top[i + 1] == 0)
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
    Serial.println("Average: ");
    Serial.println(average);
    Serial.print("Distance: ");
    Serial.println(distances_top[index - 1]);
    */

    if (average + SENSITIVITY < distances_top[index - 1] || average - SENSITIVITY > distances_top[index - 1])
    {
      return true;
    }
  }
  else if (sensor_name == SENSOR_NAMES[1])
  {
    if (last_index_b == MAX_ARRAY_SIZE)
    {
      last_index_b = 0;
    }
    distances_bottom[last_index_b++] = get_distance(sensor_name);

    double sum = 0;
    short index = MAX_ARRAY_SIZE;
    for (int i = 0; i < MAX_ARRAY_SIZE; i++)
    {
      sum += distances_bottom[i];
      //Serial.println(distances_bottom[i]);
      if (distances_bottom[i] == 0 && distances_bottom[i + 1] == 0)
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
    Serial.print("Distance: ");
    Serial.println(distances_bottom[index - 1]);
    */

    if (average + SENSITIVITY < distances_bottom[index - 1] || average - SENSITIVITY > distances_bottom[index - 1])
    {
      return true;
    }
  }
  return false;
}

long get_time_difference(long timestamp_1, long timestamp_2)
{
  return timestamp_1 - timestamp_2;
  // TODO: implement math to calculate time (in milliseconds) difference
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

void loop()
{
  /*
  if (pass_detection(SENSOR_NAMES[0]))
  {
    time_start = millis();
    Serial.println("Pass detected 1");
  }
  if (pass_detection(SENSOR_NAMES[1]))
  {
    time_end = millis();
    Serial.println("Pass detected 2");
  }
  if (time_start != 0 && time_end != 0)
  {
    Serial.print("Time difference: ");
    Serial.println(get_time_difference(time_end, time_start));
  }
  */

  Serial.print("TOP: ");
  Serial.println(pass_detection(SENSOR_NAMES[0]));
  Serial.print("BOTTOM: ");
  Serial.println(pass_detection(SENSOR_NAMES[1]));
  delay(500);
}