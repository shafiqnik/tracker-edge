/*
 * Copyright (c) 2020 Particle Industries, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "Particle.h"

#include "tracker_config.h"
#include "tracker.h"

#include "MyConfig.h"

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);

#ifndef SYSTEM_VERSION_v400ALPHA1
PRODUCT_ID(TRACKER_PRODUCT_ID);
#endif
PRODUCT_VERSION(TRACKER_PRODUCT_VERSION);

/*** 
SerialLogHandler logHandler(115200, LOG_LEVEL_TRACE, {
    { "app.gps.nmea", LOG_LEVEL_INFO },
    { "app.gps.ubx",  LOG_LEVEL_INFO },
    { "ncp.at", LOG_LEVEL_INFO },
    { "net.ppp.client", LOG_LEVEL_INFO },
});
***/

void setup() {
    waitFor(Serial.isConnected, 15000);

    Tracker::instance().init();
    MyConfig::instance().init();
   
}

void loop()
{
    Tracker::instance().loop();
    MyConfig::instance().loop();
   

    static unsigned long lastRun = 0;
    const unsigned long interval = (5*60000); // 60 seconds

    if (millis() - lastRun >= interval) {
        lastRun = millis();
        MyConfig::instance().getGPS();
       // Particle.publish("dec 13 B .");
        MyConfig::instance().scanTag();
    }
   
}



/*
 * Copyright (c) 2020 Particle Industries, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*** 
#include "Particle.h"

#include "tracker_config.h"
#include "tracker.h"

#include "BeaconScanner.h"

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);

PRODUCT_ID(TRACKER_PRODUCT_ID);
PRODUCT_VERSION(TRACKER_PRODUCT_VERSION);

STARTUP(
    Tracker::startup();
);

Vector<BleAddress> entered, left;

void locationGenerationCallback(JSONWriter &writer, LocationPoint &point, const void *context)
{
    writer.name("entered").beginArray();
    while (!entered.isEmpty()) {
      writer.value(entered.takeFirst().toString());
    }
    writer.endArray();
    writer.name("left").beginArray();
    while (!left.isEmpty()) {
      writer.value(left.takeFirst().toString());
    }
    writer.endArray();
}

void onCallback(Beacon& beacon, callback_type type) {
  if (type == NEW && !entered.contains(beacon.getAddress())) {
    entered.append(beacon.getAddress());
  }
  if (type == REMOVED && !left.contains(beacon.getAddress())) {
    left.append(beacon.getAddress());
  }
}

void setup()
{
    Tracker::instance().init();
    Tracker::instance().location.regLocGenCallback(locationGenerationCallback);
    BLE.on();
    Scanner.setCallback(onCallback);
    Scanner.startContinuous();
}


void loop()
{
    Tracker::instance().loop();
    Scanner.loop();
}

***/

/***
 * Beacon scanner library
This library works with Particle Gen3 devices to scan for BLE advertisements and parses them for common beacon standards. Currently supported:

iBeacon
Eddystone UID, URL, and unencrypted TLM
Kontakt.io beacons (tested with Asset Tag S18-3)
The custom cold chain tutorial shows how the library can be used to monitor temperature using BLE beacons.

Installation
You can install the library through the Particle library system. The name is BeaconScanner. You can also clone the library from GitHub to include in your project or to modify it.

Usage
The following code will scan all iBeacons, Eddystones, and Kontakt.io tags nearby and automatically publish their information to the Particle Cloud.

 
#include "Particle.h"
#include "BeaconScanner.h"

SYSTEM_THREAD(ENABLED);

Beaconscanner scanner;

void setup() {
}

unsigned long scannedTime = 0;

void loop() {
  if (Particle.connected() && (millis() - scannedTime) > 10000) {
    scannedTime = millis();
    scanner.scanAndPublish(5, SCAN_KONTAKT | SCAN_IBEACON | SCAN_EDDYSTONE, "test", PRIVATE);
  }
}

***/