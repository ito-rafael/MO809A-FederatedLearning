#!/usr/bin/env bash
CUDA_VISIBLE_DEVICES=1 ./train_milenas.sh "1" 200003 1 50 "saved_models" 0.025 0.0003 2021 8 | tee ./benchmark_centralized_nas2
