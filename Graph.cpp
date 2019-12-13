#include "stdafx.h"
#include "Graph.h"

/// <summary>
/// Default constructor for graph
/// </summary>
Graph::Graph()
{

}

/// <summary>
/// Constructor for graph accepting vertices number
/// </summary>
Graph::Graph(int v)
{
	m_v = v;
	m_adj = new list<int>[v];
}

/// <summary>
/// Destructor for graph
/// </summary>
Graph::~Graph()
{
}

/// <summary>
/// Add edge to the graph
/// method to add an undirected edge
/// </summary>
void Graph::AddEdge(int v, int w)
{
	m_adj[v].push_back(w);
	m_adj[w].push_back(v);
	//cout << "v = " << v << " w = " << w << endl;
}

/// <summary>
/// Method to print connected components in an undirected graph
/// </summary>
void Graph::PrintConnectedComponents()
{
	//Mark all the vertices as not visited
	bool *visited = new bool[m_v];
	for (int v = 0; v < m_v; v++)
		visited[v] = false;

	for (int v = 0; v < m_v; v++)
	{
		if (visited[v] == false)
		{
			//print all reachable vertices
			// from v
			DFSUtil(v, visited);
			//cout << "\n";
		}
	}
}

/// <summary>
/// Find connected components
/// O(V + E)
/// </summary>
void Graph::DFSUtil(int v, bool visited[])
{
	// Mark the current node as visited and print it
	visited[v] = true;
	//cout << v << " ";
	// Recur for all the vertices
	// adjacent to this vertex
	list<int>::iterator i;
	for (i = m_adj[v].begin(); i != m_adj[v].end(); ++i)
	{
		if (!visited[*i])
		{
			DFSUtil(*i, visited);
		}
	}
}
