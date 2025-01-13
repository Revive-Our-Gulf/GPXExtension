#!/bin/bash

docker stop gpx_status
docker remove gpx_status
docker build -t gpx_status:v1 .
docker run -d --name gpx_status --restart unless-stopped --network gpx_net -p 5428:5428 gpx_status:v1