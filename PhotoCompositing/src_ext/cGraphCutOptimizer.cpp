//#include "cGraphCutOptimizer.h"
//#include <time.h>
//
//Void cGraphCutOptimizer::init(cCfgParams &rcCfg, cMatcher *pcMatcher){
//    cOptimizer::init(rcCfg, pcMatcher);
//
//    m_uiHorizontalPrecision = pcMatcher->m_uiHorizontalPrecision;
//    m_uiVerticalPrecision = pcMatcher->m_uiVerticalPrecision;
//
//    piAbsOrg = new Int[2 * m_uiNumOfLabels + 1];
//    piAbs = &piAbsOrg[m_uiNumOfLabels];
//
//    for (Int l = 0; l < Int(m_uiNumOfLabels); l++) {
//        piAbs[l] = l;
//        piAbs[-l] = l;
//    }
//
//    m_ppbNodesActive = new Bool*[m_uiNumOfCams];
//    m_ppcNodes = new Graph::node_id*[m_uiNumOfCams];
//    for (UInt uiCam = 0; uiCam < m_uiNumOfCams; uiCam++){
//        m_ppbNodesActive[uiCam] = new Bool[m_uiImSize];
//        m_ppcNodes[uiCam] = new Graph::node_id[m_uiImSize];
//    }
//}
//
//#define IS_BLOCKED(d1,d2) ((d1) < (d2)) //d2 closer
//#define INFINITY 10000
//#define CrossCostLinear(a,b) (1.0*Abs((Int)(a)-(Int)(b)))
//#define DIST(x1,y1,x2,y2) (Sqr((Int)(x1)-(Int)(x2))+Sqr((Int)(y1)-(Int)(y2)));
//
//#define SecondOrderCostLinear(a,b,c) (0.2*Abs(2*(Double)(b)-(Double)(a)-(Double)(c)))
////For every view keep list of outliers. Dla kazdego punktu lista widaków w których ten punkt najprawdopodobniej jest zas³oniety. Licz wariance na podstawie tylko nie zas³onietych widoków
//
////Dodaæ WTA na variancji kosztu
//
//Void cGraphCutOptimizer::optimize(cArray<cYUV<ImagePixelType>*> &rapcYUVInput, cArray<cCamParams<MatrixComputationalType>*> &rapcCameraParameters, cArray<UInt*> &rapuiDepthLabel, cArray<cYUV<ImagePixelType>*> &rapcYUVMask){
//    fprintf(stdout, "Graph Cut\n");
//
//    UInt uiMaxCycle = 2;
//    UInt pp, right, down;
//    Double cost_curr, cost_down, cost_right, cost_tmp;
//    UInt Wm1 = m_uiWidth - 1;
//    UInt Hm1 = m_uiHeight - 1;
//
//    Double dEnergy;
//    Double dOldEnergy = 100000000000000;
//
//    for (UInt uiCycle = 0; uiCycle < uiMaxCycle; uiCycle++){
//        for (UInt uiSource = 0; uiSource < (Int)m_uiNumOfLabels; uiSource++){
//            // starting time
//            clock_t clkTimeBegin, clkTimeEnd;
//            clkTimeBegin = clock();
//
//            fprintf(stdout, "%d\t", uiSource);
//            Graph *g = new Graph();
//
//            if (m_LowMem) m_pcMatcher->precomputeMatchingErrorsOfPlane(rapcYUVInput, rapcCameraParameters, uiSource);
//
//            dEnergy = 0.0;
//
//            for (UInt uiCam = 0; uiCam < m_uiNumOfCams; uiCam++){
//                for (pp = 0; pp < m_uiImSize; pp++){
//
//#if INPUT_MASK
//                    if (rapcYUVMask[uiCam]->m_atY[pp] == 0){
//                        m_ppcNodes[uiCam][pp] = NULL;
//                        m_ppbNodesActive[uiCam][pp] = false;
//                        continue;
//                    }
//#endif
//                    if (rapuiDepthLabel[uiCam][pp] == uiSource){
//                        m_ppcNodes[uiCam][pp] = NULL;
//                        m_ppbNodesActive[uiCam][pp] = false;
//                    }
//                    else {
//                        m_ppcNodes[uiCam][pp] = g->add_node();
//                        m_ppbNodesActive[uiCam][pp] = true;
//                    }
//                }
//            }
//
//            //*
//            //Smoothing term in z domain
//            for (UInt uiCamId = 0; uiCamId < m_uiNumOfCams; uiCamId++){
//                cMatrix<Double> PcIP(4, 4);
//                MatrixMultiply(rapcCameraParameters[m_uiCenterCam]->m_mProjMat, rapcCameraParameters[uiCamId]->m_mInvProjMat, PcIP);
//
//                for (UInt uiY = 0, uiPos = 0; uiY < m_uiHeight; uiY++) {
//                    for (UInt uiX = 0; uiX < m_uiWidth; uiX++, uiPos++) {
//
//                        UInt uiLabel = rapuiDepthLabel[uiCamId][uiPos];
//                        Double dInvZ = m_adLabel2InvZ[uiLabel];
//#if GLOBAL_LABELS
//                        Double dZ = (1 / dInvZ - PcIP.at(2, 3)) / (uiX * PcIP.at(2, 0) + uiY* PcIP.at(2, 1) + PcIP.at(2, 2)); //Project Z value from center cam to current cam
//#else
//                        Double dZ = 1/dInvZ;
//#endif
//                        dInvZ = m_adLabel2InvZ[uiSource];
//#if GLOBAL_LABELS
//                        Double dZSource = (1 / dInvZ - PcIP.at(2, 3)) / (uiX * PcIP.at(2, 0) + uiY* PcIP.at(2, 1) + PcIP.at(2, 2)); //Project Z value from center cam to current cam
//#else
//                        Double dZSource = 1/dInvZ;
//#endif
//                        //Horizontal
//
//#if INPUT_MASK
//                        if (rapcYUVMask[uiCamId]->m_atY[uiPos] != 0 || rapcYUVMask[uiCamId]->m_atY[uiPos + 1] != 0)
//#endif
//
//                        if (uiX < m_uiWidth - 1) {
//                            UInt uiLabelPlusOne = rapuiDepthLabel[uiCamId][uiPos + 1];
//                            dInvZ = m_adLabel2InvZ[uiLabelPlusOne];
//#if GLOBAL_LABELS
//                            Double dZPlusOne = (1 / dInvZ - PcIP.at(2, 3)) / ((uiX + 1) * PcIP.at(2, 0) + uiY* PcIP.at(2, 1) + PcIP.at(2, 2)); //Project Z value from center cam to current cam
//#else
//                            Double dZPlusOne = 1/dInvZ;
//#endif
//                            dInvZ = m_adLabel2InvZ[uiSource];
//#if GLOBAL_LABELS
//                            Double dZSourcePlusOne = (1 / dInvZ - PcIP.at(2, 3)) / ((uiX + 1) * PcIP.at(2, 0) + uiY* PcIP.at(2, 1) + PcIP.at(2, 2)); //Project Z value from center cam to current cam
//#else
//                            Double dZSourcePlusOne = 1/dInvZ;
//#endif
//
//                            Double E00 = 0.0;
//                            Double E01 = 0.0;
//                            Double E10 = 0.0;
//                            Double E11 = 0.0;
//                            if (m_ppbNodesActive[uiCamId][uiPos] && m_ppbNodesActive[uiCamId][uiPos + 1]) {
//                                E00 = m_dSmoothingCoeff * CrossCostLinear(dZ, dZPlusOne);
//                            }
//                            if (m_ppbNodesActive[uiCamId][uiPos]) {
//                                E01 = m_dSmoothingCoeff * CrossCostLinear(dZ, dZSourcePlusOne);
//                            }
//                            if (m_ppbNodesActive[uiCamId][uiPos + 1]) {
//                                E10 = m_dSmoothingCoeff * CrossCostLinear(dZSource, dZPlusOne);
//                            }
//                            if (true) {
//                                E11 = m_dSmoothingCoeff * CrossCostLinear(dZSource, dZSourcePlusOne); //0;
//                            }
//
//
//                            if (m_ppbNodesActive[uiCamId][uiPos]) {
//                                if (m_ppbNodesActive[uiCamId][uiPos + 1]) {
//                                    g->add_term2(m_ppcNodes[uiCamId][uiPos], m_ppcNodes[uiCamId][uiPos + 1], E00, E01, E10, E11);
//                                }
//                                else {
//                                    g->add_term1(m_ppcNodes[uiCamId][uiPos], E01, E11);
//                                }
//                            }
//                            else {
//                                if (m_ppbNodesActive[uiCamId][uiPos + 1]) {
//                                    g->add_term1(m_ppcNodes[uiCamId][uiPos + 1], E10, E11);
//                                }
//                                else {
//                                    //Nothig to add
//                                    dEnergy += E11;
//                                }
//                            }
//                        }
//
//                        //Vertical
//
//#if INPUT_MASK
//                        if (rapcYUVMask[uiCamId]->m_atY[uiPos] != 0 || rapcYUVMask[uiCamId]->m_atY[uiPos + m_uiWidth] != 0)
//#endif
//
//                        if (uiY < m_uiHeight - 1) {
//                            UInt uiLabelPlusOne = rapuiDepthLabel[uiCamId][uiPos + m_uiWidth];
//                            dInvZ = m_adLabel2InvZ[uiLabelPlusOne];
//#if GLOBAL_LABELS
//                            Double dZPlusOne = (1 / dInvZ - PcIP.at(2, 3)) / (uiX * PcIP.at(2, 0) + (uiY + 1)* PcIP.at(2, 1) + PcIP.at(2, 2)); //Project Z value from center cam to current cam
//#else
//                            Double dZPlusOne = 1/dInvZ;
//#endif
//                            dInvZ = m_adLabel2InvZ[uiSource];
//#if GLOBAL_LABELS
//                            Double dZSourcePlusOne = (1 / dInvZ - PcIP.at(2, 3)) / (uiX * PcIP.at(2, 0) + (uiY + 1)* PcIP.at(2, 1) + PcIP.at(2, 2)); //Project Z value from center cam to current cam
//#else
//                            Double dZSourcePlusOne = 1/dInvZ;
//#endif
//
//                            Double E00 = 0.0;
//                            Double E01 = 0.0;
//                            Double E10 = 0.0;
//                            Double E11 = 0.0;
//                            if (m_ppbNodesActive[uiCamId][uiPos] && m_ppbNodesActive[uiCamId][uiPos + m_uiWidth]) {
//                                E00 = m_dSmoothingCoeff * CrossCostLinear(dZ, dZPlusOne);
//                            }
//                            if (m_ppbNodesActive[uiCamId][uiPos]) {
//                                E01 = m_dSmoothingCoeff * CrossCostLinear(dZ, dZSourcePlusOne);
//                            }
//                            if (m_ppbNodesActive[uiCamId][uiPos + m_uiWidth]) {
//                                E10 = m_dSmoothingCoeff * CrossCostLinear(dZSource, dZPlusOne);
//                            }
//                            if (true) {
//                                E11 = m_dSmoothingCoeff * CrossCostLinear(dZSource, dZSourcePlusOne); //0.0
//                            }
//                            if (m_ppbNodesActive[uiCamId][uiPos]) {
//                                if (m_ppbNodesActive[uiCamId][uiPos + m_uiWidth]) {
//                                    g->add_term2(m_ppcNodes[uiCamId][uiPos], m_ppcNodes[uiCamId][uiPos + m_uiWidth], E00, E01, E10, E11);
//                                }
//                                else {
//                                    g->add_term1(m_ppcNodes[uiCamId][uiPos], E01, E11);
//                                }
//                            }
//                            else {
//                                if (m_ppbNodesActive[uiCamId][uiPos + m_uiWidth]) {
//                                    g->add_term1(m_ppcNodes[uiCamId][uiPos + m_uiWidth], E10, E11);
//                                }
//                                else {
//                                    //Nothig to add
//                                    dEnergy += E11;
//                                }
//                            }
//                        }
//
//                    }
//                }
//            }
//            //*/
//
//            cMatrix<Double> IPP(4, 4);
//            cMatrix<Double> pix_dst(4, 1);
//
//            //* 
//            //InterView Smothness
//            cMatrix<Double> PcIP(4, 4);
//
//            for (UInt uiCamId = 0; uiCamId < m_uiNumOfCams; uiCamId++){
//                MatrixMultiply(rapcCameraParameters[m_uiCenterCam]->m_mProjMat, rapcCameraParameters[uiCamId]->m_mInvProjMat, PcIP);
//
//
//                for (Int uiCamTargetId = (uiCamId - MATCH_NEIGHBORS); uiCamTargetId <= (uiCamId + MATCH_NEIGHBORS); uiCamTargetId++){
//                    if (uiCamTargetId < 0 || uiCamTargetId == uiCamId || uiCamTargetId >= Int(m_uiNumOfCams)) continue;
//
//                    MatrixMultiply(rapcCameraParameters[uiCamTargetId]->m_mProjMat, rapcCameraParameters[uiCamId]->m_mInvProjMat, IPP);
//
//                    Double tmpw[3], tmph[3], tmp1[3];
//
//                    for (Int i = 0; i < 3; i++) tmp1[i] = IPP.at(i, 2);
//
//                    for (UInt uiY = 0, uiPos = 0; uiY < m_uiHeight; uiY++) {
//                        for (Int i = 0; i < 3; i++) tmph[i] = uiY*IPP.at(i, 1) + tmp1[i];
//                        for (UInt uiX = 0; uiX < m_uiWidth; uiX++, uiPos++) {
//                            for (Int i = 0; i < 3; i++) tmpw[i] = uiX*IPP.at(i, 0) + tmph[i];
//                            UInt uiLabel = rapuiDepthLabel[uiCamId][uiPos];
//
//                            if (uiLabel != uiSource) {
//                                // Calc Position in Target View
//                                Double dInvZ = m_adLabel2InvZ[uiLabel];
//#if GLOBAL_LABELS
//                                dInvZ = (uiX * PcIP.at(2, 0) + uiY* PcIP.at(2, 1) + PcIP.at(2, 2)) / (1 / dInvZ - PcIP.at(2, 3)); //Project Z value from center cam to current cam
//#endif
//
//                                for (Int i = 0; i < 3; i++) pix_dst.at(i, 0) = dInvZ*IPP.at(i, 3) + tmpw[i];
//
//                                UInt uiPosWinTargetView = int(pix_dst.at(0, 0) / pix_dst.at(2, 0) + 0.5);
//                                UInt uiPosHinTargetView = int(pix_dst.at(1, 0) / pix_dst.at(2, 0) + 0.5);
//                                UInt uiPixelPositionInTargetView = uiPosHinTargetView*m_uiWidth + uiPosWinTargetView;
//
//                                // If Target Pixel is visible
//                                if (uiPosWinTargetView >= 0 && uiPosWinTargetView < m_uiWidth &&
//                                    uiPosHinTargetView >= 0 && uiPosHinTargetView < m_uiHeight){
//                                    // Get Lable of Target Pixel
//                                    UInt uiTargetLabel = rapuiDepthLabel[uiCamTargetId][uiPixelPositionInTargetView];
//                                    if (uiTargetLabel == uiLabel){
//                                        Double Eaa = 0.0;
//                                        Double E00 = Min(m_pcMatcher->matchingError(rapuiDepthLabel, rapcCameraParameters, uiCamId, uiCamTargetId, uiX, uiY, uiLabel) - 20, 0);
//                                        Double Ea0 = 0;
//                                        Double E0a = 0;
//
//#if INPUT_MASK
//                                        if (rapcYUVMask[uiCamId]->m_atY[uiPos] == 0 || rapcYUVMask[uiCamTargetId]->m_atY[uiPixelPositionInTargetView] == 0){
//                                            continue;
//                                        }
//#endif
//
//                                        if ((m_ppbNodesActive[uiCamId][uiPos] == true) && (m_ppbNodesActive[uiCamTargetId][uiPixelPositionInTargetView] == true)){
//                                            g->add_term2(m_ppcNodes[uiCamId][uiPos], m_ppcNodes[uiCamTargetId][uiPixelPositionInTargetView], E00, E0a, Ea0, Eaa);
//                                        }
//                                        else {
//                                            g->add_term1(m_ppcNodes[uiCamId][uiPos], E0a, Eaa);
//                                        }
//                                    }
//                                }
//                            }
//
//                            //Switch do uiSource
//                            {
//                                // Calc Position in Target View
//                                Double dInvZ = m_adLabel2InvZ[uiSource];
//#if GLOBAL_LABELS
//                                dInvZ = (uiX * PcIP.at(2, 0) + uiY* PcIP.at(2, 1) + PcIP.at(2, 2)) / (1 / dInvZ - PcIP.at(2, 3)); //Project Z value from center cam to current cam
//#endif
//                                for (Int i = 0; i < 3; i++) pix_dst.at(i, 0) = dInvZ*IPP.at(i, 3) + tmpw[i];
//
//                                UInt uiPosWinTargetView = int(pix_dst.at(0, 0) / pix_dst.at(2, 0) + 0.5);
//                                UInt uiPosHinTargetView = int(pix_dst.at(1, 0) / pix_dst.at(2, 0) + 0.5);
//                                UInt uiPixelPositionInTargetView = uiPosHinTargetView*m_uiWidth + uiPosWinTargetView;
//
//                                // If Target Pixel is visible
//                                if (uiPosWinTargetView >= 0 && uiPosWinTargetView < m_uiWidth &&
//                                    uiPosHinTargetView >= 0 && uiPosHinTargetView < m_uiHeight){
//                                    // Get Lable of Target Pixel
//                                    UInt uiTargetLabel = rapuiDepthLabel[uiCamTargetId][uiPixelPositionInTargetView];
//                                    Double E0a = 0;
//                                    Double Ea0 = 0;
//                                    Double Eaa = Min(m_pcMatcher->matchingError(rapuiDepthLabel, rapcCameraParameters, uiCamId, uiCamTargetId, uiX, uiY, uiSource) - 20, 0);
//
//#if INPUT_MASK
//                                    if (rapcYUVMask[uiCamId]->m_atY[uiPos] == 0 || rapcYUVMask[uiCamTargetId]->m_atY[uiPixelPositionInTargetView] == 0){
//                                        continue;
//                                    }
//#endif
//
//                                    if (m_ppbNodesActive[uiCamId][uiPos] == true){
//                                        if (m_ppbNodesActive[uiCamTargetId][uiPixelPositionInTargetView] == true){
//                                            g->add_term2(m_ppcNodes[uiCamId][uiPos], m_ppcNodes[uiCamTargetId][uiPixelPositionInTargetView], 0, E0a, Ea0, Eaa);
//                                        }
//                                        else {
//                                            g->add_term1(m_ppcNodes[uiCamId][uiPos], E0a, Eaa);
//                                        }
//                                    }
//                                    else {
//                                        if (m_ppbNodesActive[uiCamTargetId][uiPixelPositionInTargetView] == true){
//                                            g->add_term1(m_ppcNodes[uiCamTargetId][uiPixelPositionInTargetView], Ea0, Eaa);
//                                        }
//                                        else {
//                                            dEnergy += Eaa;
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//            //*/
//
//            Graph::flowtype flow = g->maxflow();
//            dEnergy += flow;
//            if (dEnergy < dOldEnergy) {
//                fprintf(stdout, "E: %f\n", dEnergy);
//                for (UInt uiPos = 0; uiPos < m_uiImSize; uiPos++) {
//                    for (UInt uiCamId = 0; uiCamId < m_uiNumOfCams; uiCamId++) {
//
//#if INPUT_MASK
//                        if (rapcYUVMask[uiCamId]->m_atY[uiPos] == 0){
//                            continue;
//                        }
//#endif
//
//                        if (m_ppbNodesActive[uiCamId][uiPos] == true) {
//                            if (g->what_segment(m_ppcNodes[uiCamId][uiPos]) != Graph::SOURCE){
//                                rapuiDepthLabel[uiCamId][uiPos] = uiSource;
//                            }
//                        }
//                    }
//                }
//                dOldEnergy = dEnergy;
//            }
//            else {
//                fprintf(stdout, "E: * %f\n", dEnergy);
//            }
//            delete g;
//
//            m_pcMatcher->setCurrentLabels(rapuiDepthLabel);
//
//#if GRAPH_CUT_OPTIMIZER_DEBUG_DEPTH_OUTPUT //ddepth
//            cYUV<DepthPixelType>* pcYUVDepth = new cYUV<DepthPixelType>(m_uiWidth, m_uiHeight, DEPTH_BPS, 400);
//            for (UInt uiCamId = 0; uiCamId < m_uiNumOfCams; uiCamId++) {
//                for (UInt pp = 0; pp < m_uiImSize; pp++){
//                    pcYUVDepth->m_atY[pp] = (DepthPixelType)m_auiLabel2Depth[rapuiDepthLabel[uiCamId][pp]];
//                }
//
//                char buff[100];
//#if DEPTH_BPS==8
//                sprintf_s(buff, "ddepth_%dx%d_cf400_8bps_cam%d.yuv", m_uiWidth,m_uiHeight,uiCamId);
//#elif DEPTH_BPS==16
//                sprintf_s(buff, "ddepth_%dx%d_cf400_16bps_cam%d.yuv", m_uiWidth, m_uiHeight, uiCamId);
//#endif
//                String buffAsStdStr = buff;
//
//                pcYUVDepth->frameWriter(buffAsStdStr, true);
//            }
//            delete pcYUVDepth;
//#endif
//
//            clkTimeEnd = clock();
//        } //source		
//    } //cycle
//
//    return;
//}