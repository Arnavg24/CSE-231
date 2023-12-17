#!/bin/bash
string="$1"
formatted_date=$(date -d "$string")
echo "$formatted_date"