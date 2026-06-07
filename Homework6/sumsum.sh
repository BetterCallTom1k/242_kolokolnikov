#!/bin/bash

sum() {
    local total=0
    for n in "$@"; do
        if ! [[ "$n" =~ ^-?[0-9]+$ ]]; then
            return 1
        fi
        total=$(( total + n ))
    done
    echo "$total"
}

read line1
read line2

s1=$(sum $line1 2>/dev/null)
[ $? -eq 0 ] || s1=0

s2=$(sum $line2 2>/dev/null)
[ $? -eq 0 ] || s2=0

if [ "$s1" -eq "$s2" ]; then
    echo "Equal"
else
    echo "Not equal"
fi
