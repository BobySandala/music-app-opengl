#!/bin/bash

echo "Building project..."

make

if [ $? -eq 0 ]; then
    echo "Build successful"
    echo "Running application..."
    ./music_player
else
    echo "Build failed"
fi