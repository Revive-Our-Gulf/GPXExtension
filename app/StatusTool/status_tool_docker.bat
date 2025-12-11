@echo off
REM Stop the container if running
docker stop gpx_status

REM Remove the container if exists
docker rm gpx_status

REM Build the Docker image
docker build -t rogengineering/gpx_status:latest app/StatusTool

REM Create the network if it doesn't exist
docker network inspect gpx_net >nul 2>nul
IF ERRORLEVEL 1 docker network create gpx_net

REM Run the container
docker run -d --name gpx_status --restart unless-stopped --network gpx_net -p 5428:5428 rogengineering/gpx_status:latest

echo StatusTool container started.
pause