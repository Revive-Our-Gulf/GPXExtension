FROM debian:12-slim

# Install Packages
RUN apt-get update -y
RUN apt-get upgrade -y
RUN apt-get install -y build-essential
RUN apt-get install -y cmake
RUN apt-get install -y git
RUN apt-get install -y libopencv-dev
RUN apt-get install -y libssl-dev
RUN apt-get install -y libgtest-dev
RUN apt-get install -y uuid-dev
RUN apt-get install -y libzip-dev
RUN apt-get install -y libasio-dev
RUN apt-get install -y libssl-dev
RUN apt-get install -y libmariadb3
RUN apt-get install -y libmariadb-dev
RUN apt-get install -y libjsoncpp-dev

# Create the NVLib Library
RUN mkdir Libraries
WORKDIR /Libraries
RUN git clone https://gitlab.com/project437/nvlib.git
RUN mv nvlib NVLib
RUN mkdir NVLib/build
WORKDIR /Libraries/NVLib/build
RUN cmake ..
RUN cmake --build .

# Create Installs folder
WORKDIR /
RUN mkdir Installs

# Install MariaDB C++ connector
WORKDIR /Installs
COPY mariadb-connector-cpp-1.1.5-debian-bookworm-aarch64.tar.gz .
RUN tar -xvzf mariadb-connector-cpp-1.1.5-debian-bookworm-aarch64.tar.gz
RUN rm -r mariadb-connector-cpp-1.1.5-debian-bookworm-aarch64.tar.gz
WORKDIR /Installs/mariadb-connector-cpp-1.1.5-debian-bookworm-aarch64
RUN install -d /usr/include/mariadb/conncpp
RUN install -d /usr/include/mariadb/conncpp/compat
RUN install ./include/mariadb/* /usr/include/mariadb/; exit 0 
RUN install ./include/mariadb/conncpp/* /usr/include/mariadb/conncpp; exit 0 
RUN install ./include/mariadb/conncpp/compat/* /usr/include/mariadb/conncpp/compat
RUN install -d /usr/lib/mariadb
RUN install -d /usr/lib/mariadb/plugin
RUN install ./lib/mariadb/libmariadbcpp.so /usr/lib
RUN install ./lib/mariadb/plugin/* /usr/lib/mariadb/plugin

# Fix the version of MariaDB C++ connector
WORKDIR /Installs
RUN git clone https://github.com/mariadb-corporation/mariadb-connector-cpp.git
RUN mkdir mariadb-connector-cpp/build
WORKDIR /Installs/mariadb-connector-cpp/build
#RUN cmake ..
#RUN cmake --build .
#RUN install ./libmariadbcpp.so /usr/lib

# Create App folder
#WORKDIR /
#RUN mkdir App

# Create Logger Application
#WORKDIR /App
#COPY Resources Resources
#COPY Logger Logger
#COPY LoggerLib LoggerLib
#COPY CMakeLists_docker.txt CMakeLists.txt
#RUN cmake . -B build
#RUN cmake --build build -j

# Launch the Logger
#WORKDIR /App/build/Logger
#ENTRYPOINT [ "./Logger" ]