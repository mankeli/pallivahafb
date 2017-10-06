# pallivahafb
Systemd/Linux framebuffer driver for free Pallivaha GPU.

Based on q40fb.c from kernel tree. There's probably a better driver (simplefb.c or so I've heard) for simple memory-mapped truecolor devices.

This came to be because I anyway needed to rewrite the color carrier generation, and thought that it would be useful not to hardcode the I/Q amplitude calculations to the FPGA. 

Supports 320x200x8 mode.

