    void handle_root() {
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
      html += static_html_handle_root;
      html += "<div class=\"set\"><a href=\"/settings\">⚙</a></div>";
      html += "<a href=\"/show\"><button class=\"g\">EXTENSIVE DATA</button></a>";
      html += "</body></html>\n";
      server.send(200, "text/html", html);
      yield();
    }

    void http_root_refresh() {
      server.send(200, "text/html", html_fixed_data());
      yield();
    } 

    String html_fixed_data() {
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
      //html += "<span class=\"cloud\"> pulse counter <br>" + String(PulseCounter) + "</span>";
      html += "<span class=\"cloud\"> energy now <br>" + calculate_k_M(Wh) + "Wh</span>";
      html += "<span class=\"cloud\"> energy avg/day <br>" + calculate_k_M(avg_wh_day) + "Wh/day</span>";
      html += "<span class=\"cloud\"> energy/month <br>" + calculate_k_M(avg_wh_day * numberOfDaysInMonth( year(), month())) +"Wh</span>";
      html += "<span class=\"cloud\"> power <br>" + calculate_k_M(Watts) + "W</span>";
      html += "<span class=\"cloud\"> power avg <br>" + calculate_k_M(avg_watts) + "W</span>";
      html += "<span class=\"cloud\"> peak power <br>" + calculate_k_M(WattsMax) + "W</span>";
      html += "<span class=\"cloud\"> price now<br>" + calculate_k_M(avg_price) + "€</span>";
      html += "<span class=\"cloud\"> price avg/day <br>" + calculate_k_M(avg_price_day) + "€</span>";
      html += "<span class=\"cloud\"> price/month<br>" +  calculate_k_M(avg_price_month) + "€</span>";      
      return html;
    } 
