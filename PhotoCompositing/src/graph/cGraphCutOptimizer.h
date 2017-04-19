#ifndef __CGRAPHCUTOPTIMIZER_H__
#define __CGRAPHCUTOPTIMIZER_H__

#include "graph.h"

class cGraphCutOptimizer
{
    int *piAbsOrg;
    int *piAbs;  
    int m_uiNumOfCams = 1;
    unsigned int m_uiHorizontalPrecision;
    unsigned int m_uiVerticalPrecision;

    bool **m_ppbNodesActive;
    Graph::node_id **m_ppcNodes;

    //cArray<cMatrix<MatrixComputationalType>*> m_apmatIPP;
public:
    void init(cCfgParams &rcCfg, cMatcher* pcMatcher);
    void optimize(cArray<cYUV<ImagePixelType>*> &rapcYUVInput,
                  cArray<cCamParams<MatrixComputationalType>*> &rapcCameraParameters,
                  cArray<UInt*> &rapuiDepthLabel,
                  cArray<cYUV<ImagePixelType>*> &rapcYUVMask);
};

#endif