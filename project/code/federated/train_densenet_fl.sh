#!/bin/bash

set -e
#cd "$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"/
# Download the CIFAR-10 dataset
#python -c "from torchvision.datasets import CIFAR10; CIFAR10('./data', download=True)"


#-------------------
GPU=$1
run_id=$2
LAMBDA_VALID=$3
EPOCH=$4
MODEL_FILE=$5
LR=$6
SEED=$7
#-------------------


# get number of clients (defalt = 2)
#clients="${1:-16}"
clients=10

#-------------------
# start server
#-------------------
echo "Starting server"
python densenet_server.py &
sleep 10

#-------------------
# start clients
#-------------------
for i in $(seq 0 $(($clients - 1))); do
    echo "Starting client $i"
    sleep 5
    python3 ./densenet_client.py \
	--client_id $i \
        --train_portion 0.8 \
        --gpu $GPU \
        --run_id $run_id \
        --lambda_valid_regularizer $LAMBDA_VALID \
        --batch_size 64 \
        --epochs $EPOCH \
        --model_path $MODEL_FILE \
	--seed $SEED \
	&
done

# Enable CTRL+C to stop all background processes
trap "trap - SIGTERM && kill -- -$$" SIGINT SIGTERM
# Wait for all background processes to complete
wait
