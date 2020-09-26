#pragma once

#include <optional>

class BTree {
 public:
  BTree(int order);
  ~BTree();

  bool Search(int key);
  void Insert(int key);
  void Delete(int key);

  void Print();

 private:
  class Node {
   public:
    // the number of keys currently stored in the node
    int n;

    // n keys are stored in nondecreasing order in the node
    int *keys;

    Node **children;

    // TRUE if the node is a leaf and FALSE otherwise
    bool leaf;
  };

  class SearchInternalResult {
   public:
    SearchInternalResult(Node *node, int index) {
      this->node = node;
      this->index = index;
    }

    Node *node;
    int index;
  };

  void Create();
  std::optional<SearchInternalResult> SearchInternal(Node *node, int key);
  void InsertNonFull(Node *node, int key);
  void SplitChild(Node *node, int index);
  void PrintNode(Node *node);

  Node *root;
  int t;
};