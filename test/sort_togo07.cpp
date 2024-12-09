

void sort_topo07() {
  string name = "sort_topo07";
  stringstream output;
  //! data ------------------------------------
  DGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'A','B','C','D','E'};
  for (int idx = 0; idx < 5; idx++) {
    model.add(vertices[idx]);
  }
  model.connect('A', 'B');
  model.connect('B', 'D');
  model.connect('B', 'C');
  model.connect('B', 'E');

  TopoSorter<char> topoSorter(&model, &intKeyHash);
  DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::BFS);

  //! expect ----------------------------------
  string expect = "BFS Topological Sort: A->B->C->D->E->NULL";
  //! output ----------------------------------
  cout << "BFS Topological Sort: ";
  for (auto it = result.begin(); it != result.end(); it++) {
    cout << *it << "->";
  }
  cout << "NULL";

  //! remove data -----------------------------
  model.clear();


}