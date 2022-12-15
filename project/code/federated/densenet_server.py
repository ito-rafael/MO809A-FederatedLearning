#!/usr/bin/env python

import flwr as fl
from flwr.common import Metrics
from typing import List, Tuple

#---------------------------------------
# Define metric aggregation function
#---------------------------------------
def weighted_average(metrics: List[Tuple[int, Metrics]]) -> Metrics:
    # Multiply accuracy of each client by number of examples used
    accuracies = [num_examples * m["accuracy"] for num_examples, m in metrics]
    examples = [num_examples for num_examples, _ in metrics]

    # Aggregate and return custom metric (weighted average)
    return {"accuracy": sum(accuracies) / sum(examples)}

#---------------------------------------
# Additional config
#---------------------------------------
def fit_config(server_round: int):
    """Return training configuration dict for each round."""
    config = {
        #"batch_size": 32,
        "current_round": server_round,
        "local_epochs": 10,
    }
    return config

#---------------------------------------
# Define strategy
#---------------------------------------
strategy = fl.server.strategy.FedAvg(
    #evaluate_metrics_aggregation_fn=weighted_average,
    #fraction_evaluate=0.01,	# Fraction of clients used during validation
    #min_evaluate_clients = 1,	# Minimum number of clients used during validation
    #fraction_fit=0.1,		# Sample 10% of available clients for the next round
    #min_available_clients=2,	
    #min_fit_clients=2,		# Minimum number of clients to be sampled for the next round

    min_available_clients=5,
    min_fit_clients=5,
    #min_available_clients=4,
    #min_fit_clients=4,
    on_fit_config_fn=fit_config,

)

#---------------------------------------
# Start Flower server
#---------------------------------------
fl.server.start_server(
    server_address="0.0.0.0:8080",
    config=fl.server.ServerConfig(num_rounds=100),
    strategy=strategy,
)
