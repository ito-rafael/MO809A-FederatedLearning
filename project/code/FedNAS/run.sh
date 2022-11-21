#!/bin/bash
set -e
#cd "$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"/
# Download the CIFAR-10 dataset
#python -c "from torchvision.datasets import CIFAR10; CIFAR10('./data', download=True)"

# get number of clients (defalt = 2)
clients="${1:-2}"

#-------------------
# start server
#-------------------
echo "Starting server"
python server.py &
sleep 3

#-------------------
# start clients
#-------------------
for i in $(seq $clients); do
    echo "Starting client $i"
    python client.py &
done

# Enable CTRL+C to stop all background processes
trap "trap - SIGTERM && kill -- -$$" SIGINT SIGTERM
# Wait for all background processes to complete
wait
