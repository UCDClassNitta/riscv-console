# Check that base image exists (Removed changed to cjnitta/riscv_base)
#$IMAGE_BASE="riscv_base"
#$IMAGE_BASE_ID = docker images -q $IMAGE_BASE
#if(!$IMAGE_BASE_ID){
#    Write-Output "$IMAGE_BASE does not exist, building:"
#    Write-Output "  docker build -t $IMAGE_BASE -f Dockerfile.base ."
#    docker build -t $IMAGE_BASE -f Dockerfile.base .
#}

# Check that dev image exists
$IMAGE_DEV = "riscv_console_dev"
$IMAGE_DEV_ID = docker images -q $IMAGE_DEV
if(!$IMAGE_DEV_ID){
    Write-Output "$IMAGE_DEV does not exist, building:"
    Write-Output "  docker build --load -t $IMAGE_DEV -f Dockerfile ."
    docker build --load -t $IMAGE_DEV -f Dockerfile .
}

# Create workspace directory if doesn't exist
if(!(Test-Path "workspace")){
    New-Item -ItemType Directory -Path "workspace"
}

$CURRENT_IP=(
    Get-NetIPConfiguration |
    Where-Object {
        $_.IPv4DefaultGateway -ne $null -and
        $_.NetAdapter.Status -ne "Disconnected"
    }
).IPv4Address.IPAddress
# Check if container exists is running or if needs to be restarted
$CONTAINER_DEV="riscv_console_run"
$CONTAINER_DEV_ID=docker ps -q -f name=$CONTAINER_DEV
if(!$CONTAINER_DEV_ID){
    # Container not currently running
    $CONTAINER_DEV_ID= docker ps -aq -f status=exited -f name=$CONTAINER_DEV
    if(!$CONTAINER_DEV_ID){
        # Container doesn't exist
        $CUR_PATH = pwd
        $CUR_PATH = ($CUR_PATH -replace "\\","/")
        Write-Output "$CONTAINER_DEV does not exist, running:"
        Write-Output "  docker run -it -v ${CUR_PATH}:/code -e DISPLAY=${CURRENT_IP}:0 --env-file .env.dev.win --name $CONTAINER_DEV $IMAGE_DEV"
        docker run -it -v ${CUR_PATH}:/code -e DISPLAY=${CURRENT_IP}:0 --env-file .env.dev.win --name $CONTAINER_DEV $IMAGE_DEV
        exit 0
    }
    else{
        # Restart the container and reconnect
        Write-Output "$CONTAINER_DEV not running, restarting:"
        Write-Output "  docker restart $CONTAINER_DEV"
        docker restart $CONTAINER_DEV 
    }
}

$MOUNTED=(docker inspect --format='{{(index .Mounts 0).Source}}' $CONTAINER_DEV)
Write-Output "Your Docker Container is mounted to ${MOUNTED}"
Write-Output "Your current directory is $(pwd)"

#https://stackoverflow.com/questions/5466329/whats-the-best-way-to-determine-the-location-of-the-current-powershell-script
#$PSCommandPath: Contains the full path and file name of the script that is being run.
$SCRIPT_DIR=(Split-Path -Parent $PSCommandPath)
Write-Output "Your current shell script is running in ${SCRIPT_DIR}"

if($MOUNTED -ne $SCRIPT_DIR)
{
    Write-Output "ERROR: The file you are running is NOT in your MOUNTED Volume!"
    Write-Output "ERROR: The file you are running is NOT in your MOUNTED Volume!"
    Write-Output "ERROR: The file you are running is NOT in your MOUNTED Volume!"
    Write-Output "ERROR: The file you are running is NOT in your MOUNTED Volume!"
    Write-Output "ERROR: The file you are running is NOT in your MOUNTED Volume!"

    Write-Output "************************************************************************************************"
    Write-Output "Options to correct the error: "
    Write-Output "(1) Move all your files from the current location to your mounted path: $MOUNTED"
    Write-Output "---- OR ----"
    Write-Output "(2) Stop the container in your Docker application, remove the container, re-run ./rvconsole.ps1"
    Write-Output "************************************************************************************************"
    exit 0
}

# Container is running, exec bash
Write-Output "docker exec -it -e DISPLAY=${CURRENT_IP}:0 $CONTAINER_DEV bash"
docker exec -it -e DISPLAY=${CURRENT_IP}:0 $CONTAINER_DEV bash