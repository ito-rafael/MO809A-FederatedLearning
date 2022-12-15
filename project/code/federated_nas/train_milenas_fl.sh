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
clients=5

#-------------------
# start server
#-------------------
echo "Starting server"
python milenas_server.py &
sleep 10

#-------------------
# start clients
#-------------------
for i in $(seq 0 $(($clients - 1))); do
    GPU_ID=$(($i%5))
    echo "Starting client $i in GPU $GPU_ID"
    sleep 5
    CUDA_VISIBLE_DEVICES=$i python3 ./milenas_client.py \
	--client_id $i \
        --train_portion 0.5 \
        --gpu $GPU_ID \
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
