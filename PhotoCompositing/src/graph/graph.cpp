/* graph.cpp */
/*
    Copyright 2001 Vladimir Kolmogorov (vnk@cs.cornell.edu), Yuri Boykov (yuri@csd.uwo.ca).

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


//#include <stdio.h>
#include "graph.h"

Graph::Graph(void (*err_function)(char *))
{
	error_function = err_function;
	node_block = new Block<node>(NODE_BLOCK_SIZE, error_function);
	arc_block  = new Block<arc>(NODE_BLOCK_SIZE, error_function);
	flow = 0;
}

Graph::~Graph()
{
	delete node_block;
	delete arc_block;
}

Graph::node_id Graph::add_node()
{
	node *i = node_block -> New();

	i -> first = NULL;
	i -> tr_cap = 0;

	return (node_id) i;
}

void Graph::add_edge(node_id from, node_id to, captype cap, captype rev_cap)
{
	arc *a, *a_rev;

	a = arc_block -> New(2);
	a_rev = a + 1;

	a -> sister = a_rev;
	a_rev -> sister = a;
	a -> next = ((node*)from) -> first;
	((node*)from) -> first = a;
	a_rev -> next = ((node*)to) -> first;
	((node*)to) -> first = a_rev;
	a -> head = (node*)to;
	a_rev -> head = (node*)from;
	a -> r_cap = cap;
	a_rev -> r_cap = rev_cap;
}

void Graph::set_tweights(node_id i, captype cap_source, captype cap_sink)
{
	flow += (cap_source < cap_sink) ? cap_source : cap_sink;
	((node*)i) -> tr_cap = cap_source - cap_sink;
}

void Graph::add_tweights(node_id i, captype cap_source, captype cap_sink)
{
	register captype delta = ((node*)i) -> tr_cap;
	if (delta > 0) cap_source += delta;
	else           cap_sink   -= delta;
	flow += (cap_source < cap_sink) ? cap_source : cap_sink;
	((node*)i) -> tr_cap = cap_source - cap_sink;
}

void Graph::add_term3(node_id x, node_id y, node_id z,
                              captype E000, captype E001,
                              captype E010, captype E011,
                              captype E100, captype E101,
                              captype E110, captype E111)
{
	register captype pi = (E000 + E011 + E101 + E110) - (E100 + E010 + E001 + E111);
	register captype delta;
	register node_id u;
  flowtype Econst =0;

	if (pi >= 0)
	{
		Econst += E111 - (E011 + E101 + E110);

		add_tweights(x, E101, E001);
		add_tweights(y, E110, E100);
		add_tweights(z, E011, E010);

		delta = (E010 + E001) - (E000 + E011); /* -pi(E[x=0]) */
		//assert(delta >= 0); /* check regularity */
		add_edge(y, z, delta, 0);

		delta = (E100 + E001) - (E000 + E101); /* -pi(E[y=0]) */
		//assert(delta >= 0); /* check regularity */
		add_edge(z, x, delta, 0);

		delta = (E100 + E010) - (E000 + E110); /* -pi(E[z=0]) */
		//assert(delta >= 0); /* check regularity */
		add_edge(x, y, delta, 0);

		if (pi > 0)
		{
			u = add_node();
			add_edge(x, u, pi, 0);
			add_edge(y, u, pi, 0);
			add_edge(z, u, pi, 0);
			add_tweights(u, 0, pi);
		}
	}
	else
	{
		Econst += E000 - (E100 + E010 + E001);

		add_tweights(x, E110, E010);
		add_tweights(y, E011, E001);
		add_tweights(z, E101, E100);

		delta = (E110 + E101) - (E100 + E111); /* -pi(E[x=1]) */
		//assert(delta >= 0); /* check regularity */
		add_edge(z, y, delta, 0);

		delta = (E110 + E011) - (E010 + E111); /* -pi(E[y=1]) */
		//assert(delta >= 0); /* check regularity */
		add_edge(x, z, delta, 0);

		delta = (E101 + E011) - (E001 + E111); /* -pi(E[z=1]) */
		//assert(delta >= 0); /* check regularity */
		add_edge(y, x, delta, 0);

		u = add_node();
		add_edge(u, x, -pi, 0);
		add_edge(u, y, -pi, 0);
		add_edge(u, z, -pi, 0);
		add_tweights(u, -pi, 0);
	}
}

void Graph::add_term2(node_id x, node_id y, captype A, captype B, captype C, captype D)
{
	/* 
	   E = A A  +  0   B-A
	       D D     C-D 0
	   Add edges for the first term
	*/
	add_tweights(x, D, A);
	B -= A; C -= D;

	/* now need to represent
	   0 B
	   C 0
	*/

	//assert(B + C >= 0); /* check regularity */
  //fprintf(stdout,"\nError B+C>=0\n");
	if (B < 0)
	{
		/* Write it as
		   B B  +  -B 0  +  0   0
		   0 0     -B 0     B+C 0
		*/
		add_tweights(x, 0, B); /* first term */
		add_tweights(y, 0, -B); /* second term */
		add_edge(x, y, 0, B+C); /* third term */
	}
	else if (C < 0)
	{
		/* Write it as
		   -C -C  +  C 0  +  0 B+C
		    0  0     C 0     0 0
		*/
		add_tweights(x, 0, -C); /* first term */
		add_tweights(y, 0, C); /* second term */
		add_edge(x, y, B+C, 0); /* third term */
	}
	else /* B >= 0, C >= 0 */
	{
		add_edge(x, y, B, C);
	}
}

 void Graph::add_term1(node_id x, captype A, captype B)
{
	add_tweights(x, B, A);
}

