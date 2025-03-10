#!/usr/bin/env python3
def uleb128(num):
    while (num >= 128):
        print((num & 0x7f) | 0x80, end=',')
        num >>= 7
    print(num, end=',')

def emit(start, length):
    uleb128(start)
    uleb128(length)
    print('')

import sys
start = 0
last = 0
length = 0
off = 0
for line in sys.stdin:
    line.strip()
    num = int(line, 16)
    if num == last + 1:
        length += 1
    else:
        if length > 0: emit(off, length)
        off = num - start - length
        start = num
        length = 1
    last = num
if length > 0: emit(off, length)

