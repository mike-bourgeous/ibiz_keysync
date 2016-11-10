# Toy code to interface with the iBIZ KeySync serial keyboard

The iBIZ KeySync was a battery-powered keyboard from the year 2000 designed for
use with Pocket PC PDAs.  I found some at a surplus sale and got one working
with a USB-to-RS232 adapter.

See http://blog.mikebourgeous.com/2016/11/09/resurrecting-a-dinosaur-the-ibiz-keysync-keyboard-for-pocket-pc/

#### Compiling

```bash
make
```

#### Running

```bash
./keysync /dev/ttyUSB0
```

#### Dumping raw data

```bash
# Some of these settings may be unnecessary
stty -F /dev/ttyUSB0 \
  9600 cs8 -parenb -cstopb \
  raw -echo -crtscts -echok -echoctl -echoke -iexten -hupcl \
  intr undef \
  quit undef \
  erase undef \
  kill undef \
  eof undef \
  start undef \
  stop undef \
  susp undef \
  rprnt undef \
  werase undef \
  lnext undef \
  flush undef

od -tx1 -w1 /dev/ttyUSB0
```
