#!/bin/bash

if [ ! -d "Result" ]; then
    mkdir Result
fi

> Result/output.txt

while read -r line 
do

    line="${line#"${line%%[![:space:]]*}"}"
    line="${line%"${line##*[![:space:]]}"}"
    
    read x y operation < <(echo "$line")
    if [[ "product" == "$operation" ]]; then 
        ans=$((x * y))
    
    elif [[ "xor" == "$operation" ]]; then
        ans=$((x ^ y))

    elif [[ "compare" == "$operation" ]]; then
        if [ $x -gt $y ]; then
            ans=$x
        elif [ $x == $y ]; then
            ans="Both numbers are equal to $x"
        else
            ans=$y
        fi
    else
        ans="unknown operation"
    fi    

    echo "Result is $ans" >> Result/output.txt
done < input.txt
