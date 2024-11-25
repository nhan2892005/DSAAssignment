/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DGraphModel.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 19:36
 */

#ifndef DGRAPHMODEL_H
#define DGRAPHMODEL_H
#include "graph/AbstractGraph.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"
#include "hash/xMap.h"
#include "stacknqueue/PriorityQueue.h"
#include "sorting/DLinkedListSE.h"


//////////////////////////////////////////////////////////////////////
///////////// GraphModel: Directed Graph Model    ////////////////////
//////////////////////////////////////////////////////////////////////


template<class T>
class DGraphModel: public AbstractGraph<T>{
private:
public:
    DGraphModel(
            bool (*vertexEQ)(T&, T&), 
            string (*vertex2str)(T&) ): 
        AbstractGraph<T>(vertexEQ, vertex2str){
    }
    
    /*
    ! void connect(T from, T to, float weight = 0)
    ? Functionality: 
        * Adds a directed edge from vertex from to vertex to with weight weight (default is 0).
    ? Exceptions:
        * Throws VertexNotFoundException if either vertex does not exist.
    */
    void connect(T from, T to, float weight=0){

        // Retrieve the VertexNode objects corresponding to vertices from and to.
        VertexNode* fromNode = this->getVertexNode(from);
        VertexNode* toNode = this->getVertexNode(to);

        // If any vertex does not exist, throw a VertexNotFoundException.
        if (fromNode == nullptr) {
            throw VertexNotFoundException(this->vertex2Str(from));
        }
        if (toNode == nullptr) {
            throw VertexNotFoundException(this->vertex2Str(to));
        }

        // Connect from-to
        fromNode->connect(toNode, weight);
    }
    /*
    ! void disconnect(T from, T to)
    ? Functionality: 
        * Removes the edge from vertex from to vertex to.
    ? Exceptions:
        * VertexNotFoundException: If either vertex does not exist.
        * EdgeNotFoundException: If no edge exists between the vertices.
    */
    void disconnect(T from, T to){
        // Retrieve the VertexNode objects corresponding to vertices from and to.
        VertexNode* fromNode = this->getVertexNode(from);
        VertexNode* toNode = this->getVertexNode(to);

        // If any vertex does not exist, throw a VertexNotFoundException.
        if (fromNode == nullptr) {
            throw VertexNotFoundException(this->vertex2Str(from));
        }
        if (toNode == nullptr) {
            throw VertexNotFoundException(this->vertex2Str(to));
        }

        // Check if the edge exists
        Edge* edge = fromNode->getEdge(toNode);
        if (edge == nullptr) {
            stringstream edge_os;
            edge_os << "E("
                << this->vertex2Str(from)
                << ","
                << this->vertex2Str(to)
                << ")";
            throw EdgeNotFoundException(edge_os.str());
        }

        // Remove the edge
        fromNode->removeTo(toNode);
    }

    /*
    ! void remove(T vertex)
    ? Functionality: 
        * Removes a vertex and all edges connected to it.
    ? Exceptions:
        * VertexNotFoundException: If the vertex does not exist.
    */
    void remove(T vertex){
        //TODO
        // Retrieve the VertexNode corresponding to the vertex to be removed.
        VertexNode* node = this->getVertexNode(vertex);

        // If the vertex does not exist, throw a VertexNotFoundException.
        if (node == nullptr) {
            throw VertexNotFoundException(this->vertex2Str(vertex));
        }

        // Iterate through the graph’s vertices, removing all edges connected to or from the vertex to be removed.
        typename DLinkedList<VertexNode*>::Iterator it = this->nodeList.begin();
        while (it != this->nodeList.end()) {
            VertexNode* vertexNode = *it;
            if (this->connected(vertexNode->vertex, vertex)) {
                vertexNode->removeTo(node);
            }
            if (this->connected(vertex, vertexNode->vertex)) {
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
        * Creates a new directed graph from a list of vertices and edges.
    ? Exceptions: 
        * None
    */
    static DGraphModel<T>* create(
            T* vertices, int nvertices, Edge<T>* edges, int nedges,
            bool (*vertexEQ)(T&, T&),
            string (*vertex2str)(T&)){
        // Create a new DGraphModel object
        DGraphModel<T>* graph = new DGraphModel<T>(vertexEQ, vertex2str);

        // Add all vertices in vertices to the graph
        for (int i = 0; i < nvertices; i++) {
            graph->add(vertices[i]);
        }

        // Add all edges in edges to the graph
        for (int i = 0; i < nedges; i++) {
            graph->connect(edges[i].from, edges[i].to, edges[i].weight);
        }

        // Return a pointer to the created graph
        return graph;
    }
};

#endif /* DGRAPHMODEL_H */

