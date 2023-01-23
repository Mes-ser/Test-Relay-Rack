# Test Relay Rack

A project to control Test Rack wirelessly via ESP32 and without platform depedency

- Based on Arduino enviroment
- Web interface
- Web files stored in SPIFFS
- Data exchange with use of events (no one wants to refresh page every time)
- Different reset patterns (Smart Bulbs Manufacures use different on/off patterns to reset their bulbs)

## Strcuture
- data  <- Files used for Web UI
- docs  <- Some documents/notes that can be helpful
- test_relay_rack   <- Main folder with code
    - test_relay_rack.ino   <- source file for arduino (ESP32)
    - wificonfig.h          <- SSID and password used to connect to network

## TODO:
- Run reset patterns on different core (Right now there's problem with watchdog)
- Patterns can be added wia WEB interface
- OTA Upgrade (Don't want to connect to ESP32 every time)
- Better WebUI
- ...
