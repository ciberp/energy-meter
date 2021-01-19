   void init_ntp() {
      //Serial.println("waiting for ntp");
      SendLog("ERROR: waiting for ntp.");
      NTPclient.begin(SETTINGS.NTP_hostname, SETTINGS.timezone_GMT);
      setSyncInterval(18000);
      setSyncProvider(getNTPtime);
      yield();
      SendLog("INFO: NTP init.");
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);
    }

    time_t getNTPtime(void)
    {
      return NTPclient.getNtpTime();
    }

    // popravi izpis 0 -> 00 oz 5 -> 05
    String print_time(int num) {
      if (num < 10) {
        return "0" + String(num);
      }
      if (num < 0 || num > 59) {
        return "00";
      }
      return String(num);
    }    

    String show_time() {
      String _hour, _minute, _second;
      if (hour() < 10) {
        _hour = "0" + String(hour());
      }
      else {
        _hour = String(hour());
      }
      if (minute() < 10) {
        _minute = "0" + String(minute());
      }
      else {
        _minute = String(minute());
      }
      if (second() < 10) {
        _second = "0" + String(second());
      }
      else {
        _second = String(second());
      }
      return _hour + ":" + _minute + ":" + _second;
    }

    String show_date() {
      return String(day()) + "." + String(month()) + "." + String(year());
    }

    uint8_t numberOfDaysInMonth( uint16_t year, uint8_t month )
    {
      if ( month == 2 ) 
        return ( (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0) ) ? 29 : 28;
        
      else if ( month == 4 || month == 6 || month == 9 || month == 11 ) 
          return 30;
          
      return 31;
    } 
    
