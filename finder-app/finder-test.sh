#!/bin/sh
# Tester script for assignment 1 and assignment 2
# Author: Siddhant Jajoo

set -e
set -u

NUMFILES=10
WRITESTR=AELD_IS_FUN
WRITEDIR=/tmp/aesd-data
username=$(cat /etc/finder-app/conf/username.txt)
OUT_FILE=/tmp/assignment4-result.txt 

if [ $# -lt 3 ]
then
	echo "Using default value ${WRITESTR} for string to write"
	if [ $# -lt 1 ]
	then
		echo "Using default value ${NUMFILES} for number of files to write"
	else
		NUMFILES=$1
	fi	
else
	NUMFILES=$1
	WRITESTR=$2
	WRITEDIR=/tmp/aesd-data/$3
fi

MATCHSTR="The number of files are ${NUMFILES} and the number of matching lines are ${NUMFILES}"

echo "Writing ${NUMFILES} files containing string ${WRITESTR} to ${WRITEDIR}"

rm -rf "${WRITEDIR}"

# create $WRITEDIR if not assignment1
assignment=`cat /etc/finder-app/conf/assignment.txt`

if [ "$assignment" != 'assignment1' ]
then
	mkdir -p "$WRITEDIR"

	# Check if WRITEDIR was created successfully
	if [ -d "$WRITEDIR" ]
	then
		echo "$WRITEDIR created"
	else
		exit 1
	fi
fi

# Write files using the writer utility
for i in $( seq 1 $NUMFILES)
do
	writer "$WRITEDIR/${username}$i.txt" "$WRITESTR"
done

# Run the finder command
OUTPUTSTRING=$(finder.sh "$WRITEDIR" "$WRITESTR")

# Output the results to the OUT_FILE
echo "$OUTPUTSTRING" > "$OUT_FILE"

# Remove temporary files
rm -rf "$WRITEDIR/${username}*.txt"  # Clean up only the files created

# Check for matching output and print success or failure
set +e
echo "$OUTPUTSTRING" | grep "${MATCHSTR}" > /dev/null
if [ $? -eq 0 ]; then
	echo "success"
	exit 0
else
	echo "failed: expected  ${MATCHSTR} in ${OUTPUTSTRING} but instead found"
	exit 1
fi

