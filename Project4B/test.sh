#!/bin/bash

err=0

./lab4b --period=3 --scale=C --log=logfile <<-EOF
STOP
START
SCALE=F
PERIOD=3
OFF
EOF

ret=$?
if [ $ret -ne 0 ]
then
  echo "ERROR: Return code NOT valid"
  err=$(( err + 1 ))
else
  echo "Valid Return code"
fi

grep "STOP" logfile &> /dev/null
if [ $ret -ne 0 ]
then
  echo "ERROR: STOP NOT found"
  err=$(( err + 1 ))
else
  echo "STOP found"
fi

grep "START" logfile &> /dev/null
if [ $ret -ne 0 ]
then
  echo "ERROR: START NOT found"
  err=$(( err + 1 ))
else
  echo "START found"
fi

grep "SCALE=F" logfile &> /dev/null
if [ $ret -ne 0 ]
then
  echo "ERROR: SCALE=F NOT found"
  err=$(( err + 1 ))
else
  echo "SCALE=F found"
fi

grep "PERIOD=3" logfile &> /dev/null
if [ $ret -ne 0 ]
then
  echo "ERROR: PERIOD=3 NOT found"
  err=$(( err + 1 ))
else
  echo "PERIOD=3 found"
fi

grep "SHUTDOWN" logfile &> /dev/null
if [ $ret -ne 0 ]
then
  echo "ERROR: SHUTDOWN NOT found"
  err=$(( err + 1 ))
else
  echo "SHUTDOWN found"
fi


if [ $err -eq 0 ]
then
  echo "Smoke Test PASSES"
else
  echo "Smoke Test FAILS"
fi
