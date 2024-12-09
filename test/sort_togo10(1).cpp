

void sort_topo10()
{
    string name = "sort_topo10";
    stringstream output;
    //! data ------------------------------------
    DGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'F','G','B', 'C', 'D', 'E', 'A'};
    for (int idx = 0; idx < 7; idx++)
    {
        model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.connect('B', 'D');
    model.connect('B', 'C');
    model.connect('B', 'E');
    model.connect('C', 'G');
    model.connect('G', 'F');
    model.connect('E', 'F');

    TopoSorter<char> topoSorter(&model, &intKeyHash);
    DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::DFS);

    //! expect ----------------------------------
    string expect = "DFS Topological Sort: A->B->C->D->E->G->F->NULL";
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