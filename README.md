# stroboscope
LED stroboscope

## How to build

### Build ctrl
Use Qt:
```
$ cd ctrl
$ qmake
$ make
```

### Build firmware for ESP32-S3
Use Espressif IDF.
Connect the ESP32-S3 to your PC via the USB port labeled "COM".
```
$ cd firmware
$ idf.py set-target esp32s3
$ idf.py menuconfig
$ idf.py build
$ idf.py -p PORT flash
```

## How to use

Power up the device. Connect it to PC via USB port labeled "USB".

### PWM mode

To set up PWM frequency and pulse width run command line application:
```
$ stroboscope <serial-port> -f <freq> -w <pulse_width>
```
where \<serial-port\> is COM-port name, \<freq\> is PWM frequency in Hz, \<pulse_width\> is width of pulse in microseconds.

Actual frequency, period, pulse width and duty cycle will be printed out in console.

If -w switch is omitted, pulse_width will be set up to half of period.

For example, next command will set up PWM frequency to 1000 Hz and pulse width to 750 us:
```
$ stroboscope /dev/ttyACM1 -f1000 -w750
```

and output will be like this:
```
Freq: 1000 Hz  T: 1000 us  PulseWidth: 749 us  Duty: 75%
```

### Continuous mode
If freq is 0 specified PWM mode will be switched to continuous mode.
With freq 0 -w switch value 1 is used to turn LED on, value 0 is used to turn LED off.

For example, to turn LED on with no PWM signal use command:
```
$ stroboscope <serial-port> -f0 -w1
```

To turn LED off use command:
```
$ stroboscope <serial-port> -f0 -w0
```
or just
```
$ stroboscope <serial-port> -f0
```

### "Load" output control
To switch "load" output use command:
```
$ stroboscope <serial-port> -r<X>
```

where \<X\> is 1 to switch output to ON state, and 0 is for OFF state.
