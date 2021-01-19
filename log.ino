    void SendLog(String msgtosend) {
      Serial.print("log: ");
      Serial.println(msgtosend);
      SendUDPSyslog(msgtosend);
      mqtt_send_log(String(SETTINGS.hostname) + msgtosend);
    }
    

    void SendUDPSyslog(String msgtosend) {
      if (SETTINGS.syslog[0] != '\0') { // preverim ali syslog IP nastavljen
        unsigned int msg_length = msgtosend.length();
        byte* p = (byte*)malloc(msg_length);
        memcpy(p, (char*) msgtosend.c_str(), msg_length);
        udp.beginPacket(syslogServer, 514);
        udp.write(SETTINGS.hostname);
        udp.write(p, msg_length);
        udp.endPacket();
        free(p);
      }
    }
    
    void Set_Syslog_From_Settings() {
      if (syslogServer.fromString(SETTINGS.syslog)) {
        //SendLog("INFO: Valid syslog IP Address");
        int Parts[4] = {0,0,0,0};
        int Part = 0;
        for ( int i=0; i<sizeof(SETTINGS.syslog); i++ )
        {
          char c = SETTINGS.syslog[i];
          if ( c == '.' )
          {
            Part++;
            continue;
          }
          Parts[Part] *= 10;
          Parts[Part] += c - '0';
        }
        IPAddress syslogServer( Parts[0], Parts[1], Parts[2], Parts[3] );
      } 
    }
