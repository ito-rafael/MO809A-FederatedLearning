#!/usr/bin/env python

import sys
sys.path.insert(0, "../")

from data_preprocessing.data_loader import partition_data, get_dataloader
import numpy as np
import torch
import random

seed = 0
np.random.seed(seed)
torch.manual_seed(seed)
torch.cuda.seed()
random.seed(seed)

X_train, y_train, X_test, y_test, net_dataidx_map, traindata_cls_counts = partition_data(
    'cifar10',          # args.dataset,
    './',   # args_datadir,
    '../log/cifar10',   # args_logdir,
    'hetero',           # args.partition,
    16,                 # args.client_number,
    0.5,                # args_alpha,
    args=None
)

for i in range(16):
    print(i, ': ', len(net_dataidx_map[i]), sep='')
print('----------------')

#------------------------------------------------i

counter = []
for i in range(16):
    classes = []
    for index in net_dataidx_map[i]:
        classes.append(y_train[index])
    counter.append(classes)

    print('client', i)
    for j in range(10):
        print(counter[i].count(j))
    print('--------')
