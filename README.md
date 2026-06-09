# Temporal Graph Classification Based on Fast and Exact Transitive Reduction Strategy

Source code accompanying the paper:

**Temporal Graph Classification Based on Fast and Exact Transitive Reduction Strategy**

This repository extends the implementation of temporal graph representations proposed in:

> L. Oettershagen, N. Kriege, C. Morris, and P. Mutzel.
> _Temporal Graph Kernels for Classifying Dissemination Processes_.
> SIAM International Conference on Data Mining (SDM), 2020.
> https://gitlab.com/tgpublic/tgkernel

The original TGKernel framework is used as the basis for generating static graph representations from temporal graphs. This repository introduces three additional temporal-to-static graph transformations proposed in our paper:

- DLTR (Directed Line Graph with Transitive Reduction)
- FDLTR (Fast Directed Line Graph with Transitive Reduction)
- FDLTRSM (Fast Directed Line Graph with Transitive Reduction and Smoothing)

---

## Added Functions

The following functions must be added to `TGRepresentations.h`:

```cpp
void temporalGraphStreamToDirectedLineGraphDLTR(
    TemporalGraphStream tgs,
    TemporalGraphStream &tgsout,
    unsigned int fac1);

void temporalGraphStreamToDirectedLineGraphFDLTR(
    TemporalGraphStream tgs,
    TemporalGraphStream &tgsout,
    unsigned int fac1);

void temporalGraphStreamToDirectedLineGraphFDLTRSM(
    TemporalGraphStream tgs,
    TemporalGraphStream &tgsout,
    unsigned int fac1);
```

Their implementations are provided in:

```cpp
TGRepresentations.cpp
```

The main executable must be updated to invoke the desired representation before kernel computation.

---

## Compilation

The project requires:

- C++17
- CMake
- Eigen3

Set the Eigen3 path:

```bash
export EIGEN3_INCLUDE_DIR=/path/to/eigen-3.x.x
```

Create a build directory:

```bash
mkdir release
cd release
```

Configure:

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```

Compile:

```bash
make
```

The executable `tgkernel` will be generated.

---

## Datasets

The datasets used in the experiments are identical to those employed by Oettershagen et al. (2020).

Classification task identifiers:

- `_ct1` : Classification Task 1
- `_ct2` : Classification Task 2

Datasets:

- HighSchool
- Infectious
- Tumblr
- DBLP
- Facebook
- MIT

---

## Generating Static Graph Representations

The proposed methods generate static graph representations from temporal graph streams.

Available representations:

| Representation | Description                                                       |
| -------------- | ----------------------------------------------------------------- |
| DL             | Directed Line Graph                                               |
| DLTR           | Directed Line Graph + Exact Transitive Reduction                  |
| FDLTR          | Fast Directed Line Graph + Exact Transitive Reduction             |
| FDLTRSM        | Fast Directed Line Graph + Exact Transitive Reduction + Smoothing |

The generated static graphs can subsequently be used by any graph kernel implementation.

---

## Classification

After generating the static graphs, classification is performed using standard graph kernels.

Following the experimental protocol described in the paper, we employ:

- Weisfeiler-Lehman (WL) subtree kernel
- 3-node Graphlet (GL) kernel

implemented in:

> Morris et al., TUDataset graph kernel framework.

The generated static graph files can be directly exported and used as input for the classification stage.

---

## Reproducing Experiments

Example execution:

```bash
./tgkernel highschool_ct1/highschool_ct1 16 0.5 0.5
./tgkernel highschool_ct2/highschool_ct2 16 0.5 0.5
./tgkernel infectious_ct1/infectious_ct1 16 0.5 0.5
./tgkernel infectious_ct2/infectious_ct2 16 0.5 0.5
./tgkernel tumblr_ct1/tumblr_ct1 16 0.5 0.5
./tgkernel tumblr_ct2/tumblr_ct2 16 0.5 0.5
./tgkernel dblp_ct1/dblp_ct1 16 0.5 0.5
./tgkernel dblp_ct2/dblp_ct2 16 0.5 0.5
./tgkernel facebook_ct1/facebook_ct1 16 0.5 0.5
./tgkernel facebook_ct2/facebook_ct2 16 0.5 0.5
```

Additional experiments:

```bash
./tgkernel mit_ct1/mit_ct1 19 0.5 0.5
./tgkernel mit_ct2/mit_ct2 19 0.5 0.5
```

---

## Citation

If you use this code, please cite:

```bibtex
@article{almeida2026,
  title={Temporal Graph Classification Based on Fast and Exact Transitive Reduction Strategy},
  author={Almeida, Carolina and others},
  year={2026}
}
```

and

```bibtex
@inproceedings{Oettershagen2020,
  title={Temporal Graph Kernels for Classifying Dissemination Processes},
  author={Oettershagen, Lutz and Kriege, Nils and Morris, Christopher and Mutzel, Petra},
  booktitle={SIAM International Conference on Data Mining},
  pages={496--504},
  year={2020}
}
```

---

## Contact

For questions regarding the extensions proposed in this work, please contact the authors of:

**Temporal Graph Classification Based on Fast and Exact Transitive Reduction Strategy**.
