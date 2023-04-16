#pragma once
#ifndef _Graph_h_included
#define _Graph_h_included

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include<boost/graph/copy.hpp>
struct VertexProperty
{
    std::string label;
    int position;
};
struct EdgeProperty
{
    std::set<std::string> state;
};
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, VertexProperty, EdgeProperty> Graph;

#endif // !_Graph_h_included
