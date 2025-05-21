#!/bin/bash

CONTAINER_NAME="gpx_status"
BASE_IMAGE_NAME="rogengineering/$CONTAINER_NAME"

display_usage() {
    echo "Usage:"
    echo "  $0 build                   - Build the Docker image locally (uses 'latest' tag)"
    echo "  $0 pull <tag>              - Pull the Docker image with specified tag from registry"
    echo "  $0 run <tag>               - Run the Docker image with specified tag"
    exit 1
}

if [ $# -eq 0 ]; then
    display_usage
fi

docker stop $CONTAINER_NAME 2>/dev/null || true
docker rm $CONTAINER_NAME 2>/dev/null || true

case "$1" in
    build)
        IMAGE_TAG="latest"
        echo "Building Docker image with tag: $IMAGE_TAG"
        docker build -t "$BASE_IMAGE_NAME:$IMAGE_TAG" .
        ;;
    pull)
        if [ $# -ne 2 ]; then
            echo "Error: Tag must be specified for pull command."
            display_usage
        fi
        IMAGE_TAG="$2"
        echo "Pulling Docker image from registry with tag: $IMAGE_TAG"
        docker pull "$BASE_IMAGE_NAME:$IMAGE_TAG"
        ;;
    run)
        if [ $# -ne 2 ]; then
            echo "Error: Tag must be specified for run command."
            display_usage
        fi
        IMAGE_TAG="$2"
        echo "Running Docker image with tag: $IMAGE_TAG"
        ;;
    *)
        echo "Invalid parameter: $1"
        display_usage
        ;;
esac

echo "Starting container..."
docker run -d --name $CONTAINER_NAME --restart unless-stopped --network gpx_net -p 5428:5428 $BASE_IMAGE_NAME:$IMAGE_TAG