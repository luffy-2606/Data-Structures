#include <iostream>
#include <queue>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val){
        data = val;
        left = nullptr;
        right = nullptr;
    }
};

class BST {
    Node* root;

    bool insertR(Node*& curr, int val) {
        if (!curr) {
            curr = new Node(val);
            return true;
        }
        if (val == curr->data)
            return false;
        else if (val < curr->data)
            return insertR(curr->left, val);
        else
            return insertR(curr->right, val);
    }

    Node* copyTree(Node* otherRoot) {
        if (!otherRoot) 
            return nullptr;
        Node* newNode = new Node(otherRoot->data);
        newNode->left = copyTree(otherRoot->left);
        newNode->right = copyTree(otherRoot->right);
        return newNode;
    }

    void inorderR(Node* curr) {
        if (!curr) 
            return;
        inorderR(curr->left);
        cout << curr->data << " ";
        inorderR(curr->right);
    }

    Node* searchR(Node* curr, int key) {
        if (!curr) 
            return nullptr;

        if (curr->data == key)
            return curr;
        else if (key < curr->data)
            return searchR(curr->left, key);
        else
            return searchR(curr->right, key);
    }

    void bstToDLLR(Node* curr, Node*& head, Node*& prev) {
        if (!curr) 
            return;
        bstToDLLR(curr->left, head, prev);

        if (!head) 
            head = curr;
        curr->left = prev;
        if (prev) 
            prev->right = curr;
        prev = curr;

        bstToDLLR(curr->right, head, prev);
    }

    Node* kthLargestR(Node* curr, int& k) {
        if (!curr) 
            return nullptr;
        Node* right = kthLargestR(curr->right, k);
        if (right) 
            return right;
        k--;
        if (k == 0) 
            return curr;
        return kthLargestR(curr->left, k);
    }

    void storeInorder(Node* curr, vector<int>& vals) {
        if (!curr) 
            return;
        storeInorder(curr->left, vals);
        vals.push_back(curr->data);
        storeInorder(curr->right, vals);
    }
    void destroyTree(Node* curr) {
        if (!curr) 
            return;          
        destroyTree(curr->left);    
        destroyTree(curr->right);   
        delete curr;                
    }

public:
    BST() {
        root = nullptr;
    }
    ~BST() {
        destroyTree(root);
        root = nullptr;
    }


    bool insert(int val) {
        return insertR(root, val);
    }

    BST(const BST& other) {
        root = copyTree(other.root);
    }

    void print() {
        cout << "Inorder traversal: ";
        inorderPrint();
        cout << endl;
        cout << "Level order traversal: ";
        levelorderPrint();
        cout << endl;
    }

    void inorderPrint() {
        inorderR(root);
    }

    void levelorderPrint() {
        if (!root) 
            return;
        queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            Node* curr = q.front(); 
            q.pop();
            cout << curr->data << " ";
            if (curr->left) 
                q.push(curr->left);
            if (curr->right) 
                q.push(curr->right);
        }
    }

    Node* search(int key) {
        return searchR(root, key);
    }

    Node* bstToDLL() {
        Node* head = nullptr;
        Node* prev = nullptr;
        bstToDLLR(root, head, prev);
        root = nullptr;
        return head; 
    }

    int countPairs(BST& bst1, BST& bst2, int target) {
        vector<int> vals1, vals2;
        bst1.storeInorder(bst1.root, vals1);
        bst2.storeInorder(bst2.root, vals2);

        int i = 0;
        int j = vals2.size() - 1;
        int count = 0;

        while (i < vals1.size() && j >= 0) {
            int sum = vals1[i] + vals2[j];
            if (sum == target) {
                count++; 
                i++; 
                j--;
            }
            else if (sum < target)
                i++;
            else
                j--;
        }
        return count;
    }

    int kthLargest(int k) {
        Node* result = kthLargestR(root, k);
        if (result)
            return result->data;
        return 0; //in case if k is greater than BST
    }
};

int main() {
    BST t1;
    t1.insert(10);
    t1.insert(5);
    t1.insert(15);
    t1.insert(2);
    t1.insert(7);

    cout << "Tree 1:" << endl;
    t1.print();

    BST t2;
    t2.insert(8);
    t2.insert(3);
    t2.insert(12);
    t2.insert(6);
    t2.insert(14);

    cout << "\nTree 2:" << endl;
    t2.print();

    cout << "\nSearch 7 in Tree 1: ";
    if (t1.search(7))
        cout << "Found" << endl;
    else        
        cout << "Not Found" << endl;

    cout << "\n3rd Largest Element in Tree 1: " << t1.kthLargest(3) << endl;

    cout << "\nCount of pairs with sum 20 from Tree1 & Tree2: " << t1.countPairs(t1, t2, 20) << endl;

    cout << "\nBST to Doubly Linked List (t1): ";
    Node* head = t1.bstToDLL();
    for (Node* curr = head; curr != nullptr; curr = curr->right)
        cout << curr->data << " ";
    cout << endl;

    return 0;
}
