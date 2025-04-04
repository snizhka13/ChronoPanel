#include "functions.h"
#include "data.h"

void WiFiConnection() {
  Serial.println("Підключення до Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Підключено!");

  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org.ua");
  Serial.println("Час налаштовано через NTP!");
}


void Time() {
  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org.ua");
  if (!getLocalTime(&timeInfo)) {
    Serial.println("Не вдалося отримати час!");
    return;
  }

  char timeStr[6];
  strftime(timeStr, sizeof(timeStr), "%H:%M", &timeInfo);

  matrix.displayText(timeStr, PA_CENTER, 50, 1000, PA_PRINT, PA_NO_EFFECT);
  matrix.displayAnimate();
}


void checkPlayer() {
  if (player.available() && player.readType() == DFPlayerPlayFinished) {
    isPlaying = false;
  }
}

void enqueueTrack(int trackNumber) {
  if (queueSize < 10) {
    playQueue[queueSize++] = trackNumber;
    Serial.print("Трек додано в чергу: ");
    Serial.println(trackNumber);
  }
}

void processQueue() {
  if (!isPlaying && queueSize > 0) {
    player.play(playQueue[0]);
    isPlaying = true;
    for (int i = 1; i < queueSize; i++) {
      playQueue[i - 1] = playQueue[i];
    }
    queueSize--;
  }
}


void Alarm() {
  int newAlertStatus = getAlertStatus();
  static bool wasAlarmPlayed = false;
  static bool wasNotAlarmPlayed = false;
  if (newAlertStatus != alertStatus) {
    alertStatus = newAlertStatus;
    if (alertStatus == 1) {
      if (!wasAlarmPlayed) {
        enqueueTrack(1);
        wasAlarmPlayed = true;
        wasNotAlarmPlayed = false;
      }
    } else if (alertStatus == 0) {
      if (!wasNotAlarmPlayed) {
        enqueueTrack(2);
        wasNotAlarmPlayed = true;
        wasAlarmPlayed = false;
      }
    }
  }
  if (newAlertStatus == 1) {
    matrix.displayClear();
    matrix.displayText("Alarm!", PA_CENTER, 50, 1000, PA_PRINT, PA_NO_EFFECT);

    unsigned long startTime = millis();
    while (millis() - startTime < 3000) {
      matrix.displayAnimate();
    }
    matrix.displayClear();
  }
}

void DHT() {
  float temp = checkTemperature();
  if (temp == 0.0) return;
  const char* message;
  static bool wasOnPlayed = false;
  static bool wasOffPlayed = false;

  if (temp <= 20.0) {
    message = "ON";
    DHTMessage = true;

    if (!wasOnPlayed) {
      enqueueTrack(7);
      wasOnPlayed = true;
      wasOffPlayed = false;
    }
  } else if (temp >= 28.0) {
    message = "OFF";
    DHTMessage = true;

    if (!wasOffPlayed) {
      enqueueTrack(8);
      wasOffPlayed = true;
      wasOnPlayed = false;
    }
  }

  if (DHTMessage) {
    Serial.println(message);
    matrix.displayClear();
    matrix.displayText(message, PA_CENTER, 50, 1000, PA_PRINT, PA_NO_EFFECT);

    unsigned long startTime = millis();
    while (millis() - startTime < 3000) {
      matrix.displayAnimate();
    }

    matrix.displayClear();
    DHTMessage = false;
  }
}

void MQ7() {
  int air = checkAir();
  if (air == 0) return;
  const char* message;
  static bool wasOpenPlayed = false;
  Serial.println(air);
  if (air >= 3000) {
    message = "OPEN";
    MQ7Message = true;
    if (!wasOpenPlayed) {
      enqueueTrack(5);
      wasOpenPlayed = true;
    }
    Serial.println(air);
    Serial.println(message);
    matrix.displayClear();
    matrix.displayText(message, PA_CENTER, 50, 1000, PA_PRINT, PA_NO_EFFECT);

    unsigned long startTime = millis();
    while (millis() - startTime < 3000) {
      matrix.displayAnimate();
    }

    matrix.displayClear();
    MQ7Message = false;
  }
}

void Breaks() {
  bool isBreakTime = false;
  const char* message;
  for (int i = 0; i < numBreaks; i++) {
    if (timeInfo.tm_hour == breaks[i].startHour && timeInfo.tm_min == breaks[i].startMinute) {
      message = "START";
      isBreakTime = true;
      enqueueTrack(3);
    } else if (timeInfo.tm_hour == breaks[i].endHour && timeInfo.tm_min == breaks[i].endMinute) {
      message = "FINISH";
      isBreakTime = true;
      if (!isPlaying) {
        player.play(4);
        checkPlayer();
        isPlaying = true;
      }
    }
  }
  if (isBreakTime) {
    Serial.println(message);
    matrix.displayClear();
    matrix.displayText(message, PA_CENTER, 50, 1000, PA_PRINT, PA_NO_EFFECT);

    unsigned long startTime = millis();
    while (millis() - startTime < 3000) {
      matrix.displayAnimate();
    }

    matrix.displayClear();
    isBreakTime = false;
  }
}

void Messages() {
  if (millis() - lastMessageTime >= messageInterval) {
    lastMessageTime = millis();
    message = true;
    int randomIndex = random(0, sizeof(messages) / sizeof(messages[0]));
    matrix.displayText(messages[randomIndex], PA_CENTER, 50, 1000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    while (!matrix.displayAnimate())
      ;
    matrix.displayClear();
    message = false;
  }
}