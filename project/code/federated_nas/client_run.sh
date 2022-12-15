#!/bin/bash
set -e

#CUDA_VISIBLE_DEVICES=0 ./train_milenas_fl.sh "0" 200003 1 50 "saved_models" 0.025 | tee benchmark_federated_nas
#-------------------
GPU=5
run_id=200003
LAMBDA_VALID=1
#EPOCH=50
EPOCH=100
MODEL_FILE="saved_models"
LR=0.025
SEED=42
#-------------------

#clients=2
echo "Starting client $i"
python3 ./milenas_client.py \
    --client_id 0 \
    --train_portion 0.5 \
    --gpu $GPU \
    --run_id $run_id \
    --lambda_valid_regularizer $LAMBDA_VALID \
    --batch_size 64 \
    --epochs $EPOCH \
    --model_path $MODEL_FILE \
    --seed $SEED \
    &
