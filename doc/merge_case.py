#!/usr/bin/env python3
def uleb128(num):
    while (num >= 128):
        print((num & 0x7f) | 0x80, end=',')
        num >>= 7
    print(num, end=',')

def sleb128(num):
    if num >= 0:
        while (num >= 64):
            print((num & 0x7f) | 0x80, end=',')
            num >>= 7
        print(num, end=',')
    else:
        while (num <= -64):
            print((num & 0x7f) | 0x80, end=',')
            num >>= 7
        print(num & 0x7f, end=',')
        
def emit(start, length, offset):
    uleb128(start)
    uleb128(length)
    sleb128(offset)
    print('')


start = 0
off = 0
diff = 0
stride = 0
length = 0
last = 0
import sys
for line in open('casefold.txt'):
    upper_str, lower_str = line.split()
    upper = int(upper_str, 16)
    lower = int(lower_str, 16)
    if stride > 0 and upper == last + stride and lower == upper + diff:
        length += stride
    else:
        if length > 0: emit(off, length, diff)
        off = upper - start - length
        start = upper
        diff = lower - upper
        length = 1
        stride = 2 if diff == 1 else 1
    last = upper
if length > 0: emit(off, length, diff)
