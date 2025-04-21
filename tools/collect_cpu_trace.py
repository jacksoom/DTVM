#!/bin/python3
import sys


def main():
    """
    Usage: ./collect_cpu_trace.py interp.log/singlepass.log
    Show: cpu instructions traces when executing wasm instance
    Show: memory mov instructions count(only in x86-64)
    you can get trace log by
    ./tools/gdb_trace.py ./build/dtvm --mode 1 --enable-gdb-tracing-hook -f fib ./example/fib.0.wasm --args 10 singlepass.log
    """
    log_filepath = sys.argv[1]
    with open(log_filepath, "r") as f:
        lines = f.readlines()
    found_begin = False
    found_end = False
    inner_lines = []
    for line in lines:
        if found_begin and found_end:
            break
        if found_begin and line.startswith("=>"):
            inner_lines.append(line)
        else:
            if "__begin_cpu_tracing__" in line:
                found_begin = True
            elif "__end_cpu_tracing__" in line:
                found_end = True

    print("cpu instructions:")
    if len(inner_lines) < 10000:
        print('\n'.join(inner_lines))
    else:
        print('inner lines count: %d' % len(inner_lines))
        print('\n'.join(inner_lines[0:100]))
        print('...')
    print("cpu instructions in wasm executing is: %d" % len(inner_lines))
    mov_inst_count = 0
    call_inst_count = 0
    cmp_inst_count = 0
    for line in inner_lines:
        if "\tmov " in line and '(' in line and ')' in line and '%' in line:
            mov_inst_count += 1
        elif "\tpush" in line and '%' in line:
            mov_inst_count += 1
        elif "\tpop" in line and '%' in line:
            mov_inst_count += 1
        elif "\tcallq " in line or ("\tcall " in line and '0x' in line):
            call_inst_count += 1
        elif "\tcmp " in line:
            cmp_inst_count += 1
    print("memory mov instructions count: %d" % mov_inst_count)
    print("callq instructions count: %d" % call_inst_count)
    print("cmp instructions count: %d" % cmp_inst_count)


if __name__ == '__main__':
    main()
