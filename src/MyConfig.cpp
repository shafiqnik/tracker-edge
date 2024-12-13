
#include "MyConfig.h"
#include "config_service.h"
#include "Particle.h"
#include "tracker_location.h"



MyConfig *MyConfig::_instance;



void MyConfig::init() {
    static ConfigObject mineDesc("mine", {
        ConfigInt("contrast", &contrast, 0, 255),
        ConfigFloat("tempLow", &tempLow, -100.0, 200.0),
        ConfigStringEnum(
            "fruit",
            {
                {"apple",  (int32_t) Fruits::APPLE},
                {"grape",  (int32_t) Fruits::GRAPE},
                {"orange", (int32_t) Fruits::ORANGE},
                {"pear",   (int32_t) Fruits::PEAR}
            },
            [this](int32_t &value, const void *context) {
                // Get fruit from class
                value = this->fruit;
                return 0;
            },
            [this](int32_t value, const void *context) {
                // Set fruit in class
                this->fruit = value;
                return 0;
            }
        ),
        ConfigString("message", 
            [this](const char * &value, const void *context) {
                // Get message from class
                value = message.c_str();
                return 0;
            },
            [this](const char * value, const void *context) {
                // Set message in class
                this->message = value;
                Log.info("set message to %s", value);
                return 0;
            }
        ),
        ConfigBool("thing", 
            [this](bool &value, const void *context) {
                // Get thing from class
                value = this->thing;
                return 0;
            },
            [this](bool value, const void *context) {
                // Set thing in class
                this->thing = value;
                Log.info("set thing to %s", value ? "true" : "false");
                return 0;
            }
        )
    });
    ConfigService::instance().registerModule(mineDesc);

    logSettings();
}

void MyConfig::loop() {

    static unsigned long lastLog = 0;
    if (millis() - lastLog >= 10000) {
        lastLog = millis();
        logSettings();
    }
}

void MyConfig::logSettings() {
    Log.info("contrast=%ld tempLow=%lf fruit=%ld message=%s thing=%s", 
        contrast, tempLow, fruit, message.c_str(), thing ? "true" : "false");
} 

// static 
MyConfig &MyConfig::instance() {
    if (!_instance) {
        _instance = new MyConfig();
    }
    return *_instance;
}

void MyConfig::getGPS() { 
    LocationService &location = LocationService::instance();
    LocationPoint point;

    if (location.isLockStable()) { // Ensure GPS lock is stable
        // Fetch the current GPS data
        if (location.getLocation(point)) {
            // Format data into JSON
            char data[128];
            snprintf(data, sizeof(data), "{\"lat\":%f,\"lon\":%f}", point.latitude, point.longitude);

            // Publish data to the cloud
            Particle.publish("gps", data, PRIVATE);
            Log.info("Published GPS data: %s", data);
        } else {
            Log.warn("Failed to retrieve GPS location");
        }
    } else {
        Log.info("GPS lock is not stable yet.");
    }
}


/*** 
//unsigned long scannedTime = 0;

void publishRawBLEMessage(const BleScanResult &scanResult) {

    
    String macAddresses = "";

    LocationService &location = LocationService::instance();
    //LocationPoint point;
    
    String macStr = scanResult.address().toString();
      
    if (macStr.startsWith("AC:23:3F")) {
               
        macAddresses += macStr + ", ";
        //Particle.publish("BLE_Raw dec13a", macStr, PRIVATE);
        //Particle.publish("mac addresses, ", macAddresses, PRIVATE);
        Particle.publish("mac addresses", "Length: " + String(macAddresses.length()) + ", Addresses: " + macAddresses, PRIVATE);
                        
    }

  
}

this code only publishes one tag id at a time
***/

unsigned long startTime = millis();

void publishRawBLEMessage(const BleScanResult &scanResult) {
    static String macAddresses = "";

    if (millis() - startTime < 60000) {
        LocationService &location = LocationService::instance();
        //LocationPoint point;
        
        String macStr = scanResult.address().toString();
          
        if (macStr.startsWith("AC:23:3F")) {
            macAddresses += macStr + ", ";
            //Particle.publish("BLE_Raw dec13a", macStr, PRIVATE);
            //Particle.publish("mac addresses, ", macAddresses, PRIVATE);
            //Particle.publish("mac addresses", "Length: " + String(macAddresses.length()) + ", Addresses: " + macAddresses, PRIVATE);
             Particle.publish("tags: " , macAddresses, PRIVATE);

        }
    } else {
        // Code to run after 30 seconds
        startTime = millis(); // Reset startTime to run again
    }
}

void MyConfig::scanTag() {
    BLE.on();
    BLE.setScanTimeout(60000); // Set scan timeout to 10 seconds
    BLE.scan([](const BleScanResult &scanResult) {
        publishRawBLEMessage(scanResult);
        
    });
    //BLE.off();
    //delay(100000);
  
}






MyConfig::MyConfig() {
}

MyConfig::~MyConfig() {
}


/***
#include "MyConfig.h"
#include "config_service.h"

MyConfig *MyConfig::_instance;

void MyConfig::init() {
    static ConfigObject mineDesc("mine", {
        ConfigInt("contrast", &contrast, 0, 255),
        ConfigFloat("tempLow", &tempLow, -100.0, 200.0),
        ConfigStringEnum(
            "fruit",
            {
                {"apple",  (int32_t) Fruits::APPLE},
                {"grape",  (int32_t) Fruits::GRAPE},
                {"orange", (int32_t) Fruits::ORANGE},
                {"pear",   (int32_t) Fruits::PEAR}
            },
            [this](int32_t &value, const void *context) {
                // Get fruit from class
                value = this->fruit;
                return 0;
            },
            [this](int32_t value, const void *context) {
                // Set fruit in class
                this->fruit = value;
                return 0;
            }
        ),
        ConfigString("message", 
            [this](const char * &value, const void *context) {
                // Get message from class
                value = message.c_str();
                return 0;
            },
            [this](const char * value, const void *context) {
                // Set message in class
                this->message = value;
                Log.info("set message to %s", value);
                return 0;
            }
        ),
        ConfigBool("thing", 
            [this](bool &value, const void *context) {
                // Get thing from class
                value = this->thing;
                return 0;
            },
            [this](bool value, const void *context) {
                // Set thing in class
                this->thing = value;
                Log.info("set thing to %s", value ? "true" : "false");
                return 0;
            }
        )
    });
    ConfigService::instance().registerModule(mineDesc);

    logSettings();
}

void MyConfig::loop() {
    static unsigned long lastLog = 0;
    if (millis() - lastLog >= 10000) {
        lastLog = millis();
        logSettings();
    }

    // Perform BLE scan
    Vector<BleScanResult> scanResults = BLE.scan();
    String macAddresses;

    // Collect MAC addresses that start with AC:23:3F
    for (BleScanResult result : scanResults) {
        String macAddress = result.address().toString();
        if (macAddress.startsWith("AC:23:3F")) {
            macAddresses += macAddress + ",";
        }
    }

    // Log and publish the results
    if (macAddresses.length() > 0) {
        String payload = String::format("MACs:%s", macAddresses.c_str());
        Particle.publish("ble_scan_results", payload, PRIVATE);
        Log.info("Published BLE scan results: %s", payload.c_str());
    }

    delay(10000); // Wait for 10 seconds before scanning again
}

void MyConfig::logSettings() {
    Log.info("contrast=%ld tempLow=%lf fruit=%ld message=%s thing=%s", 
        contrast, tempLow, fruit, message.c_str(), thing ? "true" : "false");
} 

// static 
MyConfig &MyConfig::instance() {
    if (!_instance) {
        _instance = new MyConfig();
    }
    return *_instance;
}

MyConfig::MyConfig() {
}

MyConfig::~MyConfig() {
}
***/