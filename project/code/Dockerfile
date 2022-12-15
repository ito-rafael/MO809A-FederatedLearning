# description: MO809A project: FL + NAS comparison
#   - centralized: DenseNet
#   - centralized_nas: MiLeNAS
#   - federated: FedAvg (on DenseNet)
#   - federated_nas: FedNAS (FL on MiLeNAS)
# author: Rafael Ito
# version:
#   - v0.1: first release
#   - v0.2: clone FedNAS git repo

#FROM conda/miniconda3
FROM itorafael/conda-dev:v0.2

ARG GIT_USER="chaoyanghe"
ARG GIT_REPO="MiLeNAS"
ARG CONDA_ENV="milenas"

# update system
USER root
RUN apt update && apt upgrade -y

# clone repo
RUN git clone "https://github.com/$GIT_USER/$GIT_REPO.git"
WORKDIR $GIT_REPO

# create conda env
RUN conda create --name $CONDA_ENV python=3
RUN conda init bash
SHELL ["conda", "run", "-n", "milenas", "/bin/bash", "-c"]
RUN conda install -n $CONDA_ENV pytorch torchvision torchaudio cudatoolkit=10.2 -c pytorch
RUN echo 'conda activate '$CONDA_ENV >> ~/.bashrc

# copy CIFAR-10 data
COPY common/data/cifar-10-batches-py data/cifar-10-batches-py

# additional packages
RUN pip install -r requirements.txt
RUN pip install \
    flwr

#COPY ./federated_nas/server.py .
#COPY ./federated_nas/client.py .
#COPY ./federated_nas/run.sh .
#COPY ./centralized .
#COPY ./centralized_nas .
#COPY ./federated .
#COPY ./federated_nas .


WORKDIR /mo809
ARG GIT_USER="chaoyanghe"
ARG GIT_REPO="FedNAS"
RUN git clone "https://github.com/$GIT_USER/$GIT_REPO.git"

RUN apt install -y procps

# prevent container from dying
ENTRYPOINT ["tail", "-f", "/dev/null"]
