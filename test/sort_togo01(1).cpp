

void sort_topo01() {
  string name = "sort_topo01";
  stringstream output;
  //! data ------------------------------------
  Queue<char> queue;
  queue.push('A');
  queue.push('B');
  queue.push('C');

  //! expect ----------------------------------
  string expect = "0 A B C D 1";

  //! output ----------------------------------
  cout << queue.empty() << " " << queue.pop() << " ";
  queue.push('D');
  cout << queue.pop() << " ";
  cout << queue.pop() << " ";
  cout << queue.pop() << " " << queue.empty();

  //! remove data -----------------------------


}
