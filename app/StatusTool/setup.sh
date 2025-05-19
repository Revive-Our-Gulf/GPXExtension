#!/bin/bash

IMAGE_NAME="rogengineering/gpx_status:latest"
CONTAINER_NAME="gpx_status"
PLATFORM="linux/arm/v7"  # Change to arm64 if your Pi is 64-bit

if [ $# -eq 0 ]; then
    echo "Usage: $0 [build|pull|export]"
    echo "  build  - Build the Docker image locally for Raspberry Pi and push to registry"
    echo "  pull   - Pull the Docker image from registry and deploy container"
    echo "  export - Build image and export as tar to transfer manually"
    exit 1
fi

docker stop $CONTAINER_NAME 2>/dev/null || true
docker rm $CONTAINER_NAME 2>/dev/null || true

case "$1" in
    build)
        echo "Building and pushing Docker image for Raspberry Pi..."
        docker buildx build --platform $PLATFORM -t $IMAGE_NAME --push .
        ;;
    pull)
        echo "Pulling Docker image from registry..."
        docker pull $IMAGE_NAME
        ;;
    export)
        echo "Building and exporting Docker image for Raspberry Pi..."
        docker buildx build --platform $PLATFORM -t $IMAGE_NAME --output type=docker .
        docker save $IMAGE_NAME -o gpx_status.tar
        echo "Image saved as gpx_status.tar. Transfer this to the Raspberry Pi and load it with:"
        echo "  docker load -i gpx_status.tar"
        ;;
    *)
        echo "Invalid parameter: $1"
        echo "Usage: $0 [build|pull|export]"
        exit 1
        ;;
esac

echo "Starting container..."
docker run -d --name $CONTAINER_NAME --restart unless-stopped --network gpx_net -p 5428:5428 $IMAGE_NAME
