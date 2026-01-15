#include <iostream>
using namespace std;

class HashTable;

class HashItem {
    friend class HashTable;
private:
    int key;       // key
    string value;  // value
    int status;    // 0: Empty, 1: Deleted, 2: Occupied
public:
    HashItem() {
        key = -1;
        value = "";
        status = 0; 
    }
};

class HashTable {
private:
    HashItem* hashArray;
    int capacity;
    int currentElements;

    int getNextCandidateIndex(int key, int i) {
        return (key % capacity + i) % capacity;
    }

    void doubleCapacity() {
        int oldCapacity = capacity;
        capacity *= 2;
        HashItem* oldArray = hashArray;
        hashArray = new HashItem[capacity];
        currentElements = 0;

        for (int i = 0; i < oldCapacity; i++) {
            if (oldArray[i].status == 2) {
                insert(oldArray[i].key, oldArray[i].value);
            }
        }
        delete[] oldArray;
    }

public:
    HashTable() {
        capacity = 10;
        hashArray = new HashItem[capacity];
        currentElements = 0;
    }

    HashTable(int const capacity) {
        if (capacity < 1) {
            cout << "ERROR! Capacity cannot be less than 1" << endl;
            this->capacity = 10;
        }
        else {
            this->capacity = capacity;
        }
        hashArray = new HashItem[this->capacity];
        currentElements = 0;
    }

    void insert(int const key, string const value) {
        int index = key % capacity;
        int i = 0;
        while (hashArray[index].status == 2) {
            index = getNextCandidateIndex(key, ++i);
        }
        hashArray[index].key = key;
        hashArray[index].value = value;
        hashArray[index].status = 2;
        currentElements++;

        if (currentElements >= 0.75 * capacity) {
            doubleCapacity();
        }
    }

    bool deleteKey(int const key) {
        int index = key % capacity;
        int i = 0;
        while (hashArray[index].status != 0) { 
            if (hashArray[index].status == 2 && hashArray[index].key == key) {
                hashArray[index].status = 1; 
                return true;
            }
            index = getNextCandidateIndex(key, ++i);
        }
        return false;
    }

    string get(int const key) const {
        int index = key % capacity;
        int i = 0;
        while (hashArray[index].status != 0) {
            if (hashArray[index].status == 2 && hashArray[index].key == key) {
                return hashArray[index].value;
            }
            index = (key % capacity + ++i) % capacity;
        }
        return "not found";
    }

    ~HashTable() {
        delete[] hashArray;
    }
};

int main() {
    HashTable hashtable(5);

    hashtable.insert(101, "Alice");
    hashtable.insert(201, "Bob");
    hashtable.insert(301, "Charlie");
    hashtable.insert(401, "David");
    hashtable.insert(501, "Eva");

    string name = hashtable.get(201);
    cout << "Value at key 201: " << name << endl;

    bool deleted = hashtable.deleteKey(301);
    if (deleted) {
        cout << "Key 301 deleted successfully." << endl;
    }
    else {
        cout << "Key 301 not found!" << endl;
    }

    name = hashtable.get(301);
    cout << "Value at key 301: " << name << endl;

    return 0;
}
