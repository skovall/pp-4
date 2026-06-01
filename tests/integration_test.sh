#!/bin/bash
set -e

response=$(curl -s http://localhost:8080/)
if [[ "$response" != Hello\ from\ CI/CD\ #[0-9]* ]]; then
    echo "Root endpoint test failed"
    exit 1
fi

health=$(curl -s http://localhost:8080/health)
if [[ "$health" != "OK" ]]; then
    echo "Health check failed"
    exit 1
fi

metrics=$(curl -s http://localhost:8080/metrics)
if [[ ! "$metrics" =~ http_requests_total\ [0-9]+ ]]; then
    echo "Metrics endpoint failed"
    exit 1
fi

echo "Integration tests passed"
