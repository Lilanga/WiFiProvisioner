#ifndef WIFIPROVISIONER_H
#define WIFIPROVISIONER_H

#include <Arduino.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <WebServer.h>

namespace WiFiProvisioner
{

  typedef bool (*InputCheckCallback)(const String &);
  typedef void (*FactoryResetCallback)();
  typedef void (*OnProvisionCallback)();

  class WiFiProvisioner
  {
  public:
    WiFiProvisioner();
    ~WiFiProvisioner();
    String AP_NAME = "";
    String AP_PREFIX = "ESP32";
    String HTML_TITLE = "Welcome to Wifi Provision";
    String THEME_COLOR = "dodgerblue";
    String SVG_LOGO = R"rawliteral(
  <svg id="Layer_1" version="1.1" viewBox="0 0 512 512" width="5rem" height="5rem" preserveAspectRatio="xMidYMid meet">
  <style type="text/css">
	.st0{fill:#1B3954;}
	.st1{fill:#16ADE1;}
</style>
<g><g><path class="st0" d="M190.4,308.5h-108c19.5,28.2,51.8,59.7,104.5,93.8c21.5,13.9,46,20.8,70.6,20.8c2.3,0,4.7-0.1,7-0.2    l-54.3-154.4L190.4,308.5z"/><path class="st0" d="M320.8,302l-54,120.8c21.3-1.5,42.4-8.4,61.1-20.5c44.3-28.7,73.9-55.5,93.6-80.2h-90.6L320.8,302z"/><g><polygon class="st1" points="363.5,269.5 318.1,178.9 271.2,283.8 218.1,132.7 157.6,255.9 24.4,255.9 24.4,294.5 181.7,294.5      212.2,232.4 267,388.1 320.1,269.2 339.6,308.1 487.6,308.1 487.6,269.5    "/></g><path class="st0" d="M148.9,241.9l71.3-145.3l52.6,149.5l44.6-99.9l54.7,109.4h83.5c3-12.8,3.8-24.3,3.8-34.5    c0-3.6-0.2-7.2-0.5-10.8c-0.2,0-0.5,0-0.7,0l-49.3,0.4c0,0,0,0-0.1,0c-3.8,0-7-3.1-7-6.9c0-3.9,3.1-7,6.9-7.1l48.2-0.4    c-11.6-59.9-64.3-105.2-127.6-105.2c-26.5,0-51.1,7.9-71.6,21.5c-21.1-14.9-46.9-23.7-74.8-23.7c-71.8,0-130,58.2-130,130    c0,7,0.4,14.7,1.6,23H148.9z M376.5,202.1c0.1-0.4,0.2-0.9,0.4-1.3c0.2-0.4,0.4-0.8,0.6-1.2c0.3-0.4,0.5-0.7,0.9-1.1    c0.3-0.3,0.7-0.6,1.1-0.9c0.4-0.3,0.8-0.5,1.2-0.6c0.4-0.2,0.9-0.3,1.3-0.4c0.9-0.2,1.8-0.2,2.7,0c0.4,0.1,0.9,0.2,1.3,0.4    c0.4,0.2,0.8,0.4,1.2,0.6c0.4,0.3,0.7,0.6,1.1,0.9c1.3,1.3,2,3.1,2,4.9c0,0.5,0,0.9-0.1,1.4c-0.1,0.4-0.2,0.9-0.4,1.3    c-0.2,0.4-0.4,0.8-0.6,1.2c-0.3,0.4-0.5,0.7-0.9,1.1c-0.3,0.3-0.7,0.6-1.1,0.9c-0.4,0.3-0.8,0.5-1.2,0.6c-0.4,0.2-0.9,0.3-1.3,0.4    c-0.5,0.1-0.9,0.1-1.4,0.1c-0.5,0-0.9,0-1.4-0.1c-0.4-0.1-0.9-0.2-1.3-0.4c-0.4-0.2-0.8-0.4-1.2-0.6c-0.4-0.3-0.7-0.5-1.1-0.9    c-0.3-0.3-0.6-0.7-0.9-1.1c-0.3-0.4-0.5-0.8-0.6-1.2c-0.2-0.4-0.3-0.9-0.4-1.3c-0.1-0.5-0.1-0.9-0.1-1.4    C376.3,203,376.4,202.5,376.5,202.1z"/></g></g></svg>
)rawliteral";
    String PROJECT_TITLE = "BitsFactory IoT";
    String PROJECT_INFO = "Select a network to connect your device";
    String INPUT_TEXT = "Verification Code";
    String INPUT_PLACEHOLDER = "Enter verification code";
    String INPUT_LENGTH = "";
    String FOOTER_INFO = "Copyright © 2024";
    String INPUT_INVALID_LENGTH = "Invalid input length";
    String INPUT_NOT_VALID = "Invalid verification code";
    String CONNECTION_SUCCESSFUL = "Connection successful, device will proceed with "
                                   "a successful connection.";
    String RESET_CONFIRMATION_TEXT =
        "This will unlink the device from your account, and you will need to "
        "re-provision it as a new device.";

    void connectToWiFi();
    void setupAccessPointAndServer();
    void resetCredentials();
    void setInputCheckCallback(InputCheckCallback callback);
    void setFactoryResetCallback(FactoryResetCallback callback);
    void setOnProvisionCallback(OnProvisionCallback callback);
    void setConnectionTimeout(unsigned int timeout);
    void setShowInputField(bool value);
    void setRestartOnSuccess(bool value);
    void enableSerialDebug(bool enable);
    String getNetworkSSID();
    String getNetworkKey();

  private:
    void serverLoop();
    void releaseResources();
    void stopServerLoop(bool value);
    void serveRootPage();
    void handleRootRequest();
    void handleUpdateRequest();
    void handleConfigureRequest();
    void connectToWiFiNetwork(const String &ssid, const String &password);
    void handleSuccessfulConnection(bool codeok, const String &ssid,
                                    const String &password);
    void handleUnsuccessfulConnection(const String &ssid);
    void resetToFactorySettings();
    void sendBadRequestResponse();
    void saveNetworkConnectionDetails(const String &ssid, const String &password);
    bool connectToExistingWiFINetwork();
    void debugPrintln(const char *message);
    void debugPrintln(const String &message);
    int convertRRSItoLevel(int rssi);
    String getAvailableNetworks();
    InputCheckCallback inputCheckCallback;
    FactoryResetCallback factoryResetCallback;
    OnProvisionCallback onProvisionCallback;
    WebServer *m_server;
    DNSServer *m_dns_server;
    Preferences m_preferences;
    IPAddress apIP;
    IPAddress netMsk;
    bool showInputField = false;
    bool restartOnSuccess = false;
    bool stopLoopFlag = false;
    bool serialDebug = false;
    unsigned int connectionTimeout = 1500;
    unsigned int wifiDelay = 100;
    unsigned int newWifiConnectionTimeout = 10000;
    const byte DNS_PORT = 53;
  };

} // namespace WiFiProvisioner

#endif
