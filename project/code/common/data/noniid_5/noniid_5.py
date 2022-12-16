#!/usr/bin/env python

import sys
sys.path.insert(0, "../..")     # /mo809/common

from data_preprocessing.data_loader import partition_data, get_dataloader
import numpy as np
import torch
import random
import pickle

seed = 0
np.random.seed(seed)
torch.manual_seed(seed)
torch.cuda.seed()
random.seed(seed)

X_train, y_train, X_test, y_test, net_dataidx_map, traindata_cls_counts = partition_data(
    'cifar10',          # args.dataset,
    '../',   # args_datadir,
    '../log/cifar10',   # args_logdir,
    'hetero',           # args.partition,
    5,                  # args.client_number,
    0.5,                # args_alpha,
    args=None
)

for i in range(5):
    print(i, ': ', len(net_dataidx_map[i]), sep=' ')
    with open('client_' + str(i).zfill(2), 'wb') as fp:
        pickle.dump(net_dataidx_map[i], fp)
print('----------------')

# to unpickle
with open('client_' + str(i).zfill(2), 'rb') as fp:
      data = pickle.load(fp)

#------------------------------------------------i

counter = []
for i in range(5):
    classes = []
    for index in net_dataidx_map[i]:
        classes.append(y_train[index])
    counter.append(classes)

    print('client', i)
    for j in range(10):
        print(counter[i].count(j))
    print('--------')
