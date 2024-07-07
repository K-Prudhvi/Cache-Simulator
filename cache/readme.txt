---> Compile the program using "g++ l6.c".
---> Run it using "./a.out".
---> cache.config,cache.access files should be there in same directory.
---> cache.config file should contain the configuration of cache.
---> cache.access file should contain the addresses.
INPUT FORMAT:

SIZE_OF_CACHE (number)
BLOCK_SIZE (number)
ASSOCIATIVITY (number)
REPLACEMENT_POLICY (FIFO or LRU or RANDOM)
WRITEBACK_POLICY (WB or WT)

EXAMPLE:
input:
cache.config:
16384
512
0
LRU
WB
cache.access:
R: 0x1234
W: 0x5678
R: 0x9ABC
W: 0xDEFF
R: 0x1234
R: 0x9ABC
W: 0x5678
R: 0xDEFF
R: 0x1234
W: 0xDEFF
R: 0x9ABC
W: 0x5678
output:
Address: 0x1234, Set: 0x0 ,Miss,Tag: 0x9
Address: 0x5678, Set: 0x0 ,Miss,Tag: 0x2b
Address: 0x9abc, Set: 0x0 ,Miss,Tag: 0x4d
Address: 0xdeff, Set: 0x0 ,Miss,Tag: 0x6f
Address: 0x1234, Set: 0x0 ,Hit, Tag: 0x9
Address: 0x9abc, Set: 0x0 ,Hit, Tag: 0x4d
Address: 0x5678, Set: 0x0 ,Hit, Tag: 0x2b
Address: 0xdeff, Set: 0x0 ,Hit, Tag: 0x6f
Address: 0x1234, Set: 0x0 ,Hit, Tag: 0x9
Address: 0xdeff, Set: 0x0 ,Hit, Tag: 0x6f
Address: 0x9abc, Set: 0x0 ,Hit, Tag: 0x4d
Address: 0x5678, Set: 0x0 ,Hit, Tag: 0x2b
