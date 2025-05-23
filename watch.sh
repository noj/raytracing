#!/bin/bash

while true; do
    inotifywait -e modify,create,delete . && make view
    echo "rebuilt"
done
