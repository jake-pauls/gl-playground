#!/bin/sh
# Cleans out premake output and Makefiles

# Remove build folder and intermediates
rm -rf build && rm -rf obj

# Remove auto-generated compile commands (clangd)
rm -rf compile_commands && rm compile_commands.json

# Remove Makefiles
rm Makefile && rm *.make && rm vendor/*.make
