

void sort_topo15()
{
    string name = "sort_topo15";
    stringstream output;
    //! data ------------------------------------
    DGraphModel<char> model(&charComparator, &vertex2str);
     char vertices[] = {'F', 'G', 'B', 'C', 'D', 'E', 'A', 'X', 'Y', 'V'};
    for (int idx = 0; idx < 10; idx++)
    {
        model.add(vertices[idx]);
    }
    model.connect('F', 'Y');
    model.connect('D', 'X');
    model.connect('X', 'Y');
    model.connect('Y', 'V');

    TopoSorter<char> topoSorter(&model, &intKeyHash);
    DLinkedList<char> result = topoSorter.dfsSort(false);

    //! expect ----------------------------------
    string expect = "DFS Topological Sort: A->E->D->X->C->B->G->F->Y->V->NULL";
    //! output ----------------------------------
    cout << "DFS Topological Sort: ";
    for (auto it = result.begin(); it != result.end(); it++)
    {
        cout << *it << "->";
    }
    cout << "NULL";

    //! remove data -----------------------------
    model.clear();

    //! result ----------------------------------
    return printResult(output.str(), expect, name);
}