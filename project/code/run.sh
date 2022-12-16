#!/usr/bin/env bash
docker run \
    --gpus device=${1:-0} \
    --privileged \
    --userns=host \
    --ipc=host \
    --expose 8080 \
    --volume=$PWD/centralized:/mo809/centralized:rw \
    --volume=$PWD/centralized_nas:/mo809/centralized_nas:rw \
    --volume=$PWD/federated:/mo809/federated:rw \
    --volume=$PWD/federated_nas:/mo809/federated_nas:rw \
    --volume=$PWD/common:/mo809/common:rw \
    itorafael/mo809-project:v0.2 &
