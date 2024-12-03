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

        // Initialize in-degree map
        for (auto vertex : graph->vertices()) {
            inDegreeMap.put(vertex, graph->inDegree(vertex));
            if (graph->inDegree(vertex) == 0) {
                zeroInDegreeQueue.push(vertex);
            }
        }

        // Process vertices with zero in-degree
        while (!zeroInDegreeQueue.empty()) {
            T vertex = zeroInDegreeQueue.pop();
            result.add(vertex);

            for (auto neighbor : graph->getOutwardEdges(vertex)) {
                int inDegree = inDegreeMap.get(neighbor) - 1;
                inDegreeMap.put(neighbor, inDegree);
                if (inDegree == 0) {
                    zeroInDegreeQueue.push(neighbor);
                }
            }
        }

        if (sorted) {
            DLinkedListSE<T> sortedResult(result);
            sortedResult.sort();
            return sortedResult;
        }

        return result;
    }

    /*
    ! dfsSort(bool sorted=true)
    ? Functionality: 
        * Sorts the vertices of the graph in topological order using DFS.
    ? Parameters:
        * sorted: true if the input graph is already sorted
        * Default: true
    ? Return:
        * A list of vertices in topological order.
    */
    DLinkedList<T> dfsSort(bool sorted=true){
        DLinkedList<T> result;
        xMap<T, bool> visited(*hash_code);

        // Initialize visited map
        for (auto vertex : graph->vertices()) {
            visited.put(vertex, false);
        }

        // Visit all vertices
        for (auto vertex : graph->vertices()) {
            if (!visited.get(vertex)) {
                dfsVisit(vertex, visited, result);
            }
        }

        if (sorted) {
            DLinkedListSE<T> sortedResult(result);
            sortedResult.sort();
            return sortedResult;
        }

        return result;
    }

protected:

    //Helper functions
    xMap<T, int> vertex2inDegree(int (*hash)(T&, int));
    xMap<T, int> vertex2outDegree(int (*hash)(T&, int));
    DLinkedList<T> listOfZeroInDegrees();
    void dfsVisit(T vertex, xMap<T, bool>& visited, DLinkedList<T>& result) {
        visited.put(vertex, true);
        for (auto neighbor : graph->getOutwardEdges(vertex)) {
            if (!visited.get(neighbor)) {
                dfsVisit(neighbor, visited, result);
            }
        }
        result.add(0, vertex); // Add to the front of the list to maintain topological order
    }

}; //TopoSorter
template<class T>
int TopoSorter<T>::DFS = 0;
template<class T>
int TopoSorter<T>::BFS = 1;

/////////////////////////////End of TopoSorter//////////////////////////////////


#endif /* TOPOSORTER_H */