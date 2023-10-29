# Solnyshko_BLE

IMPORTANT: changed interaction protocol. Now communication using JSON


Relay control:

-`{"relay": 1 / 0}` : on and off relay

Timer control:
`{"timer": {
    "action" : "...",    
    "time" : 10000
  }}`
  
`action` : 
- `"set"` : start timer on specified milliseconds (not including preheat)
- `"stop"` : stop timer and return lamp into `OFF` state
- `"pause"` : pause timer (only works when `PAUSED` lamp state)
- `"resume"` : resume timer (only works when `ACTIVE` lamp state)
 

Command examples:
 

  -  `{"relay": 1}`
  -  `{"timer": { "action" : "set", "time" : 10000 }}`
 -  `{"timer": { "action" : "pause" }}`
