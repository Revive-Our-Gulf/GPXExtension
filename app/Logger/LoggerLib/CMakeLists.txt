#--------------------------------------------------------
# CMake for generating the LoggerLib
#
# @author: Wild Boar
#
# Date Created: 2024-07-25
#--------------------------------------------------------

# Create Library
add_library (LoggerLib STATIC
	ArgUtils.cpp
	RandomRovComms.cpp
	Repository.cpp
	RovCommsFactory.cpp
	FileRovComms.cpp
	HttpRovComms.cpp
)
