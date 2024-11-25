#--------------------------------------------------------
# Top-Level: Backup
#
# @author: Wild Boar
#
# @Date Created: 2024-11-25
#--------------------------------------------------------

cmake_minimum_required(VERSION 3.0.0)
project(Backup VERSION 0.1.0)

# Set the base path of the libraries
set(LIBRARY_BASE "$ENV{HOME}/Libraries")

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add OpenCV to the solution
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Include OpenSSL
find_package(OpenSSL REQUIRED)

# Add the NVLib library to the folder
add_library(NVLib STATIC IMPORTED)
set_target_properties(NVLib PROPERTIES
    IMPORTED_LOCATION "${LIBRARY_BASE}/NVLib/build/NVLib/libNVLib.a"
    INTERFACE_INCLUDE_DIRECTORIES "${LIBRARY_BASE}/NVLib"
)

# Add the MariaDB connector
include_directories("/usr/include/mariadb")
add_library(mariadbcpp STATIC IMPORTED)
set_property(TARGET mariadbcpp PROPERTY IMPORTED_LOCATION "/usr/lib/libmariadbcpp.so")

# Create the executable
add_executable(Backup
    Source.cpp
    Repository.cpp
)

# Add link libraries                               
target_link_libraries(Backup NVLib ${OpenCV_LIBS} mariadbcpp OpenSSL::SSL uuid)

# Copy Config across
add_custom_target(config_copy ALL
	COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/config ${CMAKE_BINARY_DIR}
    DEPENDS ${MY_TARGET}
)
