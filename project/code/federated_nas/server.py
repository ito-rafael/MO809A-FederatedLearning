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
# Define strategy
#---------------------------------------
strategy = fl.server.strategy.FedAvg(
    #evaluate_metrics_aggregation_fn=weighted_average,
    fraction_evaluate=0.01,
    min_evaluate_clients = 1,
    fraction_fit=0.1,
    min_available_clients=2,
    min_fit_clients=2
)

#---------------------------------------
# Start Flower server
#---------------------------------------
fl.server.start_server(
    server_address="0.0.0.0:8080",
    config=fl.server.ServerConfig(num_rounds=3),
    #config=fl.server.ServerConfig(num_rounds=50),
    strategy=strategy,
)