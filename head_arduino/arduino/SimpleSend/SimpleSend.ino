// Enable transmission from device
void setTX()
{
  digitalWrite(7, HIGH);
}

// Enable receive from master
void setRX()
{
  digitalWrite(7, LOW);
}

void setup() {
  // initialize both serial ports:
  Serial.begin(9600);
  Serial1.begin(9600);
  setTX();
}

void loop() {
  Serial1.println("test_rs485");
  delay(100);
}
