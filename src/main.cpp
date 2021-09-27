#include <Arduino.h>

#define TRIGGER_ABOVE 2
#define ECHO_ABOVE 3

#define ECHO_BELOW 4
#define TRIGGER_BELOW 5

#define MAX_ARRAY_SIZE 255

const String SENSOR_NAMES[2] = {"ABOVE", "BELOW"};
long distances[MAX_ARRAY_SIZE];
int last_index = 0;
long time_start; // TODO: implement math to calculate time difference
long time_end;

long get_distance(String sensor_name)
{
  if (sensor_name == SENSOR_NAMES[0])
  {
    digitalWrite(TRIGGER_ABOVE, LOW);
    delay(5);
    digitalWrite(TRIGGER_ABOVE, HIGH);
    delay(10);
    digitalWrite(TRIGGER_ABOVE, LOW);
    long dauer = pulseIn(ECHO_ABOVE, HIGH);
    long distance = (dauer / 2) * 0.03432;
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
    long distance = (dauer / 2) * 0.03432;
    return distance;
  }
}

boolean pass_detection(String sensor_name)
{
  // TODO: Different distances arrays for different sensors
  distances[last_index++] = get_distance(sensor_name);

  long sum = 0;
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
  float average = sum / index;

  Serial.println(average);
  Serial.println(distances[index - 1]);

  if (average + 5 < distances[index - 1] || average - 5 > distances[index - 1])
  {
    return true;
  }

  return false;
}

void setup()
{
  // clears the array
  for (int i = 0; i < MAX_ARRAY_SIZE; i++)
  {
    distances[i] = 0;
  }

  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(TRIGGER_ABOVE, OUTPUT); // Trigger-Pin ist ein Ausgang
  pinMode(ECHO_ABOVE, INPUT);     // Echo-Pin ist ein Eingang
  pinMode(TRIGGER_BELOW, OUTPUT); // Trigger-Pin ist ein Ausgang
  pinMode(ECHO_BELOW, INPUT);     // Echo-Pin ist ein Eingang
}

void loop()
{
  delay(2000);
  Serial.println("-------------------------------");
  Serial.println(pass_detection(SENSOR_NAMES[0]));
  // put your main code here, to run repeatedly:
}