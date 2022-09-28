# Logical Expressiveness Of GNNs Over Multi-edge
Graphs

Code for the paper Logical Expressiveness Of GNNs Over Multi-edge
Graphs.

Builds based on https://github.com/juanpablos/GNN-logic/tree/master/src

## Install

Run `pip install -r requirements.txt` to install all dependencies.

## Generate synthetic graphs

`generate_temporal/generate.cpp` for dataset generation of synthetic graphs. In order to generate datasets consist of transformed graphs, please refer to `generate_temporal/generate_trans.cpp`

## Replicate results

Run the command in `python src/main.py [dataset] [time_range] [num_relation]`. The results will be printed to console and logged in `src/logging/results`. A single file will collect the last epoch for each experiment for each dataset.

A description for different datasets and the specific arguments they needed are as follows:

```
tp1 2 1
tp2 2 1
tp3 2 1
tp1_trans 1 2
tp2_trans 1 2
tp3_trans 1 2
tp4 10 3
tp4_trans 1 30
mutag 1 46
aifb 1 90
brain 12 10
```

