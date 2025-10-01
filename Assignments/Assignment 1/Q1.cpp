#include <iostream>
#include <vector>
using namespace std;

class CList {
    class Node {
    public:
        Node* next;
        int compNum;
        int passCount;
        Node(int compNum1 = 0, int c = 0) {
            compNum = compNum1;
            passCount = c;
            next = nullptr;
        }
    };

    Node* head;
    Node* tail;

public:
    CList() {
        head = nullptr;
        tail = nullptr;
    }

    void insertNewComps(int UcompNum, int pCount) {
        Node* newNode = new Node(UcompNum, pCount);
        if (head == nullptr) {
            head = newNode;
            tail = head;
            head->next = head; 
        }
        else {
            tail->next = newNode;
            tail = newNode;
            tail->next = head; 
        }
    }

    void passengerExit(vector<int>& exits) {
        if (head == nullptr)
            return;
        Node* move = head;
        int i = 0;
        do {
            move->passCount = move->passCount - exits[i];
            if (move->passCount < 0) 
                move->passCount = 0; 
            move = move->next;
            i++;
        } while (move != head && i < exits.size());
    }

    void redistribute(Node* start) {
        Node* move = start;
        int extra = 0;
        do {
            if (move->passCount > 10) {
                extra = move->passCount - 10;
                move->passCount = 10;
                move = move->next;
                move->passCount = move->passCount + extra;
            }
            else {
                move = move->next;
            }
        } while (move != start);

        if (start->passCount > 10) {
            cout << start->passCount - 10 << " extra people were added" << endl;
            start->passCount = 10;
        }
    }

    void mergeRule() {
        if (head == nullptr)
            return;
        Node* move = head;
        Node* prev = tail;
        do {
            if (move->passCount < 3) {
                Node* nextComp = move->next;
                nextComp->passCount = nextComp->passCount + move->passCount;
                prev->next = nextComp;

                if (move == head) 
                    head = nextComp;
                if (move == tail) 
                    tail = prev;

                delete move; 
                move = nextComp;
                redistribute(move);
            }
            else {
                prev = move;
                move = move->next;
            }
        } while (move != head);
    }

    void display() {
        if (head == nullptr) {
            cout << "No compartments!" << endl;
        }
        else {
            Node* move = head;
            cout << "Final Train State:" << endl;
            do {
                cout << "Compartment " << move->compNum << " -> Passengers: " << move->passCount << endl;
                move = move->next;
            } while (move != head);
        }
    }
};

int main() {
    vector<int> initial = { 5, 2, 8, 10, 4 };
    CList train;
    for (int i = 0; i < initial.size(); i++) {
        train.insertNewComps(100 + i, initial[i]);
    }

    vector<int> exits = { 1, 1, 3, 0, 2 };
    train.passengerExit(exits);

    train.mergeRule();

    train.display();

    return 0;
}
