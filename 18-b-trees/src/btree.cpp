#include <iostream>
#include "btree.h"

BTree::BTree(int m) {
  t = m;
  Create();
}

BTree::~BTree() {
  // TODO: delete all the node
  delete[] root->keys;
  delete[] root->children;
  delete root;
}

void BTree::Create() {
  auto node = new Node();
  node->leaf = true;
  node->n = 0;
  node->keys = new int[2 * t - 1];
  node->children = new Node *[2 * t];
  root = node;
}

bool BTree::Search(int key) { return SearchInternal(root, key).has_value(); }

std::optional<BTree::SearchInternalResult> BTree::SearchInternal(Node *node,
                                                                 int key) {
  int i = 0;
  for (; i < node->n; i++) {
    if (key <= node->keys[i]) {
      break;
    }
  }

  if (i < node->n && key == node->keys[i]) {
    return SearchInternalResult(node, i);
  } else if (node->leaf) {
    return std::nullopt;
  } else {
    return SearchInternal(node->children[i], key);
  }
}

void BTree::Insert(int key) {
  BTree::Node *r = root;
  if (r->n == 2 * t - 1) {
    auto s = new Node();
    s->leaf = false;
    s->n = 0;
    s->keys = new int[2 * t - 1];
    s->children = new Node *[2 * t];

    root = s;
    s->children[0] = r;
    Print();

    SplitChild(s, 0);
    InsertNonFull(s, key);
  } else {
    InsertNonFull(r, key);
  }
}

void BTree::InsertNonFull(Node *node, int key) {
  // std::cout << "inserting a key to node: " << node << std::endl;
  int i = node->n - 1;
  if (node->leaf) {
    for (; i >= 0; i--) {
      if (key >= node->keys[i]) {
        break;
      }
      node->keys[i + 1] = node->keys[i];
    }
    node->keys[i + 1] = key;
    node->n += 1;
  } else {
    for (; i >= 0; i--) {
      if (key >= node->keys[i]) {
        break;
      }
    }
    i = i + 1;
    if (node->children[i]->n == 2 * t - 1) {
      SplitChild(node, i);
      if (key > node->keys[i]) {
        i = i + 1;
      }
    }
    InsertNonFull(node->children[i], key);
  }
}

void BTree::SplitChild(Node *node, int index) {
  auto z = new Node();
  // the node to split
  auto y = node->children[index];
  z->leaf = y->leaf;
  z->n = t - 1;
  z->keys = new int[2 * t - 1];
  z->children = new Node *[2 * t];
  // copy a latter half (t - 1) of the keys
  for (int j = 0; j < t - 1; j++) {
    z->keys[j] = y->keys[j + t];
  }
  // copy a latter half (t) of the children
  if (!y->leaf) {
    for (int j = 0; j < t; j++) {
      z->children[j] = y->children[j + t];
    }
  }
  y->n = t - 1;
  // shift the latter half of the children in the parent node to the right
  for (int j = node->n; j >= index + 1; j--) {
    node->children[j + 1] = node->children[j];
  }
  // install a new node to the parent node's children
  node->children[index + 1] = z;
  // shift the latter half of the keys in the parent node to the right
  for (int j = node->n - 1; j >= index; j--) {
    node->keys[j + 1] = node->keys[j];
  }
  // install the splitting key to the parent node's keys
  node->keys[index] = y->keys[t - 1];
  node->n += 1;
}

void BTree::Delete(int key) { DeleteInternal(root, key); }

void BTree::DeleteInternal(Node *node, int key) {
  if (node->leaf) {
    std::cout << "delete the key from a leaf: " << node << std::endl;
    // search key in the node
    int i = 0;
    for (; i < node->n; i++) {
      if (key <= node->keys[i]) {
        break;
      }
    }

    if (i < node->n && key == node->keys[i]) {
      std::cout << "the key is found at " << node->keys[i] << " in the node "
                << node << std::endl;
      for (int j = i; j < node->n - 1; j++) {
        node->keys[j] = node->keys[j + 1];
      }

      node->n -= 1;
    } else {
      std::cout << "the key is not found" << std::endl;
    }
  } else {
    // search key in the node
    int i = 0;
    for (; i < node->n; i++) {
      if (key <= node->keys[i]) {
        break;
      }
    }

    if (i < node->n && key == node->keys[i]) {
      // key is in the node
      std::cout << "found a key: " << node->keys[i] << " in the node " << node
                << std::endl;

    } else {
      std::cout << "determined a child: " << i << " (" << node->children[i]
                << ")" << std::endl;
      auto node_to_delete = node->children[i];
      if (node->children[i]->n < t) {
        // check siblings
        if (i < node->n && node->children[i + 1]->n >= t) {
          std::cout << "will execute 3a with the right sibling: "
                    << node->children[i + 1] << std::endl;
          auto child = node->children[i];
          auto sibling = node->children[i + 1];
          // move a key from the node to the child
          // assume the child is not the rightmost child
          child->keys[child->n] = node->keys[i];
          child->n += 1;
          // move the leftmost key from the sibling to the node
          node->keys[i] = sibling->keys[0];
          // move the leftmost child from the sibling to the child
          child->children[child->n] = sibling->children[0];
          for (int j = 0; j < sibling->n - 1; j++) {
            sibling->keys[j] = sibling->keys[j + 1];
            sibling->children[j] = sibling->children[j + 1];
          }
          sibling->children[sibling->n - 1] = node->children[sibling->n];
          sibling->n -= 1;
        } else if (i > 0 && node->children[i - 1]->n >= t) {
          std::cout << "will execute 3a with the left sibling: "
                    << node->children[i - 1] << std::endl;
          auto child = node->children[i];
          auto sibling = node->children[i - 1];
          // move a key from the node to the child
          child->children[child->n + 1] = child->children[child->n];
          for (int j = child->n - 1; j >= 0; j--) {
            child->keys[j + 1] = child->keys[j];
            child->children[j + 1] = child->children[j];
          }
          child->keys[0] = node->keys[i - 1];
          child->n += 1;
          // move the rightmost key from the sibling to the node
          node->keys[i - 1] = sibling->keys[sibling->n - 1];
          // move the rightmost child from the sibling to the child
          child->children[0] = sibling->children[sibling->n];
          sibling->n -= 1;
        } else if (i < node->n && node->children[i + 1]->n < t) {
          std::cout << "will execute 3b with the right sibling: "
                    << node->children[i + 1] << std::endl;
          node_to_delete = MergeChildren(node, i);
        } else if (i > 0 && node->children[i - 1]->n < t) {
          std::cout << "will execute 3b with the left sibling: "
                    << node->children[i - 1] << std::endl;
          node_to_delete = MergeChildren(node, i - 1);
        }
      }

      std::cout << "delete the key from a child: " << node_to_delete
                << std::endl;
      DeleteInternal(node_to_delete, key);
    }
  }
}

BTree::Node *BTree::MergeChildren(Node *node, int index) {
  // merge the child and the sibling using the parent key as a median
  auto child = node->children[index];
  auto sibling = node->children[index + 1];
  child->keys[child->n] = node->keys[index];
  child->n += 1;
  for (int j = 0; j < sibling->n; j++) {
    child->keys[child->n + j] = sibling->keys[j];
    child->children[child->n + j] = sibling->children[j];
  }
  child->children[child->n + sibling->n] = sibling->children[sibling->n];
  child->n += sibling->n;
  // delete the parent key and the children at the right-side
  for (int j = index; j < node->n - 1; j++) {
    node->keys[j] = node->keys[j + 1];
    node->children[j + 1] = node->children[j + 2];
  }
  node->n -= 1;
  if (node->n == 0) {
    root = child;
    delete[] node->keys;
    delete[] node->children;
    delete node;
  }
  // delete the sibling node
  delete[] sibling->keys;
  delete[] sibling->children;
  delete sibling;

  return child;
}

void BTree::Print() { PrintNode(root); }

void BTree::PrintNode(Node *node) {
  std::cout << "node: " << node << std::endl;
  std::cout << "  n: " << node->n << std::endl;
  std::cout << std::boolalpha << "  leaf: " << node->leaf << std::endl;
  for (int i = 0; i < node->n; i++) {
    if (!node->leaf) {
      std::cout << "  child " << i << ": " << node->children[i] << std::endl;
    }
    std::cout << "  key " << i << ": " << node->keys[i] << std::endl;
  }
  if (!node->leaf) {
    std::cout << "  child " << node->n << ": " << node->children[node->n]
              << std::endl;
    for (int i = 0; i <= node->n; i++) {
      if (node->children[i] != nullptr) {
        PrintNode(node->children[i]);
      }
    }
  }
}
