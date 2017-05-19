#ifndef __GRAPHCUTOPTIMIZER_H__
#define __GRAPHCUTOPTIMIZER_H__

#include "graph.h"

class GraphCutOptimizer
{
private:
    int *piAbsOrg;
    int *piAbs;  
    int m_uiNumOfCams = 1;
    unsigned int m_uiHorizontalPrecision;
    unsigned int m_uiVerticalPrecision;

    bool **m_ppbNodesActive;
    Graph::node_id **m_ppcNodes;

    //cArray<cMatrix<MatrixComputationalType>*> m_apmatIPP;
public:
    void init(cMatcher* pcMatcher);
    void optimize(cArray<cCamParams<MatrixComputationalType>*> &rapcCameraParameters,
                  cArray<UInt*> &rapuiDepthLabel);
};

#endif
