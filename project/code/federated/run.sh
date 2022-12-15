#!/usr/bin/env bash
CUDA_VISIBLE_DEVICES=3 ./train_densenet_fl.sh "3" 200003 1 100 "saved_models" 0.025 1234 | tee benchmark_federated5

# Enable CTRL+C to stop all background processes
trap "trap - SIGTERM && kill -- -$$" SIGINT SIGTERM
# Wait for all background processes to complete
wait
