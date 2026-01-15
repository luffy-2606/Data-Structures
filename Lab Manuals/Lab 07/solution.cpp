#include <iostream>
using namespace std;

template <typename T>
class Node {
public:
    T data;
    Node* left;
    Node* right;
    Node(T d = 0) {
        data = d;
        left = nullptr;
        right = nullptr;
    }
};

template <class type>
class BinaryTree {
private:
    Node<type>* root;

    void insertSimple(Node<type>*& node, type d) {
        if (node == nullptr) {
            node = new Node<type>(d);
        }
        else {
            if (node->left == nullptr)
                insertSimple(node->left, d);
            else
                insertSimple(node->right, d);
        }
    }

    bool searchR(Node<type>* node, type key) {
        if (node == nullptr)
            return false;
        if (node->data == key)
            return true;
        return searchR(node->left, key) || searchR(node->right, key);
    }

    void inorderR(Node<type>* node) {
        if (node == nullptr)
            return;
        inorderR(node->left);
        cout << node->data << " ";
        inorderR(node->right);
    }

    bool deleteR(Node<type>*& node, type key) {
        if (node == nullptr)
            return false;
        if (node->data == key) {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                node = nullptr;
            }
            else if (node->left == nullptr) {
                Node<type>* temp = node;
                node = node->right;
                delete temp;
            }
            else if (node->right == nullptr) {
                Node<type>* temp = node;
                node = node->left;
                delete temp;
            }
            else {
                node->data = node->left->data;
                deleteR(node->left, node->left->data);
            }
            return true;
        }
        return deleteR(node->left, key) || deleteR(node->right, key);
    }

public:
    BinaryTree() {
        root = nullptr;
    }

    void insert(type d) {
        insertSimple(root, d);
    }

    bool search(type key) {
        return searchR(root, key);
    }

    void inorder() {
        inorderR(root);
        cout << endl;
    }

    void deleteNode(type key) {
        if (deleteR(root, key))
            cout << "Deleted " << key << endl;
        else
            cout << key << " not found.\n";
    }
};

template <class type>
class BST {
private:
    Node<type>* root;

    void insertR(Node<type>*& node, type d) {
        if (node == nullptr) {
            node = new Node<type>(d);
        }
        else if (d < node->data)
            insertR(node->left, d);
        else
            insertR(node->right, d);
    }

    bool searchR(Node<type>* node, type key) {
        if (node == nullptr)
            return false;
        if (node->data == key)
            return true;
        else if (key < node->data)
            return searchR(node->left, key);
        else
            return searchR(node->right, key);
    }

    void inorderR(Node<type>* node) {
        if (node == nullptr)
            return;
        inorderR(node->left);
        cout << node->data << " ";
        inorderR(node->right);
    }

    Node<type>* findMin(Node<type>* node) {
        if (node->left == nullptr)
            return node;
        return findMin(node->left);
    }

    Node<type>* deleteR(Node<type>* node, type key) {
        if (node == nullptr)
            return node;

        if (key < node->data)
            node->left = deleteR(node->left, key);
        else if (key > node->data)
            node->right = deleteR(node->right, key);
        else {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                node = nullptr;
            }
            else if (node->left == nullptr) {
                Node<type>* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                Node<type>* temp = node->left;
                delete node;
                return temp;
            }
            else {
                Node<type>* temp = findMin(node->right);
                node->data = temp->data;
                node->right = deleteR(node->right, temp->data);
            }
        }
        return node;
    }

    void rangeR(Node<type>* node, type low, type high) {
        if (node == nullptr)
            return;
        if (low < node->data)
            rangeR(node->left, low, high);
        if (node->data >= low && node->data <= high)
            cout << node->data << " ";
        if (node->data < high)
            rangeR(node->right, low, high);
    }

    Node<type>* findSecondLargest(Node<type>* node, Node<type>*& parent) {
        if (node->right == nullptr)
            return node;
        parent = node;
        return findSecondLargest(node->right, parent);
    }

    int height(Node<type>* node) {
        if (node == NULL)
            return 0;
        int l = height(node->left);
        int r = height(node->right);
        return (l > r ? l : r) + 1;
    }

    void printLevel(Node<type>* node, int level) {
        if (node == NULL)
            return;
        if (level == 1)
            cout << node->data << " ";
        else {
            printLevel(node->left, level - 1);
            printLevel(node->right, level - 1);
        }
    }

public:
    BST() {
        root = nullptr;
    }

    void insert(type d) {
        insertR(root, d);
    }

    bool search(type key) {
        return searchR(root, key);
    }

    void inorder() {
        inorderR(root);
        cout << endl;
    }

    void deleteNode(type key) {
        root = deleteR(root, key);
    }

    void rangeSearch(type low, type high) {
        cout << "Range [" << low << "," << high << "]: ";
        rangeR(root, low, high);
        cout << endl;
    }

    void secondLargest() {
        if (root == nullptr || (root->left == nullptr && root->right == nullptr)) {
            cout << "Not enough nodes.\n";
            return;
        }

        Node<type>* parent = nullptr;
        Node<type>* largest = findSecondLargest(root, parent);

        if (largest->left != nullptr) {
            Node<type>* temp = largest->left;
            while (temp->right != nullptr)
                temp = temp->right;
            cout << "Second largest: " << temp->data << endl;
        }
        else if (parent != nullptr) {
            cout << "Second largest: " << parent->data << endl;
        }
    }

    void levelOrder() {
        int h = height(root);
        for (int i = 1; i <= h; i++) {
            printLevel(root, i);
        }
        cout << endl;
    }
};

int main() {
    cout << "=== SIMPLE BINARY TREE ===\n";
    BinaryTree<int> bt;
    bt.insert(1);
    bt.insert(2);
    bt.insert(3);
    bt.insert(4);
    bt.insert(5);

    cout << "Inorder traversal: ";
    bt.inorder();

    cout << "Search 3: ";
    if (bt.search(3))
        cout << "Found" << endl;
    else
        cout << "Not found" << endl;

    bt.deleteNode(3);
    cout << "After deletion: ";
    bt.inorder();

    cout << "\n=== BINARY SEARCH TREE ===\n";
    BST<int> bst;
    bst.insert(50);
    bst.insert(30);
    bst.insert(70);
    bst.insert(20);
    bst.insert(40);
    bst.insert(60);
    bst.insert(80);

    cout << "Inorder (sorted): ";
    bst.inorder();

    cout << "Search 40: ";
    if (bst.search(40))
        cout << "Found" << endl;
    else
       cout << "Not found" << endl;
    bst.deleteNode(70);
    cout << "After deleting 70: ";
    bst.inorder();

    bst.rangeSearch(25, 65);
    bst.secondLargest();

    cout << "Level Order Traversal: ";
    bst.levelOrder(); // 50 30 20 40 60 80
    return 0;
}
