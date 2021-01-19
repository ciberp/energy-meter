
    void handle_reset_max_value() {
//      if (!server.authenticate(SETTINGS.web_user, SETTINGS.web_pass)) {
//          return server.requestAuthentication();
//      }
      MaxAnalogValue = 0;
      SendLog("INFO: MaxAnalogValue reset!");
      server.send(200, "text/html", BackURLSystemSettings);
    }

    void handle_reset_avg_value() {
//      if (!server.authenticate(SETTINGS.web_user, SETTINGS.web_pass)) {
//          return server.requestAuthentication();
//      }
      AvgAnalogValue = 0;
      SendLog("INFO: AvgAnalogValue reset!");
      server.send(200, "text/html", BackURLSystemSettings);
    }

    void handle_Settings() {
        if (!server.authenticate(SETTINGS.web_user, SETTINGS.web_pass)) {
          return server.requestAuthentication();
        }
        String html = "<!DOCTYPE html><html><head>";
        html += "<title>" + String(SETTINGS.hostname) + " settings</title>";
        html += "<meta charset=\"UTF-8\">\n";
        if (SETTINGS.css_link[0] == '\0') { // preverim ali je css settings prazen, potem nalozim default css
          //html += css_string();
          html += FPSTR(CSS);
        }
        else {
          html += "<link rel=\"stylesheet\" type=\"text/css\" href=\"" + String(SETTINGS.css_link) + "\">"; 
        }
        html += "</head><body class=\"f9\">";
        server.sendContent(html);
        delay(10);
        yield();
        html = ""; // ne brisi
        html += "<table><tr><th colspan=\"9\">Exporter settings to influxdb, carbon, redis</th></tr>";
        html += "<tr><th>#</th><th>type</th><th>hostname/ip</th><th>port</th><th>interval[sec]</th><th>db name</th><th>db username</th><th>db password</th></tr>";
        html += http_data_collector_config();
        html += "<tr><td><form method='get' action='flxadd' ></td><td><select name=\"flxty\" ><option value=\"1\">influxdb_tcp</option><option value=\"4\">influxdb_udp</option><option value=\"2\">carbon_udp</option><option value=\"3\">redis_tcp</option></select></td><td>";
        html += "<input name='flxho' value='' maxlength='100' type='text' size='15'/></td><td>";
        html += "<input name='flxpo' value='' maxlength='6' type='text' size='6'/></td><td>";
        html += "<input name='flxin' value='' maxlength='6' type='text' type='text' size='6'/></td><td>";
        html += "<input name='flxdn' value='' maxlength='15' type='text' size='6'/></td><td>";
        html += "<input name='flxdu' value='' maxlength='15' type='text' size='6'/></td><td>";
        html += "<input name='flxdp' value='' maxlength='15' type='password' size='6'/>";
        html += "<button type=\"submit\" class=\"d\">+</button></form><a href=\"/flxrem\"><button class=\"d\">-</button></a></td><tr></table>";
        server.sendContent(html);
        delay(10);
        yield();
        String MonthDropDown = "<select name=\"month\"><option value=\"1\" selected>January</option><option value=\"2\">February</option><option value=\"3\">March</option><option value=\"4\">April</option><option value=\"5\">May</option><option value=\"6\">June</option><option value=\"7\">July</option><option value=\"8\">August</option><option value=\"9\">September</option><option value=\"10\">October</option><option value=\"11\">November</option><option value=\"12\">December</option></select>";
        String YearDropDown = "<select name=\"year\"><option value=\"" + String(year()) + "\" selected>" + String(year()) + "</option><option value=\"" + String(year() - 1) + "\">" + String(year() - 1) + "</option></select>";
        html = ""; // ne brisi
        html += "<table>"; // ne brisi
        html += "<tr><th colspan=\"5\">Pulse counter settings monthly energy, energy/day, power avg</th></tr>";
        html += "<tr><th>pulse value</th><th>energy [Wh]</th><th>energy/day [Wh]</th><th>power avg [W]</th><th>month</th></tr>";
        html += "<tr><td><form method='get' action='setpls'><input name='plsnum' value='' maxlength='7' type='text' size='7'/><button type=\"submit\" class=\"d\">set</button></form></td>";
        html += "<td><form method='get' action='setmth'><input name='engnum' value='' maxlength='7' type='text' size='7'/></td>";
        html += "<td><input name='whdavgnum' value='' maxlength='7' type='text' size='7'/></td>";
        html += "<td><input name='pwravgnum' value='' maxlength='7' type='text' size='7'/></td>";
        html += "<td>" + MonthDropDown + YearDropDown +"<button type=\"submit\" class=\"d\">set</button></form></td></tr>";
        html += "</table>";
        server.sendContent(html);
        delay(10);
        yield();
        html = ""; // ne brisi
        html += "<form method='get' action='commit' id=\"commit\" >";
        html += "<table>"; // ne brisi
        html += "<tr><th colspan=\"8\">Pulse counting settings</th><th colspan=\"2\">price = cent/kwh * kwh + fixed price</th></tr>";
        html += "<tr><th>pulse avg</th><th>pulse avg</th><th>pulse max</th><th>pulse max</th><th>pulse val percent</th><th>number of pulses</th><th>for number of Wh</th><th>1KWh</th><th>cents/kWh</th><th>fixed price [EUR]</th></tr>";
        html += "<tr><td>" + String(AvgAnalogValue) + "</td><td>";
        html += "<a href=\"/rstavg\"><button >Reset</button></a></td><td>";
        html += String(MaxAnalogValue) + "</td><td>";
        html += "<a href=\"/rstmax\"><button >Reset</button></a></td><td>";
        html += "<input name='papc' value='" + String(SETTINGS.pulse_avg_perc_val) + "' maxlength='3' type='text' pattern='[0-9.]{0,15}' size='3'/></td><td>";
        html += "<input name='pcp' value='" + String(SETTINGS.calc_pulse) + "' maxlength='3' type='text' pattern='[0-9.]{0,15}' size='6' /></td><td>";
        html += "<input name='pcw' value='" + String(SETTINGS.calc_kwh) + "' maxlength='3' type='text' pattern='[0-9.]{0,15}' size='6' /></td><td>";
        html +=  String((1000 * SETTINGS.calc_pulse) / SETTINGS.calc_kwh) + "pulse</td><td>";
        html += "<input name='prc' value='" + String(SETTINGS.price_kwh) + "' maxlength='9' type='text' pattern='[0-9.]{0,9}' size='6' /></td><td>";
        html += "<input name='eur' value='" + String(SETTINGS.price_eur) + "' maxlength='9' type='text' pattern='[0-9.]{0,9}' size='6' /></td></tr>";
        html += "</table>";
        server.sendContent(html);
        delay(10);
        yield();
        html = ""; // ne brisi
        html += "<table>"; // ne brisi
        html += "<tr><th colspan=\"3\">Reset counter settings</th></tr>";
        html += "<tr><th>day in month</th><th>hour</th><th>enabled</th></tr>";
        html += "<tr><td><input name='rday' value='" + String(SETTINGS.reset_day) + "' maxlength='4' type='text' pattern='[0-9]{0,2}' size='6'/></td><td>";
        html += "<input name='rhour' value='" + String(SETTINGS.reset_hour) + "' maxlength='3' type='text' pattern='[0-9]{0,2}' size='6' /></td><td>";
        if (SETTINGS.reset_enabled) {
          html += "<input type='checkbox' name='rena' value='1' checked/>";
        } else {
          html += "<input type='checkbox' name='rena' value='1'/>"; 
        }
        html += "</td></tr></table>";        
        server.sendContent(html);
        delay(10);
        yield();
        html = "<table>"; // ne brisi
        html += "<tr><th colspan=\"4\">Webhook</th></tr>";
        html += "<tr><th>#</th><th>url</th><th>interval[sec]</th><th>enabled</th></tr>";
        html += "<tr><td>1</td><td><input name='whu1' value='" + String(SETTINGS.webhook1_url) + "' maxlength='150' type='text' size='45' /></td><td>";
        html += "<input name='whi1' value='" + String(SETTINGS.webhook1_interval) + "' maxlength='3' type='text' size='4'/></td><td>";
        if (SETTINGS.webhook1_enabled) {
          html += "<input type='checkbox' name='whe1' value='1' checked/>";
        } else {
          html += "<input type='checkbox' name='whe1' value='1'/>"; 
        }
        html += "</td><tr>";
        html += "<tr><td>2</td><td><input name='whu2' value='" + String(SETTINGS.webhook2_url) + "' maxlength='150' type='text' size='45' /></td><td>";
        html += "<input name='whi2' value='" + String(SETTINGS.webhook2_interval) + "' maxlength='3' type='text' size='4'/></td><td>";
        if (SETTINGS.webhook2_enabled) {
          html += "<input type='checkbox' name='whe2' value='1' checked/>";
        } else {
          html += "<input type='checkbox' name='whe2' value='1'/>"; 
        }
        html += "</td><tr>";        
        html += "</table>";
        server.sendContent(html);
        delay(10);
        yield();
        html = ""; // ne brisi
        html += "<table><tr><th colspan=\"6\">MQTT settings</th></tr>";
        html += "<tr><th>broker/ip</th><th>port</th><th>interval [sec]</th><th>username</th><th>password</th><th>enabled</th></tr>";
        html += "<tr><td><input name='mqbr' value='" + String(SETTINGS.mqtt_server) + "' maxlength='100' type='text' size='15'/></td><td>";;
        html += "<input name='mqprt' value='" + String(SETTINGS.mqtt_port) + "' maxlength='6' type='text' pattern='[0-9]{0,6}' size='6'/></td><td>";
        html += "<input name='mqint' value='" + String(SETTINGS.mqtt_interval) + "' maxlength='6' type='text' pattern='[0-9]{0,6}' size='6'/></td><td>";
        html += "<input name='mqkey' value='" + String(SETTINGS.mqtt_key) + "' maxlength='25' type='text' size='15'/></td><td>";
        html += "<input name='mqsec' value='" + String(SETTINGS.mqtt_secret) + "' maxlength='25' type='password' size='15'/></td><td>";
        if (SETTINGS.mqtt_enabled) {
          html += "<input type='checkbox' name='mqena' value='1' checked/>";
        } else {
          html += "<input type='checkbox' name='mqena' value='1'/>"; 
        }
        html += "</td></tr></table>";
        server.sendContent(html);
        delay(10);
        yield();
        html = "<table>"; // ne brisi
        html += "<tr><th colspan=\"5\">System settings - " + Build + "</th></tr>";
        html += "<tr><th>hostname</th><th>ntp hostname</th><th>timezone GMT</th><th>CSS url</th><th>syslog ip</th></tr>";
        html += "<tr><td><input name='hostname' value='" + String(SETTINGS.hostname) + "' maxlength='150' type='text' size='15'/></td><td>";
        html += "<input name='ntp' value='" + String(SETTINGS.NTP_hostname) + "' maxlength='150' type='text' size='15'/></td><td>";
        html += "<input name='gmt' value='" + String(SETTINGS.timezone_GMT) + "' maxlength='3' type='text' size='6' /></td><td>";
        html += "<input name='css' value='" + String(SETTINGS.css_link) + "' maxlength='150' type='text' size='25' /></td><td>";
        html += "<input name='syslog' value='" + String(SETTINGS.syslog) + "' maxlength='20' type='text' size='15'/></td></tr>";
        html += "<tr><td colspan=\"5\">" + ESP.getFullVersion() + "</td></tr>";
        html += "</table>";
        server.sendContent(html);
        delay(10);
        yield();
        html = "<table>"; // ne brisi
        html += "<tr><th colspan=\"6\">IP settings</th></tr>";
        html += "<tr><th>hostname</th><th>interface</th><th>address</th><th>mask</th><th>gateway</th></tr>";
        for (auto a : addrList) {
          if (a.isLegacy()) {
            html += "<tr><td>" + String(a.ifhostname()) + "</td><td>" + String(a.ifname().c_str()) + "</td><td>" + String(a.toString().c_str()) + "</td><td>" + String(a.netmask().toString().c_str()) + "</td><td>" + String(a.gw().toString().c_str()) + "</td></tr>";
          }
          else {
            html += "<tr><td>" + String(a.ifhostname()) + "</td><td>" + String(a.ifname().c_str()) + "</td><td>" + String(a.toString().c_str()) + "</td><td>/</td><td>/</td></tr>";
          }
        }
        html += "</table>";
        html += "<table>"; // ne brisi
        html += "<tr><th  colspan=\"6\">System settings - login/wifi credentials</th></tr>";
        html += "<tr><th>SSID</th><th>PSK</th><th>AP SSID</th><th>AP PSK</th><th>username</th><th>password</th></tr>";
        html += "<tr><td><input name='ssid' value='" + String(SETTINGS.ssid) + "' maxlength='32' type='text' size='15' list='ssidlist'/>" + http_print_wifi_networks() + "</td><td>";
        html += "<input name='pass' value='" + String(SETTINGS.password) + "' maxlength='32' pattern='[0-9A-Za-z]{8,30}' type='password' size='15'/></td><td>";
        html += "<input name='ap_ssid' value='" + String(SETTINGS.ap_ssid) + "' maxlength='32' type='text' size='15' /></td><td>";
        html += "<input name='ap_pass' value='" + String(SETTINGS.ap_password) + "' maxlength='32' type='password' pattern='[0-9A-Za-z]{8,30}' size='15'/></td><td>";
        html += "<input name='w_user' value='" + String(SETTINGS.web_user) + "' maxlength='32' type='text' size='15'/></td><td>";
        html += "<input name='w_pass' value='" + String(SETTINGS.web_pass) + "' maxlength='32' type='password' size='15'/></td></tr>";
        html += "</table><table>";
        html += "<tr><th>wifi output power</th><th>ssid rssi channel encryption</th><th>wifi bssid status</th><th>username</th><th>password</th></tr>";
        html += "<tr><td><input name='w_pwr' value='" + String(SETTINGS.wifi_outpwr) + "' maxlength='3' type='text' size='6' />dBm</td><td>";
        html += print_wifi_networks() + "</td><td>";
        html += WiFi.BSSIDstr()+ "<br>" + String(wl_status_to_string(WiFi.status())) + "</td><td>";
        html += "<input name='w_user' value='" + String(SETTINGS.web_user) + "' maxlength='32' type='text' size='15'/></td><td>";
        html += "<input name='w_pass' value='" + String(SETTINGS.web_pass) + "' maxlength='32' type='password' size='15'/></td></tr>";
        html += "</table></form>";
        server.sendContent(html);
        delay(10);
        yield();
        html = ""; // ne brisi
        html += "<button class=\"g\" form=\"commit\" type=\"submit\">SAVE</button>";
        //html += "<a href=\"/save\"><button class=\"g\">SAVE</button></a>";
        html += "<a href=\"/\"><button class=\"g\">HOME</button></a>";
        html += "<a href=\"/disap\"><button class=\"g\">ONLY STATION MODE</button></a>";
        html += "<a href=\"/enapsta\"><button class=\"g\">AP + STATION MODE</button></a>";
        html += "<a href=\"/recon\"><button class=\"g\">RECONNECT</button></a>";
        html += "<a href=\"/default\"><button class=\"g\">LOAD DEFAULTS</button></a>";
        html += "<a href=\"/settings\"><button class=\"g\">REFRESH</button></a>";
        html += "<a href=\"/espupdate\"><button class=\"g\">OTA UPDATE</button></a>";
        html += "<a href=\"/reboot\"><button class=\"g\">REBOOT</button></a>";
        html += "<a href=\"/crashinfo\"><button class=\"g\">VIEW CRASH INFO</button></a>";
        html += "<a href=\"/clearcrash\"><button class=\"g\">CLEAR CRASH INFO</button></a>";
        html += "<a href=\"/ntpupdate\"><button class=\"g\">NTP UPDATE</button></a>";
        html += "</body></html>\n";
        server.sendContent(html); // poslje paket
        delay(10);
        yield();
        server.client().stop(); // sporocimo clientu, da smo poslali vse!
        yield();
    }
