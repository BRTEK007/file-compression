#!/bin/bash

# Exit immediately if a command fails
set -e

ORIGINAL_DIR="$(pwd)"

build_dir() {
    local target_dir="$1"

    echo "==> Building: $target_dir"

    # Navigate to the target directory
    cd "$target_dir"

    # Create build directory if it doesn't exist
    mkdir -p build
    cd build

    # Run cmake and make
    cmake ..
    make

    # Return to original directory
    cd "$ORIGINAL_DIR"
}

# === List of directories to build ===
build_dir "oddcod"
build_dir "oddcod_examples"
