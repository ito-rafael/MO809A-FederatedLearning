#!/usr/bin/env python

CLIENT = 0

import pickle

with open('client_' + str(CLIENT).zfill(2), 'rb') as fp:
    data = pickle.load(fp)
