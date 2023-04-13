set -e

DOCKER_COMPOSE_VERSION="v2.17.2"

if [ ! -x "$(command -v docker)" ]; then
    echo "Docker is not installed!"
    exit 1
fi

cd container

verify_checksum() {
    checksum=$(curl -s -L "$docker_compose_url.sha256" | cut -d " " -f 1)
    set +e
    echo "$checksum docker-compose" | sha256sum -c --status
    status=$?
    set -e
    if [ $status -ne 0 ]; then
        echo "Checksum verification failed: expected $checksum, got $(sha256sum docker-compose)"
        echo "Run this script again to retry download"
        rm -f docker-compose
        exit 1
    fi    
}

if [ ! -f docker-compose ]; then
    arch="$(uname -m)"
    if [ "$(uname)" == "Darwin" ]; then
        if [ "$arch" == "arm64" ] || [ "$arch" == "arm" ] || [ "$arch" == "aarch64" ]; then
            echo "Apple Silicon detected, using aarch64 binary"
            docker_compose_url="https://github.com/docker/compose/releases/download/$/docker-compose-darwin-aarch64"
        else 
            echo "Intel Mac detected, using x86_64 binary"
            docker_compose_url="https://github.com/docker/compose/releases/download/$DOCKER_COMPOSE_VERSION/docker-compose-darwin-aarch64"
        fi
    else
        if [ "$arch" == "arm64" ] || [ "$arch" == "arm" ] || [ "$arch" == "aarch64" ]; then
            echo "ARM on Linux detected, using aarch64 binary"
            docker_compose_url="https://github.com/docker/compose/releases/download/$DOCKER_COMPOSE_VERSION/docker-compose-linux-aarch64"
        else
            echo "Intel / AMD on Linux detected, using x86_64 binary"
            docker_compose_url="https://github.com/docker/compose/releases/download/$DOCKER_COMPOSE_VERSION/docker-compose-linux-x86_64"
        fi
    fi

    echo "docker-compose binary not found, downloading... (this may take a few seconds)"
    trap verify_checksum EXIT
    wget -q --show-progress -O docker-compose "$docker_compose_url"
    chmod +x docker-compose

    verify_checksum
    trap - EXIT
else
    echo "docker-compose binary found, skipping download"
fi

if [ ! -f prometheus.yml ]; then
    cp prometheus.yml.example prometheus.yml
fi

# Replace PICO_IP in prometheus.yml with the IP of the Pico
if grep -q PICO_IP "prometheus.yml"; then
    echo ""
    echo "We need your Pico's IP address to continue. This is printed to the terminal when you run the web server with the example solution."
    echo "Do not include the port number or any other characters, just the IP address, e.g. 196.168.1.2"
    echo -n "Pico IP: "
    read PICO_IP
    sed -i -e "s/PICO_IP/$PICO_IP/g" ./prometheus.yml
fi

cleanup() {
    ./docker-compose down
}

trap cleanup EXIT

./docker-compose up