#include <iostream>
#include "btree.h"

const int NUM_ELEMENT = 10;

int main(int argc, char **argv) {
  auto btree = new BTree(3);
  btree->Print();

  for (int i = 0; i < NUM_ELEMENT; i++) {
    btree->Insert(i);
    std::cout << "inserted " << i << std::endl;
    // btree->Print();
  }

  for (int i = 0; i < NUM_ELEMENT; i++) {
    auto result = btree->Search(i);
    std::cout << std::boolalpha << "searched for " << i << ": " << result
              << std::endl;
  }

  btree->Print();
  for (int i = 0; i < 1; i++) {
    btree->Delete(i);
    std::cout << "deleted " << i << std::endl;
    btree->Print();
  }
  for (int i = 0; i < 3; i++) {
    auto index = NUM_ELEMENT - 1 - i;
    btree->Delete(index);
    std::cout << "deleted " << index << std::endl;
    btree->Print();
  }

  delete btree;

  return 0;
}