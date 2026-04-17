urlMega = Desktop/projects/IntegralGenerator/arduino-mega
urlUno = Desktop/projects/IntegralGenerator/arduino-uno

# Compilation of arduino uno

how to compile it:
```
arduino-cli compile --fqbn arduino:avr:uno (Desktop/projects/IntegralGenerator or just the url where the project it's in)
```

how to update the arduino:
```
arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino/avr/uno (the url above)
```

and finally how to monitor:
```
arduino-cli monitor -p /dev/ttyUSB0 -c baudrate=115200
```

# Compilation of arduino mega

compilation command:
```
arduino-cli compile --fqbn arduino:avr:uno (url of the project)
```

upload:
```
arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:mega (url of the project)
```

monitor
```
arduino-cli monitor -p /dev/ttyUSB0 -c baudrate=115200
```
