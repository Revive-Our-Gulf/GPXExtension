#!/bin/bash

CONTAINER_NAME="gpx_status"
IMAGE_NAME="rogengineering/$CONTAINER_NAME:latest"

if [ $# -eq 0 ]; then
    echo "Usage: $0 [build|push|pull]"
    echo "  build  - Build the Docker image locally"
    echo "  pull   - Pull the Docker image from registry and deploy container"
    echo "  run   - Run the Docker image"
    exit 1
fi

docker stop $CONTAINER_NAME 2>/dev/null || true
docker rm $CONTAINER_NAME 2>/dev/null || true

case "$1" in
    build)
        echo "Building Docker image..."
        docker build -t "$IMAGE_NAME" .
        ;;
    pull)
        echo "Pulling Docker image from registry..."
        docker pull "$IMAGE_NAME"
        ;;
    run)
        echo "Running Docker image..."
        ;;
    *)
        echo "Invalid parameter: $1"
        echo "Usage: $0 [build|push|pull]"
        exit 1
        ;;
esac

echo "Starting container..."
docker run -d --name gpx_status --restart unless-stopped --network gpx_net -p 5428:5428 rogengineering/gpx_status:latest