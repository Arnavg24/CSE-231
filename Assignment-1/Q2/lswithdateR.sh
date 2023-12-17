#!/bin/bash
filename="$1"
timestamp=$(ls -l "$filename" | awk '{print $6 " " $7 " " $8}')
formatted_date=$(date -R --date="$timestamp")
echo "$formatted_date"
