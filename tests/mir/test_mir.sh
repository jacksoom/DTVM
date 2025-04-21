#!/bin/bash

set -e

#Get command return value
ircompiler -h > /dev/null 2>&1
ircompiler_return_value=$?
FileCheck --version > /dev/null 2>&1
filecheck_return_value=$?

#Judging the command configuration environment and executing the test
if [ $ircompiler_return_value -eq 0 ] && [ $filecheck_return_value -eq 0 ]
then
    lit *.ir -v
else
    echo "Test exception, ircompiler or FileCheck command does not exist!"
fi

#clear output data
rm -rf Output