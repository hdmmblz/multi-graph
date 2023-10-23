# Calibrate and Boost Logical Expressiveness of GNN Over Multi-Relational and Temporal Graphs

Code for the paper Calibrate and Boost Logical Expressiveness of GNN Over Multi-Relational and Temporal Graphs。

Builds based on https://github.com/juanpablos/GNN-logic/tree/master/src

## Install

Run `pip install -r requirements.txt` to install all dependencies.

## Generate synthetic graphs

`generate_temporal/generate.cpp` for dataset generation of synthetic graphs. In order to generate datasets consist of transformed graphs, please refer to `generate_temporal/generate_trans.cpp`

## Replicate results

Download datasets from https://drive.google.com/file/d/1t8oKegE79ctV5aiMKyklmXbV5-fCODgk/view?usp=sharing, then unzip it under `src/`

Temporal Real-world dataset are from https://www.dropbox.com/s/yo74hwj7k8alpuj/ICML2022-Data-Submit.zip?dl=0&file_subpath=%2FICML2022-Data-Submit

Static Real-world dataset are from https://github.com/yangjunhuiyjh/RGCN which is preprocessed by adding reverse relations, downloaded from `torch_geometric`

Run the command as `python src/main.py [dataset] [time_range] [num_relation]`. The results will be printed to console and logged in `src/logging/results`. A single file will collect the last epoch for each experiment for each dataset.

A description for different datasets and the specific arguments they needed are as follows:

```
python src/main.py tp1 2 1                          #\varphi_1
python src/main.py tp2 2 1                          #\varphi_2
python src/main.py tp3 2 1                          #\varphi_3
python src/main.py tp1_trans 1 2                    #transformed \varphi_1
python src/main.py tp2_trans 1 2                    #transformed \varphi_2
python src/main.py tp3_trans 1 2                    #transformed \varphi_3
python src/main.py tp4 10 3                         #\varphi_4
python src/main.py tp4_trans 1 30                   #transformed \varphi_4
python src/main.py mutag 1 46                       #mutag
python src/main.py aifb 1 90                        #aifb
python src/main.py brain 12 10                      #brain-10
```

