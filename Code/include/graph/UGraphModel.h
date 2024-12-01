/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   UGraphModel.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 15:16
 */

#ifndef UGRAPHMODEL_H
#define UGRAPHMODEL_H

#include "graph/AbstractGraph.h"
#include "stacknqueue/PriorityQueue.h"

//////////////////////////////////////////////////////////////////////
///////////// UGraphModel: Undirected Graph Model ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class UGraphModel : public AbstractGraph<T>
{
private:
    // Add typedef to make code cleaner
    typedef typename AbstractGraph<T>::VertexNode VertexNode;
    // typedef typename AbstractGraph<T>::Edge Edge;
    typedef typename AbstractGraph<T>::Iterator Iterator;
public:
    // class UGraphAlgorithm;
    // friend class UGraphAlgorithm;

    UGraphModel(
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }

    /*
    ! void connect(T from, T to, float weight = 0)
    ? Functionality: 
        * Adds an undirected edge between vertices from and to with weight weight (default is 0).
    ? Exceptions:
        * VertexNotFoundException: If either vertex does not exist.
    */
    void connect(T from, T to, float weight = 0)
    {
        
        // Retrieve the VertexNode objects corresponding to vertices from and to.
        VertexNode *fromNode = this->getVertexNode(from);
        VertexNode *toNode = this->getVertexNode(to);

        // If any vertex does not exist, throw a VertexNotFoundException.
        if (fromNode == nullptr)
        {
            throw VertexNotFoundException(this->vertex2str(from));
        }
        if (toNode == nullptr)
        {
            throw VertexNotFoundException(this->vertex2str(to));
        }

        // Connect from-to
        // If from and to are the same, add a self-loop.
        if (fromNode->equals(toNode))
        {
            fromNode->connect(toNode, weight);
        }
        // If from and to are different, add two edges (one from from to to and one from to to from).
        else
        {
            fromNode->connect(toNode, weight);
            toNode->connect(fromNode, weight);
        }
    }
    /*
    ! void disconnect(T from, T to)
    ? Functionality: 
        * Removes the undirected edge between vertices from and to.
    ? Exceptions:
        * VertexNotFoundException: If either vertex does not exist.
        * EdgeNotFoundException: If no edge exists between the vertices.
    */
    void disconnect(T from, T to)
    {
        // Retrieve the VertexNode objects corresponding to vertices from and to.
        VertexNode *fromNode = this->getVertexNode(from);
        VertexNode *toNode = this->getVertexNode(to);

        // If any vertex does not exist, throw a VertexNotFoundException.
        if (fromNode == nullptr)
        {
            throw VertexNotFoundException(this->vertex2str(from));
        }
        if (toNode == nullptr)
        {
            throw VertexNotFoundException(this->vertex2str(to));
        }

        // Check if the edge exists
        typename AbstractGraph<T>::Edge *edge = fromNode->getEdge(toNode);
        if (edge == nullptr)
        {
            stringstream edge_os;
            edge_os << "E("
                    << this->vertex2str(from)
                    << ","
                    << this->vertex2str(to)
                    << ")";
            throw EdgeNotFoundException(edge_os.str());
        }

        // Remove the edge
        // If from and to are the same, remove the self-loop.
        if (fromNode->equals(toNode))
        {
            fromNode->removeTo(toNode);
        }
        // If from and to are different, remove both edges (one from from to to and one from to to from).
        else
        {
            fromNode->removeTo(toNode);
            toNode->removeTo(fromNode);
        }
    }

    /*
    ! void remove(T vertex)
    ? Functionality: 
        * Removes a vertex and all edges connected to it.
    ? Exceptions:
        * VertexNotFoundException: If the vertex does not exist.
    */
    void remove(T vertex)
    {
        // Retrieve the VertexNode corresponding to the vertex to be removed.
        VertexNode *node = this->getVertexNode(vertex);

        // If the vertex does not exist, throw a VertexNotFoundException.
        if (node == nullptr)
        {
            throw VertexNotFoundException(this->vertex2str(vertex));
        }

        // Iterate through the graph’s vertices, removing all edges connected to or from the vertex to be removed.
        typename DLinkedList<VertexNode *>::Iterator it = this->nodeList.begin();
        while (it != this->nodeList.end())
        {
            VertexNode *vertexNode = *it;
            if (this->connected(vertexNode->getVertex(), vertex))
            {
                vertexNode->removeTo(node);
            }
            if (this->connected(vertex, vertexNode->getVertex()))
            {
                node->removeTo(vertexNode);
            }
            it++;
        }

        // Remove the vertex from the graph’s vertex list.
        this->nodeList.removeItem(node, &VertexNode::free);       
    }

    /*
    ! create
    ? Functionality: 
        * Creates a new undirected graph from a list of vertices and edges.
    ? Exceptions: 
        * None.
    */
    static UGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        // Create a new UGraphModel object
        UGraphModel<T> *graph = new UGraphModel<T>(vertexEQ, vertex2str);

        // Add all vertices in vertices to the graph
        for (int i = 0; i < nvertices; i++)
        {
            graph->add(vertices[i]);
        }

        // Add all edges in edges to the graph
        for (int i = 0; i < nedges; i++)
        {
            graph->connect(edges[i].from, edges[i].to, edges[i].weight);
        }

        // Return a pointer to the created graph
        return graph;
    }
};

template <class T>
class UGraphAlgorithm {
    private:
    protected:
    public:
    UGraphModel<T> minSpanningTree(UGraphModel<T>* graph){
        UGraphModel<T> mst(0, 0);
        return mst;
    }
};

#endif /* UGRAPHMODEL_H */
