/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AbstractGraph.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 17:48
 */

#ifndef ABSTRACTGRAPH_H
#define ABSTRACTGRAPH_H
#include "graph/IGraph.h"
#include <string>
#include <sstream>
using namespace std;


template<class T>
class AbstractGraph: public IGraph<T>{
public:
    class Edge; //forward declaration
    class VertexNode; //forward declaration
    class Iterator; //forward declaration
    
private:
protected:
    //Using the adjacent list technique, so need to store list of nodes (nodeList)
    DLinkedList<VertexNode*> nodeList; 
    
    //Function pointers:
    bool (*vertexEQ)(T&, T&); //to compare two vertices
    string (*vertex2str)(T&); //to obtain string representation of vertices
    
    
    VertexNode* getVertexNode(T& vertex){
        typename DLinkedList<VertexNode*>::Iterator it = nodeList.begin();
        while(it != nodeList.end()){
            VertexNode* node = *it;
            if(vertexEQ(node->vertex, vertex) ) return node;
            it++;
        }
        return 0;
    }
    string vertex2Str(VertexNode& node){
        return vertex2str(node.vertex);
    }
    string edge2Str(Edge& edge){
        stringstream os;
        os << "E("
                << vertex2str(edge.from->vertex)
                << ","
                << vertex2str(edge.to->vertex)
                << ")";
        return os.str();
    }
    
public:
    AbstractGraph(
            bool (*vertexEQ)(T&, T&)=0, 
            string (*vertex2str)(T&)=0){
        
        this->vertexEQ = vertexEQ;
        this->vertex2str = vertex2str;
    }
    virtual ~AbstractGraph(){
        this->clear();
    }
    
    typedef bool (*vertexEQFunc)(T&, T&);
    typedef string (*vertex2strFunc)(T&);
    vertexEQFunc getVertexEQ(){
        return this->vertexEQ;
    }
    vertex2strFunc getVertex2Str(){
        return this->vertex2str;
    }

    
//////////////////////////////////////////////////////////////////////
//////////////////// IMPLEMENTATION of IGraph API ////////////////////
//////////////////////////////////////////////////////////////////////
    /* connect, disconnect, and remove: will be overridden in:
     *  >> UGraphModel
     *  >> DGraphModel
     */
    virtual void connect(T from, T to, float weight=0) =0;
    virtual void disconnect(T from, T to)=0;
    virtual void remove(T vertex)=0;
    
    
    
    /* The following are common methods for UGraphModel and DGraphModel
     */

    /*
    ! contains: check if a vertex is in the graph
    ? Parameters:
        * T vertex: the vertex to check
    ? Return:
        * true if the vertex is in the graph, and false otherwise.
    */
    virtual bool contains(T vertex){
        VertexNode* node = getVertexNode(vertex);
        return node != nullptr;
    }

    /*
    ! connected(T from, T to)
    ? Functionality: 
        * Checks whether there is an edge between vertices from and to.
    ? Exceptions: 
        * Throws VertexNotFoundException if a vertex is not found.
    */
    virtual bool connected(T from, T to){
        VertexNode* fromNode = getVertexNode(from);
        VertexNode* toNode = getVertexNode(to);
        if (fromNode == nullptr) {
            throw VertexNotFoundException(this->vertex2str(from));
        }
        if (toNode == nullptr) {
            throw VertexNotFoundException(this->vertex2str(to));
        }
        return fromNode->getEdge(toNode) != nullptr;
    }

    /*
    ! add: add a vertex to the graph
    */
    virtual void add(T vertex) {
        // Check if the vertex is already in the graph
        if (contains(vertex)) {
            return;
        }
        // If not, create a new VertexNode and add to nodeList
        VertexNode* newNode = new VertexNode(vertex, vertexEQ, vertex2str);
        nodeList.add(newNode);
    }
    
    virtual float weight(T from, T to){
        if (!contains(from)) {
            throw VertexNotFoundException(this->vertex2str(from));
        }
        if (!contains(to)) {
            throw VertexNotFoundException(this->vertex2str(to));
        }
        if (!connected(from, to)) {
            stringstream edge_os;
            edge_os << "E("
                << vertex2str(from)
                << ","
                << vertex2str(to)
                << ")";
            throw EdgeNotFoundException(edge_os.str());
        }
        Edge* edge = getVertexNode(from)->getEdge(getVertexNode(to));
        return edge->weight;
    }

    /*
    ! getOutwardEdges, getInwardEdges
    ? Functionality: 
        * Returns a list of outward edges from the vertex from.
        * Returns a list of inward edges to the vertex to.
        * The list of edges is represented by a list of vertices.
        * The list of vertices is represented by a DLinkedList<T>.
    ? Exceptions:
        * Throws VertexNotFoundException if the vertex from or to is not found.
    */
    virtual DLinkedList<T> getOutwardEdges(T from){
        VertexNode* node = getVertexNode(from);
        if (node == nullptr) {
            throw VertexNotFoundException(this->vertex2str(from));
        }
        return node->getOutwardEdges();
    }
    
    virtual DLinkedList<T>  getInwardEdges(T to){
        VertexNode* node = getVertexNode(to);
        if (node == nullptr) {
            throw VertexNotFoundException(this->vertex2str(to));
        }
        DLinkedList<T> inList;
        for (auto vertex : nodeList) {
            if (vertex->getEdge(node) != nullptr) {
                inList.add(vertex->vertex);
            }
        }
        return inList;
    }
    
    /*
    ! size
    ? Functionality: 
        * Returns the number of vertices in the graph.
    ? Exceptions:
        * None.
    */
    virtual int size() {
        return nodeList.size();
    }

    /*
    ! empty
    ? Functionality: 
        * Checks whether the graph is empty.
    ? Exceptions:
        * None.
    */
    virtual bool empty(){
        return nodeList.size() == 0;
    }

    /*
    ! clear
    ? Functionality: 
        * Clears the graph by removing all vertices and edges.
    ? Exceptions:
        * None.
    */
    virtual void clear(){
        typename DLinkedList<VertexNode*>::Iterator it = nodeList.begin();
        typename DLinkedList<VertexNode*>::Iterator next_it;
        while(it != nodeList.end()){
            VertexNode* node = *it;
            next_it = it;
            next_it++;
            while (next_it != nodeList.end()) {
                VertexNode* nextNode = *next_it;
                if (node->getEdge(nextNode) != nullptr) {
                    node->removeTo(nextNode);
                }
                next_it++;
            }
            it++;
        }
        
        it = nodeList.begin();
        while (it != nodeList.end()) {
            VertexNode* node = *it;
            it.remove(VertexNode::free);
            it++;
        }
    }

    /*
    ! inDegree(T vertex)
    ? Functionality: 
        * Returns the in-degree (number of incoming edges) of the vertex vertex.
    ? Exceptions:
        * Throws VertexNotFoundException if the vertex vertex is not found.
    */
    virtual int inDegree(T vertex){
        VertexNode* node = getVertexNode(vertex);
        if (node == nullptr) {
            throw VertexNotFoundException(this->vertex2str(vertex));
        }
        return node->inDegree();
    }

    /*
    ! outDegree(T vertex)
    ? Functionality: 
        * Returns the out-degree (number of outgoing edges) of the vertex vertex.
    ? Exceptions: 
        * Throws VertexNotFoundException if the vertex vertex is not found.
    */
    virtual int outDegree(T vertex){
        VertexNode* node = getVertexNode(vertex);
        if (node == nullptr) {
            throw VertexNotFoundException(this->vertex2str(vertex));
        }
        return node->outDegree();
    }
    
    /*
    ! vertices()
    ? Functionality: 
        * Returns a list of all vertices in the graph.
    ? Exceptions: 
        * None.
    */
    virtual DLinkedList<T> vertices(){
        DLinkedList<T> vertices;
        for (auto node : nodeList) {
            vertices.add(node->vertex);
        }
        return vertices;
    }

    void println(){
        cout << this->toString() << endl;
    }
    virtual string toString(){
        string mark(50, '=');
        stringstream os;
        os << mark << endl;
        os << left << setw(12) << "Vertices:" << endl;
        typename DLinkedList<VertexNode*>::Iterator nodeIt = nodeList.begin();
        while(nodeIt != nodeList.end()){
            VertexNode* node = *nodeIt;
            os << node->toString() << endl;
            nodeIt++;
        }
        string mark2(30, '-');
        os << mark2 << endl;
        
        os << left << setw(12) << "Edges:" << endl;
        
        nodeIt = nodeList.begin();
        while(nodeIt != nodeList.end()){
            VertexNode* node = *nodeIt;
            
            typename DLinkedList<Edge*>::Iterator edgeIt = node->adList.begin();
            while(edgeIt != node->adList.end()){
                Edge* edge = *edgeIt;
                os << edge->toString() << endl;
                
                edgeIt++;
            }
            
            nodeIt++;
        }
        os << mark << endl;
        return os.str();
    }
    
    /* begin, end: will be used to traverse on graph's vertices
     * example: assume that "graph" is a graph.
     * Code:
     *      AbstractGraph<T>::Iterator it;
     *      for(it = graph.begin(); it != graph.end(); it++){
     *          T vertex = *it; 
     *          //continue to process vertex here!
     *      }
     */
    Iterator begin(){
        return Iterator(this, true);
    }
    Iterator end(){
        return Iterator(this, false);
    }

//////////////////////////////////////////////////////////////////////
////////////////////////  INNER CLASSES DEFNITION ////////////////////
//////////////////////////////////////////////////////////////////////
    
public:
//BEGIN of VertexNode    
    class VertexNode{
    private:
        bool visited;
        template<class U>
        friend class UGraphModel; //UPDATED: added
        T vertex;
        int inDegree_, outDegree_, cache_inDegree_;
        DLinkedList<Edge*> adList; 
        friend class Edge;
        friend class AbstractGraph;
        
        bool (*vertexEQ)(T&, T&);
        string (*vertex2str)(T&);
        
    public:
        VertexNode():adList(&DLinkedList<Edge*>::free, &Edge::edgeEQ){}
        VertexNode(T vertex, bool (*vertexEQ)(T&, T&), string (*vertex2str)(T&))
            :adList(&DLinkedList<Edge*>::free, &Edge::edgeEQ){
            this->vertex = vertex;
            this->vertexEQ = vertexEQ;
            this->vertex2str = vertex2str;
            this->outDegree_ = this->inDegree_ = 0;
        }

        static void free(VertexNode* node){
            delete node;
        }

        T& getVertex(){
            return vertex;
        }

        /*
        ! equals
        ? Functionality: 
            * Compares this vertex with another node for equality using the vertexEQ function.
        ? Exceptions: 
            * None.
        */
        bool equals(VertexNode* node){
            if (vertexEQ != 0) 
                return vertexEQ(this->vertex, node->vertex);
            else return this->vertex == node->vertex;
        }

        /*
        ! connect
        ? Parameters:
            * VertexNode* to: the destination vertex
            * float weight: the weight of the edge
        ? Functionality: 
            * Connects this vertex to another vertex to by creating an edge
            * with the specified weight (default is 0).
        ? Exceptions: None.
        */
        void connect(VertexNode* to, float weight=0){
            // Check if the edge already exists
            for (auto edge : adList) {
                if ((edge->to)->equals(to)) {
                    edge->updateWeight(weight);
                    return;
                }
            }

            // Create a new edge and add it to the adjacency list
            Edge* newEdge = new Edge(this, to, weight);
            adList.add(newEdge);

            // Update the inDegree and outDegree of the vertices
            to->inDegree_++;
            this->outDegree_++;
        }

        /*
        ! getOutwardEdges
        ? Functionality: 
            * Returns a list of outward edges from this vertex.
        ? Exceptions: 
            * None.
        */
        DLinkedList<T> getOutwardEdges(){
            DLinkedList<T> outList;
            for (auto edge : adList) {
                outList.add(edge->to->vertex);
            }
            return outList;
        }

        /*
        ! getEdge
        ? Functionality: 
            * Retrieves the edge connecting this vertex to the specified vertex
            * to. Returns nullptr if no such edge exists.
        ? Exceptions: None
        */
        Edge* getEdge(VertexNode* to){
            for (auto edge : adList) {
                if ((edge->to)->equals(to)) {
                    return edge;
                }
            }
            return nullptr;
        }
        
        /*
        ! removeTo
        ? Functionality: 
            * Removes the edge connecting this vertex to the specified vertex to.
        ? Exceptions: 
            * None
        */
        void removeTo(VertexNode* to){
            // Find the edge connecting this vertex to the specified vertex
            typename DLinkedList<Edge*>::Iterator it = adList.begin();
            while (it != adList.end()) {
                Edge* edge = *it;
                if ((edge->to)->equals(to)) {
                    it.remove(Edge::free);
                    break;
                }
                it++;
            }

            // Update the inDegree and outDegree of the vertices
            to->inDegree_--;
            this->outDegree_--;
        }

        /*
        ! inDegree
        ? Functionality: 
            * Returns the in-degree of this vertex.
        ? Exceptions: 
            * None.
        */
        int inDegree(){
            return this->inDegree_;
        }

        /*
        ! outDegree
        ? Functionality: 
            * Returns the out-degree of this vertex.
        ? Exceptions:
            * None.
        */
        int outDegree(){
            return this->outDegree_;
        }

        /*
        ! checkVisited
        ? Functionality: 
            * Checks whether this vertex has been visited.
        ? Exceptions:
            * None.
        */
        bool isVisited(){
            return this->visited;
        }

        /*
        ! visit
        ? Functionality: 
            * Marks this vertex as visited.
        ? Exceptions:
            * None.
        */
        void visit(bool reset = true){
            if (reset) this->visited = true;
            else this->visited = false;
        }

        void cacheInDegree(){
            this->cache_inDegree_ = inDegree_;
        }

        void descreaseInDegree(){
            this->inDegree_--;
        }

        void restoreInDegree(){
            this->inDegree_ = this->cache_inDegree_;
        }

        string getVertexStr(){
            stringstream os;
            if (vertex2str != 0) os << vertex2str(vertex);
            else os << vertex;
            return os.str();
        }

        string toString(){
            stringstream os;
            os << "V("
                    << this->getVertexStr() << ", "
                    << "in: " << this->inDegree_ << ", "
                    << "out: " << this->outDegree_ << ")";
            return os.str();
        }
    };
//END of VertexNode    
    
//BEGIN of Edge
    class Edge{
    private:
        VertexNode* from;
        VertexNode* to;
        float weight;
        friend class VertexNode;
        friend class AbstractGraph;
        
    public:
        Edge(){}
        Edge(VertexNode* from, VertexNode* to, float weight=0){
            this->from = from;
            this->to = to;
            this->weight = weight;
        }
        
        /*
        ! bool equals(Edge* edge)
        ? Functionality: 
            * Compares the current edge with another edge. 
            * Returns true if both edges have the same source and destination vertices, 
            * and false otherwise.
        ? Exceptions: 
            * None.
        */
        bool equals(Edge* edge){
            return (this->from->equals(edge->from) && this->to->equals(edge->to));
        }

        static bool edgeEQ(Edge*& edge1, Edge*& edge2){
            return edge1->equals(edge2);
        }

        static void free(Edge* edge){
            delete edge;
        }

        void updateWeight(float weight){
            this->weight = weight;
        }

        string toString(){
            stringstream os;
            os << "E("
                    << this->from->getVertexStr()
                    << ","
                    << this->to->getVertexStr()
                    << ","
                    << this->weight
                    << ")";
            return os.str();
        }
    };
 //END of Edge
    

//BEGIN of Iterator
public:
    class Iterator{
    private:
        typename DLinkedList<VertexNode*>::Iterator nodeIt;
        
    public:
        Iterator(AbstractGraph<T>* pGraph=0, bool begin=true){
            if(begin) {
                if(pGraph != 0) nodeIt = pGraph->nodeList.begin();
            }
            else{
                if(pGraph != 0) nodeIt = pGraph->nodeList.end();
            }
        }
        Iterator& operator=(const Iterator& iterator){
            this->nodeIt = iterator.nodeIt;
            return *this;
        }
        
        T& operator*(){
            return (*nodeIt)->vertex;
        }
        
        bool operator!=(const Iterator& iterator){
            return nodeIt != iterator.nodeIt;
        }
        // Prefix ++ overload 
        Iterator& operator++(){
            nodeIt++;
            return *this; 
        }
        // Postfix ++ overload 
        Iterator operator++(int){
            Iterator iterator = *this; 
            ++*this; 
            return iterator; 
        }
    };
//END of Iterator
};

#endif /* ABSTRACTGRAPH_H */

