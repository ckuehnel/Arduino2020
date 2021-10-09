// here are all non-public data 

char* LYWSD03MMC_ADDR[] = 
{
    "a4:c1:38:3c:e4:e2",
    "a4:c1:38:76:97:b3"
};

static int deviceCount = sizeof LYWSD03MMC_ADDR / sizeof LYWSD03MMC_ADDR[0];

BLEClient* pClient;

// sleep between to runs in seconds 30*60=30min
#define SLEEP_DURATION 30 * 10 // Sleep for 5 min
// emergency hibernate countdown in seconds
#define EMERGENCY_HIBERNATE 3 * 60
// how often should the battery be read - in run count
#define BATTERY_INTERVAL 6
// how often should a device be retried in a run when something fails
#define RETRY 3
