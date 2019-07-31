#!/bin/bash

# NAME: Kanisha Shah
# EMAIL: kanishapshah@gmail.com
# ID: 504958165


echo "Copying data to file" > file1

# *** 1. check for exit code 0 for files ... copy successful ***
./lab0 --input="file1" --output="file2"
exit_code=`echo $?`

if [ $exit_code -eq  0 ]
then
  echo "Copy succesful for files"
else
  echo "Copy NOT successful for files"
fi

chmod "u+r" file2
`cmp file1 file2 &> /dev/null`
error_code=`echo $?`
if [ $error_code -eq  0 ]
then
  echo "Content in both the files is same"
else
  echo "Content is NOT same in both the files"
fi


# *** 2. check for exit code 0 for std arguents ... copy successful ***
echo "Hello" | ./lab0 &> /dev/null
exit_code=`echo $?`

if [ $exit_code -eq  0 ]
then
  echo "Copy succesful for std arguments"
else
  echo "Copy NOT successful for std arguments"
fi

stdout=`echo "Hello" | ./lab0`
if [ $stdout ==  "Hello" ]
then
  echo "Content of stdout and stdin is same"
else
  echo "Content of stdout and stdin is NOT same"
fi


# *** 3. check for exit code 1 ... unrecognized argument ***
./lab0 --usage &> /dev/null
exit_code=`echo $?`

if [ $exit_code -eq  1 ]
then
  echo "Can detect unrecognized argument"
else
  echo "Can NOT detect unrecognized argument"
fi


# *** 4. check for exit code 2 ... unable to open input file ***
./lab0 --input="nofile" --output="file2" &> /dev/null
exit_code=`echo $?`

if [ $exit_code -eq  2 ]
then
  echo "Can detect invalid input file"
else
  echo "Can NOT detect invalid input file"
fi


# *** 5. check for exit code 3 ... unable to open output file ***
touch newfile
chmod "u-w" newfile
./lab0 --input="file1" --output="newfile" &> /dev/null
exit_code=`echo $?`

if [ $exit_code -eq  3 ]
then
  echo "Can detect invalid output file"
else
  echo "Can NOT detect invalid output file"
fi


# *** 6. check for exit code 4 ... caught and received SIGSEGV ***
./lab0 --input="file1" --output="file2" --segfault --catch &> /dev/null
exit_code=`echo $?`

if [ $exit_code -eq  4 ]
then
  echo "Segmentation fault caused and received "
else
  echo "Segmentation Fault Not received"
fi


rm -f file1 file2 newfile
