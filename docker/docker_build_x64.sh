#!/usr/bin/env bash

if [ "$1" = "prepare" ]; then
  echo "done prepare docker build context"
  ls
else
  cd docker
  docker build . --platform linux/x86_64 -f Dockerfile -t dtvmdev1/dtvm-dev-x64:latest
  cd ..
fi
