

void sort_topo16() {
  string name = "sort_topo16";
  //! data ------------------------------------
  DGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'A', 'B', 'C', 'D'};
  for (int idx = 0; idx < 4; idx++) {
    model.add(vertices[idx]);
  }
  TopoSorter<char> topoSorter(&model, &intKeyHash);
  DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::DFS);

  //! expect ----------------------------------
  string expect = "DFS Topological Sort: A->B->C->D->NULL";


  cout << "DFS Topological Sort: ";
  for (auto it = result.begin(); it != result.end(); it++) {
    cout << *it << "->";
  }
  cout << "NULL";

  //! remove data -----------------------------
  model.clear();


}
