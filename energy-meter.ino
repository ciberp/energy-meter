    #include <ESP8266WiFi.h>
    #include <WiFiClient.h>
    #include <ESP8266WebServer.h>
    #include <EEPROM.h>
    #include <WiFiUdp.h>
    #include <ntp.h>
    #include <Time.h>
    #include <ESP8266HTTPClient.h>
    #include <MQTTClient.h>
    #include "EspSaveCrash.h"
    #include <ArduinoJson.h>
    #include <AddrList.h>
    #include "Statistic.h"
  
    
    static const char CSS[] PROGMEM =
    "<style>"
    "@import url(http://fonts.googleapis.com/css?family=Roboto:400,500,700,300,100);"
    "html {"
    "font-family: \"Roboto\", helvetica, arial, sans-serif;"
    "text-rendering: optimizeLegibility;"
    "font-weight: 400 !important;"
    "font-size: 100% !important;"
    "}"
    ".f9{"
    "max-width: 95%;"
    "background: #FAFAFA;"
    "padding: 10px;"
    "margin: 10px auto;"
    "box-shadow: 1px 1px 25px rgba(0, 0, 0, 0.35);"
    "border-radius: 10px;"
    "border: 3px solid #305A72;"
    "}"
    ".set a:link, a:visited {"
    "position: absolute;"
    "left: 96%;"
    "top: 2%;"
    "font-size: 150%;"
    "text-decoration: none;"
    "color: #305A72;"
    "}"
    ".f9 ul{"
    "padding:0;"
    "margin:0;"
    "list-style:none;"
    "}"
    ".f9 ul li{"
    "display: block;"
    "margin-bottom: 10px;"
    "min-height: 35px;"
    "}"
    ".f9 ul li  .field-style{"
    "box-sizing: border-box;"
    "-webkit-box-sizing: border-box;"
    "-moz-box-sizing: border-box;"
    "padding: 8px;"
    "outline: none;"
    "border: 1px solid #B0CFE0;"
    "}.f9 ul li  .field-style:focus{"
    "box-shadow: 0 0 5px #B0CFE0;"
    "border:1px solid #B0CFE0;"
    "}"
    ".f9 ul li .field-full{"
    "width: 100%;"
    "}"
    ".f9 ul li .field-split{"
    "font-size: 100%;"
    "}"
    ".f9 ul li input.align-left{"
    "float:left;"
    "}"
    ".f9 ul li input.align-right{"
    "float:right;"
    "}"
    ".f9 ul li textarea{"
    "width: 100%;"
    "height: 100px;"
    "}"
    ".g {"
    "-moz-box-shadow: inset 0px 1px 0px 0px #3985B1;"
    "-webkit-box-shadow: inset 0px 1px 0px 0px #3985B1;"
    "box-shadow: inset 0px 1px 0px 0px #3985B1;"
    "background-color: #216288;"
    "border: 1px solid #17445E;"
    "display: inline-block;"
    "cursor: pointer;"
    "color: #FFFFFF;"
    "padding: 8px 18px;"
    "text-decoration: none;"
    "font: 12px Arial, Helvetica, sans-serif;"
    "width: 33%;"
    "height: 8%;"
    "margin: 2px;"
    "}"
    "#div {"
    "width: 100%;"
    "}"
    ".cloud {"
    "background: #F1F1F1;"
    "border-radius: 0.4em;"
    "-moz-border-radius: 0.4em;"
    "-webkit-border-radius: 0.4em;"
    "color: #333;"
    "display: inline-block;"
    "margin-right: 5px;"
    "text-align: center;"
    "margin-bottom: 10px;"
    "border:1px solid #B0CFE0;"
    "padding: 3px 3px;"
    "min-width: 10.1%; "
    "}"
    ".f9 ul li input[type=\"submit\"], {"
    "-moz-box-shadow: inset 0px 1px 0px 0px #3985B1;"
    "-webkit-box-shadow: inset 0px 1px 0px 0px #3985B1;"
    "box-shadow: inset 0px 1px 0px 0px #3985B1;"
    "background-color: #216288;"
    "border: 1px solid #17445E;"
    "display: inline-block;"
    "cursor: pointer;"
    "color: #FFFFFF;"
    "padding: 8px 18px;"
    "text-decoration: none;"
    "font: 12px Arial, Helvetica, sans-serif;"
    "font-size: 250% !important;"
    "}"
    ".f9 ul li input[type=\"button\"]:hover,"
    ".f9 ul li input[type=\"submit\"]:hover {"
    "background: linear-gradient(to bottom, #2D77A2 5%, #337DA8 100%);"
    "background-color: #28739E;"
    "}"
    "input[type=\"text\"], select, .d {"
    "font-size: 100% !important;"
    "}"
    "table {"
    "border:1px solid #B0CFE0;"
    "color: #333;"
    "width: 100%;"
    "margin-bottom: 10px;"
    "font-size: 100% !important;"
    "}"
    "td, th {"
    "border: 1px solid transparent;"
    "height: 8%; "
    "}"
    "th {"
    "font-weight: bold;"
    "}"
    "td {"
    "background: #FAFAFA;"
    "text-align: center;"
    "}"
    "th {"
    "background: #DFDFDF;"
    "font-weight: bold;"
    "}"
    "td {  "
    "background: #FAFAFA;"
    "text-align: center;"
    "}"
    "@media screen and (max-width: 999px) {"
    "html {"
    "font-size: 250% !important;"
    "}"
    ".f9 ul li .field-split{"
    "font-size: 250% !important;"
    "}"
    ".g {"
    "font-size: 66% !important;"
    "width: 32.5% !important;"
    "height: 150px !important;"
    "}"
    ".cloud {"
    "min-width: 31.8% !important; "
    "}"
    "table {"
    "font-size: 40% !important;"
    "}"
    "}"
    "</style>";

    //static const char static_html_handle_show[] PROGMEM =
    String  static_html_handle_show =
    "</head><body class=\"f9\">"
    "<script type=\"text/javascript\">function loadContent(){var xmlhttp;if (window.XMLHttpRequest){xmlhttp = new XMLHttpRequest();}xmlhttp.onreadystatechange = function(){\n"
    "if (xmlhttp.readyState == XMLHttpRequest.DONE ){if(xmlhttp.status == 200){document.getElementById(\"showdiv\").innerHTML = xmlhttp.responseText;\n"
    "setTimeout(loadContent, 200);}}}\n"
    "xmlhttp.open(\"GET\", \"/showdiv\", true); xmlhttp.send(); } loadContent(); </script>\n"
    "<div id=\"showdiv\">Checking ...</div><br>";

    //static const char static_html_handle_root[] PROGMEM =
    String static_html_handle_root =
    "</head><body class=\"f9\">"
    "<script type=\"text/javascript\">function loadContent(){var xmlhttp;if (window.XMLHttpRequest){xmlhttp = new XMLHttpRequest();}xmlhttp.onreadystatechange = function(){\n"
    "if (xmlhttp.readyState == XMLHttpRequest.DONE ){if(xmlhttp.status == 200){document.getElementById(\"div\").innerHTML = xmlhttp.responseText;\n"
    "setTimeout(loadContent, 200);}}}\n"
    "xmlhttp.open(\"GET\", \"/div\", true); xmlhttp.send(); } loadContent(); </script>\n"
    "<span id=\"div\">Checking ...</span><br>";


    String data_collector_type_1 = "influxdb_tcp";
    String data_collector_type_2 = "carbon_udp";
    
//        REASON_DEFAULT_RST      = 0,   /* normal startup by power on */
//        REASON_WDT_RST         = 1,   /* hardware watch dog reset */
//        REASON_EXCEPTION_RST   = 2,   /* exception reset, GPIO status won’t change */
//        REASON_SOFT_WDT_RST      = 3,   /* software watch dog reset, GPIO status won’t change */
//        REASON_SOFT_RESTART    = 4,   /* software restart ,system_restart , GPIO status won’t change */
//        REASON_DEEP_SLEEP_AWAKE   = 5,   /* wake up from deep-sleep */
//        REASON_EXT_SYS_RST      = 6      /* external system reset */ - See more at: http://www.esp8266.com/viewtopic.php?f=32&t=13388#sthash.Pi59f7lq.dpuf 
    
    WiFiUDP udp;
    unsigned int localPort = 2390;
    // default IP, povozijo ga nastavitve iz EEPROM-a
    IPAddress syslogServer(0, 0, 0, 0);
    String Build = "20210112";

    // TODO 
    // dodaj izracun EUR pri vnosu za nazaj, da vpise izracunan price
    // popravi show izpis, tako da je vsak mesec v svoji vrstici aka <br>
    // ce je izracun eur, pravilen, daodaj zapis v influx se price
    // v json dodaj pricenow, priceday in price calculated month, preimenuj pulse -> pulsecounter, 
    // zamenjaj mqtt library
    

    // ntp
    time_t getNTPtime(void);
    NTP NTPclient;
    //#define GMT +0 //timezone
        
    //ADC_MODE(ADC_VCC);
    unsigned long wifi_connect_previous_millis = 0;
    
    ESP8266WebServer server(80);
    //const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
    const char* serverIndex = "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
    "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
        "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
    "<div id='prg'>progress: 0%</div>"
    "<script>"
    "$('form').submit(function(e){"
        "e.preventDefault();"
          "var form = $('#upload_form')[0];"
          "var data = new FormData(form);"
          " $.ajax({"
                "url: '/update',"
                "type: 'POST',"               
                "data: data,"
                "contentType: false,"                  
                "processData:false,"  
                "xhr: function() {"
                    "var xhr = new window.XMLHttpRequest();"
                    "xhr.upload.addEventListener('progress', function(evt) {"
                        "if (evt.lengthComputable) {"
                            "var per = evt.loaded / evt.total;"
                            "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
                        "}"
                   "}, false);"
                   "return xhr;"
                "},"                                
                "success:function(d, s) {"    
                    "console.log('success!')"
               "},"
                "error: function (a, b, c) {"
                "}"
              "});"
    "});"
    "</script>";
    
    // Tell it where to store your config data in EEPROM
    #define EEPROM_START 0     // začetek  
    #define EEPROM_SIZE  3071  // velikost za EPROM

    // Sizes
    #define StringLenghtNormal 25
    #define StringLenghtURL 100
    #define StringLenghtIP 15
    #define data_collector_Max_Clients 5
    
    #define Minimum_update_interval 5 // http post & domoticz update & MQTT & influxDB
    #define Not_used_pin_number 100
    #define LoadDefaultNumber 2021
    
    typedef struct {
      // wifi
      char ssid[StringLenghtNormal];
      char password[StringLenghtNormal];
      char ap_ssid[StringLenghtNormal];
      char ap_password[StringLenghtNormal];
      char web_user[32];
      char web_pass[32];
      int wifi_outpwr;
      char css_link[StringLenghtURL];
      char syslog[StringLenghtIP];
      char NTP_hostname[StringLenghtNormal];
      int  timezone_GMT;
      int defaults;
      char hostname[StringLenghtNormal];
      boolean mqtt_enabled; 
      char mqtt_server[StringLenghtURL];
      int mqtt_port;
      int mqtt_interval;
      char mqtt_key[StringLenghtNormal];
      char mqtt_secret[StringLenghtNormal];
      int data_collector_num_configured;
      char data_collector_host[data_collector_Max_Clients][StringLenghtNormal];
      int data_collector_port[data_collector_Max_Clients];
      char data_collector_db_name[data_collector_Max_Clients][StringLenghtNormal];
      char data_collector_db_username[data_collector_Max_Clients][StringLenghtNormal];
      char data_collector_db_password[data_collector_Max_Clients][StringLenghtNormal];
      int data_collector_push_interval[data_collector_Max_Clients];
      int data_collector_type[data_collector_Max_Clients];
      int pulse_start_value;
      int calc_pulse;
      int calc_kwh;
      float price_kwh;
      float price_eur;
      int reset_day;
      int reset_hour;
      float pulse_avg_perc_val;
      boolean reset_enabled;
      char webhook1_url[150];
      int webhook1_interval;
      boolean webhook1_enabled; 
      char webhook2_url[150];
      int webhook2_interval;
      boolean webhook2_enabled;
           
    } ObjectSettings;
    ObjectSettings SETTINGS;

    // HTML back url for root and settings
    String BackURLSystemSettings="<meta http-equiv=\"refresh\" content=\"0;url=/settings\">";
    //String BackURLShow="<meta http-equiv=\"refresh\" content=\"0;url=/show\">";
    String BackURLRoot = "<meta http-equiv=\"refresh\" content=\"0;url=/\">";
    
    // main loop interval
    unsigned long previousMillis = 0;
    unsigned long previousMillisMinis = 0;
    unsigned long previousMQTTReconnect = 0;
    unsigned long previousMQTTInterval = 0;
    unsigned long previousNTPReconnect = 0;
    unsigned long previousDataCollector[data_collector_Max_Clients];
    unsigned long previousWebhook1Interval = 0;
    unsigned long previousWebhook2Interval = 0;

    //mqtt
    WiFiClient MQTT_Client;        // http
    MQTTClient mqtt_client;
    
    // pins, 
    // ! pazi ob boot-u, normal mode: D8=LOW, D4=HIGH, D3=HIGH  
    // D0  = 16  = PWM Motor 3x
    // D1  =  5  = BME280 (I2C SCL)
    // D2  =  4  = BME280 (I2C SDA)
    // D3! =  0  = PWM Motor 1x
    // D4! =  2  = Servo Inner
    // D5  = 14  = DS18B20
    // D6  = 12  = Rele 1
    // D7  = 13  = Servo Ext
    // D8! = 15  = Rele 2
    // D9  =  3  = DHT22 1
    // D10 =  1  = DHT22 2

    // time
    long days = 0;
    long hours = 0;
    long mins = 0;
    unsigned long secs = 0;
    unsigned long uptime = 0;
    unsigned long previousMillisUptime = 0;
    unsigned long previousPulseCheckMillis = 0;

    // analog read 0-1024
    int AvgAnalogValue = 0;
    int MaxAnalogValue = 0;
    unsigned long PulseCounter = 0;
    unsigned long PulseCounterPrevious = 0;
    unsigned long PulseCounterErrorCheck = 0;
    Statistic AnalogValueStats(100); 
    float Wh = 0; // izracunan podatek za Wh
    float previous_Wh = 0;
    unsigned long ntp_previous_seconds = 0;
    float Watts = 0;  // izracunan podatek za Watts
    float WattsMax = 0;
    boolean PulseLock = true;
    int PulseCounterErrorState = 0;
    boolean PulseCheckLock = true;
    boolean ResetCountersLock = true;
    int year_stamp[13];
    float Wh_year[13];
    float WhDayAVG_year[13];
    float WattsAVG_year[13];
    float avg_watts;
    float avg_wh_day;
    float avg_price;
    float avg_price_day;
    float avg_price_month;
        
    void setup(void)
    {
      Serial.begin(115200);
      loadConfig();
      if (SETTINGS.defaults != LoadDefaultNumber) {
        handle_load_defaults();
      }
      ConnectToWifi();
      udp.begin(localPort);
      server.on("/espupdate", HTTP_GET, [](){
        //server.sendHeader("Connection", "close");
        //server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/html", serverIndex);
      });
      server.on("/update", HTTP_POST, [](){
        server.sendHeader("Connection", "close");
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/plain", (Update.hasError())?"FAIL":"OK");
        ESP.restart();
      },[](){
        HTTPUpload& upload = server.upload();
        if(upload.status == UPLOAD_FILE_START){
          //Serial.setDebugOutput(true);
          //WiFiUDP::stopAll();
          //Serial.printf("Update: %s\n", upload.filename.c_str());
          String TempString = " UPDATE: ESPupdate, uploading file ";
          TempString += upload.filename.c_str();
          SendLog(TempString);
          uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
          if(!Update.begin(maxSketchSpace)){//start with max available size
            //Update.printError(Serial);
            SendLog("UPDATE: Error no space...");
          }
        } else if(upload.status == UPLOAD_FILE_WRITE){
          if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
            //Update.printError(Serial);
            SendLog("UPDATE: Error upload file write...");
          }
        } else if(upload.status == UPLOAD_FILE_END){
          if(Update.end(true)){ //true to set the size to the current progress
            //Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
            SendLog("UPDATE: Success, Rebooting... ");
          } else {
            //Update.printError(Serial);
            SendLog("UPDATE: Error upload file end...");
          }
          //Serial.setDebugOutput(false);
        }
        yield();
      });
      server.begin();
      server.on("/cfg", http_dump_settings_json);
      server.on("/pulse", http_pulse);
      server.on("/", handle_root);
      server.on("/div",http_root_refresh);
      server.on("/scan", http_WiFi_Scan);
      server.on("/stat", http_Status);
      server.on("/clearcrash", http_clear_crash_info);
      server.on("/crashinfo", http_crash_info);
      server.on("/ntpupdate", init_ntp);
      yield();
      server.on("/flxadd", []() {
        if (SETTINGS.data_collector_num_configured < 0 || SETTINGS.data_collector_num_configured > data_collector_Max_Clients) {
          SETTINGS.data_collector_num_configured = 0;
        }
        if (SETTINGS.data_collector_num_configured <= data_collector_Max_Clients) {
          strcpy (SETTINGS.data_collector_host[SETTINGS.data_collector_num_configured], server.arg("flxho").c_str());
          SETTINGS.data_collector_port[SETTINGS.data_collector_num_configured] = server.arg("flxpo").toInt();
          strcpy (SETTINGS.data_collector_db_name[SETTINGS.data_collector_num_configured], server.arg("flxdn").c_str());
          strcpy (SETTINGS.data_collector_db_username[SETTINGS.data_collector_num_configured], server.arg("flxdu").c_str());
          strcpy (SETTINGS.data_collector_db_password[SETTINGS.data_collector_num_configured], server.arg("flxdp").c_str());
          if (server.arg("flxin").toInt() < Minimum_update_interval) {
            SETTINGS.data_collector_push_interval[SETTINGS.data_collector_num_configured] = Minimum_update_interval;
          }
          else {
            SETTINGS.data_collector_push_interval[SETTINGS.data_collector_num_configured] = server.arg("flxin").toInt(); 
          }          
          SETTINGS.data_collector_type[SETTINGS.data_collector_num_configured] = server.arg("flxty").toInt();
          SETTINGS.data_collector_num_configured++;
        }
        server.send(200, "text/html", BackURLSystemSettings);
      });
      yield();
      server.on("/setpls", []() {
        if (server.arg("plsnum").toInt() > 0) {
          PulseCounter = server.arg("plsnum").toInt();
          web_set_counter();
        }
        server.send(200, "text/html", BackURLSystemSettings);
        yield();        
      });
      server.on("/flxrem", http_data_collector_remove_client);
      server.on("/show", handle_show);
      server.on("/showdiv",http_show_refresh);
      server.on("/rstmax", handle_reset_max_value);
      server.on("/rstavg", handle_reset_avg_value);
      server.on("/json",handle_json);
      server.on("/default",http_handle_load_defaults);
      server.on("/settings",handle_Settings);
      server.on("/reboot",handle_reboot);
      server.on("/save",saveConfig);
      server.on("/commit", []() {
        strcpy (SETTINGS.ssid, server.arg("ssid").c_str());        
        strcpy (SETTINGS.password, server.arg("pass").c_str());
        strcpy (SETTINGS.ap_ssid, server.arg("ap_ssid").c_str());        
        strcpy (SETTINGS.ap_password, server.arg("ap_pass").c_str());
        strcpy (SETTINGS.web_user, server.arg("w_user").c_str());        
        strcpy (SETTINGS.web_pass, server.arg("w_pass").c_str());
        if (server.arg("w_pwr").toInt() < 0 or server.arg("w_pwr").toInt() > 20) {
          SETTINGS.wifi_outpwr = 5;
        } else {
          SETTINGS.wifi_outpwr = server.arg("w_pwr").toInt();
        }
        if (server.arg("papc").toInt() < 2 or server.arg("papc").toInt() > 99) {
          SETTINGS.pulse_avg_perc_val = 18;
        } else {
          SETTINGS.pulse_avg_perc_val = server.arg("papc").toInt();
        }
        strcpy (SETTINGS.css_link, server.arg("css").c_str());
        strcpy (SETTINGS.syslog, server.arg("syslog").c_str());
        strcpy (SETTINGS.NTP_hostname, server.arg("ntp").c_str());
        SETTINGS.timezone_GMT = server.arg("gmt").toInt();  
        strcpy (SETTINGS.hostname, server.arg("hostname").c_str());
        // mqtt
        if (server.arg("mqena").toInt() == 1) {
          SETTINGS.mqtt_enabled = true;        
        }
        else {
          SETTINGS.mqtt_enabled = false;
        }
        strcpy (SETTINGS.mqtt_server, server.arg("mqbr").c_str());
        SETTINGS.mqtt_port = server.arg("mqprt").toInt();
        if (server.arg("mqint").toInt() < Minimum_update_interval) {
          SETTINGS.mqtt_interval = Minimum_update_interval;
        }
        else {
          SETTINGS.mqtt_interval = server.arg("mqint").toInt(); 
        }
        strcpy (SETTINGS.mqtt_key, server.arg("mqkey").c_str());
        strcpy (SETTINGS.mqtt_secret, server.arg("mqsec").c_str());
        // energy-meter settings
        if (server.arg("psv").toInt() < 0) {
          SETTINGS.pulse_start_value = 65;
        }
        else {
          SETTINGS.pulse_start_value = server.arg("psv").toInt(); 
        }
        if (server.arg("pcp").toInt() < 0) {
          SETTINGS.calc_pulse = 3;
        }
        else {
          SETTINGS.calc_pulse = server.arg("pcp").toInt(); 
        }
        if (server.arg("pcw").toInt() < 0) {
          SETTINGS.calc_kwh = 25;
        }
        else {
          SETTINGS.calc_kwh = server.arg("pcw").toInt(); 
        }
        if (server.arg("prc").toFloat() < 0) {
          SETTINGS.price_kwh = 10;
        }
        else {
          SETTINGS.price_kwh = server.arg("prc").toFloat(); 
        }
        if (server.arg("eur").toFloat() < 0) {
          SETTINGS.price_eur = 20;
        }
        else {
          SETTINGS.price_eur = server.arg("eur").toFloat(); 
        }
        // reset counters
        if (server.arg("rena").toInt() == 1) {
          SETTINGS.reset_enabled = true;        
        }
        else {
          SETTINGS.reset_enabled = false;
        }
        if (server.arg("rday").toInt() < 1 || server.arg("rday").toInt() > 31) {
          SETTINGS.reset_day = 1;
        }
        else {
          SETTINGS.reset_day = server.arg("rday").toInt(); 
        }
        if (server.arg("rhour").toInt() < 0 || server.arg("rhour").toInt() > 59) {
          SETTINGS.reset_hour = 0;
        }
        else {
          SETTINGS.reset_hour = server.arg("rhour").toInt(); 
        }
        //webhook
        strcpy (SETTINGS.webhook1_url, server.arg("whu1").c_str());
        SETTINGS.webhook1_interval = server.arg("whi1").toInt();
        if (SETTINGS.webhook1_interval < 1) {
          SETTINGS.webhook1_interval = 1;
        }
        if (server.arg("whe1").toInt() == 1) {
          SETTINGS.webhook1_enabled = true;        
        }
        else {
          SETTINGS.webhook1_enabled = false;
        }
        strcpy (SETTINGS.webhook2_url, server.arg("whu2").c_str());
        SETTINGS.webhook2_interval = server.arg("whi2").toInt();
        if (SETTINGS.webhook2_interval < 1) {
          SETTINGS.webhook2_interval = 1;
        }
        if (server.arg("whe2").toInt() == 1) {
          SETTINGS.webhook2_enabled = true;        
        }
        else {
          SETTINGS.webhook2_enabled = false;
        }
        // end
        server.send(200, "text/html", BackURLSystemSettings);
        Set_Syslog_From_Settings();
        init_ntp();
        saveConfig();
        });
      yield();
      // set history values
      server.on("/setmth", []() {
        if (server.arg("engnum").toInt() > 0 && server.arg("whdavgnum").toInt() > 0) {
          Wh_year[server.arg("month").toInt()] = server.arg("engnum").toInt();
          WhDayAVG_year[server.arg("month").toInt()] = server.arg("whdavgnum").toInt();
          WattsAVG_year[server.arg("month").toInt()] = server.arg("pwravgnum").toInt();
          year_stamp[server.arg("month").toInt()] = server.arg("year").toInt();
        }
        server.send(200, "text/html", BackURLSystemSettings);
      });
      yield();
      server.on("/resetcounter",UserResetCounters);
      server.on("/enapsta",handle_enable_ap_sta);
      server.on("/disap",handle_disable_ap);
      server.on("/recon",ConnectToWifi);    
      server.begin();
      mqtt_init();
      init_ntp();
      init_history_measures();
      String BootText;
      BootText += "BOOT: Time " + String(millis()) + "ms";
      BootText += " Firmware ver.: " + Build + "-" + ESP.getFullVersion();
      BootText += " BootMode: " + String(ESP.getBootMode());
      BootText += " BootVersion: " + String(ESP.getBootVersion());
      BootText += " FreeHeap: " + String(ESP.getFreeHeap());
      BootText += " CPUFreqMhz: " + String(ESP.getCpuFreqMHz());
      BootText += " ResetInfo: " + String(ESP.getResetInfo());
      BootText += " Time/Date:" + show_time() + " " + show_date();
      SendLog(BootText);
      http_WiFi_Scan();
    }

    void loop(void) {
      // MAIN LOOP //
      server.handleClient();
      yield();
      data_collector_send_data();
      yield();
      mqtt_loop();
      yield();
      mqtt_send_data();
      yield();
      http_webhook1_client_loop();
      yield();
      http_webhook2_client_loop();
      if (millis() - previousMillisUptime >= 1000) {
        previousMillisUptime = millis();
        uptime++;
      }
      if (millis() - previousPulseCheckMillis >= 600000) {
        previousPulseCheckMillis = millis();
        yield();
        // tole se izvaja na 10min
        CheckPulseForError_loop();
      }
      if (millis() - previousMillis >= 50) {
        previousMillis = millis();
        yield();
        // tole se izvaja na 50msec
        read_sensor_loop();
        calculate_Wh_loop();
        ResetCounters_loop();
        calculate_avg_energy_per_day_loop();
        calculate_price_loop();
      }
      if (now() - ntp_previous_seconds >= 300) {
        ntp_previous_seconds = now();
        yield();
        // tole se izvaja na 5min
        calculate_Watts_loop();
        calculate_avg_watts_loop();
      }     
    }


    
    void http_Status() {
      SendLog("INFO: View status.");
      String html = "<!DOCTYPE html><html><head>";
      html += "<title>status</title>";  
      html += "<meta charset=\"UTF-8\">\n";
      html += "Wifi.status=";
      html += String(WiFi.status());
      html += "<br>";
      server.send(200, "text/html", html);
    }

    byte String_To_Byte(String MyString) {
      byte MyByte;
      char temp [25];
      MyString.toCharArray(temp, 8);
      char* pos = temp;
      MyByte = strtol(pos, &pos, 8);
      return MyByte;
    }

    




    String urlencode(String str)
    {
        String encodedString="";
        char c;
        char code0;
        char code1;
        char code2;
        for (int i =0; i < str.length(); i++){
          c=str.charAt(i);
          if (c == ' '){
            encodedString+= '+';
          } else if (isalnum(c)){
            encodedString+=c;
          } else{
            code1=(c & 0xf)+'0';
            if ((c & 0xf) >9){
                code1=(c & 0xf) - 10 + 'A';
            }
            c=(c>>4)&0xf;
            code0=c+'0';
            if (c > 9){
                code0=c - 10 + 'A';
            }
            code2='\0';
            encodedString+='%';
            encodedString+=code0;
            encodedString+=code1;
            //encodedString+=code2;
          }
          yield();
        }
        return encodedString;
    }


   
    void handle_reboot() {
      if (!server.authenticate(SETTINGS.web_user, SETTINGS.web_pass)) {
        return server.requestAuthentication();
      }
      SendLog("INFO: Restart requested."); 
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);
      delay(300);
      ESP.restart();
    }
      
    // cifra v mesec 1 = Januar, ...
    String Number_to_month_name(int month_number) {     
      switch (month_number) {
        case 1:
          return "Januar";
        case 2:
          return "Februar";
        case 3:
          return "March";
        case 4:
          return "April";
        case 5:
          return "May";
        case 6:
          return "June";
        case 7:
          return "July";
        case 8:
          return "August";
        case 9:
          return "September";
        case 10:
          return "October";
        case 11:
          return "November";
        case 12:
          return "December";
      }
    }
    

    
