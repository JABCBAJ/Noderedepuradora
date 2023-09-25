const long InternalReferenceVoltage = 1137;  // pro mini


int Vcc_Read() {
  digitalWrite(25, HIGH);
  analogReadResolution(12); // Set ADC resolution to 12 bits
  float adcValue = analogRead(25); // Use ADC pin 35 (GPIO 35) for ESP32's internal voltage reference

  // Calculate the battery voltage using the known internal reference voltage (1100 mV for ESP32)
  static float batteryVoltage = (InternalReferenceVoltage * 4096) / adcValue;

  return (int)batteryVoltage;
}