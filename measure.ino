    void init_history_measures() {
      for (int i = 1; i <= 12; i++) {
         year_stamp[i] = 0;
         Wh_year[i] = 0;
         WhDayAVG_year[i] = 0;
         WattsAVG_year[i] = 0;
      }
      WattsMax = 0;
    }

    void read_sensor_loop() {
      int AnalogValue = analogRead(A0);
      AnalogValueStats.add(AnalogValue);
      AvgAnalogValue = AnalogValueStats.average();
      MaxAnalogValue = AnalogValueStats.maximum();
      if (AnalogValue > (MaxAnalogValue - MaxAnalogValue * SETTINGS.pulse_avg_perc_val / 100) && PulseLock) { // začetek pulza
        PulseCounter++;
        PulseLock = false;
      }
      if (AnalogValue < (AvgAnalogValue + AvgAnalogValue * SETTINGS.pulse_avg_perc_val / 100) && !PulseLock) { // konec pulza
        PulseLock = true;
      }
    }
    
    void web_set_counter() {
      // ko rocno popravimo counter, zaradi nadgradnje, power off, ali drugih razlogov ...
      calculate_Wh_loop();
      previous_Wh = Wh;
    }

    void calculate_Wh_loop() {
      Wh = PulseCounter * SETTINGS.calc_kwh / (float) SETTINGS.calc_pulse; // float zato, da je razultat tudi float
    }

    String calculate_k_M(float value) {
      if (value >= 1000000) {
        return String(value/1000000) + "M";
      }
      else if (value >= 1000 && value < 1000000) {
        return String(value/1000) + "k";
      }
      else {
        return String(value);
      }
    }

    void calculate_Watts_loop() {
      if (Wh > 0) {
        if (Wh > previous_Wh) { // da ne dobim napacnega izracuna, ko je (Wsec < previous_Wsec) overflow in dobim MW :)
          float  Wh_delta = (float) Wh - (float) previous_Wh;
          Watts = (float) Wh_delta * 12; // ce je 5min interval x12, ce je 1min interval x60, ce je 1h interval x1
          if (Watts > WattsMax) {
            WattsMax = Watts;
          }
          previous_Wh = Wh;
        }
      }
    }

    void calculate_avg_watts_loop() {
      if (Wh > 0) {
        avg_watts = Wh / (((float) day() - 1) * 24 + (float) hour() + (float) minute() / 60);
      }
    }

    void calculate_avg_energy_per_day_loop() {
      if (Wh > 0) {
        avg_wh_day = Wh / ((float) day() - 1 + (float) hour() / 24 + (float) minute() / 1440);
      }
    }

    void calculate_price_loop() {
      if (Wh > 0) {
        // price now
        avg_price = (Wh / 1000) * (SETTINGS.price_kwh / 100) + SETTINGS.price_eur;
        // price per day
        avg_price_day = (avg_wh_day / 1000) * (SETTINGS.price_kwh / 100) + (SETTINGS.price_eur / numberOfDaysInMonth( year(), month()) );
        // price per month
        avg_price_month = (avg_wh_day / 1000) * numberOfDaysInMonth( year(), month()) * (SETTINGS.price_kwh / 100) + SETTINGS.price_eur;
      }
    }    

    void CheckPulseForError_loop() {   
        if (PulseCounter == PulseCounterPrevious) {
              SendLog("ERROR: Pulse not changed in 10min!");
              PulseCounterErrorState = 1;
        }
        else {
          PulseCounterPrevious = PulseCounter;
          PulseCounterErrorState = 0;
        }
      }

    void http_pulse() {
      PulseCounter++;
      String html = BackURLRoot;
      server.send(200, "text/html", html);
    }

    void UserResetCounters() {
      ResetCountersLock = false;
      PulseCounter = 0; // reset pulse
      //avg_wh_day = 0;     // reset wh per day
      previous_Wh = 0;
      SendLog("INFO: Reset counters done by user.");
      String html = BackURLRoot;
      server.send(200, "text/html", html);
    }    

    void ResetCounters_loop() { // reset stevcev glede na dan v mesecu
      if (SETTINGS.reset_enabled) {
        if (day() == SETTINGS.reset_day && hour() == SETTINGS.reset_hour && ResetCountersLock) {
          ResetCountersLock = false;
          PulseCounter = 0; // reset pulse
          WattsMax = 0;     // reset power max
          //avg_wh_day = 0;     // reset wh per day
          previous_Wh = 0;
          SendLog("INFO: Reset counters done by scheduler.");
        }
        else if (day() == SETTINGS.reset_day && hour() == SETTINGS.reset_hour && !ResetCountersLock){
          ResetCountersLock = false;
        }
        else {
          ResetCountersLock = true;
        }
        // zapisovanje mesecnih podatkov v array
        year_stamp[month()] = year();
        Wh_year[month()] = Wh;
        WhDayAVG_year[month()] = avg_wh_day;
        WattsAVG_year[month()] = avg_watts;
      }
    } 
