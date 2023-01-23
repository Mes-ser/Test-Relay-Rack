# Test Relay Rack

A project to control Test Rack wirelessly via ESP32 and without platform depedency

- Based on Arduino enviroment
- Web interface
- Web files stored in SPIFFS
- Data exchange with use of events (no one wants to refresh page every time)
- Different reset patterns (Smart Bulbs Manufacures use different on/off patterns to reset their bulbs)

## TODO:
- Run reset patterns on diferent core (Right now there's problem with watchdog)
- Patterns can be added wia WEB interface
- OTA Upgrade (Don't want to connect to ESP32 every time)
- Better WebUI
- ...
