#!/bin/sh
# Runs make commands, execute from root directory

# Run Premake
premake5 gmake2

# Run GNU Make
make

# Generate `compile-commands.json` for clangd (uses debug symbols by default)
# https://github.com/tarruda/premake-export-compile-commands
premake5 export-compile-commands
cp compile_commands/debug.json compile_commands.json

# Run project
./build/Debug/gl-playground
