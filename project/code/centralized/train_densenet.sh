#!/usr/bin/env bash

#CUDA_VISIBLE_DEVICES=0 ./search_algorithm/run_densenet.sh "0" 200003 1 50 "saved_models" 0.025 0.0003 2021 8

GPU=$1
run_id=$2
LAMBDA_VALID=$3
EPOCH=$4
MODEL_FILE=$5
LR=$6
SEED=$7

python3 ./train_densenet.py \
    --train_portion 0.8 \
    --gpu $GPU \
    --run_id $run_id \
    --lambda_valid_regularizer $LAMBDA_VALID \
    --batch_size 64 \
    --epochs $EPOCH \
    --model_path $MODEL_FILE \
    --seed $7
