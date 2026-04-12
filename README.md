url = Desktop/projects/IntegralGenerator

how to compile it:
```arduino-cli compile --fqbn arduino:avr:uno (Desktop/projects/IntegralGenerator or just the url where the project it's in)```

how to update the arduino:
```arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino/avr/uno (the url above)```

and finally how to monitor:
```arduino-cli monitor -p /dev/ttyUSB0 -c baudrate=115200```
