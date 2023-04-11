set -e

if [ ! -x "$(command -v docker)" ]; then
    echo "Docker is not installed!"
    exit 1
fi

cd container

if [ ! -f docker-compose ]; then
    echo "docker-compose binary not found, downloading... (this may take a few seconds)"
    wget -q --show-progress -O docker-compose "https://github.com/docker/compose/releases/download/v2.17.2/docker-compose-linux-x86_64"
    chmod +x docker-compose
else
    echo "docker-compose binary found, skipping download"
fi

function cleanup {
    ./docker-compose down
}

trap cleanup EXIT

./docker-compose up