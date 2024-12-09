

void sort_topo23() {
  string name = "sort_topo23";
  //! data ------------------------------------
  DGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'1', '2', '3', '7', '8', '6', '4', '5'};
  for (int idx = 0; idx < 8; idx++) {
    model.add(vertices[idx]);
  }
  TopoSorter<char> topoSorter(&model, &intKeyHash);
  DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::DFS);

  //! expect ----------------------------------
  string expect = "DFS Topological Sort: 1->2->3->4->5->6->7->8->NULL";


  cout << "DFS Topological Sort: ";
  for (auto it = result.begin(); it != result.end(); it++) {
    cout << *it << "->";
  }
  cout << "NULL";

  //! remove data -----------------------------
  model.clear();


}
