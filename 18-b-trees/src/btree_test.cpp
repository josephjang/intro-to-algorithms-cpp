#include <iostream>
#include "btree.h"

int main(int argc, char **argv) {
  auto btree = new BTree(3);
  btree->Print();

  for (int i = 0; i < 10; i++) {
    btree->Insert(i);
    std::cout << "inserted " << i << std::endl;
    btree->Print();
  }

  for (int i = 0; i < 10; i++) {
    auto result = btree->Search(i);
    std::cout << std::boolalpha << "searched for " << i << ": " << result
              << std::endl;
  }

  delete btree;

  return 0;
}