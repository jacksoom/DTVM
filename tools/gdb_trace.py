#!/usr/bin/python3
# coding: utf8
# Usage: ./tools/gdb_trace.py ./build/dtvm --mode 1 --enable-gdb-tracing-hook -f fib ./example/fib.0.wasm --args 10 singlepass.log

import os
import os.path
import sys
from subprocess import *
import threading
import time
from time import sleep
import io

class GlobalsConfig(object):
    def __init__(self):
        self.cpu_tracing_began = False

def main():
    zen_program = sys.argv[1]
    zen_log_path = sys.argv[-1]
    zen_args = sys.argv[2:-1]

    print("command args: %s" % ' '.join(zen_args))
    # start gdb

    p = Popen("gdb %s" % zen_program, shell=True, stdin=PIPE, stdout=PIPE, stderr=PIPE) # stdout=sys.stdout when debug

    def p_send(s):
        print("sending: %s" % s)
        p.stdin.write((s+'\n').encode('utf8'))
        p.stdin.flush()

    gls = GlobalsConfig()

    def wait_end_cpu_tracing_log_task(p, gls, loop):
        print("thread wait_end_cpu_tracing_log_task running")
        while loop:
            in_backtrace = False
            for line in p.stdout:
                if 'Backtrace' in str(line):
                    in_backtrace = True
                line_printed = False
                if in_backtrace:
                    print(line.decode('utf8'))
                    line_printed = True
                if 'failed to load module' in str(line):
                    p.kill()
                    return
                if 'Backtrace unavailable' in str(line):
                    p.kill()
                    return
                if '__begin_cpu_tracing__' in str(line):
                    gls.cpu_tracing_began = True
                    print("found __begin_cpu_tracing__")
                if not gls.cpu_tracing_began:
                    sleep(0.05)
                    continue
                if not line_printed:
                    print(line.decode('utf8'))
                    line_printed = True
                if '__end_cpu_tracing__' in str(line) or 'endCPUTracing' in str(line):
                    print("subprocess ended cpu tracing")
                    p.kill()
                    return
    wait_end_thread = threading.Thread(target=wait_end_cpu_tracing_log_task, args=[p, gls, True])
    wait_end_thread.start()

    sleep(10) # wait gdb started. too small will cause Make breakpoint pending on future shared library load?

    p_send('b src/runtime/runtime.cpp:580') # startCpuTracingIfEnabled func
    p_send("run %s" % ' '.join(zen_args))
    sleep(3) # wait run to startCPUTracing

    p.stdin.write("set logging on\n".encode('utf8'))
    p.stdin.flush()
    p.stdin.write("set height 0\n".encode('utf8'))
    p.stdin.flush()
    sleep(1)

    p.stdin.write("while 1\nx/i $pc\nstepi\nend\n".encode('utf8')) # loop stepi and log instruction
    p.stdin.flush()

    def task1(p):
        p.wait()
    t1 = threading.Thread(target=task1, args=[p])
    t1.start()


    wait_end_thread.join()
    t1.join()
    #  move gdb.txt to zen_log_path
    if os.path.isfile('gdb.txt'):
        os.rename('gdb.txt', zen_log_path)
        print('log writen to %s' % zen_log_path)



if __name__ == '__main__':
    main()
