#!/bin/bash

IMAGE_NAME="rogengineering/gpx_status:latest"
CONTAINER_NAME="gpx_status"

if [ $# -eq 0 ]; then
    echo "Usage: $0 [build|push|pull]"
    echo "  build  - Build the Docker image locally"
    echo "  push   - Push the Docker image to registry"
    echo "  pull   - Pull the Docker image from registry and deploy container"
    exit 1
fi

docker stop $CONTAINER_NAME 2>/dev/null || true
docker rm $CONTAINER_NAME 2>/dev/null || true

case "$1" in
    build)
        echo "Building Docker image..."
        docker build -t "$IMAGE_NAME" .
        ;;
    push)
        echo "Pushing Docker image to registry..."
        docker push "$IMAGE_NAME"
        ;;
    pull)
        echo "Pulling Docker image from registry..."
        docker pull "$IMAGE_NAME"
        ;;
    *)
        echo "Invalid parameter: $1"
        echo "Usage: $0 [build|push|pull]"
        exit 1
        ;;
esac

# Only start container if it's not the push-only command
if [ "$1" != "push" ]; then
    echo "Starting container..."
    docker run -d --name "$CONTAINER_NAME" --restart unless-stopped --network gpx_net -p 5428:5428 "$IMAGE_NAME"
fi