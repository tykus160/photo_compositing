#include <iostream>

#include "bitmap/RGBPixel.h"
#include "bitmap/BMP.h"
#include "matrix/Matrix.h"

#include "graph/graph.h"

#include "graph/Mask.h"
#include "graph/GraphCutOptimizer.h"

void graphTest();

int main(int argc, char** argv)
{
    std::cout << "Hello world!" << std::endl;

    if (argc >= 2)
    {
        std::cout << "Loading file: " << argv[1] << std::endl;
        BMP bitmap(argv[1]);
        
        int numOfRed = 0;
        int numOfGreen = 0;
        int numOfBlue = 0;
        for (int j = 0; j < bitmap.getHeight(); ++j)
        {
            for (int i = 0; i < bitmap.getWidth(); ++i)
            {
                RGBPixel* px = bitmap(i, j);
                if (px->isBlue())
                {
                    ++numOfBlue;
                }
                else if (px->isGreen())
                {
                    ++numOfGreen;
                }
                else if (px->isRed())
                {
                    ++numOfRed;
                }
            }
        }
        std::cout << "Red: " << numOfRed << ", Green: " << numOfGreen << ", Blue: " << numOfBlue << std::endl;

        Mask mask(&bitmap);
        mask.createLabels();
        std::cout << mask << std::endl;
    }

    GraphCutOptimizer opt(2);
    opt.optimize();
    
    //graphTest();

    std::cout << "Press enter to close..." << std::endl;
    std::cin.get();
}

// Example from graphcut lib
void graphTest()
{
    Graph::node_id nodes[2];
    Graph *g = new Graph();

    nodes[0] = g->add_node();
    nodes[1] = g->add_node();
    g->set_tweights(nodes[0], 1, 5);
    g->set_tweights(nodes[1], 2, 6);
    g->add_edge(nodes[0], nodes[1], 3, 4);

    Graph::flowtype flow = g->maxflow();

    printf("Flow = %d\n", flow);
    printf("Minimum cut:\n");
    if (g->what_segment(nodes[0]) == Graph::SOURCE)
        printf("node0 is in the SOURCE set\n");
    else
        printf("node0 is in the SINK set\n");
    if (g->what_segment(nodes[1]) == Graph::SOURCE)
        printf("node1 is in the SOURCE set\n");
    else
        printf("node1 is in the SINK set\n");

    delete g;
}