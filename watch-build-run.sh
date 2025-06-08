#!/bin/bash

FILE="hexagon.c"
BUILD_CMD="make"
RUN_CMD="make run"

echo "Watching $FILE for changes. Press Ctrl+C to stop."

# Ensure inotifywait is installed
if ! command -v inotifywait >/dev/null; then
    echo "Error: inotifywait is not installed. Install it via: sudo apt install inotify-tools"
    exit 1
fi

while true; do
    # Wait for write or close-write events
    inotifywait -e close_write "$FILE" >/dev/null 2>&1

    echo "[$(date '+%H:%M:%S')] Change detected. Rebuilding..."
    if $BUILD_CMD; then
        echo "[$(date '+%H:%M:%S')] Build successful. Running..."
        $RUN_CMD
    else
        echo "[$(date '+%H:%M:%S')] Build failed."
    fi
done