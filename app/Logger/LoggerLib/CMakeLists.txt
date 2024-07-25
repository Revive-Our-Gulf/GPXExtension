#--------------------------------------------------------
# CMake for generating the LoggerLib
#
# @author: Wild Boar
#
# Date Created: 2024-07-25
#--------------------------------------------------------

# Setup include directories
include_directories("../" "${LIBRARY_BASE}/NVLib")

# Create Library
add_library(LoggerLib STATIC
    ArgUtils.cpp
)
