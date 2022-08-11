// This sketch provide the functionality of OTA Firmware Upgrade
#include "WiFi.h"
#include "HttpsOTAUpdate.h"
// This sketch shows how to implement HTTPS firmware update Over The Air.
// Please provide your WiFi credentials, https URL to the firmware image and the server certificate.

static const char *ssid     = "Yang Mana Hayo";  // your network SSID (name of wifi network)
static const char *password = "wkwkwklimakali"; // your network password

static const char *url = "https://anonfiles.com/Va47r63ay5/arduino-esp32-ota-s3.ino_bin"; //state url of your firmware image

static const char *server_certificate = "-----BEGIN CERTIFICATE-----\n" \
"MIIFMTCCBBmgAwIBAgISBH/ADhl429XDysDsbDl+TppTMA0GCSqGSIb3DQEBCwUA\n" \
"MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD\n" \
"EwJSMzAeFw0yMjA3MjgyMDUxNTdaFw0yMjEwMjYyMDUxNTZaMBgxFjAUBgNVBAMT\n" \
"DWFub25maWxlcy5jb20wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDF\n" \
"3KaLYg/Ophbd9rdaIg6/kSp9ov8Cn7YlEi8dmTq2UdQFdDtv+lTfjN8VCoSQ2YoT\n" \
"/1+zwqHzJddlsa7+0kmJiFVvWyNb3xmHO2tblSJNohKgJq8hKoMa/Jeyv4bdr4Gb\n" \
"DJM+rfWrrmrP8+q3mon/n6lJUnkaTxzR+5yAeBONGVAM4Lf87UoUhdkdg/vnMmhc\n" \
"N6dYJYcwUZBuXYCIJjfRZstzzYkz9s70u/gf330TYI/YyC4aHQESWQAMS4feq8m0\n" \
"TBiFDptDxZ/S1SgmT/vkmCl5SfcsDqKjYPzQdHeNAOWvkQRiLj86hp4JYqtx6EGw\n" \
"DsIam6WPp217wrCnvK/bAgMBAAGjggJZMIICVTAOBgNVHQ8BAf8EBAMCBaAwHQYD\n" \
"VR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMAwGA1UdEwEB/wQCMAAwHQYDVR0O\n" \
"BBYEFBdY5DI307I60QgrCEMQRH+6aQ+VMB8GA1UdIwQYMBaAFBQusxe3WFbLrlAJ\n" \
"QOYfr52LFMLGMFUGCCsGAQUFBwEBBEkwRzAhBggrBgEFBQcwAYYVaHR0cDovL3Iz\n" \
"Lm8ubGVuY3Iub3JnMCIGCCsGAQUFBzAChhZodHRwOi8vcjMuaS5sZW5jci5vcmcv\n" \
"MCsGA1UdEQQkMCKCDWFub25maWxlcy5jb22CEXd3dy5hbm9uZmlsZXMuY29tMEwG\n" \
"A1UdIARFMEMwCAYGZ4EMAQIBMDcGCysGAQQBgt8TAQEBMCgwJgYIKwYBBQUHAgEW\n" \
"Gmh0dHA6Ly9jcHMubGV0c2VuY3J5cHQub3JnMIIBAgYKKwYBBAHWeQIEAgSB8wSB\n" \
"8ADuAHUAKXm+8J45OSHwVnOfY6V35b5XfZxgCvj5TV0mXCVdx4QAAAGCRsuAmwAA\n" \
"BAMARjBEAiBR7USavbUNOKV8dsxIYdo650qun07lMNVb0A4itgSPxAIgK+nJSRMw\n" \
"X8EIi+T8qsPka0zqz/zW/zoAaUfyt86sekoAdQBvU3asMfAxGdiZAKRRFf93FRwR\n" \
"2QLBACkGjbIImjfZEwAAAYJGy4GfAAAEAwBGMEQCIB8hNBQjd7J3fVA/5Md4cZL5\n" \
"6/ZU5+ISwxObA5UVOO6yAiAU1/5M+zw2hU7Xu0HWbDppjFAgUIW4PzXJegmUmyY9\n" \
"GjANBgkqhkiG9w0BAQsFAAOCAQEArxTpJZNmT0vA6PbiNqZDapm4aCzxUPfmMJZr\n" \
"OwCFqCH0QQNk48Ilkqaax9oHQOXL9G311YCztCffqdM/0nx6M/BEdzORPcMyUkA4\n" \
"ZQJPbZUHgrawKJZaKS7C8eUPUApSsn5Pf2/4yx1AfRiO24nfl/v4tC08IyjbVFjr\n" \
"tMsmHf7TNvCYUJq81A0Wg4jpsjIJLqZ5CZmb4xd8B6/rvlVfmYqAauNCaO4veLp/\n" \
"kpB0XA8tYyK52nRPZJzGmzVcxt6aT8nqZCye7Z9cutES3iZgn7CpsKS8l62OBRHv\n" \
"et9ATOlp+pwRDnAv2IHxaeG9m3rVLjOW0mdly0Kb+K3MF9eh5g==\n" \
"-----END CERTIFICATE-----";

static HttpsOTAStatus_t otastatus;

void HttpEvent(HttpEvent_t *event)
{
    switch(event->event_id) {
        case HTTP_EVENT_ERROR:
            Serial.println("Http Event Error");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            Serial.println("Http Event On Connected");
            break;
        case HTTP_EVENT_HEADER_SENT:
            Serial.println("Http Event Header Sent");
            break;
        case HTTP_EVENT_ON_HEADER:
            Serial.printf("Http Event On Header, key=%s, value=%s\n", event->header_key, event->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            break;
        case HTTP_EVENT_ON_FINISH:
            Serial.println("Http Event On Finish");
            break;
        case HTTP_EVENT_DISCONNECTED:
            Serial.println("Http Event Disconnected");
            break;
    }
}

void setup(){

    Serial.begin(115200);
    Serial.print("Attempting to connect to SSID: ");
    WiFi.begin(ssid, password);

    // attempt to connect to Wifi network:
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }

    Serial.print("Connected to ");
    Serial.println(ssid);
    
    HttpsOTA.onHttpEvent(HttpEvent);
    Serial.println("Starting OTA");
    HttpsOTA.begin(url, server_certificate); 

    Serial.println("Please Wait it takes some time ...");
}

void loop(){
    otastatus = HttpsOTA.status();
    Serial.println(otastatus);
    if(otastatus == HTTPS_OTA_SUCCESS) { 
        Serial.println("Firmware written successfully. To reboot device, call API ESP.restart() or PUSH restart button on device");
    } else if(otastatus == HTTPS_OTA_FAIL) { 
        Serial.println("Firmware Upgrade Fail");
    }
    delay(1000);
}