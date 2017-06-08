/*
 * Self-automatic image compositing using Graph Cut optimization.
 * Program input:
 * - 1st arg - path to prepared mask
 * - next args - paths to images which would take participance in optimization
 *
 * author: W.Tyczynski
 */
#include <iostream>

#include "ErrorCodes.h"

#include "bitmap/RGBPixel.h"
#include "bitmap/BMP.h"
#include "matrix/Matrix.h"

#include "graph/Mask.h"
#include "graph/GraphCutOptimizer.h"

void endMessage();

int main(int argc, char** argv)
{
    std::cout << "Hello world!" << std::endl;

    if (argc >= 2)
    {
        try
        {
            std::cout << "Loading file: " << argv[1] << std::endl;
            BMP bitmap(argv[1]);

            Mask mask(&bitmap);
            mask.createLabels();

            GraphCutOptimizer opt(2);
            opt.addLabel(&bitmap);
            opt.addLabel(&bitmap);
            opt.addMask(&mask);
            opt.optimize();
        }
        catch (std::runtime_error& e)
        {
            std::cout << e.what() << std::endl;
            endMessage();
            return ErrorCodes::WRONG_INPUT_FILE;
        }
    }

    endMessage();
}

// Example from graphcut lib
/*
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
*/

void endMessage()
{
    std::cout << "Press enter to close..." << std::endl;
    std::cin.get();
}
