//#ifndef __CGRAPHCUTOPTIMIZER_H__
//#define __CGRAPHCUTOPTIMIZER_H__
//
//#include "TLibCommon\TypeDef.h"
//#include "cOptimizer.h"
//#include "graph.h"
//
//class cGraphCutOptimizer : public cOptimizer{
//    Int *piAbsOrg;
//    Int *piAbs;  
//
//    UInt m_uiHorizontalPrecision;
//    UInt m_uiVerticalPrecision;
//
//    Bool **m_ppbNodesActive;
//    Graph::node_id **m_ppcNodes;
//
//    cArray<cMatrix<MatrixComputationalType>*> m_apmatIPP;
//public:
//    Void init(cCfgParams &rcCfg, cMatcher *pcMatcher);
//    Void optimize(cArray<cYUV<ImagePixelType>*> &rapcYUVInput, cArray<cCamParams<MatrixComputationalType>*> &rapcCameraParameters, cArray<UInt*> &rapuiDepthLabel, cArray<cYUV<ImagePixelType>*> &rapcYUVMask);
//};
//
//#endif