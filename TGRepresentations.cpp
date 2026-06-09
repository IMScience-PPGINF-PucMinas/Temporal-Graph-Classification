/**
 * Extensions proposed in:
 *
 * Carolina Almeida et al.
 * Temporal Graph Classification Based on Fast and Exact Transitive Reduction Strategy
 *
 * Based on:
 * Oettershagen et al. (2020)
 * Temporal Graph Kernels for Classifying Dissemination Processes
 */

#include "TGRepresentations.h"

void TGRepresentations::temporalGraphStreamToDirectedLineGraphDLTR(TemporalGraphStream tgs, TemporalGraphStream &tgsout, unsigned int fac1)
{

    struct Node
    {
        NodeId id;
        Label label;
        NodeId uid;
        NodeId vid;
        Time t;
        bool operator==(const Node &other) const
        {
            return id == other.id;
        }
    };

    NodeId nid = 0;
    vector<Node> nodes;
    bool new_node;
    int **matrix;
    Label **saveLabels;
    int size_edges = tgs.edges.size();
    int nodesTSize = tgs.timed_node_labels.size();
    saveLabels = new Label *[nodesTSize];

    for (int i = 0; i < nodesTSize; i++)
    {
        saveLabels[i] = new Label[nodesTSize];
    }

    for (int i = 0; i < nodesTSize; i++)
    {
        for (int j = 0; j < nodesTSize; j++)
        {
            saveLabels[i][j] = 0;
        }
    }

    for (int i = 0; i < size_edges; i++)
    {
        TemporalEdge e = tgs.edges[i];
        new_node = true;
        int j = i + 1;
        if (e.u_id < e.v_id && j <= size_edges)
        {
            while (j <= size_edges)
            {
                if (tgs.edges[j].u_id == e.v_id && tgs.edges[j].v_id == e.u_id && tgs.edges[j].t > e.t)
                {
                    new_node = false;
                    break;
                }
                j++;
            }
        }
        if (new_node)
        {
            Node n;
            n.id = nid++;
            n.uid = e.u_id;
            n.vid = e.v_id;
            n.t = e.t;
            if (saveLabels[e.v_id][e.u_id] > 0 || saveLabels[e.u_id][e.v_id] > 0)
            {
                n.label = fac1 * (1 + tgs.getNodeLabelAtTime(e.u_id, e.t)) + (1 + tgs.getNodeLabelAtTime(e.v_id, e.t + 1)) + saveLabels[e.v_id][e.u_id] + saveLabels[e.u_id][e.v_id];
            }
            else
            {
                n.label = fac1 * (1 + tgs.getNodeLabelAtTime(e.u_id, e.t)) + (1 + tgs.getNodeLabelAtTime(e.v_id, e.t + 1));
            }

            nodes.push_back(n);
            tgsout.static_node_labels.push_back(n.label);
        }
        else
        {
            saveLabels[e.u_id][e.v_id] += fac1 * (1 + tgs.getNodeLabelAtTime(e.v_id, e.t)) + (1 + tgs.getNodeLabelAtTime(e.u_id, e.t + 1));
        }
    }
    for (int i = 0; i < nodesTSize; i++)
    {
        delete[] saveLabels[i];
    }
    delete[] saveLabels;

    matrix = new int *[nid];
    for (int i = 0; i < nid; i++)
    {
        matrix[i] = new int[nid];
    }

    for (int i = 0; i < nid; i++)
    {
        Node n = nodes[i];
        for (int j = i + 1; j < nid; j++)
        {
            Node m = nodes[j];
            if ((n.vid == m.uid || n.vid == m.vid || n.uid == m.uid || n.uid == m.vid) && n.t < m.t)
            {
                matrix[n.id][m.id] = (n.t == m.t - 1) ? 1 : 0;
            }
            else
            {
                matrix[i][j] = -1;
            }
        }
    }
    for (int x = 0; x < nid; x++)
    {
        for (int y = x + 1; y < nid; y++)
        {
            if (matrix[x][y] != -1)
            {
                for (int z = y + 1; z < nid; z++)
                {
                    if (matrix[y][z] != -1)
                    {
                        matrix[x][z] = -1;
                    }
                }
                TemporalEdge e(x, y, matrix[x][y]);
                tgsout.edges.push_back(e);
                cout << char(x + 'a') << " " << char(y + 'a') << endl;
            }
        }
    }

    for (int i = 0; i < nid; i++)
    {
        delete[] matrix[i];
    }
    delete[] matrix;

    tgsout.label = tgs.label;
}

void TGRepresentations::temporalGraphStreamToDirectedLineGraphFDLTR(TemporalGraphStream tgs, TemporalGraphStream &tgsout,
                                                                    unsigned int fac1)
{
    buildTransitiveReduction(tgs, tgsout, fac1);
    tgsout.label = tgs.label;
}

void TGRepresentations::temporalGraphStreamToDirectedLineGraphFDLTRSM(TemporalGraphStream tgs, TemporalGraphStream &tgsout,
                                                                      unsigned int fac1)
{
    buildTransitiveReduction(tgs, tgsout, fac1);

    bool changed = true;
    while (changed)
    {
        changed = false;
        int num_nodes = tgsout.get_num_nodes();

        std::vector<int> indegree(num_nodes, 0);
        std::vector<int> outdegree(num_nodes, 0);

        for (const auto &e : tgsout.edges)
        {
            outdegree[e.u_id]++;
            indegree[e.v_id]++;
        }

        for (int v = 0; v < num_nodes; ++v)
        {
            if (indegree[v] == 1 && outdegree[v] == 1)
            {
                NodeId u = -1;
                int pred_edge_idx = -1;
                for (int i = 0; i < (int)tgsout.edges.size(); ++i)
                {
                    if (tgsout.edges[i].v_id == v)
                    {
                        u = tgsout.edges[i].u_id;
                        pred_edge_idx = i;
                        break;
                    }
                }

                NodeId w = -1;
                int succ_edge_idx = -1;
                for (int i = 0; i < (int)tgsout.edges.size(); ++i)
                {
                    if (tgsout.edges[i].u_id == v)
                    {
                        w = tgsout.edges[i].v_id;
                        succ_edge_idx = i;
                        break;
                    }
                }

                if (u != -1 && w != -1 && u != w)
                {
                    bool edge_exists = false;
                    for (const auto &e : tgsout.edges)
                    {
                        if (e.u_id == u && e.v_id == w)
                        {
                            edge_exists = true;
                            break;
                        }
                    }

                    if (!edge_exists && pred_edge_idx >= 0 && succ_edge_idx >= 0)
                    {
                        TemporalEdge new_edge(u, w, tgsout.edges[pred_edge_idx].t);
                        tgsout.edges.push_back(new_edge);
                    }

                    if (succ_edge_idx > pred_edge_idx)
                    {
                        tgsout.edges.erase(tgsout.edges.begin() + succ_edge_idx);
                        tgsout.edges.erase(tgsout.edges.begin() + pred_edge_idx);
                    }
                    else
                    {
                        tgsout.edges.erase(tgsout.edges.begin() + pred_edge_idx);
                        tgsout.edges.erase(tgsout.edges.begin() + succ_edge_idx);
                    }

                    if (v < (int)tgsout.static_node_labels.size())
                    {
                        tgsout.static_node_labels.erase(tgsout.static_node_labels.begin() + v);
                    }

                    for (auto &e : tgsout.edges)
                    {
                        if (e.u_id > v)
                            e.u_id--;
                        if (e.v_id > v)
                            e.v_id--;
                    }

                    changed = true;
                    break;
                }
            }
        }
    }

    tgsout.label = tgs.label;
}

void TGRepresentations::buildTransitiveReduction(TemporalGraphStream &tgs, TemporalGraphStream &tgsout, unsigned int fac1)
{
    struct Node
    {
        NodeId id;
        Label label;
        NodeId uid;
        NodeId vid;
        Time t;
        bool operator==(const Node &other) const
        {
            return id == other.id;
        }
    };

    NodeId nid = 0;
    vector<Node> nodes;

    for (TemporalEdge &e : tgs.edges)
    {
        Node n;
        n.id = nid++;
        n.uid = e.u_id;
        n.vid = e.v_id;
        n.t = e.t;

        n.label = fac1 * (1 + tgs.getNodeLabelAtTime(e.u_id, e.t)) + (1 + tgs.getNodeLabelAtTime(e.v_id, e.t + 1));

        nodes.push_back(n);
        tgsout.static_node_labels.push_back(n.label);
    }

    vector<vector<int>> succ(nid);
    for (int i = 0; i < nid; ++i)
    {
        for (int j = 0; j < nid; ++j)
        {
            if (nodes[i].vid == nodes[j].uid && nodes[i].t < nodes[j].t)
            {
                succ[i].push_back(j);
            }
        }
    }

    vector<int> order(nid);
    for (int i = 0; i < nid; ++i)
        order[i] = i;
    sort(order.begin(), order.end(), [&](int a, int b)
         { return nodes[a].t < nodes[b].t; });

    vector<unordered_set<int>> reach(nid);
    for (int idx = nid - 1; idx >= 0; --idx)
    {
        int u = order[idx];
        for (int v : succ[u])
        {
            reach[u].insert(v);
            for (int r : reach[v])
                reach[u].insert(r);
        }
    }

    for (int u = 0; u < nid; ++u)
    {
        for (int v : succ[u])
        {
            bool redundant = false;
            for (int w : succ[u])
            {
                if (w == v)
                    continue;
                if (reach[w].count(v) > 0)
                {
                    redundant = true;
                    break;
                }
            }
            if (!redundant)
            {
                int flag = (nodes[u].t == nodes[v].t - 1) ? 1 : 0;
                TemporalEdge e(u, v, flag);
                tgsout.edges.push_back(e);
            }
        }
    }
}
