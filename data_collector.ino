
    
    String http_data_collector_config(void) {
      String html = "";
      for(int n = 0; n < SETTINGS.data_collector_num_configured; n++) {
        if (SETTINGS.data_collector_type[n] == 1) {
          html += "<tr><td>" + String(n) + "</td><td>influxdb_tcp</td><td>" + String(SETTINGS.data_collector_host[n]) + "</td><td>" + String(SETTINGS.data_collector_port[n]) + "</td><td>" + String(SETTINGS.data_collector_push_interval[n]) + "</td><td>" + String(SETTINGS.data_collector_db_name[n]) + "</td><td>" + String(SETTINGS.data_collector_db_username[n]) + "</td><td>" + String(SETTINGS.data_collector_db_password[n]) + "</td></tr>"; 
        }
        if (SETTINGS.data_collector_type[n] == 2) {
          html += "<tr><td>" + String(n) + "</td><td>carbon_udp</td><td>" + String(SETTINGS.data_collector_host[n]) + "</td><td>" + String(SETTINGS.data_collector_port[n]) + "</td><td>" + String(SETTINGS.data_collector_push_interval[n]) + "</td><td>" + String(SETTINGS.data_collector_db_name[n]) + "</td><td>" + String(SETTINGS.data_collector_db_username[n]) + "</td><td>" + String(SETTINGS.data_collector_db_password[n]) + "</td></tr>"; 
        }
        if (SETTINGS.data_collector_type[n] == 3) {
          html += "<tr><td>" + String(n) + "</td><td>redis_tcp</td><td>" + String(SETTINGS.data_collector_host[n]) + "</td><td>" + String(SETTINGS.data_collector_port[n]) + "</td><td>" + String(SETTINGS.data_collector_push_interval[n]) + "</td><td>" + String(SETTINGS.data_collector_db_name[n]) + "</td><td>" + String(SETTINGS.data_collector_db_username[n]) + "</td><td>" + String(SETTINGS.data_collector_db_password[n]) + "</td></tr>"; 
        }
        if (SETTINGS.data_collector_type[n] == 4) {
          html += "<tr><td>" + String(n) + "</td><td>influxdb_udp</td><td>" + String(SETTINGS.data_collector_host[n]) + "</td><td>" + String(SETTINGS.data_collector_port[n]) + "</td><td>" + String(SETTINGS.data_collector_push_interval[n]) + "</td><td>" + String(SETTINGS.data_collector_db_name[n]) + "</td><td>" + String(SETTINGS.data_collector_db_username[n]) + "</td><td>" + String(SETTINGS.data_collector_db_password[n]) + "</td></tr>"; 
        }
      }    
      return html;
    }

    void http_data_collector_remove_client() {
      if (SETTINGS.data_collector_num_configured > 0) {
        SETTINGS.data_collector_num_configured--; 
      }
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);
    }

    void data_collector_send_data() {
      for(int x = 0; x < SETTINGS.data_collector_num_configured; x++) {
        if (millis() - previousDataCollector[x] >= SETTINGS.data_collector_push_interval[x] * 1000) {
          previousDataCollector[x] = millis();
          String InfluxDataStructure = "";
          String CarbonDataStructure = "";
          String RedisDataStructure = "";

//          InfluxDataStructure += "voltage,host=" + String(host) + ",name=Vcc5V value=" + String(adc_value[voltage_measure_address[0]]/43) + "\n";
//          InfluxDataStructure += "voltage,host=" + String(host) + ",name=VccInput value=" + String(adc_value[voltage_measure_address[1]]/43) + "\n";
//          InfluxDataStructure += "uptime,host=" + String(host) + ",name=Uptime value=" + String(uptime) + "\n";
//          InfluxDataStructure += "wifi,host=" + String(host) + ",name=Rssi value=" + String(WiFi.RSSI()) + "\n";
//          InfluxDataStructure += "heap,host=" + String(host) + ",name=Free value=" + String(ESP.getFreeHeap()) + "\n";
//          InfluxDataStructure += "heap,host=" + String(host) + ",name=Fragmentation value=" + String(ESP.getFreeHeap()) + "\n";
//          InfluxDataStructure += "heap,host=" + String(host) + ",name=MaxFreeBlockSize value=" + String(ESP.getFreeHeap()) + "\n";               

          
          InfluxDataStructure += "uptime,host=" + String(SETTINGS.hostname) + ",name=Uptime value=" + String(uptime) + "\n";
          InfluxDataStructure += "voltage,host=" + String(SETTINGS.hostname) + ",name=Vcc value=" + String((float)ESP.getVcc()/890) + "\n";
          InfluxDataStructure += "wifi,host=" + String(SETTINGS.hostname) + ",name=Rssi value=" + String(WiFi.RSSI()) + "\n";
          InfluxDataStructure += "heap,host=" + String(SETTINGS.hostname) + ",name=Free value=" + String(ESP.getFreeHeap()) + "\n";
          //CarbonDataStructure += String(SETTINGS.hostname) + ".system.voltage " + String((int)analogRead(A0)) + " " + now() + "\n";
          CarbonDataStructure += String(SETTINGS.hostname) + ".system.uptime " + String((unsigned long)millis()) + " " + now() + "\n";
          CarbonDataStructure += String(SETTINGS.hostname) + ".system.wifi " + String(WiFi.RSSI()) + " " + now() + "\n";
          CarbonDataStructure += String(SETTINGS.hostname) + ".system.freeheap " + String(ESP.getFreeHeap()) + " " + now() + "\n";
      
          String RedisTmpString = ""; 
          RedisDataStructure += String("*3\r\n") + "$5\r\n" + "LPUSH\r\n";
          RedisTmpString += "{\"system\": {\"uptime\": \"" + String(uptime)+ "\", ";
          RedisTmpString += "\"wifi\": \"" + String(WiFi.RSSI()) + "\", ";
          RedisTmpString += "\"freeheap\": \"" + String(ESP.getFreeHeap()) + "\"}}";
          RedisDataStructure += "$" + String(strlen(SETTINGS.hostname)) + "\r\n" + String(SETTINGS.hostname) + "\r\n" + "$" + String(RedisTmpString.length()) + "\r\n" + RedisTmpString + "\r\n";
          if ( SETTINGS.data_collector_type[x] == 1 ) {
            send_to_influxdb_tcp(InfluxDataStructure, x);
          }
          else if ( SETTINGS.data_collector_type[x] == 4 ) {
            send_to_influxdb_udp(InfluxDataStructure, x);
          }
          else if ( SETTINGS.data_collector_type[x] == 2 ) {
            send_to_carbon(CarbonDataStructure, x);
          }
          else if (SETTINGS.data_collector_type[x] == 3) {
            send_to_redis(RedisDataStructure, x);
          }
          InfluxDataStructure = "";
          CarbonDataStructure = "";
          RedisDataStructure = "";
          InfluxDataStructure += "pulse,host=" + String(SETTINGS.hostname) + ",name=analog_avg value=" + String(AvgAnalogValue) + "\n";
          InfluxDataStructure += "pulse,host=" + String(SETTINGS.hostname) + ",name=analog_max value=" + String(MaxAnalogValue) + "\n";
          InfluxDataStructure += "pulse,host=" + String(SETTINGS.hostname) + ",name=analog value=" + String(analogRead(A0)) + "\n";
          InfluxDataStructure += "pulse,host=" + String(SETTINGS.hostname) + ",name=counter value=" + String(PulseCounter) + "\n";
          InfluxDataStructure += "energy,host=" + String(SETTINGS.hostname) + ",name=current value=" + String(Wh) + "\n";
          InfluxDataStructure += "energy,host=" + String(SETTINGS.hostname) + ",name=daily value=" + String(avg_wh_day) + "\n";
          InfluxDataStructure += "energy,host=" + String(SETTINGS.hostname) + ",name=month_calculated value=" + String(avg_wh_day * numberOfDaysInMonth( year(), month())) + "\n";
          InfluxDataStructure += "power,host=" + String(SETTINGS.hostname) + ",name=current value=" + String(Watts) + "\n";
          InfluxDataStructure += "power,host=" + String(SETTINGS.hostname) + ",name=max value=" + String(WattsMax) + "\n";
          InfluxDataStructure += "power,host=" + String(SETTINGS.hostname) + ",name=avg value=" + String(avg_watts) + "\n";
          CarbonDataStructure += String(SETTINGS.hostname) + ".pulse.value " + String(PulseCounter) + " " + now() + "\n";
          CarbonDataStructure += String(SETTINGS.hostname) + ".analog.value " + String(analogRead(A0)) + " " + now() + "\n";
          CarbonDataStructure += String(SETTINGS.hostname) + ".energy.value " + String(Wh) + " " + now() + "\n";
          CarbonDataStructure += String(SETTINGS.hostname) + ".energydaily.value " + String(avg_wh_day) + " " + now() + "\n";
          CarbonDataStructure += String(SETTINGS.hostname) + ".energycalcmonth.value " + String(avg_wh_day * numberOfDaysInMonth( year(), month())) + " " + now() + "\n";
          CarbonDataStructure += String(SETTINGS.hostname) + ".power.value " + String(Watts) + " " + now() + "\n";
          CarbonDataStructure += String(SETTINGS.hostname) + ".powermax.value " + String(WattsMax) + " " + now() + "\n";
          CarbonDataStructure += String(SETTINGS.hostname) + ".poweravg.value " + String(avg_watts) + " " + now() + "\n";
          //TODO dodaj v carbon in redis 30min
          RedisTmpString = ""; 
          RedisDataStructure = String("*3\r\n") + "$5\r\n" + "LPUSH\r\n";
          RedisTmpString += "{\"energy\": {\"pulse\": \"" + String(PulseCounter) + "\", ";
          RedisTmpString += "\"analog\": \"" + String(analogRead(A0)) + "\", ";
          RedisTmpString += "\"energy\": \"" + String(Wh) + "\", ";
          RedisTmpString += "\"energydaily\": \"" + String(avg_wh_day) + "\", ";
          RedisTmpString += "\"energycalcmonth\": \"" + String(calculate_k_M(avg_wh_day * numberOfDaysInMonth( year(), month()))) + "\", ";
          RedisTmpString += "\"powermax\": \"" + String(WattsMax) + "\", ";
          RedisTmpString += "\"poweravg\": \"" + String(avg_watts) + "\", ";
          RedisTmpString += "\"power\": \"" + String(Watts) + "\"}}";
          RedisDataStructure += "$" + String(strlen(SETTINGS.hostname)) + "\r\n" + String(SETTINGS.hostname) + "\r\n" + "$" + String(RedisTmpString.length()) + "\r\n" + RedisTmpString + "\r\n";
          if ( SETTINGS.data_collector_type[x] == 1 ) {
            send_to_influxdb_tcp(InfluxDataStructure, x);
          }
          else if ( SETTINGS.data_collector_type[x] == 4 ) {
            send_to_influxdb_udp(InfluxDataStructure, x);
          }
          else if ( SETTINGS.data_collector_type[x] == 2 ) {
            send_to_carbon(CarbonDataStructure, x);
          }
          else if (SETTINGS.data_collector_type[x] == 3) {
            send_to_redis(RedisDataStructure, x);
          }
        }
      }
    }
    
    void redis_data_prepair(String MetricIdent, String MetricType, String MetricValue, int x) {
      String RedisTmpString = ""; 
      String RedisDataStructure = "";
      RedisDataStructure = String("*3\r\n") + "$5\r\n" + "LPUSH\r\n";
      RedisTmpString += "{\"" + MetricIdent + "\": {\"" + MetricType + "\": \"" + MetricValue + "\"}}";
      RedisDataStructure += "$" + String(strlen(SETTINGS.hostname)) + "\r\n" + String(SETTINGS.hostname) + "\r\n" + "$" + String(RedisTmpString.length()) + "\r\n" + RedisTmpString + "\r\n";
      send_to_redis(RedisDataStructure, x);
    }
    
    void send_to_redis(String content, int x) {
      WiFiClient redis_client;
      redis_client.setTimeout(50);
      if (!redis_client.connect(SETTINGS.data_collector_host[x], SETTINGS.data_collector_port[x])) {
        SendLog("ERROR: redis (" + String(SETTINGS.data_collector_host[x]) + ":" + String(SETTINGS.data_collector_port[x]) + ") connection failed.");
        return;
      }
      yield();
      redis_client.print(content);
      redis_client.stop();
    }

    void send_to_influxdb_udp (String msgtosend, int x) {
        unsigned int msg_length = msgtosend.length();
        byte* p = (byte*)malloc(msg_length);
        memcpy(p, (char*) msgtosend.c_str(), msg_length);
        udp.beginPacket(SETTINGS.data_collector_host[x], SETTINGS.data_collector_port[x]);
        udp.write(p, msg_length);
        udp.endPacket();
        free(p);
        yield();
    }

    void send_to_influxdb_tcp (String content, int x) {
      HTTPClient http;
      // Create POST message
      http.begin("http://" + String(SETTINGS.data_collector_host[x]) + ":" + String(SETTINGS.data_collector_port[x]) + "/write?db=" + String(SETTINGS.data_collector_db_name[x]));
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      if (SETTINGS.data_collector_db_username[x][0] != '\0' && SETTINGS.data_collector_db_password[x][0] != '\0' ) {
        http.setAuthorization(SETTINGS.data_collector_db_username[x], SETTINGS.data_collector_db_password[x]);
      }
      int httpResponseCode = http.POST(content);
      if (httpResponseCode != 200 && httpResponseCode != 201 && httpResponseCode != 204 ) {
        //http.writeToStream(&Serial);
        SendLog("ERROR: influxdb (" + String(SETTINGS.data_collector_host[x]) + ":" + String(SETTINGS.data_collector_port[x]) + ") http code:" + String(httpResponseCode));
      }
      http.end();
    }

    void send_to_carbon (String msgtosend, int x) {
      if (WiFi.status() == WL_CONNECTED) {
        unsigned int msg_length = msgtosend.length();
        byte* p = (byte*)malloc(msg_length);
        memcpy(p, (char*) msgtosend.c_str(), msg_length);
        udp.beginPacket(SETTINGS.data_collector_host[x], SETTINGS.data_collector_port[x]);
        //udp.write(SETTINGS.hostname);
        udp.write(p, msg_length);
        udp.endPacket();
        free(p);
        yield();
      }
    }
