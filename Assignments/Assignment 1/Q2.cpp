#include <iostream>
using namespace std;

template <typename T>
class List {
    class Node {
    public:
        T data;
        Node* next;
        Node(T d) {
            data = d;
            next = nullptr;
        }
    };

    Node* head;

public:
    List() { 
        head = nullptr;
    }

    ~List() {
        while (head != nullptr) {
            Node* del = head;
            head = head->next;
            delete del;
        }
    }

    void insertNode(T d) {
        Node* newNode = new Node(d);
        if (head == nullptr) {
            head = newNode;
        }
        else {
            Node* move = head;
            while (move->next != nullptr) 
                move = move->next;
            move->next = newNode;
        }
    }

    List<List<T>*>* rearrange(int k) {
        List<List<T>*>* newList = new List<List<T>*>();  //initialize list of lists

        Node* move = head;
        while (move != nullptr) {
            List<T>* subList = new List<T>();
            int count = 0;
            while (move != nullptr && count < k) {
                subList->insertNode(move->data);
                move = move->next;
                count++;
            }
            newList->insertNode(subList);
        }
        return newList;
    }

    void display() {
        Node* move = head;
        while (move != nullptr) {
            cout << move->data;
            if (move->next != nullptr) //so that is doesnt print arow after last node
                cout << " -> ";
            move = move->next;
        }
    }

    void displayGroups() {
        Node* move = head;
        while (move != nullptr) {
            cout << "[ ";
            move->data->display();
            cout << " ]";
            if (move->next != nullptr) //so that is doesnt print arow after last node
                cout << " -> ";
            move = move->next;
        }
        cout << endl;
    }
};

int main() {
    List<int> l1;

    for (int i = 1; i <= 12; i++) 
        l1.insertNode(i);

    cout << "Original List: ";
    l1.display();

    int k = 5;
    List<List<int>*>* subL1 = l1.rearrange(k);

    cout << "\n\nNew sublists : " << endl;
    subL1->displayGroups();

    return 0;
}
