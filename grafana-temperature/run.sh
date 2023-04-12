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

function cleanup {
    ./docker-compose down
}

trap cleanup EXIT

./docker-compose up