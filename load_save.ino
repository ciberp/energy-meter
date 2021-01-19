    void saveConfig() {
      SETTINGS.defaults = LoadDefaultNumber;
      EEPROM.begin(EEPROM_SIZE);
      delay(10);
      EEPROM.put(EEPROM_START, SETTINGS);
      yield();
      EEPROM.commit();
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);
      Serial.println("Settings saved!");
      SendLog("INFO: Settings saved.");
    }
    
    void loadConfig() {
      EEPROM.begin(EEPROM_SIZE);
      delay(10);
      EEPROM.get(EEPROM_START, SETTINGS);
      yield();
      if (SETTINGS.defaults == LoadDefaultNumber) {
        Set_Syslog_From_Settings();
        SendLog("INFO: Settings loaded.");
        // Check for invalid settings
        if (SETTINGS.data_collector_num_configured > data_collector_Max_Clients && SETTINGS.data_collector_num_configured < 0) {
          SETTINGS.data_collector_num_configured = 0;
        }
        if (SETTINGS.pulse_start_value <= 0) {
          SETTINGS.pulse_start_value = 65;
        }
        if (SETTINGS.pulse_start_value <= 0) {
          SETTINGS.pulse_avg_perc_val = 18;
        }
        if (SETTINGS.calc_pulse <= 0) {
          SETTINGS.calc_pulse = 3;
        }
        if (SETTINGS.calc_kwh <= 0) {
          SETTINGS.calc_kwh = 25;
        }
        if (SETTINGS.reset_day < 0) {
          SETTINGS.reset_day = 1;
        }
        if (SETTINGS.reset_hour < 0) {
          SETTINGS.reset_hour = 0;
        }
        if (SETTINGS.price_eur < 0) {
          SETTINGS.price_eur = 29;
        }
        if (SETTINGS.price_kwh < 0) {
          SETTINGS.price_kwh = 5;
        }
      }
      else {
        handle_load_defaults();
      }
    }
    
    void http_handle_load_defaults() {
      if (!server.authenticate(SETTINGS.web_user, SETTINGS.web_pass)) {
          return server.requestAuthentication();
      }
      handle_load_defaults();
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);
    }
     
    void handle_load_defaults() {
      // wifi
      strcpy (SETTINGS.ssid, "ssid"); //ne potrebujemo imamo dropdown
      strcpy (SETTINGS.password, "password");
      strcpy (SETTINGS.ap_ssid, "energy-meter");
      strcpy (SETTINGS.ap_password, "password");
      strcpy (SETTINGS.web_user, "admin");
      strcpy (SETTINGS.web_pass, "password");
      SETTINGS.wifi_outpwr = 5;
      SETTINGS.pulse_avg_perc_val = 15;
      strcpy (SETTINGS.css_link, "");
      strcpy (SETTINGS.syslog, "");
      strcpy (SETTINGS.hostname, "energy-meter");
      strcpy (SETTINGS.NTP_hostname, "ntp1.arnes.si");
      SETTINGS.timezone_GMT = 1;
      SETTINGS.data_collector_num_configured = 0;
      strcpy (SETTINGS.mqtt_server, "");
      SETTINGS.mqtt_port = 1883;
      SETTINGS.mqtt_interval = 10;
      SETTINGS.mqtt_enabled = 0;
      strcpy (SETTINGS.mqtt_key, "");
      strcpy (SETTINGS.mqtt_secret, "");
      strcpy (SETTINGS.data_collector_host[0], "");
      SETTINGS.data_collector_port[0] = 8086;
      strcpy (SETTINGS.data_collector_db_name[0], "db");
      strcpy (SETTINGS.data_collector_db_username[0], "");
      strcpy (SETTINGS.data_collector_db_password[0], "");
      SETTINGS.data_collector_push_interval[0] = 30;
      SETTINGS.data_collector_type[0] = 1;
      SETTINGS.pulse_start_value = 80;
      SETTINGS.calc_pulse = 3;
      SETTINGS.calc_kwh = 25;
      SETTINGS.price_kwh = 10.4;
      SETTINGS.price_eur = 30;
      SETTINGS.reset_day = 1;
      SETTINGS.reset_hour = 0;
      SETTINGS.reset_enabled = true;
      SETTINGS.webhook1_enabled = false;
      SETTINGS.webhook1_interval = 10;
      strcpy (SETTINGS.webhook1_url, "");
      SETTINGS.webhook2_enabled = false;
      SETTINGS.webhook2_interval = 10;
      strcpy (SETTINGS.webhook2_url, "");
      // clear crash
      http_clear_crash_info();
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);
      SendLog("INFO: Defaults loaded."); 
    }
