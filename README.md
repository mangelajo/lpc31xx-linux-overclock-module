lpc31xx-overclock-linux-module
------------------------------

Ideally we should export an interface under sys or procfs, 
but this is enough for now.

Usage:
   insmod lpc31xx_clock mhz=270


It can cause problems with running usb transfers as it will glitch the
main clock of the system. So, consider this just a proof of concept,
and a tool for development, but don't use it for production.


