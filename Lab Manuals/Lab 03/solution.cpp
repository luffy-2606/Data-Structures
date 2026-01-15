#include <iostream>
#include <string>
using namespace std;

template <typename T>
class List {
    class Node {
    public:
        T data;
        Node* next;
        Node(T d) : data(d), next(nullptr) {}
        friend class List<T>;
    };

    Node* head;
    Node* tail;

public:
    List() : head(nullptr), tail(nullptr) {}

    List(const List<T>& obj) : head(nullptr), tail(nullptr) {
        Node* move = obj.head;
        while (move != nullptr) {
            insertTail(move->data);
            move = move->next;
        }
    }

    ~List() {
        while (head != nullptr) {
            Node* del = head;
            head = head->next;
            delete del;
        }
        tail = nullptr;
    }

    void insertHead(const T& d) {
        Node* newNode = new Node(d);
        if (!head) {
            head = tail = newNode;
        }
        else {
            newNode->next = head;
            head = newNode;
        }
    }

    void insertTail(const T& d) {
        Node* newNode = new Node(d);
        if (!head) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    void insert(const T& d, int index) {
        if (index < 0) {
            cout << "Index cannot be less than 0" << endl;
            return;
        }
        if (index == 0) {
            insertHead(d);
            return;
        }
        Node* newNode = new Node(d);
        Node* move = head;
        Node* prev = nullptr;
        int i = 0;
        while (move && i < index) {
            prev = move;
            move = move->next;
            i++;
        }
        if (i != index) {
            cout << "Index is out of bounds" << endl;
            delete newNode;
            return;
        }
        prev->next = newNode;
        newNode->next = move;
        if (!newNode->next)
            tail = newNode;
    }

    void removeHead() {
        if (!head) {
            cout << "List is empty" << endl;
            return;
        }
        Node* del = head;
        head = head->next;
        delete del;
        if (!head)
            tail = nullptr;
    }

    void removeTail() {
        if (!head) {
            cout << "List is empty" << endl;
            return;
        }
        if (head == tail) {
            delete tail;
            head = tail = nullptr;
            return;
        }
        Node* temp = head;
        while (temp->next != tail) {
            temp = temp->next;
        }
        delete tail;
        tail = temp;
        tail->next = nullptr;
    }

    void remove(int index) {
        if (index < 0) {
            cout << "Index cannot be less than 0" << endl;
            return;
        }
        if (index == 0) {
            removeHead();
            return;
        }
        Node* move = head;
        Node* prev = nullptr;
        int i = 0;
        while (move && i < index) {
            prev = move;
            move = move->next;
            i++;
        }
        if (!move) {
            cout << "Index is out of bounds" << endl;
            return;
        }
        prev->next = move->next;
        if (move == tail)
            tail = prev;
        delete move;
    }

    int getSize() const {
        int count = 0;
        Node* temp = head;
        while (temp) {
            count++;
            temp = temp->next;
        }
        return count;
    }

    T& operator[](int index) const {
        if (index < 0 || index >= getSize()) throw out_of_range("Index out of range");
        Node* current = head;
        for (int i = 0; i < index; i++) current = current->next;
        return current->data;
    }

    List<List<T>> separateEquals() const {
        List<List<T>> result;

        for (Node* current = head; current; current = current->next) {
            T val = current->data;
            bool alreadyGrouped = false;

            for (int i = 0; i < result.getSize(); i++) {
                if (result[i].getSize() > 0 && result[i][0] == val) {
                    alreadyGrouped = true;
                    break;
                }
            }

            if (!alreadyGrouped) {
                List<T> group;
                for (Node* scan = head; scan; scan = scan->next) {
                    if (scan->data == val) {
                        group.insertTail(scan->data);
                    }
                }
                result.insertTail(group);
            }
        }

        return result;
    }

    void rotateRight(int k) {
        int size = getSize();
        if (size <= 1 || k % size == 0)
            return;
        k = k % size;
        int split = size - k;
        Node* temp = head;
        for (int i = 1; i < split; i++)
            temp = temp->next;
        Node* newHead = temp->next;
        temp->next = nullptr;
        tail->next = head;
        head = newHead;
        temp = head;
        while (temp->next)
            temp = temp->next;
        tail = temp;
    }

    void rotateLeft(int k) {
        int size = getSize();
        if (size <= 1 || k % size == 0)
            return;
        k = k % size;
        Node* temp = head;
        for (int i = 1; i < k; i++)
            temp = temp->next;
        Node* newHead = temp->next;
        temp->next = nullptr;
        tail->next = head;
        head = newHead;
        temp = head;
        while (temp->next)
            temp = temp->next;
        tail = temp;
    }

    void reverse() {
        Node* prev = nullptr;
        Node* curr = head;
        tail = head;
        while (curr) {
            Node* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        head = prev;
    }

    void print() const {
        Node* temp = head;
        cout << "[";
        while (temp) {
            cout << temp->data;
            if (temp->next)
                cout << ", ";
            temp = temp->next;
        }
        cout << "]" << endl;
    }

    Node* getHead() const {
        return head;
    }
};

template <typename T>
class CList {
    class Node {
    public:
        T data;
        Node* next;
        Node(T d) : data(d), next(nullptr) {}
        friend class List<T>;
    };

    Node* head;
    Node* tail;

public:
    CList() : head(nullptr), tail(nullptr) {}

    CList(const List<T>& obj) : head(nullptr), tail(nullptr) {
        if (!obj.head) return;
        Node* move = obj.head;
        do {
            insertTail(move->data);
            move = move->next;
        } while (move != obj.head);
    }

    ~CList() {
        if (!head) return;
        tail->next = nullptr;
        while (head) {
            Node* del = head;
            head = head->next;
            delete del;
        }
        tail = nullptr;
    }

    void insertHead(const T& d) {
        Node* newNode = new Node(d);
        if (!head) {
            head = tail = newNode;
            tail->next = head;
        }
        else {
            newNode->next = head;
            head = newNode;
            tail->next = head;
        }
    }

    void insertTail(const T& d) {
        Node* newNode = new Node(d);
        if (!head) {
            head = tail = newNode;
            tail->next = head;
        }
        else {
            tail->next = newNode;
            tail = newNode;
            tail->next = head;
        }
    }

    void insert(const T& d, int index) {
        if (index < 0) {
            cout << "Index cannot be less than 0" << endl;
            return;
        }
        if (index == 0) {
            insertHead(d);
            return;
        }
        Node* newNode = new Node(d);
        Node* move = head;
        Node* prev = nullptr;
        int i = 0;
        do {
            if (i == index) break;
            prev = move;
            move = move->next;
            i++;
        } while (move != head);

        if (i != index) {
            cout << "Index is out of bounds" << endl;
            delete newNode;
            return;
        }
        prev->next = newNode;
        newNode->next = move;
        if (prev == tail) tail = newNode;
        tail->next = head;
    }

    void removeHead() {
        if (!head) {
            cout << "List is empty" << endl;
            return;
        }
        if (head == tail) {
            delete head;
            head = tail = nullptr;
            return;
        }
        Node* del = head;
        head = head->next;
        tail->next = head;
        delete del;
    }

    void removeTail() {
        if (!head) {
            cout << "List is empty" << endl;
            return;
        }
        if (head == tail) {
            delete tail;
            head = tail = nullptr;
            return;
        }
        Node* temp = head;
        while (temp->next != tail) {
            temp = temp->next;
        }
        delete tail;
        tail = temp;
        tail->next = head;
    }

    void remove(int index) {
        if (index < 0) {
            cout << "Index cannot be less than 0" << endl;
            return;
        }
        if (index == 0) {
            removeHead();
            return;
        }
        Node* move = head;
        Node* prev = nullptr;
        int i = 0;
        do {
            if (i == index) break;
            prev = move;
            move = move->next;
            i++;
        } while (move != head);

        if (i != index || move == head) {
            cout << "Index is out of bounds" << endl;
            return;
        }
        prev->next = move->next;
        if (move == tail) tail = prev;
        tail->next = head;
        delete move;
    }

    void rotateLeft(int k) {
        int size = getSize();
        if (size <= 1 || k % size == 0) 
            return;
        k = k % size;
        for (int i = 0; i < k; i++) {
            head = head->next;
            tail = tail->next;
        }
    }

    void rotateRight(int k) {
        int size = getSize();
        if (size <= 1 || k % size == 0) 
            return;
        k = k % size;
        int steps = size - k;
        for (int i = 0; i < steps; i++) {
            head = head->next;
            tail = tail->next;
        }
    }

    int getSize() const {
        if (!head) return 0;
        int count = 0;
        Node* temp = head;
        do {
            count++;
            temp = temp->next;
        } while (temp != head);
        return count;
    }
    T& operator[](int index) const {
        if (index < 0 || index >= getSize()) throw out_of_range("Index out of range");
        Node* current = head;
        for (int i = 0; i < index; i++) current = current->next;
        return current->data;
    }

    void reverse() {
        if (!head || head == tail) return;
        Node* prev = tail;
        Node* curr = head;
        do {
            Node* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        } while (curr != head);
        tail = head;
        head = prev;
    }

    void print() const {
        if (!head) {
            cout << "[]\n";
            return;
        }
        Node* temp = head;
        cout << "[";
        do {
            cout << temp->data;
            temp = temp->next;
            if (temp != head) cout << ", ";
        } while (temp != head);
        cout << "]\n";
    }

    Node* getHead() const {
        return head;
    }

    CList<CList<T>> separateEquals() const {
        CList<CList<T>> result;

        for (Node* current = head; current != tail; current = current->next) {
            T val = current->data;
            bool alreadyGrouped = false;

            for (int i = 0; i < result.getSize(); i++) {
                if (result[i].getSize() > 0 && result[i][0] == val) {
                    alreadyGrouped = true;
                    break;
                }
            }

            if (!alreadyGrouped) {
                CList<T> group;
                for (Node* scan = head; scan!=tail; scan = scan->next) {
                    if (scan->data == val) {
                        group.insertTail(scan->data);
                    }
                }
                result.insertTail(group);
            }
        }

        return result;
    }

};


template <typename T>
class CircularQueue {
private:
    CList<T> queue;

public:

    void push_back(const T& val) {
        queue.insertTail(val);
    }

    const T& pop_front() {
        if (queue.getSize() == 0) {
            throw out_of_range("Queue is empty");
        }
        T frontVal = queue[0];
        queue.removeHead();
        return frontVal;
    }

    T& get_front() const {
        if (queue.getSize() == 0) {
            throw out_of_range("Queue is empty");
        }

        return const_cast<T&>(queue[0]);
    }

    void next() {
        queue.rotateLeft(1);
    }

    void print() const {
        queue.print();
    }

    int size() const {
        return queue.getSize();
    }

    bool empty() const {
        return queue.getSize() == 0;
    }
};

int main() {

    //Circular Linked List
    CList<int> lst;
    lst.insertHead(3);
    lst.insertTail(5);
    lst.insertTail(3);
    lst.insertTail(3);
    lst.insertTail(5);
    lst.insertTail(1);

    cout << "Original Circular list: ";
    lst.print();

    lst.remove(2);
    cout << "After removing index 2: ";
    lst.print();

    lst.rotateRight(2);
    cout << "After rotating right by 2: ";
    lst.print();

    lst.rotateLeft(3);
    cout << "After rotating left by 3: ";
    lst.print();

    lst.reverse();
    cout << "After reversing: ";
    lst.print();

    cout << "Element at index 1: " << lst[1] << endl;


    //Singly Linked List
    List<int> lst1;
    lst1.insertTail(1);
    lst1.insertTail(1);
    lst1.insertTail(2);
    lst1.insertTail(2);
    lst1.insertTail(2);

    List<List<int>> equals = lst1.separateEquals();
    cout << "Separate equals:" << endl;

    cout << endl << endl;

    auto outer = equals.getHead();
    while (outer) {
        outer->data.print();
        outer = outer->next;
    }
    cout << endl << endl;

    //Circular Queue
    CircularQueue<int> cq;
    cq.push_back(10);
    cq.push_back(20);
    cq.push_back(30);

    cout << "Queue: ";
    cq.print(); 

    cq.next(); 
    cout << "After next(): ";
    cq.print(); 

    cout << "Front: " << cq.get_front(); 

    cq.pop_front();
    cout << "After pop_front(): ";
    cq.print(); 


    return 0;
}
