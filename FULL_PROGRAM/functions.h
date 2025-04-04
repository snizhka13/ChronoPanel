int getAlertStatus() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String fullUrl = String(apiUrl) + "?token=" + token;
    http.begin(fullUrl);
    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
      String payload = http.getString();
      Serial.println("Отримані дані:");
      Serial.println(payload);

      DynamicJsonDocument doc(4096);
      auto error = deserializeJson(doc, payload);
      if (error) {
        Serial.print("Помилка розбору JSON: ");
        Serial.println(error.c_str());
        return -1;
      }

      return (payload == "\"A\"") ? 1 : 0;
    } else {
      Serial.printf("Помилка HTTP: %d\n", httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Немає з'єднання з Wi-Fi.");
  }
  return -1;
}

float checkTemperature() {
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  float temperature = data.temperature;

  if (isnan(temperature)) {
    Serial.println("Помилка зчитування даних з DHT11!");
    return 0.0;
  }

  Serial.printf("Температура: %.1f°C  ->", temperature);

  return temperature;
}

int checkAir(){
   int value = analogRead(MQ7_PIN);  
  return value;
}