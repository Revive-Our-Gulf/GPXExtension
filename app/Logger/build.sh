#!/bin/bash

docker stop gpx_logger
docker remove gpx_logger
docker build -t rogengineering/gpx_logger:latest .
docker run -d --name gpx_logger --restart unless-stopped --network gpx_net rogengineering/gpx_logger:latest