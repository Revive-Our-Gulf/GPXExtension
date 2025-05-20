#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: $0 [build|pull]"
    echo "  build - Build the Docker image locally and deploy container"
    echo "  pull  - Pull the Docker image from registry and deploy container"
    exit 1
fi

docker stop gpx_status 2>/dev/null || true
docker rm gpx_status 2>/dev/null || true

case "$1" in
    build)
        echo "Building Docker image locally..."
        docker build -t rogengineering/gpx_status:latest .
        ;;
    pull)
        echo "Pulling Docker image from registry..."
        docker pull rogengineering/gpx_status:latest
        ;;
    *)
        echo "Invalid parameter: $1"
        echo "Usage: $0 [build|pull]"
        exit 1
        ;;
esac

echo "Starting container..."
docker run -d --name gpx_status --restart unless-stopped --network gpx_net -p 5428:5428 rogengineering/gpx_status:latest