#ifndef TOPOSORTER_H
#define TOPOSORTER_H
#include "graph/DGraphModel.h"
#include "list/DLinkedList.h"
#include "sorting/DLinkedListSE.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"

/*
! TopoSorter
? Functionality: 
    * Topological sorting for directed graphs.
? Mode:
    * DFS: Depth-first search - 0
    * BFS: Breadth-first search - 1
    * Default: DFS
? Usage:
    * TopoSorter<int> sorter(graph);
    * DLinkedList<int> sortedList = sorter.sort(TopoSorter<int>::DFS);
    * DLinkedList<int> sortedList = sorter.sort(TopoSorter<int>::BFS);
*/
template<class T>
class TopoSorter{
public:
    static int DFS;
    static int BFS; 
    
protected:
    DGraphModel<T>* graph;
    int (*hash_code)(T&, int);
    
public:
    TopoSorter(DGraphModel<T>* graph, int (*hash_code)(T&, int)=0)
    :graph(graph), hash_code(hash_code){
    }  

    /*
    ! sort(int mode=0, bool sorted=true)
    ? Functionality: 
        * Sorts the vertices of the graph in topological order.
    ? Parameters:
        * mode: DFS or BFS
        * sorted: true if the input graph is already sorted
        * Default: DFS
    ? Return:
        * A list of vertices in topological order.
    */
    DLinkedList<T> sort(int mode=0, bool sorted=true){
        if (mode == BFS) {
            return bfsSort(sorted);
        } else {
            return dfsSort(sorted);
        }
    }

    /*
    ! bfsSort(bool sorted=true)
    ? Functionality: 
        * Sorts the vertices of the graph in topological order using BFS.
    ? Parameters:
        * sorted: true if the input graph is already sorted
        * Default: true
    ? Return:
        * A list of vertices in topological order.
    */
    DLinkedList<T> bfsSort(bool sorted=true){ 
        DLinkedList<T> result;
        Queue<T> zeroInDegreeQueue;
        xMap<T, int> inDegreeMap(*hash_code);

        DLinkedList<T> vertices = graph->vertices();

        // Initialize in-degree map
        for (auto vertex : vertices) {
            inDegreeMap.put(vertex, graph->inDegree(vertex));
            if (graph->inDegree(vertex) == 0) {
                zeroInDegreeQueue.push(vertex);
            }
        }

        // Process vertices with zero in-degree
        while (!zeroInDegreeQueue.empty()) {
            T vertex = zeroInDegreeQueue.pop();
            result.add(vertex);

            DLinkedList<T> neighbors = graph->getOutwardEdges(vertex);

            for (auto neighbor : neighbors) {
                int inDegree = inDegreeMap.get(neighbor) - 1;
                inDegreeMap.put(neighbor, inDegree);
                if (inDegree == 0) {
                    zeroInDegreeQueue.push(neighbor);
                }
            }
        }

        return result;
    }

    /*
    ! dfsSort(bool sorted=true)
    ? Functionality: 
        * Sorts the vertices of the graph in topological order using DFS.
    ? Parameters:
        * sorted: true -> need to sort vertices before Topo sort
        * Default: true
    ? Return:
        * A list of vertices in topological order.
    */
    DLinkedList<T> dfsSort(bool sorted=true){
        DLinkedList<T> result;
        xMap<T, bool> visited(*hash_code);

        // Get vertices and sort them if needed
        DLinkedList<T> vertices = graph->vertices();

        // Initialize visited map
        for (auto vertex : vertices) {
            visited.put(vertex, false);
        }

        // Visit each vertex in order
        for (auto vertex : vertices) {
            if (!visited.get(vertex)) {
                dfsVisit(vertex, visited, result, sorted);
            }
        }

        return result;
    }

protected:
    void dfsVisit(T vertex, xMap<T, bool>& visited, DLinkedList<T>& result, bool sorted) {
        visited.put(vertex, true);
        
        // Get and sort outward edges
        DLinkedList<T> neighbors = graph->getOutwardEdges(vertex);
        
        // Visit unvisited neighbors in sorted order
        for (auto neighbor : neighbors) {
            if (!visited.get(neighbor)) {
                dfsVisit(neighbor, visited, result, sorted);
            }
        }
        
        // Add current vertex to beginning of result
        // This ensures topological order
        result.add(0, vertex);
    }

    xMap<T, int> vertex2inDegree(int (*hash)(T&, int)){
        xMap<T, int> inDegrees(*hash);
        DLinkedList<T> vertices = graph->vertices();
        for (auto vertex : vertices) {
            inDegrees.put(vertex, graph->inDegree(vertex));
        }
        return inDegrees;
    }
    xMap<T, int> vertex2outDegree(int (*hash)(T&, int)){
        xMap<T, int> outDegrees(*hash);
        DLinkedList<T> vertices = graph->vertices();
        for (auto vertex : vertices) {
            outDegrees.put(vertex, graph->outDegree(vertex));
        }
        return outDegrees;
    }
    DLinkedList<T> listOfZeroInDegrees() {
        DLinkedList<T> result;
        xMap<T, int> inDegrees = vertex2inDegree(hash_code);
        for (auto vertex : inDegrees.keys()) {
            if (inDegrees.get(vertex) == 0) {
                result.add(0, vertex);
            }
        }
        return result;
    }

}; //TopoSorter
template<class T>
int TopoSorter<T>::DFS = 0;
template<class T>
int TopoSorter<T>::BFS = 1;

/////////////////////////////End of TopoSorter//////////////////////////////////


#endif /* TOPOSORTER_H */