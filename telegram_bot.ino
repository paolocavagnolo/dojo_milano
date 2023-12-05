#define USE_CLIENTSSL true
#define DATA_OUT 19

#include <AsyncTelegram2.h>

// Timezone definition
#include <time.h>
#define MYTZ "CET-1CEST,M3.5.0,M10.5.0/3"

#ifdef ESP8266
#include <ESP8266WiFi.h>
BearSSL::WiFiClientSecure client;
BearSSL::Session session;
BearSSL::X509List certificate(telegram_cert);

#elif defined(ESP32)
#include <WiFi.h>
#include <WiFiClient.h>
#if USE_CLIENTSSL
#include <SSLClient.h>
#include "tg_certificate.h"
WiFiClient base_client;
SSLClient client(base_client, TAs, (size_t)TAs_NUM, A0, 1, SSLClient::SSL_ERROR);
#else
#include <WiFiClientSecure.h>
WiFiClientSecure client;
#endif
#endif

AsyncTelegram2 myBot(client);

const char* ssid = "****";                                      // SSID WiFi network
const char* pass = "****";                                        // Password  WiFi network
const char* token = "****";     // Telegram token

int64_t groupId = -4073758034;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(DATA_OUT, OUTPUT);

  Serial.begin(115200);
  Serial.println("\nStarting TelegramBot...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  delay(500);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }

#ifdef ESP8266
  // Sync time with NTP, to check properly Telegram certificate
  configTime(MYTZ, "time.google.com", "time.windows.com", "pool.ntp.org");
  //Set certficate, session and some other base client properies
  client.setSession(&session);
  client.setTrustAnchors(&certificate);
  client.setBufferSizes(1024, 1024);
#elif defined(ESP32)
  // Sync time with NTP
  configTzTime(MYTZ, "time.google.com", "time.windows.com", "pool.ntp.org");
#if USE_CLIENTSSL == false
  client.setCACert(telegram_cert);
#endif
#endif

  // Set the Telegram bot properies
  myBot.setUpdateTime(2000);
  myBot.setTelegramToken(token);

  // Check if all things are ok
  Serial.print("\nTest Telegram connection... ");
  myBot.begin() ? Serial.println("OK") : Serial.println("NOK");
}

unsigned long tOut = 0;
bool sOut = false;

void loop() {

  TBMessage msg;

  if (myBot.getNewMessage(msg)) {
    if (msg.chatId == groupId) {
      if (msg.text == "/apri") {

        digitalWrite(DATA_OUT, HIGH);
        digitalWrite(LED_BUILTIN, HIGH);

        sOut = true;
        tOut = millis();

        String message;
        message = "ok";
        myBot.sendMessage(msg, message);
      }
    }
  }

  if (sOut) {
    if ((millis() - tOut) > 2000) {
      digitalWrite(DATA_OUT, LOW);
      digitalWrite(LED_BUILTIN, LOW);
      sOut = false;
    }
  }
}
