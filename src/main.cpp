#include <WiFi.h>
#include <AsyncTCP.h>
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include <Logging.h>
#include <ModbusBridgeWiFi.h>
#include <ModbusClientRTU.h>
#include "config.h"
#include "pages.h"
#include "esp_task_wdt.h"
#include <ESPmDNS.h>

#define WDT_TIMEOUT 10 // Timeout in seconds

AsyncWebServer webServer(80);
Config config;
Preferences prefs;
ModbusClientRTU *MBclient;
ModbusBridgeWiFi MBbridge;
WiFiManager wm;

void setup()
{
  debugSerial.begin(115200);
  dbgln();

  dbgln("[config] load")
      prefs.begin("modbusRtuGw");
  config.begin(&prefs);
  debugSerial.end();
  debugSerial.begin(config.getSerialBaudRate(), config.getSerialConfig());

  dbgln("[wifi] start");
  WiFi.mode(WIFI_STA);
  wm.setClass("invert");
  auto reboot = false;
  wm.setAPCallback([&reboot](WiFiManager *wifiManager)
                   { reboot = true; });
  wm.setTimeout(60);
  wm.setConnectRetries(3);
  wm.autoConnect();
  if (reboot)
  {
    ESP.restart();
  }
  // Initialize mDNS
  if (!MDNS.begin(config.getHostname().c_str()))
  { 
    Serial.println("Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }
  dbgln("[wifi] finished");

  dbgln("[modbus] start");
  MBUlogLvl = LOG_LEVEL_WARNING;
  RTUutils::prepareHardwareSerial(modbusSerial);
#if defined(RX_PIN) && defined(TX_PIN)
  // use rx and tx-pins if defined in platformio.ini
  modbusSerial.begin(config.getModbusBaudRate(), config.getModbusConfig(), RX_PIN, TX_PIN);
  dbgln("Use user defined RX/TX pins");
#else
  // otherwise use default pins for hardware-serial2
  modbusSerial.begin(config.getModbusBaudRate(), config.getModbusConfig());
#endif

  MBclient = new ModbusClientRTU(config.getModbusRtsPin());
  MBclient->setTimeout(1000);
  MBclient->begin(modbusSerial, 1);
  for (uint8_t i = 1; i < 248; i++)
  {
    MBbridge.attachServer(i, i, ANY_FUNCTION_CODE, MBclient);
  }
  MBbridge.start(config.getTcpPort(), 10, config.getTcpTimeout());
  dbgln("[modbus] finished");

  setupPages(&webServer, MBclient, &MBbridge, &config, &wm);
  webServer.begin();

  dbgln("[watchdog] timer started");
  esp_task_wdt_init(WDT_TIMEOUT, true); // enable panic so ESP32 restarts
  esp_task_wdt_add(NULL);               // add current thread to WDT watch

  dbgln("[setup] finished");
}

void loop()
{
  // put your main code here, to run repeatedly:
  esp_task_wdt_reset(); // reset WDT
  delay(10);
}