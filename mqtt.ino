
    void mqtt_init() {
      MQTT_Client.setTimeout(50);
      //mqtt_client.setOptions(10, true, 1000); // defaults
      mqtt_client.setOptions(2, true, 50);
      // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
      // You need to set the IP address directly.
      mqtt_client.begin(SETTINGS.mqtt_server, SETTINGS.mqtt_port, MQTT_Client);
      mqtt_client.onMessage(mqtt_receive_data);
      mqtt_connect();
    }

    void mqtt_connect() {
      if (SETTINGS.mqtt_enabled) {
        if (!mqtt_client.connect(SETTINGS.hostname, SETTINGS.mqtt_key ,SETTINGS.mqtt_secret)) {
          if (millis() - previousMQTTReconnect > 3000) {
            previousMQTTReconnect = millis();
            yield();
            mqtt_client.connect(SETTINGS.hostname, SETTINGS.mqtt_key ,SETTINGS.mqtt_secret);
            //Serial.print("mqtt-connecting...");
            SendLog("ERROR: mqtt connecting...");
          }
        }
      }
    }

    void mqtt_loop() {
      if (SETTINGS.mqtt_enabled) {
        mqtt_client.loop();
        delay(10);  // <- fixes some issues with WiFi stability
        if (!mqtt_client.connected()) {
          mqtt_connect();
        }  
      }
    }
    
    void mqtt_receive_data(String &topic, String &payload) {
      //Serial.println("incoming: " + topic + " - " + payload);
    }

    void mqtt_send_log(String log_msg) {
      if (SETTINGS.mqtt_enabled) {
        String MyTopic = SETTINGS.hostname + String("/system/log");
        mqtt_client.publish(MyTopic.c_str(), log_msg);
      }
    }


    void mqtt_send_data() {
      if (SETTINGS.mqtt_enabled) {
        if (millis() - previousMQTTInterval >= SETTINGS.mqtt_interval * 1000) {
          previousMQTTInterval = millis();
          char myfloat_in_Char[8];
          String MyTopic = SETTINGS.hostname + String("/system/uptime");
          dtostrf((unsigned long)millis(), 6, 0, myfloat_in_Char); 
          mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);
          MyTopic = SETTINGS.hostname + String("/system/wifi");
          dtostrf(WiFi.RSSI(), 6, 2, myfloat_in_Char); 
          mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);   
          MyTopic = SETTINGS.hostname + String("/system/freeheap");
          dtostrf(ESP.getFreeHeap(), 6, 0, myfloat_in_Char); 
          mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);   
          dtostrf(PulseCounter, 6, 2, myfloat_in_Char);
          MyTopic = SETTINGS.hostname + String("/pulse");
          mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);
          dtostrf(analogRead(A0), 6, 2, myfloat_in_Char);
          MyTopic = SETTINGS.hostname + String("/analog");
          mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char); 
          dtostrf(Wh, 6, 2, myfloat_in_Char);
          MyTopic = SETTINGS.hostname + String("/energy");
          mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);
          dtostrf(avg_wh_day, 6, 2, myfloat_in_Char);
          MyTopic = SETTINGS.hostname + String("/energydaily");
          mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char); 
          dtostrf(Watts, 6, 2, myfloat_in_Char);
          MyTopic = SETTINGS.hostname + String("/power");
          mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);
          dtostrf(WattsMax, 6, 2, myfloat_in_Char);
          MyTopic = SETTINGS.hostname + String("/powermax");
          mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);         
          dtostrf(avg_watts, 6, 2, myfloat_in_Char);
          MyTopic = SETTINGS.hostname + String("/poweravg");
          mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);
          dtostrf(PulseCounterErrorState, 6, 2, myfloat_in_Char);
          MyTopic = SETTINGS.hostname + String("/pulseerror");
          mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);           
        }
      }
    }
