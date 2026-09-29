
#define PIN_LED  9
#define PIN_TRIG 12
#define PIN_ECHO 13

#define SND_VEL 346.0
#define INTERVAL 25       // 25ms로 변경
#define PULSE_DURATION 10
#define _DIST_MIN 100.0
#define _DIST_MAX 300.0

#define TIMEOUT ((INTERVAL / 2) * 1000.0)
#define SCALE (0.001 * 0.5 * SND_VEL)

unsigned long last_sampling_time;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  digitalWrite(PIN_TRIG, LOW);

  Serial.begin(57600);
}

void loop() {
  float distance;

  // 25ms마다 측정
  if (millis() < (last_sampling_time + INTERVAL))
    return;

  distance = USS_measure(PIN_TRIG, PIN_ECHO);

  int led_value;

  // 100mm 이하 또는 300mm 이상 
  if (distance <= 100.0 || distance >= 300.0 || distance == 0.0) {
    led_value = 255;
  }

  // 100mm ~ 200mm
  else if (distance <= 200.0) {
    led_value = 255 - (int)((distance - 100.0) * 255.0 / 100.0);
  }

  // 200mm ~ 300mm
  else {
    led_value = (int)((distance - 200.0) * 255.0 / 100.0);
  }

  // LED 밝기 적용
  analogWrite(PIN_LED, led_value);

  // 시리얼 플로터 출력
  Serial.print("Min:");
  Serial.print(_DIST_MIN);

  Serial.print(",distance:");
  Serial.print(distance);

  Serial.print(",Max:");
  Serial.println(_DIST_MAX);


  last_sampling_time += INTERVAL;
}


// 초음파 거리 측정
float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);

  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE;
}