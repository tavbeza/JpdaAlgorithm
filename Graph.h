#ifndef TRACKER_GRAPH_H_
#define TRACKER_GRAPH_H_

#include<iostream>
#include <list>
using namespace std;

/// <summary>
/// Graph class represents a undirected graph
/// using adjacency list representation
/// </summary>
class Graph
{
public:
	/// <summary>
	/// Default constructor for graph
	/// </summary>
	Graph();

	/// <summary>
	/// Constructor for graph accepting vertices number
	/// </summary>
	Graph(int v);

	/// <summary>
	/// Destructor for graph
	/// </summary>
	virtual ~Graph();

	/// <summary>
	/// Add edge to the graph
	/// method to add an undirected edge
	/// </summary>
	void AddEdge(int v, int w);

	/// <summary>
	/// Find connected components
	/// A function used by DFS
	/// O(V + E)
	/// </summary>
	void DFSUtil(int v, bool visited[]);

	/// <summary>
	/// Method to print connected components in an undirected graph
	/// </summary>
	void PrintConnectedComponents();

private:
	/// <summary>
	/// No. of vertices
	/// </summary>
	int m_v;

	/// <summary>
	/// Pointer to an array containing adjacency lists
	/// </summary>
	std::list<int>   *m_adj;

};
#endif // TRACKER_GRAPH_H_
