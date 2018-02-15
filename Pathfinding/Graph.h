#pragma once
#include <vector>
#include "Vector2.h"
class Edge;
class Node
{
public:
	Vector2 position;
	std::vector<Edge> edges;
	
	void AddEdge(Edge e);

};

class Edge
{
public:
	Node* connectedTo;
	float weight;

	Edge(Node* node, float w = 1);
};

class Graph
{
	std::vector<Node> nodes;
public:
	Graph();
	~Graph();

	void AddNode(Node node);
	std::vector<Node*> GetAdjacentNodes(Node* node);
	std::vector<Node>& GetAllNodes();
	Node* GetNode(Vector2 position);

	float GetDistance(Node* n1, Node* n2);
};

