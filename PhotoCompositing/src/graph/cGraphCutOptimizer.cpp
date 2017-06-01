#include "cGraphCutOptimizer.h"

#include <ctime>
#include <iostream>
#include <limits>

#include "../matrix/Matrix.h"
#include "CostFunctions.h"

void cGraphCutOptimizer::init(cMatcher *pcMatcher)
{
    m_uiHorizontalPrecision = pcMatcher->m_uiHorizontalPrecision;
    m_uiVerticalPrecision = pcMatcher->m_uiVerticalPrecision;

    piAbsOrg = new int[2 * m_uiNumOfLabels + 1];
    piAbs = &piAbsOrg[m_uiNumOfLabels];

    for (int l = 0; l < int(m_uiNumOfLabels); l++)
    {
        piAbs[l] = l;
        piAbs[-l] = l;
    }

    m_ppbNodesActive = new bool*[m_uiNumOfCams];
    m_ppcNodes = new Graph::node_id*[m_uiNumOfCams];
    for (unsigned int uiCam = 0; uiCam < m_uiNumOfCams; uiCam++)
    {
        m_ppbNodesActive[uiCam] = new bool[m_uiImSize];
        m_ppcNodes[uiCam] = new Graph::node_id[m_uiImSize];
    }
}

void GraphCutOptimizer::optimize(cArray<cCamParams<MatrixComputationalType>*> &rapcCameraParameters,
                                 cArray<unsigned int*> &rapuiDepthLabel)
{
    std::cout << "Graph Cut" << std::endl;

    unsigned int uiMaxCycle = 2;
    unsigned int pp, right, down;
    double cost_curr, cost_down, cost_right, cost_tmp;
    unsigned int Wm1 = m_uiWidth - 1;
    unsigned int Hm1 = m_uiHeight - 1;

    double dEnergy;
    double dOldEnergy = std::numeric_limits<double>::max();

    for (unsigned int uiCycle = 0; uiCycle < uiMaxCycle; uiCycle++)
    {
        for (unsigned int uiSource = 0; uiSource < (int)m_uiNumOfLabels; uiSource++)
        {
            // starting time
            clock_t clkTimeBegin, clkTimeEnd;
            clkTimeBegin = clock();

            std::cout << uiSource << '\t';
            Graph *g = new Graph();

            dEnergy = 0.0;

            for (unsigned int uiCam = 0; uiCam < m_uiNumOfCams; uiCam++)
            {
                for (pp = 0; pp < m_uiImSize; pp++)
                {
                    if (rapuiDepthLabel[uiCam][pp] == uiSource)
                    {
                        m_ppcNodes[uiCam][pp] = nullptr;
                        m_ppbNodesActive[uiCam][pp] = false;
                    }
                    else
                    {
                        m_ppcNodes[uiCam][pp] = g->add_node();
                        m_ppbNodesActive[uiCam][pp] = true;
                    }
                }
            }

            //*
            //Smoothing term in z domain
            for (unsigned int uiCamId = 0; uiCamId < m_uiNumOfCams; uiCamId++)
            {
                Matrix<double> PcIP(4, 4);
                MatrixMultiply(rapcCameraParameters[m_uiCenterCam]->m_mProjMat, rapcCameraParameters[uiCamId]->m_mInvProjMat, PcIP);

                for (unsigned int uiY = 0, uiPos = 0; uiY < m_uiHeight; uiY++)
                {
                    for (unsigned int uiX = 0; uiX < m_uiWidth; uiX++, uiPos++)
                    {
                        unsigned int uiLabel = rapuiDepthLabel[uiCamId][uiPos];
                        double dInvZ = m_adLabel2InvZ[uiLabel];
#if GLOBAL_LABELS
                        double dZ = (1 / dInvZ - PcIP.at(2, 3)) / (uiX * PcIP.at(2, 0) + uiY* PcIP.at(2, 1) + PcIP.at(2, 2)); //Project Z value from center cam to current cam
#else
                        double dZ = 1/dInvZ;
#endif
                        dInvZ = m_adLabel2InvZ[uiSource];
#if GLOBAL_LABELS
                        double dZSource = (1 / dInvZ - PcIP.at(2, 3)) / (uiX * PcIP.at(2, 0) + uiY* PcIP.at(2, 1) + PcIP.at(2, 2)); //Project Z value from center cam to current cam
#else
                        double dZSource = 1/dInvZ;
#endif
                        //Horizontal

                        if (uiX < m_uiWidth - 1)
                        {
                            unsigned int uiLabelPlusOne = rapuiDepthLabel[uiCamId][uiPos + 1];
                            dInvZ = m_adLabel2InvZ[uiLabelPlusOne];
#if GLOBAL_LABELS
                            double dZPlusOne = (1 / dInvZ - PcIP.at(2, 3)) / ((uiX + 1) * PcIP.at(2, 0) + uiY* PcIP.at(2, 1) + PcIP.at(2, 2)); //Project Z value from center cam to current cam
#else
                            double dZPlusOne = 1/dInvZ;
#endif
                            dInvZ = m_adLabel2InvZ[uiSource];
#if GLOBAL_LABELS
                            double dZSourcePlusOne = (1 / dInvZ - PcIP.at(2, 3)) / ((uiX + 1) * PcIP.at(2, 0) + uiY* PcIP.at(2, 1) + PcIP.at(2, 2)); //Project Z value from center cam to current cam
#else
                            double dZSourcePlusOne = 1/dInvZ;
#endif

                            double E00 = 0.0;
                            double E01 = 0.0;
                            double E10 = 0.0;

                            if (m_ppbNodesActive[uiCamId][uiPos] && m_ppbNodesActive[uiCamId][uiPos + 1])
                            {
                                E00 = m_dSmoothingCoeff * CostFunctions::crossCostLinear(dZ, dZPlusOne);
                            }
                            if (m_ppbNodesActive[uiCamId][uiPos])
                            {
                                E01 = m_dSmoothingCoeff * CostFunctions::crossCostLinear(dZ, dZSourcePlusOne);
                            }
                            if (m_ppbNodesActive[uiCamId][uiPos + 1])
                            {
                                E10 = m_dSmoothingCoeff * CostFunctions::crossCostLinear(dZSource, dZPlusOne);
                            }
                            double E11 = m_dSmoothingCoeff * CostFunctions::crossCostLinear(dZSource, dZSourcePlusOne); //0;


                            if (m_ppbNodesActive[uiCamId][uiPos])
                            {
                                if (m_ppbNodesActive[uiCamId][uiPos + 1])
                                {
                                    g->add_term2(m_ppcNodes[uiCamId][uiPos], m_ppcNodes[uiCamId][uiPos + 1], E00, E01, E10, E11);
                                }
                                else
                                {
                                    g->add_term1(m_ppcNodes[uiCamId][uiPos], E01, E11);
                                }
                            }
                            else
                            {
                                if (m_ppbNodesActive[uiCamId][uiPos + 1])
                                {
                                    g->add_term1(m_ppcNodes[uiCamId][uiPos + 1], E10, E11);
                                }
                                else
                                {
                                    //Nothing to add
                                    dEnergy += E11;
                                }
                            }
                        }

                        //Vertical

                        if (uiY < m_uiHeight - 1)
                        {
                            unsigned int uiLabelPlusOne = rapuiDepthLabel[uiCamId][uiPos + m_uiWidth];
                            dInvZ = m_adLabel2InvZ[uiLabelPlusOne];
#if GLOBAL_LABELS
                            double dZPlusOne = (1 / dInvZ - PcIP.at(2, 3)) / (uiX * PcIP.at(2, 0) + (uiY + 1)* PcIP.at(2, 1) + PcIP.at(2, 2)); //Project Z value from center cam to current cam
#else
                            double dZPlusOne = 1/dInvZ;
#endif
                            dInvZ = m_adLabel2InvZ[uiSource];
#if GLOBAL_LABELS
                            double dZSourcePlusOne = (1 / dInvZ - PcIP.at(2, 3)) / (uiX * PcIP.at(2, 0) + (uiY + 1)* PcIP.at(2, 1) + PcIP.at(2, 2)); //Project Z value from center cam to current cam
#else
                            double dZSourcePlusOne = 1/dInvZ;
#endif

                            double E00 = 0.0;
                            double E01 = 0.0;
                            double E10 = 0.0;

                            if (m_ppbNodesActive[uiCamId][uiPos] && m_ppbNodesActive[uiCamId][uiPos + m_uiWidth])
                            {
                                E00 = m_dSmoothingCoeff * CostFunctions::crossCostLinear(dZ, dZPlusOne);
                            }
                            if (m_ppbNodesActive[uiCamId][uiPos])
                            {
                                E01 = m_dSmoothingCoeff * CostFunctions::crossCostLinear(dZ, dZSourcePlusOne);
                            }
                            if (m_ppbNodesActive[uiCamId][uiPos + m_uiWidth])
                            {
                                E10 = m_dSmoothingCoeff * CostFunctions::crossCostLinear(dZSource, dZPlusOne);
                            }
                            double E11 = m_dSmoothingCoeff * CostFunctions::crossCostLinear(dZSource, dZSourcePlusOne); //0.0

                            if (m_ppbNodesActive[uiCamId][uiPos])
                            {
                                if (m_ppbNodesActive[uiCamId][uiPos + m_uiWidth])
                                {
                                    g->add_term2(m_ppcNodes[uiCamId][uiPos], m_ppcNodes[uiCamId][uiPos + m_uiWidth], E00, E01, E10, E11);
                                }
                                else
                                {
                                    g->add_term1(m_ppcNodes[uiCamId][uiPos], E01, E11);
                                }
                            }
                            else
                            {
                                if (m_ppbNodesActive[uiCamId][uiPos + m_uiWidth])
                                {
                                    g->add_term1(m_ppcNodes[uiCamId][uiPos + m_uiWidth], E10, E11);
                                }
                                else
                                {
                                    //Nothing to add
                                    dEnergy += E11;
                                }
                            }
                        }

                    }
                }
            }
            //*/

            Matrix<double> IPP(4, 4);
            Matrix<double> pix_dst(4, 1);

            //* 
            //InterView Smothness
            Matrix<double> PcIP(4, 4);

            for (unsigned int uiCamId = 0; uiCamId < m_uiNumOfCams; uiCamId++)
            {
                MatrixMultiply(rapcCameraParameters[m_uiCenterCam]->m_mProjMat, rapcCameraParameters[uiCamId]->m_mInvProjMat, PcIP);


                for (int uiCamTargetId = (uiCamId - MATCH_NEIGHBORS); uiCamTargetId <= (uiCamId + MATCH_NEIGHBORS); uiCamTargetId++)
                {
                    if (uiCamTargetId < 0 || uiCamTargetId == uiCamId || uiCamTargetId >= int(m_uiNumOfCams))
                    {
                        continue;
                    }

                    MatrixMultiply(rapcCameraParameters[uiCamTargetId]->m_mProjMat, rapcCameraParameters[uiCamId]->m_mInvProjMat, IPP);

                    double tmpw[3], tmph[3], tmp1[3];

                    for (int i = 0; i < 3; i++) tmp1[i] = IPP.at(i, 2);

                    for (unsigned int uiY = 0, uiPos = 0; uiY < m_uiHeight; uiY++)
                    {
                        for (int i = 0; i < 3; i++)
                        {
                            tmph[i] = uiY*IPP.at(i, 1) + tmp1[i];
                        }
                        for (unsigned int uiX = 0; uiX < m_uiWidth; uiX++, uiPos++)
                        {
                            for (int i = 0; i < 3; i++)
                            {
                                tmpw[i] = uiX*IPP.at(i, 0) + tmph[i];
                            }
                            unsigned int uiLabel = rapuiDepthLabel[uiCamId][uiPos];

                            if (uiLabel != uiSource)
                            {
                                // Calc Position in Target View
                                double dInvZ = m_adLabel2InvZ[uiLabel];
#if GLOBAL_LABELS
                                dInvZ = (uiX * PcIP.at(2, 0) + uiY* PcIP.at(2, 1) + PcIP.at(2, 2)) / (1 / dInvZ - PcIP.at(2, 3)); //Project Z value from center cam to current cam
#endif

                                for (int i = 0; i < 3; i++)
                                {
                                    pix_dst.at(i, 0) = dInvZ*IPP.at(i, 3) + tmpw[i];
                                }

                                unsigned int uiPosWinTargetView = int(pix_dst.at(0, 0) / pix_dst.at(2, 0) + 0.5);
                                unsigned int uiPosHinTargetView = int(pix_dst.at(1, 0) / pix_dst.at(2, 0) + 0.5);
                                unsigned int uiPixelPositionInTargetView = uiPosHinTargetView*m_uiWidth + uiPosWinTargetView;

                                // If Target Pixel is visible
                                if (uiPosWinTargetView >= 0 && uiPosWinTargetView < m_uiWidth &&
                                    uiPosHinTargetView >= 0 && uiPosHinTargetView < m_uiHeight)
                                {
                                    // Get Lable of Target Pixel
                                    unsigned int uiTargetLabel = rapuiDepthLabel[uiCamTargetId][uiPixelPositionInTargetView];
                                    if (uiTargetLabel == uiLabel)
                                    {
                                        double Eaa = 0.0;
                                        double E00 = fmin(m_pcMatcher->matchingError(rapuiDepthLabel, rapcCameraParameters, uiCamId, uiCamTargetId, uiX, uiY, uiLabel) - 20, 0);
                                        double Ea0 = 0;
                                        double E0a = 0;

                                        if ((m_ppbNodesActive[uiCamId][uiPos] == true) && (m_ppbNodesActive[uiCamTargetId][uiPixelPositionInTargetView] == true))
                                        {
                                            g->add_term2(m_ppcNodes[uiCamId][uiPos], m_ppcNodes[uiCamTargetId][uiPixelPositionInTargetView], E00, E0a, Ea0, Eaa);
                                        }
                                        else
                                        {
                                            g->add_term1(m_ppcNodes[uiCamId][uiPos], E0a, Eaa);
                                        }
                                    }
                                }
                            }

                            //Switch do uiSource
                            {
                                // Calc Position in Target View
                                double dInvZ = m_adLabel2InvZ[uiSource];
#if GLOBAL_LABELS
                                dInvZ = (uiX * PcIP.at(2, 0) + uiY* PcIP.at(2, 1) + PcIP.at(2, 2)) / (1 / dInvZ - PcIP.at(2, 3)); //Project Z value from center cam to current cam
#endif
                                for (int i = 0; i < 3; i++) pix_dst.at(i, 0) = dInvZ*IPP.at(i, 3) + tmpw[i];

                                unsigned int uiPosWinTargetView = int(pix_dst.at(0, 0) / pix_dst.at(2, 0) + 0.5);
                                unsigned int uiPosHinTargetView = int(pix_dst.at(1, 0) / pix_dst.at(2, 0) + 0.5);
                                unsigned int uiPixelPositionInTargetView = uiPosHinTargetView*m_uiWidth + uiPosWinTargetView;

                                // If Target Pixel is visible
                                if (uiPosWinTargetView >= 0 && uiPosWinTargetView < m_uiWidth &&
                                    uiPosHinTargetView >= 0 && uiPosHinTargetView < m_uiHeight)
                                {
                                    // Get Lable of Target Pixel
                                    unsigned int uiTargetLabel = rapuiDepthLabel[uiCamTargetId][uiPixelPositionInTargetView];
                                    double E0a = 0;
                                    double Ea0 = 0;
                                    double Eaa = fmin(m_pcMatcher->matchingError(rapuiDepthLabel, rapcCameraParameters, uiCamId, uiCamTargetId, uiX, uiY, uiSource) - 20, 0);

                                    if (m_ppbNodesActive[uiCamId][uiPos] == true)
                                    {
                                        if (m_ppbNodesActive[uiCamTargetId][uiPixelPositionInTargetView] == true)
                                        {
                                            g->add_term2(m_ppcNodes[uiCamId][uiPos], m_ppcNodes[uiCamTargetId][uiPixelPositionInTargetView], 0, E0a, Ea0, Eaa);
                                        }
                                        else
                                        {
                                            g->add_term1(m_ppcNodes[uiCamId][uiPos], E0a, Eaa);
                                        }
                                    }
                                    else
                                    {
                                        if (m_ppbNodesActive[uiCamTargetId][uiPixelPositionInTargetView] == true)
                                        {
                                            g->add_term1(m_ppcNodes[uiCamTargetId][uiPixelPositionInTargetView], Ea0, Eaa);
                                        }
                                        else
                                        {
                                            dEnergy += Eaa;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            //*/

            Graph::flowtype flow = g->maxflow();
            dEnergy += flow;
            if (dEnergy < dOldEnergy)
            {
                std::cout << "E: " << dEnergy << std::endl;
                for (unsigned int uiPos = 0; uiPos < m_uiImSize; uiPos++)
                {
                    for (unsigned int uiCamId = 0; uiCamId < m_uiNumOfCams; uiCamId++)
                    {
                        if (m_ppbNodesActive[uiCamId][uiPos] == true)
                        {
                            if (g->what_segment(m_ppcNodes[uiCamId][uiPos]) != Graph::SOURCE)
                            {
                                rapuiDepthLabel[uiCamId][uiPos] = uiSource;
                            }
                        }
                    }
                }
                dOldEnergy = dEnergy;
            }
            else
            {
                std::cout << "E: * " << dEnergy << std::endl;
            }
            delete g;

            m_pcMatcher->setCurrentLabels(rapuiDepthLabel);

#if GRAPH_CUT_OPTIMIZER_DEBUG_DEPTH_OUTPUT //ddepth
            cYUV<DepthPixelType>* pcYUVDepth = new cYUV<DepthPixelType>(m_uiWidth, m_uiHeight, DEPTH_BPS, 400);
            for (UInt uiCamId = 0; uiCamId < m_uiNumOfCams; uiCamId++)
            {
                for (UInt pp = 0; pp < m_uiImSize; pp++)
                {
                    pcYUVDepth->m_atY[pp] = (DepthPixelType)m_auiLabel2Depth[rapuiDepthLabel[uiCamId][pp]];
                }

                char buff[100];
#if DEPTH_BPS==8
                sprintf_s(buff, "ddepth_%dx%d_cf400_8bps_cam%d.yuv", m_uiWidth,m_uiHeight,uiCamId);
#elif DEPTH_BPS==16
                sprintf_s(buff, "ddepth_%dx%d_cf400_16bps_cam%d.yuv", m_uiWidth, m_uiHeight, uiCamId);
#endif
                String buffAsStdStr = buff;

                pcYUVDepth->frameWriter(buffAsStdStr, true);
            }
            delete pcYUVDepth;
#endif

            clkTimeEnd = clock();
        } //source		
    } //cycle
}
