#!/bin/bash

# Check if the correct number of arguments is provided.
if [ $# -ne 2 ]; then
  echo "Error: Two arguments required: <file path> <string to write>"
  exit 1
fi

# Assign arguments to variables
writefile=$1
writestr=$2

# Create the directory path if it doesn't exist
mkdir -p "$(dirname "$writefile")"

# Write the string to the file, overwriting if it exists
echo "$writestr" > "$writefile"

# Check if the write operation was successful
if [ $? -ne 0 ]; then
  echo "Error: Could not create or write to file $writefile"
  exit 1
fi

# Exit with a success status
exit 0

