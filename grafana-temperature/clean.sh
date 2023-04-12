cd container
rm -f prometheus.yml

if [ -f docker-compose ]; then
    ./docker-compose down
    ./docker-compose rm -s -v
    rm ./docker-compose
fi

docker volume rm grafana-temperature_grafana_data
docker volume rm grafana-temperature_prometheus_data
