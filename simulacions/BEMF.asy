Version 4
SymbolType BLOCK
RECTANGLE Normal -16 -16 80 64
TEXT 32 24 Center 2 BEMF
TEXT 2 0 Left 0 A
TEXT 2 16 Left 0 B
TEXT 2 32 Left 0 C
TEXT 2 48 Left 0 N
TEXT 62 0 Right 0 HA
TEXT 62 16 Right 0 HB
TEXT 62 32 Right 0 HC
TEXT 16 58 Center 0 th
TEXT 48 58 Center 0 w
WINDOW 0 32 -28 Center 2
WINDOW 39 32 88 Center 1
SYMATTR Value BEMF
SYMATTR Prefix X
SYMATTR ModelFile BEMF.sub
SYMATTR SpiceLine KT=15e-3
SYMATTR Description Acoblament mecanic-electric del motor BLDC (fonts de BEMF + sensors Hall)
PIN 0 0 LEFT 8
PINATTR PinName A
PINATTR SpiceOrder 1
PIN 0 16 LEFT 8
PINATTR PinName B
PINATTR SpiceOrder 2
PIN 0 32 LEFT 8
PINATTR PinName C
PINATTR SpiceOrder 3
PIN 0 48 LEFT 8
PINATTR PinName N
PINATTR SpiceOrder 4
PIN 16 64 BOTTOM 8
PINATTR PinName THETA_T
PINATTR SpiceOrder 5
PIN 48 64 BOTTOM 8
PINATTR PinName WANG
PINATTR SpiceOrder 6
PIN 64 0 RIGHT 8
PINATTR PinName HA
PINATTR SpiceOrder 7
PIN 64 16 RIGHT 8
PINATTR PinName HB
PINATTR SpiceOrder 8
PIN 64 32 RIGHT 8
PINATTR PinName HC
PINATTR SpiceOrder 9
