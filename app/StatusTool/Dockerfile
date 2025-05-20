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

# Install Crow
WORKDIR /Installs
RUN git clone https://github.com/CrowCpp/Crow.git
RUN mkdir Crow/build
WORKDIR /Installs/Crow/build
RUN cmake .. -DCROW_BUILD_EXAMPLES=OFF -DCROW_BUILD_TESTS=OFF
RUN make install

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
RUN git checkout tags/1.1.5
RUN cmake ..
RUN cmake --build .
RUN install ./libmariadbcpp.so /usr/lib

# Create Plugin folder
WORKDIR /
RUN mkdir Plugin

# Generate the plugin
WORKDIR /Plugin
COPY *.h .
COPY *.cpp .
COPY CMakeLists_docker.txt CMakeLists.txt
RUN mkdir templates
COPY ./templates/* ./templates
RUN mkdir static
COPY ./static/ ./static
RUN mkdir config
COPY ./config/ ./config
RUN mkdir build
WORKDIR /Plugin/build
RUN cmake .. 
RUN make

ENV TZ=Pacific/Auckland
RUN ln -snf /user/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

EXPOSE 5428/tcp

LABEL version="1.0.1"

ARG IMAGE_NAME

LABEL permissions='\
{\
  "ExposedPorts": {\
    "5428/tcp": {}\
  },\
  "HostConfig": {\
    "Binds":["/usr/blueos/extensions/GPX:/app/logs", "/:/host_root:ro"],\
    "ExtraHosts": ["host.docker.internal:host-gateway"],\
    "PortBindings": {\
      "5428/tcp": [\
        {\
          "HostPort": ""\
        }\
      ]\
    }\
  }\
}'

ARG AUTHOR
ARG AUTHOR_EMAIL
LABEL authors='[\
    {\
        "name": "Peter Miles",\
        "email": "peter.miles@reviveourgulf.org.nz"\
    }\
]'

ARG MAINTAINER
ARG MAINTAINER_EMAIL
LABEL company='{\
        "about": "",\
        "name": "Blue Robotics",\
        "email": "support@bluerobotics.com"\
    }'
LABEL type="tool"
ARG REPO
ARG OWNER
LABEL readme='https://https://github.com/Revive-Our-Gulf/BenthicCam/blob/main/README.md'
LABEL links='{\
        "source": "https://https://github.com/Revive-Our-Gulf/BenthicCam"\
    }'

# Launch the plugin
ENTRYPOINT ["./StatusTool"]