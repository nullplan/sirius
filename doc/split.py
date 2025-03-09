#!/usr/bin/env python3

if __name__ == "__main__":
    import sys
    out = []
    for line in sys.stdin:
        line = line.strip()
        if '..' in line:
            start_hex, end_hex = line.split('..')
            try:
                start_num = int(start_hex, 16)
                end_num = int(end_hex, 16)
                for num in range(start_num, end_num + 1):
                    out.append(num)
            except ValueError:
                print(f"Invalid hexadecimal range: {line}")
        else:
            out.append(int(line, 16))
    out.sort()
    for num in out:
        print(f"{num:X}")
