#ifndef TGKERNEL_TGREPRESENTATIONS_H
#define TGKERNEL_TGREPRESENTATIONS_H

#include "TemporalGraphs.h"

#include <Eigen/Core>
#include <Eigen/Sparse>

class TGRepresentations
{
public:
    // Original temporal graph representation proposed by Oettershagen et al. (2020)
    void temporalGraphStreamToDirectedLineGraph(
        TemporalGraphStream tgs,
        TemporalGraphStream &tgsout,
        unsigned int fac1);

    // Additional temporal graph representations proposed in:
    // "Temporal Graph Classification Based on Fast and Exact Transitive Reduction Strategy"
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

private:
    // Helper method for FDLTR-based representations
    void buildTransitiveReduction(
        TemporalGraphStream &tgs,
        TemporalGraphStream &tgsout,
        unsigned int fac1);
};

#endif // TGKERNEL_TGREPRESENTATIONS_H