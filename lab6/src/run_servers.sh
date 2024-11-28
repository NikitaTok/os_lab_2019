#!/bin/bash

input="./servers.txt"
while IFS= read -r line
do
    echo "Running server on port: $line"
    ./server --port "$line" --tnum 4 &
done < "$input"