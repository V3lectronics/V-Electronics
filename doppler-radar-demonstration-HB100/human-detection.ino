/*
  HB100 Doppler effect radar demonstration used with this
  amplifying/filtering circuit: 
  https://theorycircuit.com/wp-content/uploads/2016/09/hb100-doppler-sensor-arduino-sketch.png

  code by V Electronics JGW
  https://www.youtube.com/@V_Electronics
*/

void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(A5);
  float voltage = sensorValue * (5.0 / 1023.0);
  //Serial.println(voltage);

  if (voltage > 3.8) {
    Serial.println("human");
  }

  delay(500);
}
