# Solnyshko_BLE

IMPORTANT: changed interaction protocol. Now communication using JSON

Project works over BLE. After power on device starts BLE advertising. Manufacter data (Bluetooth Core 4.1) contains `0x4E53` Reserved ID. You can use it to filter devices while scaning.

Service UUID: `4fafc201-1fb5-459e-8fcc-c5c9c331914b`

Service contains two characteristics:   
- `beb5483e-36e1-4688-b7f5-ea07361b26a8`
    - PROPERTY_READ
    - PROPERTY_WRITE
    - Is used to send commands and request device status
- `1fd32b0a-aa51-4e49-92b2-9a8be97473c9` : on and off relay
    - PROPERTY_NOTIFY
    - Is used to notify client about device status changes. Perceive notify as signal to renew local status.

# Commands

**Command** - is a way to send any task into device.
For command sending, you should make write request into `beb5483e-36e1-4688-b7f5-ea07361b26a8` characteristic. In request data you can specify JSON command in this form:

### Relay control:

`{"relay": 1 / 0}` : on and off relay

### Timer control:
    {
        "timer": {
            "action" : "...",
            "time" : 10000,
            "cycles" : 3 }
     }
  
`action` : 
- `"set"` : start timer on specified duration and cycles count (not including preheat). **Take attention**: this command starts **full** procedure, including preheating and and separate cycles, divided timer pauses. 
- `"stop"` : stop timer and return lamp into `OFF` state
- `"pause"` : pause timer (only works when `PAUSED` lamp state)
- `"resume"` : resume timer (only works when `ACTIVE` lamp state)


`time` : single cycle time, ms

`cycles` : number of cycles

### Command examples:
 

-  `{"relay": 1}`
-  `{"timer": { "action" : "set", "time" : 10000, "cycles" : 3 }}`
-  `{"timer": { "action" : "pause" }}`
 
# Status responses
**Status response** - is a JSON response on client's read request consisting off full device status. For requesting status, you should make read request `beb5483e-36e1-4688-b7f5-ea07361b26a8` characteristic. You will recive response in this form:

    {
        "state": 0-4,
        "timer": {
            "time_left":15000,
            "cycles":3,
            "cycle_time":5000 },
        "preheat": {
            "time_left":0 }
    }
    
- `state` : common device state
    0) `OFF`,
    1)   `ON`,
    2)   `PREHEATING`,
    3)   `ACTIVE`,
    4)   `PAUSED`
- `timer` : timer parameters
    - `time_left` : total cycles duration left, ms
    - `cycles` : number of procedure cycles
    - `cycle_time` : single procedure cycle duration, ms

- `preheat` : preheat parameters
    - `time_left` : preheating duration left, ms
