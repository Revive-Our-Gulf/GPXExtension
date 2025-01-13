#!/bin/bash

docker stop gpx_logger
docker remove gpx_logger
docker build -t gpx_logger:v1 .
docker run -d --name gpx_logger --restart unless-stopped --network gpx_net gpx_logger:v1