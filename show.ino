    String html_show_fixed_data() {
      long days = 0;
      long hours = 0;
      long mins = 0;
      unsigned long secs = 0;
      secs = millis()/1000; //convert milliseconds to seconds
      mins=secs/60; //convert seconds to minutes
      hours=mins/60; //convert minutes to hours
      days=hours/24; //convert hours to days
      secs=secs-(mins*60); //subtract the coverted seconds to minutes in order to display 59 secs max
      mins=mins-(hours*60); //subtract the coverted minutes to hours in order to display 59 minutes max
      hours=hours-(days*24); //subtract the coverted hours to days in order to display 23 hours max
      String html;
      html += "<span class=\"cloud\"> time <br>" + show_time() + "</span>";
      html += "<span class=\"cloud\"> date <br>" + show_date() + "</span>";
      html += "<span class=\"cloud\"> uptime <br>" + String((long)days) + "d " + String((long)hours) + "h " + String((long)mins) + "m " + String((long)secs)+ "s</span>";
      html += "<span class=\"cloud\"> wifi <br>" + String(WiFi.RSSI()) + "dB</span>";
      html += "<span class=\"cloud\"> pulse counter <br>" + String(PulseCounter) + "</span>";
      html += "<span class=\"cloud\"> analog signal <br>" + String(analogRead(A0)) + "</span>";
      html += "<span class=\"cloud\"> analog signal max<br>" + String(MaxAnalogValue) + "</span>";
      html += "<span class=\"cloud\"> analog signal avg<br>" + String(AvgAnalogValue) + "</span>";
      html += "<span class=\"cloud\"> analog signal diff<br>" + String(MaxAnalogValue - AvgAnalogValue) + "</span>";
      for (int i = 0; i <= 12; ++i) {
        if (i != month()) {
          if (Wh_year[i] != 0) {
            html += "<span class=\"cloud\"> energy " + Number_to_month_name(i) + "<br>" + calculate_k_M(Wh_year[i]) + "Wh</span>";
            html += "<span class=\"cloud\"> price " + Number_to_month_name(i) + "<br>" + calculate_k_M((Wh_year[i] / 1000) * (SETTINGS.price_kwh / 100) + SETTINGS.price_eur) + "€</span>"; 
          }
          if (WhDayAVG_year[i] != 0) {
            html += "<span class=\"cloud\"> energy/day " + Number_to_month_name(i) + "<br>" + calculate_k_M(WhDayAVG_year[i]) + "Wh/day</span>";
            html += "<span class=\"cloud\"> price/day " + Number_to_month_name(i) + "<br>" + calculate_k_M((WhDayAVG_year[i] / 1000) * (SETTINGS.price_kwh / 100) + SETTINGS.price_eur / numberOfDaysInMonth( year(), i)) + "€</span>";
          }
          if (WattsAVG_year[i] != 0) {
            html += "<span class=\"cloud\"> avg power " + Number_to_month_name(i) + "<br>" + calculate_k_M(WattsAVG_year[i]) + "W</span><br>";
          }
        }
      }
      return html;
    }    
        
    void handle_show() {
      String html = "<!DOCTYPE html><html><head>";
      html += "<title>" + String(SETTINGS.hostname) + "</title>";
      html += "<meta charset=\"UTF-8\">\n";
      if (SETTINGS.css_link[0] == '\0') { // preverim ali je css settings prazen, potem nalozim default css
        //html += css_string();
        html += FPSTR(CSS);
      }
      else {
        html += "<link rel=\"stylesheet\" type=\"text/css\" href=\"" + String(SETTINGS.css_link) + "\">"; 
      }
      html += static_html_handle_show;
      html += "<div class=\"set\"><a href=\"/settings\">⚙</a></div>";
      html += "<a href=\"/\"><button class=\"g\">HOME</button></a>";
      html += "</body></html>\n";
      server.send(200, "text/html", html);
      yield();
    }

    void http_show_refresh() {
      server.send(200, "text/html", html_show_fixed_data());
      yield();
    }
