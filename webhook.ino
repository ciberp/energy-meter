
    void http_webhook1_client_loop() {
      if (SETTINGS.webhook1_enabled) {
        if (millis() - previousWebhook1Interval >= SETTINGS.webhook1_interval * 1000) {
          previousWebhook1Interval = millis();
          yield();
          HTTPClient http;
          http.begin(SETTINGS.webhook1_url);
          http.addHeader("Content-Type", "application/json");
          int httpResponseCode = http.POST(generate_json());
          if (httpResponseCode != 200 && httpResponseCode != 201 && httpResponseCode != 204 ) { 
            SendLog("ERROR: webhook1 http error:" + String(httpResponseCode));
          }
          http.end();
        }
      }
    }

    void http_webhook2_client_loop() {
      if (SETTINGS.webhook2_enabled) {
        if (millis() - previousWebhook2Interval >= SETTINGS.webhook2_interval * 1000) {
          previousWebhook2Interval = millis();
          yield();
          HTTPClient http;
          http.begin(SETTINGS.webhook2_url);
          http.addHeader("Content-Type", "application/json");
          int httpResponseCode = http.POST(generate_json());
          if (httpResponseCode != 200 && httpResponseCode != 201 && httpResponseCode != 204 ) { 
            SendLog("ERROR: webhook2 http error:" + String(httpResponseCode));
          }
          http.end();
        }
      }
    }
