

void sort_topo25() {
  string name = "sort_topo25";
  //! data ------------------------------------
  DGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'1', '2', '3', '7', '8', '6', '4', '5'};
  for (int idx = 0; idx < 8; idx++) {
    model.add(vertices[idx]);
  }
  model.connect('1', '2');
  model.connect('1', '3');
  model.connect('1', '2');
  model.connect('1', '7');
  model.connect('1', '8');
  model.connect('1', '6');
  model.connect('1', '4');
  model.connect('1', '5');

  TopoSorter<char> topoSorter(&model, &intKeyHash);
  DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::BFS);

  //! expect ----------------------------------
  string expect = "BFS Topological Sort: 1->2->3->4->5->6->7->8->NULL";


  cout << "BFS Topological Sort: ";
  for (auto it = result.begin(); it != result.end(); it++) {
    cout << *it << "->";
  }
  cout << "NULL";

  //! remove data -----------------------------
  model.clear();


}
