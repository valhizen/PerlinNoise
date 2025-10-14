#!/usr/bin/env bash
set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Compiler and flags
CXX="g++"
CXXFLAGS="-std=c++20 -fmodules-ts -Wall -Wextra -Iinclude"
SDL_CFLAGS="$(pkg-config --cflags sdl2 ) "
SDL_LIBS="$(pkg-config --libs sdl2 ) "

# Output directory
BUILD_DIR="build"
mkdir -p "$BUILD_DIR"

# Step 1: Compile source files
echo -e "${BLUE}[2/3] Compiling main.cpp${NC}"
$CXX $CXXFLAGS $SDL_CFLAGS -c src/main.cpp -o "$BUILD_DIR/main.cpp.o"

# Step 2: Link everything together
echo -e "${BLUE}[3/3] Linking executable${NC}"
$CXX "$BUILD_DIR/main.cpp.o" $SDL_LIBS -o "$BUILD_DIR/main"

echo -e "${GREEN}✓ Build successful!${NC}"
