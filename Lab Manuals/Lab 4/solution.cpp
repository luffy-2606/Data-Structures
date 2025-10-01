#include <iostream>
using namespace std;

template <typename T>
class DoubleList {
    class Node {
    public:
        T data;
        Node* forward;
        Node* backward;
        Node(const T& d) {
            data = d;
            forward = nullptr;
            backward = nullptr;
        }
    };
    Node* head;
    Node* tail;
public:
    DoubleList() {
        head = nullptr;
        tail = nullptr;
    }
    DoubleList(const DoubleList<T>& obj) {
        Node* move = obj.head;
        while (move != nullptr) {
            insertTail(move->data);
            move = move->forward;
        }
    }
    ~DoubleList() {
        while (head != nullptr) {
            Node* del = head;
            head = head->forward;
            delete del;
        }
        
        tail = nullptr;
    }
    DoubleList<T>& operator=(const DoubleList<T>& obj) {
        if (this != &obj) {
            // Clear current list
            while (head != nullptr) {
                Node* del = head;
                head = head->forward;
                delete del;
            }
            tail = nullptr;

            // Deep copy from obj
            Node* move = obj.head;
            while (move != nullptr) {
                insertTail(move->data);
                move = move->forward;
            }
        }
        return *this;
    }
    void insertHead(const T& d) {
        Node* newNode = new Node(d);
        if (head == nullptr) {
            head = newNode;
            tail = head;
        }
        else {
            head->backward = newNode;
            newNode->forward = head;
            head = newNode;
        }
    }

    void insertTail(const T& d) {
        Node* newNode = new Node(d);
        if (tail == nullptr) {
            tail = newNode;
            head = tail;
        }
        else {
            tail->forward = newNode;
            newNode->backward = tail;
            tail = newNode;
        }
    }

    void insert(const T& d, int index) {
        if (index < 0) {
            cout << "Index can not be negative\n";
            return;
        }
        else if (index > size()) {
            cout << "Index can not be greater than size!!\n";
            return;
        }
        else if (index == 0) {
            insertHead(d);
        }
        else if (index == size()) {
            insertTail(d);
        }
        else {
            Node* newNode = new Node(d);
            Node* move = head;
            int i = 0;
            while (move != nullptr && i < index) {
                move = move->forward;
                i++;
            }
            move->backward->forward = newNode;
            newNode->forward = move;
            newNode->backward = move->backward;
            move->backward = newNode;
        }
    }

    void removeHead() {
        if (head == nullptr)
            return;
        Node* del = head;
        head = head->forward;
        if (head != nullptr)
            head->backward = nullptr;
        else
            tail = nullptr;
        delete del;
    }

    void removeTail() {
        if (tail == nullptr)
            return;
        Node* del = tail;
        tail = tail->backward;
        if (tail != nullptr)
            tail->forward = nullptr;
        else
            head = nullptr;
        delete del;
    }

    void remove(int index) {
        if (index < 0) {
            cout << "Index can not be negative\n";
            return;
        }
        else if (index >= size()) {
            cout << "Index can not be greater than or equal to size!!\n";
            return;
        }
        else if (index == 0) {
            removeHead();
        }
        else if (index == size() - 1) {
            removeTail();
        }
        else {
            Node* move = head;
            int i = 0;
            while (move != nullptr && i < index) {
                move = move->forward;
                i++;
            }
            Node* del = move;
            move->backward->forward = move->forward;
            move->forward->backward = move->backward;
            delete del;
        }
    }

    int size() const {
        Node* move = head;
        int s = 0;
        while (move != nullptr) {
            s++;
            move = move->forward;
        }
        return s;
    }

    void print() const {
        Node* move = head;
        cout << "[";
        while (move != nullptr) {
            cout << move->data << " ";
            move = move->forward;
        }
        cout << "]" << endl;
    }

private:
    class Iterator {
        Node* node;
    public:
        Iterator(Node* n) : node(n) {}
        T& operator*() {
            return node->data;
        }
        DoubleList<T>::Iterator& operator++() {
            if (node) node = node->forward;
            return *this;
        }
        DoubleList<T>::Iterator operator++(int) {
            Iterator temp = *this;
            if (node) node = node->forward;
            return temp;
        }
        DoubleList<T>::Iterator& operator--() {
            if (node) node = node->backward;
            return *this;
        }
        DoubleList<T>::Iterator operator--(int) {
            Iterator temp = *this;
            if (node) node = node->backward;
            return temp;
        }
        bool operator != (const Iterator& obj) const {
            return node != obj.node;
        }
        bool operator == (const Iterator& obj) const {
            return node == obj.node;
        }
    };

public:
    DoubleList<T>::Iterator begin() const {
        return Iterator(head);
    }
    DoubleList<T>::Iterator end() const {
        return Iterator(nullptr);
    }
    DoubleList<T>::Iterator rbegin() const {
        return Iterator(tail);
    }
    DoubleList<T>::Iterator rend() const {
        return Iterator(nullptr);
    }
};

template <typename T>
void printList(const DoubleList<T>& list) {
    cout << "[";
    for (auto it = list.begin(); it != list.end(); it++) {
        cout << *it << " ";
    }
    cout << "]" << endl;
}

template <typename T>
T max(DoubleList<T>& list) {
    auto it = list.begin();
    if (it == list.end()) {
        throw runtime_error("List is empty");
    }
    T maxVal = *it;
    for (; it != list.end(); it++) {
        if (maxVal < *it) {
            maxVal = *it;
        }
    }
    return maxVal;
}

template <typename T>
void bubbleSort(DoubleList<T>& list) {
    bool swapped = false;
    int n = list.size();
    if (n <= 1) return;
    for (int i = 0; i < n - 1; ++i) {
        auto it1 = list.begin();
        auto it2 = list.begin();
        ++it2;
        swapped = false;
        for (int j = 0; j < n - i - 1; j++, ++it1, ++it2) {
            if (*it1 > *it2) {
                swap(*it1, *it2);
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
}

template <typename T>
DoubleList<DoubleList<T>> calculateTranspose(DoubleList<DoubleList<T>>& matrix) {
    int rows = matrix.size();
    int cols = 0;
    if (matrix.begin() != matrix.end()) {
        auto& firstRow = *matrix.begin(); 
        cols = firstRow.size();
    }
    DoubleList<DoubleList<T>> result;
    for (int i = 0; i < cols; ++i) {
        DoubleList<T> row;
        for (auto it = matrix.begin(); it != matrix.end(); it++) {
            auto& inner = *it;  
            auto innerIt = inner.begin();
            for (int j = 0; j < i; j++)
                ++innerIt;
            row.insertTail(*innerIt);
        }
        result.insertTail(row);
    }
    return result;
}

int main() {
    DoubleList<int> DLL;
    DLL.insertHead(1);
    DLL.insertHead(2);
    DLL.insertHead(3);
    DLL.print();
    cout << endl;
    DLL.insertTail(4);
    DLL.insertTail(5);
    DLL.insertTail(6);
    DLL.print();
    cout << endl;
    DLL.removeHead();
    DLL.removeHead();
    DLL.removeHead();
    DLL.print();
    cout << endl;
    DLL.insert(1, 0);
    DLL.insert(2, 1);
    DLL.insert(5, 2);
    DLL.print();
    cout << "Size of List: " << DLL.size() << endl;
    cout << endl;
    bubbleSort(DLL);
    cout << "After Bubble Sort:" << endl;
    DLL.print();

    cout << "Max element in DLL: " << max(DLL) << endl;


    DoubleList<int> row1;
    DoubleList<int> row2;
    DoubleList<int> row3;
    row1.insertTail(1);
    row1.insertTail(2);
    row1.insertTail(3);
    row2.insertTail(4);
    row2.insertTail(5);
    row2.insertTail(6);
    row3.insertTail(7);
    row3.insertTail(8);
    row3.insertTail(9);
    DoubleList<DoubleList<int>> matrix;
    matrix.insertTail(row1);
    matrix.insertTail(row2);
    matrix.insertTail(row3);

    DoubleList<DoubleList<int>> transposed = calculateTranspose(matrix);

    cout << endl<<"After Transpose:"<<endl;
    for (auto it = transposed.begin(); it != transposed.end(); ++it) {
        (*it).print();
    }

    return 0;
}
