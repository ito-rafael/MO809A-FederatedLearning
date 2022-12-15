#!/usr/bin/env bash
#CUDA_VISIBLE_DEVICES=0,1,2,3,4 ./train_milenas_fl.sh "0" 200003 1 100 "saved_models" 0.025 42 | tee benchmark_federated_nas
./train_milenas_fl.sh "0" 200003 1 100 "saved_models" 0.025 42 | tee benchmark_federated_nas

# Enable CTRL+C to stop all background processes
trap "trap - SIGTERM && kill -- -$$" SIGINT SIGTERM
# Wait for all background processes to complete
wait
