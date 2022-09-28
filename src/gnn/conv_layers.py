import torch
import torch.nn as nn
import torch_geometric.nn as geom_nn
from torch_geometric.nn.conv import MessagePassing

from .mlp import MLP


class ACRConv(MessagePassing):
    def __init__(
            self,
            input_dim: int,
            output_dim: int,
            aggregate_type: str,
            readout_type: str,
            combine_type: str,
            combine_layers: int,
            num_mlp_layers: int,
            num_relation=5,
            **kwargs):

        assert aggregate_type in ["add", "mean", "max"]
        assert combine_type in ["simple", "mlp"]
        assert readout_type in ["add", "mean", "max"]

        super(ACRConv, self).__init__(aggr=aggregate_type, **kwargs)
        self.num_relation=num_relation
        self.mlp_combine = False
        if combine_type == "mlp":
            self.mlp = MLP(
                num_layers=num_mlp_layers,
                input_dim=output_dim,
                hidden_dim=output_dim,
                output_dim=output_dim)

            self.mlp_combine = True
        self.output_dim=output_dim
        self.V = MLP(
            num_layers=combine_layers,
            input_dim=input_dim,
            hidden_dim=output_dim,
            output_dim=output_dim)
        self.A=torch.nn.ModuleList()
        for i in range(self.num_relation):
            self.A.append(MLP(
                num_layers=combine_layers,
                input_dim=input_dim,
                hidden_dim=output_dim,
                output_dim=output_dim))
        self.R = MLP(
            num_layers=combine_layers,
            input_dim=input_dim,
            hidden_dim=output_dim,
            output_dim=output_dim)

        self.readout = self.__get_readout_fn(readout_type)

    def __get_readout_fn(self, readout_type):
        options = {
            "add": geom_nn.global_add_pool,
            "mean": geom_nn.global_mean_pool,
            "max": geom_nn.global_max_pool
        }
        if readout_type not in options:
            raise ValueError()
        return options[readout_type]

    def forward(self, h, edge_index, edge_attr,batch):

        # this give a (batch_size, features) tensor
        readout = self.readout(x=h, batch=batch)
        # this give a (nodes, features) tensor
        readout = readout[batch]
        out=torch.zeros((h.shape[0],self.output_dim),device=h.device)
        for i in range(self.num_relation):
            masked_edge_index=edge_index.T[torch.where(edge_attr[:,0]==i)].T
            out+=self.propagate(edge_index=masked_edge_index,h=h,typ=i,readout=readout)
        out+=self.V(h)
        return self.propagate(
            edge_index=edge_index,
            h=out,
            readout=readout,typ=-1)

    def message(self, h_j):
        return h_j

    def update(self, aggr, h, readout,typ):
        if typ>=0:
            return self.A[typ](aggr)
        else:
            updated = h + self.R(readout)

        if self.mlp_combine and typ==-1:
            updated = self.mlp(updated)

        return updated

    def reset_parameters(self):
        self.V.reset_parameters()
        self.A.reset_parameters()
        self.R.reset_parameters()
        if hasattr(self, "mlp"):
            self.mlp.reset_parameters()


class ACConv(MessagePassing):
    def __init__(
            self,
            input_dim: int,
            output_dim: int,
            aggregate_type: str,
            combine_type: str,
            combine_layers: int,
            num_mlp_layers: int,
            num_relation=5,
            **kwargs):

        assert aggregate_type in ["add", "mean", "max"]
        assert combine_type in ["simple", "mlp"]

        super(ACConv, self).__init__(aggr=aggregate_type, **kwargs)
        self.mlp_combine = False
        self.num_relation=num_relation
        self.output_dim=output_dim
        if combine_type == "mlp":
            self.mlp = MLP(
                num_layers=num_mlp_layers,
                input_dim=output_dim,
                hidden_dim=output_dim,
                output_dim=output_dim)

            self.mlp_combine = True

        self.V = MLP(
            num_layers=combine_layers,
            input_dim=input_dim,
            hidden_dim=output_dim,
            output_dim=output_dim)
        self.A=torch.nn.ModuleList()
        for i in range(self.num_relation):
            self.A.append(MLP(
                num_layers=combine_layers,
                input_dim=input_dim,
                hidden_dim=output_dim,
                output_dim=output_dim))

    def forward(self, h, edge_index,edge_attr, batch):
        out=torch.zeros((h.shape[0],self.output_dim),device=h.device)
        for i in range(self.num_relation):
            masked_edge_index=edge_index.T[torch.where(edge_attr[:,0]==i)].T
            out+=self.propagate(edge_index=masked_edge_index,h=h,typ=i)
        out+=self.V(h)
        if self.mlp_combine:
            out = self.mlp(out)
        return out
    def message(self, h_j):
        return h_j

    def update(self, aggr, h,typ):
        updated = self.A[typ](aggr)

        return updated

    def reset_parameters(self):
        self.V.reset_parameters()
        self.A.reset_parameters()
        if hasattr(self, "mlp"):
            self.mlp.reset_parameters()
