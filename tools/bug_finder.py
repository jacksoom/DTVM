#!/usr/bin/env python3

import subprocess
import os
import sys

def run(i, j):
  print("run i = %s, j = %s" % (i, j), flush=True)
  try:
    os.environ["GREEDY_FUNC_IDX_START"] = str(i)
    os.environ["GREEDY_FUNC_IDX_END"] = str(j)

    cmd = None
    cmd = ["dtvm", "test.wasm",  "--mode",  "2",  "-f",  "apply"]

    # result = subprocess.run(cmd, stdout=subprocess.PIPE, text=True, check=True)
    result = subprocess.run(cmd, stdout=subprocess.PIPE)
    print(result.stdout, flush=True)
    if "Exception" in result.stdout:
        return False
    return True
  except Exception as e:
    print(1, e)
    return False

def search(begin, end):
  error = False

  i, j = begin, end
  last_i, last_j = i, j

  while True:
    if (i >= j):
      break
    mid = int((i + j) / 2);
    if (not run(i, mid)):
      error = True
      j = mid
      last_j = j
    elif (not run(mid + 1, j)):
      error = True
      i = mid + 1
      last_i = i
    else:
      j = mid

  print("last_i: %s, last_j: %s, error: %s" % (last_i, last_j, error))

if __name__ == "__main__":
  error = search(0, 100)

  if error:
    sys.exit(1)

