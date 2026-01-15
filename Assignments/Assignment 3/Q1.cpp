#include <iostream>
#include <string>
using namespace std;

const int P = 31;
const int M = 1000000009;

struct Node {
   string username;
   int score;
   Node* next;
};

class HashTable {
private:
   int tableSize;
   Node** table;

   int hashFunction(string s) {
       int hashValue = 0;
       int pPow = 1;

       for (int i = 0; i < s.length(); i++) {
           hashValue = (hashValue + (s[i] * pPow) % tableSize) % tableSize;
           pPow = (pPow * P) % tableSize;
       }

       return hashValue % tableSize;
   }

public:
   HashTable(int size) {
       tableSize = size;
       table = new Node * [tableSize];

       for (int i = 0; i < tableSize; i++) {
           table[i] = NULL;
       }
   }

   void insertPlayer(string username, int score) {
       int index = hashFunction(username);
       Node* head = table[index];

       Node* temp = head;
       while (temp != NULL) {
           if (temp->username == username) {
               temp->score = score;
               return;
           }
           temp = temp->next;
       }

       // Insert new node at head
       Node* newNode = new Node();
       newNode->username = username;
       newNode->score = score;
       newNode->next = head;
       table[index] = newNode;
   }

   string searchPlayer(string username) {
       int index = hashFunction(username);
       Node* temp = table[index];

       while (temp != NULL) {
           if (temp->username == username) {
               return to_string(temp->score);
           }
           temp = temp->next;
       }
       return "Not Found";
   }

   void deletePlayer(string username) {
       int index = hashFunction(username);
       Node* temp = table[index];
       Node* prev = NULL;

       while (temp != NULL) {
           if (temp->username == username) {
               if (prev == NULL) {
                   table[index] = temp->next;
               }
               else {
                   prev->next = temp->next;
               }
               delete temp;
               return;
           }
           prev = temp;
           temp = temp->next;
       }
   }
};

int main() {
   int tableSize = 7;
   HashTable ht(tableSize);

   // Insert given players
   ht.insertPlayer("Alice", 1200);
   ht.insertPlayer("Bob", 950);
   ht.insertPlayer("Charlie", 1500);
   ht.insertPlayer("David", 800);
   ht.insertPlayer("Eve", 1100);

   // Queries
   string queries[] = { "Charlie", "Eve", "Frank", "Alice" };
   int qSize = 4;

   for (int i = 0; i < qSize; i++) {
       string result = ht.searchPlayer(queries[i]);
       cout << queries[i] << " -> " << result << endl;
   }

   return 0;
}
