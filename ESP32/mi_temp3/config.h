// here are all non-public data 

char* LYWSD03MMC_ADDR[] = 
{
    "a4:c1:38:3c:e4:e2",
    "a4:c1:38:76:97:b3"
};

static int deviceCount = sizeof LYWSD03MMC_ADDR / sizeof LYWSD03MMC_ADDR[0];

BLEClient* pClient;

const char*   WIFI_SSID       = "<Your SSID>";
const char*   WIFI_PASSWORD   = "<Your password>";

const char*   MQTT_HOST       = "broker.hivemq.com";
const int     MQTT_PORT       = 1883;
const char*   MQTT_CLIENTID   = "mi-temp-client";
const char*   MQTT_USERNAME   = "<Your user name>";
const char*   MQTT_PASSWORD   = "<Your password>";
const String  MQTT_BASE_TOPIC = "LYWSD03MMC"; 
const int     MQTT_RETRY_WAIT = 5000;

// sleep between to runs in seconds 30*60=30min
#define SLEEP_DURATION 30 * 10 // Sleep for 5 min
// emergency hibernate countdown in seconds
#define EMERGENCY_HIBERNATE 3 * 60
// how often should the battery be read - in run count
#define BATTERY_INTERVAL 6
// how often should a device be retried in a run when something fails
#define RETRY 3
