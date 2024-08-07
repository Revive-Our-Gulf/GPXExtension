#--------------------------------------------------------
# CMake for generating LoggerLib Testing Suite
#
# @author: Wild Boar
#
# Date Created: 2024-07-25
#--------------------------------------------------------

# Add Google Test to the test group
find_package(GTest REQUIRED)

# Setup the includes
include_directories("../")

# Create the executable
add_executable (LoggerTests
	Tests/Example_Tests.cpp
	Tests/RovComms_Tests.cpp
	Tests/Repository_Tests.cpp
	Tests/FileRovComms_Tests.cpp
)

# Add link libraries
target_link_libraries(LoggerTests LoggerLib NVLib ${OpenCV_LIBS} jsoncpp mariadbcpp OpenSSL::SSL uuid GTest::Main)

# Find the associated unit tests
gtest_discover_tests(LoggerTests)