

void sort_topo02() {
  string name = "sort_topo02";
  stringstream output;
  //! data ------------------------------------
  Stack<char> stack;
  stack.push('A');
  stack.push('B');
  stack.push('C');

  //! expect ----------------------------------
  string expect = "C 0 C D B A A 1";

  //! output ----------------------------------
  cout << stack.peek() << " " << stack.empty() << " " << stack.pop() << " ";
  stack.push('D');
  cout << stack.pop() << " ";
  cout << stack.pop() << " " << stack.peek() << " ";
  cout << stack.pop() << " " << stack.empty();

  //! remove data -----------------------------


}
