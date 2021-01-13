#!/bin/bash

# Check that base image exists
IMAGE_BASE="riscv_base"
IMAGE_BASE_ID="$(docker images -q $IMAGE_BASE)"

if [[ $IMAGE_BASE_ID == "" ]]; then
    echo "$IMAGE_BASE does not exist, building:"
    BUILD_CMD="docker build -t $IMAGE_BASE -f Dockerfile.base ."
    echo "  $BUILD_CMD"
    $BUILD_CMD
fi

# Check that dev image exists
IMAGE_DEV="riscv_console_dev"
IMAGE_DEV_ID="$(docker images -q $IMAGE_DEV)"
if [[ $IMAGE_DEV_ID == "" ]]; then
    echo "$IMAGE_DEV does not exist, building:"
    BUILD_CMD="docker build -t $IMAGE_DEV -f Dockerfile ."
    echo "  $BUILD_CMD"
    $BUILD_CMD
fi

# Add xhost
xhost + 127.0.0.1

# Check if container exists is running or if needs to be restarted
CONTAINER_DEV="riscv_console_run"
CONTAINER_DEV_ID="$(docker ps -q -f name=$CONTAINER_DEV)"
if [[ $CONTAINER_DEV_ID == "" ]]; then
    # Container not currently running
    CONTAINER_DEV_ID="$(docker ps -aq -f status=exited -f name=$CONTAINER_DEV)"
    if [[ $CONTAINER_DEV_ID == "" ]]; then
        # Container doesn't exist
        echo "$CONTAINER_DEV does not exist, running:"
        RUN_CMD="docker run -dit -v $(pwd):/code -v /tmp/.X11-unix:/tmp/.X11-unix --env-file .env.dev --name $CONTAINER_DEV $IMAGE_DEV"
        echo "  $RUN_CMD"
        $RUN_CMD
        docker exec -it $CONTAINER_DEV bash -c 'useradd cs251; mkdir /home/cs251; chown cs251 /home/cs251;'
    else
        # Restart the container and reconnect
        echo "$CONTAINER_DEV not running, restarting:"
        RESTART_CMD="docker restart $CONTAINER_DEV"
        echo "  $RESTART_CMD"
        $RESTART_CMD
    fi
fi

# Container is running, exec bash
EXEC_CMD="docker exec -it $CONTAINER_DEV bash"
echo $EXEC_CMD
echo "su cs251 -c /code/riscv-sim/bin/riscv-console-sim"
$EXEC_CMD
