
    void http_crash_info() {
      String text;
      text += "ResetInfo: " + String(ESP.getResetInfo()) + "<br>";
      text += "BootMode: " + String(ESP.getBootMode()) + "<br>";
      text += "BootVersion: " + String(ESP.getBootVersion()) + "<br>";
      text += "SdkVersion: " + String(ESP.getSdkVersion()) + "<br>";
      char tmp[8]; 
      // Note that 'EEPROM.begin' method is reserving a RAM buffer
      // The buffer size is SAVE_CRASH_EEPROM_OFFSET + SAVE_CRASH_SPACE_SIZE
      EEPROM.begin(SAVE_CRASH_EEPROM_OFFSET + SAVE_CRASH_SPACE_SIZE);
    
      byte crashCounter = EEPROM.read(SAVE_CRASH_EEPROM_OFFSET + SAVE_CRASH_COUNTER);
      if (crashCounter == 0)
      {
        text += "No any crashes saved";
        server.send(200, "text/html", text);
        yield();
        return;
      }
    
      text += "Crash information recovered from EEPROM<br>";
      int16_t readFrom = SAVE_CRASH_EEPROM_OFFSET + SAVE_CRASH_DATA_SETS;
      for (byte k = 0; k < crashCounter; k++)
      {
        uint32_t crashTime;
        EEPROM.get(readFrom + SAVE_CRASH_CRASH_TIME, crashTime);
        text += "Crash #" + String(k + 1) + " at " + String(crashTime) + "ms<br>";
    
        text += "Reason of restart: " + String(EEPROM.read(readFrom + SAVE_CRASH_RESTART_REASON)) + "<br>";
        text += "Exception cause: " + String(EEPROM.read(readFrom + SAVE_CRASH_EXCEPTION_CAUSE)) + "<br>";
    
        uint32_t epc1, epc2, epc3, excvaddr, depc;
        EEPROM.get(readFrom + SAVE_CRASH_EPC1, epc1);
        EEPROM.get(readFrom + SAVE_CRASH_EPC2, epc2);
        EEPROM.get(readFrom + SAVE_CRASH_EPC3, epc3);
        EEPROM.get(readFrom + SAVE_CRASH_EXCVADDR, excvaddr);
        EEPROM.get(readFrom + SAVE_CRASH_DEPC, depc);
        sprintf(tmp, "epc1=0x%08x ", epc1);
        text += tmp;
        sprintf(tmp, "epc2=0x%08x ", epc2);
        text += tmp;
        sprintf(tmp, "epc3=0x%08x ", epc3);
        text += tmp;
        sprintf(tmp, "excvaddr=0x%08x ", excvaddr);
        text += tmp;
        sprintf(tmp, "depc=0x%08x ", depc);
        text += tmp;       
        text += "<br>";
        uint32_t stackStart, stackEnd;
        EEPROM.get(readFrom + SAVE_CRASH_STACK_START, stackStart);
        EEPROM.get(readFrom + SAVE_CRASH_STACK_END, stackEnd);
        text += ">>>stack>>><br>";
        int16_t currentAddress = readFrom + SAVE_CRASH_STACK_TRACE;
        int16_t stackLength = stackEnd - stackStart;
        uint32_t stackTrace;
        for (int16_t i = 0; i < stackLength; i += 0x10)
        {
          sprintf(tmp, "%08x ", stackStart + i);
          text += tmp;
          for (byte j = 0; j < 4; j++)
          {
            EEPROM.get(currentAddress, stackTrace);
            sprintf(tmp, "%08x ", stackTrace);
            text += tmp;
            currentAddress += 4;
            if (currentAddress - SAVE_CRASH_EEPROM_OFFSET > SAVE_CRASH_SPACE_SIZE)
            {
              text += "<br>Incomplete stack trace saved!";
              goto eepromSpaceEnd;
            }
          }
          text += "<br>";
        }
        eepromSpaceEnd:
        text += "&lt&lt&ltstack<<<";
        readFrom = readFrom + SAVE_CRASH_STACK_TRACE + stackLength;
      }
      int16_t writeFrom;
      EEPROM.get(SAVE_CRASH_EEPROM_OFFSET + SAVE_CRASH_WRITE_FROM, writeFrom);
      EEPROM.end();
    
      // is there free EEPROM space avialable to save data for next crash?
      if (writeFrom + SAVE_CRASH_STACK_TRACE > SAVE_CRASH_SPACE_SIZE)
      {
        text += "<br>No more EEPROM space available to save crash information!<br>";
      }
      else
      {
        sprintf(tmp, "<br>EEPROM space available: 0x%04x bytes<br>", SAVE_CRASH_SPACE_SIZE - writeFrom);
        text += tmp;
      }
      server.send(200, "text/html", text);
      yield();
    }
      
    void http_clear_crash_info() {
      EEPROM.begin(SAVE_CRASH_EEPROM_OFFSET + SAVE_CRASH_SPACE_SIZE);
      // clear the crash counter
      EEPROM.write(SAVE_CRASH_EEPROM_OFFSET + SAVE_CRASH_COUNTER, 0);
      EEPROM.end();
      SendLog("INFO: Crash info cleared.");
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);
    }
