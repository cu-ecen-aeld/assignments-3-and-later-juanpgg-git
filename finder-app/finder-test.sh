#!/bin/sh
# Tester script for assignment 1 and assignment 2
# Author: Siddhant Jajoo

set -e
set -u

NUMFILES=10
WRITESTR=AELD_IS_FUN
WRITEDIR=/tmp/aeld-data
username=$(cat conf/username.txt)

if [ $# -lt 3 ]; then
    echo "Using default value ${WRITESTR} for string to write"
    if [ $# -lt 1 ]; then
        echo "Using default value ${NUMFILES} for number of files to write"
    else
        NUMFILES=$1
    fi    
else
    NUMFILES=$1
    WRITESTR=$2
    WRITEDIR=/tmp/aeld-data/$3
fi

MATCHSTR="The number of files are ${NUMFILES} and the number of matching lines are ${NUMFILES}"

echo "Writing ${NUMFILES} files containing string ${WRITESTR} to ${WRITEDIR}"

# Clean up previous data
rm -rf "${WRITEDIR}"

# create $WRITEDIR
mkdir -p "$WRITEDIR"
if [ -d "$WRITEDIR" ]; then
    echo "$WRITEDIR created"
else
    echo "Failed to create directory $WRITEDIR"
    exit 1
fi

# Clean previous build artifacts and compile the writer application
echo "Cleaning previous build artifacts and compiling the writer application"
make clean
if ! make; then
    echo "Compilation failed. Exiting."
    exit 1
fi

# Use the writer utility instead of writer.sh
for i in $(seq 1 $NUMFILES); do
    ./writer "$WRITEDIR/${username}$i.txt" "$WRITESTR"
done

OUTPUTSTRING=$(./finder.sh "$WRITEDIR" "$WRITESTR")

# Remove temporary directories
rm -rf /tmp/aeld-data

set +e
echo ${OUTPUTSTRING} | grep "${MATCHSTR}"
if [ $? -eq 0 ]; then
    echo "success"
    exit 0
else
    echo "failed: expected  ${MATCHSTR} in ${OUTPUTSTRING} but instead found"
    exit 1
fi

