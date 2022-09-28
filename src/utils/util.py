from typing import List, Set, Tuple

import networkx as nx
import torch
from sklearn.model_selection import train_test_split
from torch_geometric.data import Data


def load_data(dataset: str,
              degree_as_node_label: bool = False,
              graph_type: int = 2,
              undirected=True) -> Tuple[List[Data],
                                        Tuple[int, int, int]]:

    if graph_type == 1:
        raise NotImplementedError()

    print('Loading data...')
    graph_list: List[Data] = []

    unique_graph_label: Set[int] = set()
    unique_node_labels: Set[int] = set()
    unique_node_features: Set[int] = set()
    with open(dataset, 'r') as in_file:
        row_a=in_file.readline().strip().split(' ')
        n_graphs = int(row_a[0])
        #n_graphs=2
        n_layers = int(row_a[1])
        for _ in range(n_graphs):
            pin_file=in_file.readline().strip().split(" ")
            n_nodes, graph_label = map(
                int, pin_file)

            # register graph label (not really important)
            unique_graph_label.add(graph_label)

            graph: nx.DiGraph = nx.DiGraph()
            node_labels: List[int] = []
            if n_layers==12:
                node_features: List[List[float]]=[]
            else:
                node_features: List[List[int]] = []

            # --- READING GRAPH ----
            # for each node in the graph
            edges=[]
            edge_feature=[]
            for node_id in range(n_nodes):
                # add the index (starts at 0 to n_nodes-1)
                graph.add_node(node_id)

                # n_features, [features], node label, n_edges, [neighbors]
                # * only work for categorical node features
                if n_layers==12:
                    node_row=list(in_file.readline().strip().split(" "))
                else:
                    node_row = list(
                    map(int, in_file.readline().strip().split(" ")))

                # ---- FEATURES ----
                # first comes the number of features
                n_features = int(node_row[0])
                # if n_features > max_features:
                #     max_features = n_features
                features = []
                if n_features != 0:
                    # get all features, column 1 to n_features-1
                    if n_layers==12:
                        features = list(map(float,node_row[1:n_features + 1]))
                    else:
                        features= node_row[1:n_features + 1]
                if n_layers!=12:
                    unique_node_features.update(features)

                node_features.append(features)
                # ---- /FEATURES ----

                # ---- LABELS ----
                # TODO: support multiple labels
                node_label = int(node_row[n_features + 1])
                if node_label>=0:
                    unique_node_labels.add(node_label)

                node_labels.append(node_label)
                # ---- /LABELS ----

                # ---- EDGES ----
                # get the rest, the neighbours
                neighbors = list(map(int,node_row[n_features + 3:]))
                # register connections
                for i in range(0,len(neighbors),2):
                    edges.append([node_id, neighbors[i]])
                    edge_feature.append(neighbors[i+1])
                    #if undirected:
                        #graph.add_edge(neighbors[i], node_id,object=neighbors[i+1])

                # ---- /EDGES ----
            # --- /READING GRAPH ----

            edges = torch.tensor(edges, dtype=torch.long)
            edge_feature=torch.tensor(edge_feature,dtype=torch.long).reshape(-1,1)
            node_labels = torch.tensor(node_labels)

            # placeholder
            features = torch.tensor(node_features)
            graph_list.append(
                Data(
                    x=features,
                    edge_index=edges.t().contiguous(),
                    node_labels=node_labels,
                    edge_attr=edge_feature,
                    graph_label=torch.tensor([graph_label])
                ))

    num_features = len(unique_node_features)
    for data in graph_list:
        if n_layers!=12:
            x = torch.nn.functional.one_hot(
            data.x.squeeze(), num_features).type(torch.FloatTensor)
            data.x = x

    print(f"#Graphs: {len(graph_list)}")
    print(f"#Graphs Labels: {len(unique_graph_label)}")
    print(f"#Node Features: {len(unique_node_features)}")
    print(f"#Node Labels: {len(unique_node_labels)}")
    tmp=[]
    for i in range(0,len(graph_list),n_layers):
        tmpp=[]
        for j in range(i,i+n_layers):
            tmpp.append(graph_list[j])
        tmp.append(tmpp)
    graph_list=tmp
    return graph_list, \
        (len(unique_graph_label),
         len(unique_node_features),
         len(unique_node_labels))


def separate_data(graph_list, seed: int,
                  test_size: float = 0.2):

    return train_test_split(
        graph_list,
        random_state=seed,
        test_size=test_size,
        shuffle=True)


if __name__ == "__main__":
    a, _ = load_data(dataset="../data/train-cycle-150-50-150.txt")
    print(a[0].x)
    print(a[0].edge_index)
    print(a[0].node_labels)
    print(a[0].graph_label)
