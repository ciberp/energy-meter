

    String generate_json() {
      String json;
      json += "{";
      json += "\"pulse\": \"" + String(PulseCounter) + "\", ";
      json += "\"analog\": \"" + String(analogRead(A0)) + "\", ";
      json += "\"analogavg\": \"" + String(AvgAnalogValue) + "\", ";
      json += "\"analogmax\": \"" + String(MaxAnalogValue) + "\", ";
      json += "\"energy\": \"" + String(Wh) + "\", ";
      json += "\"energydaily\": \"" + String(avg_wh_day) + "\", ";
      json += "\"energycalcmonth\": \"" + String(calculate_k_M(avg_wh_day * numberOfDaysInMonth( year(), month()))) + "\", ";
      json += "\"power\": \"" + String(Watts) + "\", ";
      json += "\"powermax\": \"" + String(WattsMax) + "\", ";
      json += "\"poweravg\": \"" + String(avg_watts) + "\", ";
      json += "\"priceday\": \"" + String(avg_price_day) + "\", ";
      json += "\"price\": \"" + String(avg_price) + "\", ";
      json += "\"PulseError\": \"" + String(PulseCounterErrorState) + "\", "; 
      json += "\"wifi\": \"" + String(WiFi.RSSI()) + "\", ";
      json += "\"uptime\": \"" + String(uptime) + "\", ";
      json += "\"freeheap\": \"" + String(ESP.getFreeHeap()) + "\"";
      json += "}";
      return json;
    }

    void handle_json() {
      server.send(200, "application/json", generate_json());
      yield();
    } 


    String dump_settings_json() {
      //StaticJsonBuffer<1200> jsonBuffer;
      const size_t bufferSize = JSON_ARRAY_SIZE(2) + 2*JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3);
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.createObject();
      root["ssid"] = SETTINGS.ssid;
      root["password"] = SETTINGS.password;
      root["ap_ssid"] = SETTINGS.ap_ssid;
      root["ap_password"] = SETTINGS.ap_password; 
      root["web_user"] = SETTINGS.web_user;
      root["web_pass"] = SETTINGS.web_pass;
      root["css_link"] = SETTINGS.css_link;
      root["syslog"] = SETTINGS.syslog;
      root["NTP_hostname"] = SETTINGS.NTP_hostname;
      root["timezone_GMT"] = SETTINGS.timezone_GMT;
      root["hostname"] = SETTINGS.hostname;
      root["mqtt_enabled"] = SETTINGS.mqtt_enabled;
      root["mqtt_server"] = SETTINGS.mqtt_server;
      root["mqtt_server"] = SETTINGS.mqtt_server;
      root["mqtt_port"] = SETTINGS.mqtt_port;
      root["mqtt_interval"] = SETTINGS.mqtt_interval;
      root["mqtt_key"] = SETTINGS.mqtt_key;
      root["mqtt_secret"] = SETTINGS.mqtt_secret;
      root["data_collector_num_configured"] = SETTINGS.data_collector_num_configured;
      JsonArray& data_coll = root.createNestedArray("data_collector");
      for(int n = 0; n < SETTINGS.data_collector_num_configured; n++) {
        JsonObject& data_collector = data_coll.createNestedObject();
        data_collector["id"] = n;
        data_collector["host"] = SETTINGS.data_collector_host[n];
        data_collector["port"] = SETTINGS.data_collector_port[n];
        data_collector["db_name"] = SETTINGS.data_collector_db_name[n];
        data_collector["db_username"] = SETTINGS.data_collector_db_username[n];
        data_collector["db_password"] = SETTINGS.data_collector_db_password[n];
        data_collector["push_interval"] = SETTINGS.data_collector_push_interval[n];
        data_collector["type"] = SETTINGS.data_collector_type[n];
      }
      root["pulse_start_value"] = SETTINGS.pulse_start_value;
      root["calc_pulse"] = SETTINGS.calc_pulse;
      root["calc_kwh"] = SETTINGS.calc_kwh;
      root["price_kwh"] = SETTINGS.price_kwh;
      root["reset_day"] = SETTINGS.reset_day;
      root["reset_hour"] = SETTINGS.reset_hour;
      root["reset_enabled"] = SETTINGS.reset_enabled;
      JsonArray& data_energy = root.createNestedArray("energy");
      for (int i = 0; i <= 12; ++i) {
        if (i != month()) {
          if (Wh_year[i] != 0) {
            JsonObject& energy = data_energy.createNestedObject();
            energy["avg_power_" + Number_to_month_name(i) + "_" + year_stamp[i]] = calculate_k_M(WattsAVG_year[i]) + "W";
            energy["energy_" + Number_to_month_name(i) + "_" + year_stamp[i]] = calculate_k_M(Wh_year[i]) + "Wh";
            energy["energy/day_" + Number_to_month_name(i) + "_" + year_stamp[i]] = calculate_k_M(WhDayAVG_year[i]) + "Wh/day";
          }
        }
      }
      root["webhook1_url"] = SETTINGS.webhook1_url;
      root["webhook1_interval"] = SETTINGS.webhook1_interval;
      root["webhook1_enabled"] = SETTINGS.webhook1_enabled;
      root["webhook2_url"] = SETTINGS.webhook2_url;
      root["webhook2_url"] = SETTINGS.webhook2_url;
      root["webhook2_interval"] = SETTINGS.webhook2_interval;
      root["webhook2_enabled"] = SETTINGS.webhook2_enabled;
      root["version"] = ESP.getFullVersion();
      root["compiled"] = Build;
      root["chipid"] = ESP.getChipId();
      String json_output;
      root.prettyPrintTo(json_output);
      return json_output;
    }

    void http_dump_settings_json() {
      if (!server.authenticate(SETTINGS.web_user, SETTINGS.web_pass)) {
          return server.requestAuthentication();
      }
      server.send(200, "application/json", dump_settings_json());
    }
