if [ -f docker-compose ]; then
    ./container/docker-compose down
    ./container/docker-compose rm -s -v
    rm ./container/docker-compose
fi

docker volume rm grafana-temperature_grafana_data
docker volume rm grafana-temperature_prometheus_data
