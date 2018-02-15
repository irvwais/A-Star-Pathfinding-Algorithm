#include "Graph.h"
#include <math.h>


Graph::Graph()
{
}

Graph::~Graph()
{
}

void Graph::AddNode(Node node)
{
	nodes.push_back(node);
}

std::vector<Node*> Graph::GetAdjacentNodes(Node * node)
{
	std::vector<Node*> adjacentNodes;
	for (auto e : node->edges)
	{
		adjacentNodes.push_back(e.connectedTo);
	}

	return adjacentNodes;
}

std::vector<Node>& Graph::GetAllNodes()
{	
	return nodes;
}

Node * Graph::GetNode(Vector2 position)
{
	for (Node& node : nodes)
	{
		if (node.position.x == position.x && node.position.y == position.y)
			return &node;
	}
	return nullptr;
}

float Graph::GetDistance(Node * n1, Node * n2)
{
	float distance = 0;
	float dx = n1->position.x - n2->position.x;
	float dy = n1->position.y - n2->position.y;
	return sqrt(dx * dx + dy * dy);
}

void Node::AddEdge(Edge e)
{
	edges.push_back(e);
}

Edge::Edge(Node * node, float w)
{
	connectedTo = node;
	weight = w;
}
