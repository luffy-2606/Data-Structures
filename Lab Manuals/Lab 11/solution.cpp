#include <iostream>
#include <string>
using namespace std;

class Warrior {
public:
    int id;
    string name;
    int reiatsu;
    Warrior() {
        id = -1;
        name = "";
        reiatsu = 0;
    }
    Warrior(int i, string n, int r) {
        id = i;
        name = n;
        reiatsu = r;
    }

    bool operator>(const Warrior& other) const {
        return this->reiatsu > other.reiatsu;
    }

    bool operator<(const Warrior& other) const {
        return this->reiatsu < other.reiatsu;
    }

    void print() {
        cout << id << ": " << name << " - Reiatsu: " << reiatsu;
    }
};

template <typename T>
class Heap {
private:
    T* arr;
    int capacity;
    int size;

    void resize() {
        int newCap = capacity * 2;
        T* newArr = new T[newCap];
        for (int i = 0; i < size; i++) 
            newArr[i] = arr[i];
        delete[] arr;
        arr = newArr;
        capacity = newCap;
    }

    void reheapUp(int index) {
        if (index == 0) 
            return;
        int parent = (index - 1) / 2;

        if (arr[index] > arr[parent]) {
            T temp = arr[index];
            arr[index] = arr[parent];
            arr[parent] = temp;
            reheapUp(parent);
        }
    }

    void reheapDown(int index) {
        int left = index * 2 + 1;
        int right = index * 2 + 2;

        if (left >= size) 
            return;

        int maxChild = left;

        if (right < size && arr[right] > arr[left])
            maxChild = right;

        if (arr[maxChild] > arr[index]) {
            T temp = arr[index];
            arr[index] = arr[maxChild];
            arr[maxChild] = temp;
            reheapDown(maxChild);
        }
    }

public:
    Heap(int cap = 50) {
        capacity = cap;
        size = 0;
        arr = new T[capacity];
    }

    ~Heap() {
        delete[] arr;
    }

    void insert(const T& value) {
        if (size == capacity)
            resize();

        arr[size] = value;
        reheapUp(size);
        size++;
    }

    bool removeTop(T& removed) {
        if (size == 0) 
            return false;

        removed = arr[0];
        arr[0] = arr[size - 1];
        size--;
        reheapDown(0);
        return true;
    }

    void display(int index = 0) {
        if (index >= size) 
            return;

        arr[index].print();
        cout << endl;

        display(index * 2 + 1);
        display(index * 2 + 2);
    }

    void clear() {
        size = 0;
    }

    int count() { 
        return size; 
    }

    bool increaseReiatsu(int id, int increaseBy) {
        for (int i = 0; i < size; i++) {
            if (arr[i].id == id) {
                arr[i].reiatsu += increaseBy;
                reheapUp(i);
                return true;
            }
        }
        return false;
    }

    void heapSort() {
        int originalSize = size;

        T* copy = new T[size];
        for (int i = 0; i < size; i++) 
            copy[i] = arr[i];

        cout << "\nSorted Warriors (Descending by Reiatsu):\n";
        while (size > 0) {
            T temp;
            removeTop(temp);
            temp.print();
            cout << endl;
        }

        for (int i = 0; i < originalSize; i++) 
            arr[i] = copy[i];
        size = originalSize;
        delete[] copy;
    }
};

class Node {
public:
    char ch;
    int freq;
    Node* left, * right;
    bool used;

    Node() {
        ch = '\0';
        freq = 0;
        left = nullptr;
        right = nullptr;
        used = false;
    }
};

void buildFrequency(const string& str, int freq[256]) {
    for (int i = 0; i < 256; i++) 
        freq[i] = 0;
    for (int i = 0; i < str.length(); i++)
        freq[str[i]]++;
}

void pickTwoSmallest(Node nodes[], int count, int& x, int& y) {
    x = -1;
    y = -1;

    for (int i = 0; i < count; i++) {
        if (!nodes[i].used && nodes[i].freq > 0) {
            if (x == -1 || nodes[i].freq < nodes[x].freq) 
                x = i;
        }
    }

    nodes[x].used = true;

    for (int i = 0; i < count; i++) {
        if (!nodes[i].used && nodes[i].freq > 0) {
            if (y == -1 || nodes[i].freq < nodes[y].freq) 
                y = i;
        }
    }

    nodes[x].used = false;
}

Node* buildHuffmanTree(int freq[256]) {
    Node nodes[256];
    int total = 0;

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            nodes[total].ch = i;
            nodes[total].freq = freq[i];
            nodes[total].used = false;
            total++;
        }
    }

    int n = total;

    while (n > 1) {
        int a, b;
        pickTwoSmallest(nodes, total, a, b);

        Node* parent = new Node();
        parent->ch = '\0';
        parent->freq = nodes[a].freq + nodes[b].freq;
        parent->left = new Node(nodes[a]);
        parent->right = new Node(nodes[b]);

        nodes[a] = *parent;
        nodes[b].freq = 0; 
        nodes[b].used = true;

        n--;
    }

    for (int i = 0; i < total; i++)
        if (!nodes[i].used && nodes[i].freq > 0)
            return new Node(nodes[i]);

    return nullptr;
}

void generateCodes(Node* root, string code, string codes[256]) {
    if (!root) 
        return;

    if (root->ch != '\0') {
        codes[root->ch] = code;
        return;
    }

    generateCodes(root->left, code + "0", codes);
    generateCodes(root->right, code + "1", codes);
}

string encode(const string& str, string codes[256]) {
    string out = "";
    for (int i = 0; i < str.length(); i++)
        out = out + codes[str[i]];
    return out;
}

string decode(const string& bits, Node* root) {
    string output = "";
    Node* curr = root;

    for (int i = 0; i < bits.length(); i++) {
        if (bits[i] == '0') 
            curr = curr->left;
        else curr = curr->right;

        if (curr->left == nullptr && curr->right == nullptr) {
            output = output + curr->ch;
            curr = root;
        }
    }
    return output;
}

int main() {
    Heap<Warrior> heap;

    Warrior w1(1, "Ichigo", 500);
    Warrior w2(2, "Rukia", 300);
    Warrior w3(3, "Aizen", 900);
    Warrior w4(4, "Kenpachi", 850);
    Warrior w5(5, "Ulquiorra", 700);

    heap.insert(w1);
    heap.insert(w2);
    heap.insert(w3);
    heap.insert(w4);
    heap.insert(w5);

    cout << "Heap After All Insertions:\n";
    heap.display();

    cout << "\nIncreasing Reiatsu of Rukia (ID=2) by 400...\n";
    heap.increaseReiatsu(2, 400);

    cout << "\nHeap After Increasing Rukia's Reiatsu:\n";
    heap.display();

    Warrior removed;
    cout << "\nRemoving Top Warrior...\n";
    heap.removeTop(removed);

    cout << "Removed Warrior: ";
    removed.print();
    cout << endl;

    cout << "\nHeap After Removing Top Warrior:\n";
    heap.display();

    heap.heapSort();

    heap.clear();
    cout << "Warrior Count After Clearing: " << heap.count() << endl;

    // =============== Q2 ===============
    string input = "COMPRESSION_IS_COOL";
    cout << "\n\n===Q2===\n\nOriginal: " << input << endl;

    int freq[256];
    buildFrequency(input, freq);

    Node* root = buildHuffmanTree(freq);

    string codes[256];
    for (int i = 0; i < 256; i++) codes[i] = "";

    generateCodes(root, "", codes);

    cout << "\nCodes:\n";
    for (int i = 0; i < 256; i++)
        if (freq[i] > 0)
            cout << (char)i << " : " << codes[i] << endl;

    string encoded = encode(input, codes);
    cout << "\nEncoded:\n" << encoded << endl;

    string decoded = decode(encoded, root);
    cout << "\nDecoded:\n" << decoded << endl;

    return 0;
}
