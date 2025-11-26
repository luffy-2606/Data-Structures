#include <iostream>
using namespace std;

class patient {
public:
   string name;
   int age;
   string condition;
   int priority;

   patient(string str = "", int a = 0, string con = "", string ref = "") {
       if (str == "" || con == "") {
           cout<< "Incomplete Information!"<<endl;
           return;
       }

       name = str;
       age = a;
       condition = con;

       // Priority derived from condition
       if (condition == "Normal")
           priority = 3;
       else if (condition == "Critical")
           priority = 2;
       else if (condition == "Intense")
           priority = 1;
       else {
           cout << "Improper derived priority (invalid condition)" << endl;
           return;
       }

       // Strong reference priority
       if (ref == "strong")
           priority = 0;

       // Senior citizen priority
       if (age > 50)
           priority = 0;
   }

   patient(const patient& obj) {
       name = obj.name;
       age = obj.age;
       condition = obj.condition;
       priority = obj.priority;
   }

   void display() const {
       cout << "Name: " << name << ", Age: " << age << ", Condition: " << condition << ", Priority: " << priority << endl;
   }
};

class queue {
   class Node {
   public:
       patient data;
       Node* next;
       Node(patient d) : data(d), next(nullptr) {}
   };

   Node* front;
   Node* rear;

public:
   queue() {
       front = nullptr;
       rear = nullptr;
   }

   void enqueue(patient d) {
       Node* newNode = new Node(d);

       if (front == nullptr) {
           front = rear = newNode;
           return;
       }

       if (d.priority < front->data.priority) {
           newNode->next = front;
           front = newNode;
           return;
       }

       Node* temp = front;
       while (temp->next != nullptr &&
           temp->next->data.priority <= d.priority) {
           temp = temp->next;
       }

       newNode->next = temp->next;
       temp->next = newNode;

       if (temp == rear)
           rear = newNode;
   }

   void update(string name, string newCondition) {
       Node* temp = front;
       while (temp != nullptr) {
           if (temp->data.name == name) {
               temp->data.condition = newCondition;

               if (newCondition == "Normal")
                   temp->data.priority = 3;
               else if (newCondition == "Critical")
                   temp->data.priority = 2;
               else if (newCondition == "Intense")
                   temp->data.priority = 1;
               else {
                   cout << "Invalid condition while updating!" << endl;
                   return;
               }

               patient updated = temp->data;
               remove(name);
               enqueue(updated);

               return;
           }
           temp = temp->next;
       }
       cout << "Patient not found!" << endl;
   }

   void remove(string name) {
       if (front == nullptr) return;

       if (front->data.name == name) {
           Node* del = front;
           front = front->next;
           delete del;
           return;
       }

       Node* temp = front;
       while (temp->next != nullptr && temp->next->data.name != name) {
           temp = temp->next;
       }

       if (temp->next != nullptr) {
           Node* del = temp->next;
           temp->next = del->next;
           if (del == rear)
               rear = temp;
           delete del;
       }
   }

   void display() {
       Node* temp = front;
       while (temp != nullptr) {
           temp->data.display();
           temp = temp->next;
       }
   }
};

int main() {
   queue q;

   patient p1("Ali", 21, "Normal");
   patient p2("Ahmad", 29, "Critical");
   patient p3("Alex", 88, "Normal");  // senior → priority 0
   patient p4("Khan", 88, "Normal");  // senior
   patient p5("Mark", 73, "Intense"); // senior
   patient p6("Max", 13, "Critical");

   q.enqueue(p1);
   q.enqueue(p2);
   q.enqueue(p3);
   q.enqueue(p4);
   q.enqueue(p5);
   q.enqueue(p6);

   q.display();

   cout << "\nUpdating Ahmad to Intense"<<endl;
   q.update("Ahmad", "Intense");

   q.display();

   return 0;
}
