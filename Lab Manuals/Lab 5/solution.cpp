#include <iostream>
#include <string>
#include <cstring>
#include <stack>
#include <vector>
using namespace std;
template <typename T>
class LinkedList {
    class Node {
    public:
        T data;
        Node* next;

        Node(T value) : data(value), next(nullptr) {}
    };
    Node* head;

public:
    LinkedList() : head(nullptr) {}
    ~LinkedList() { }

    void push(T value)
    {
        Node* newNode = new Node(value); 
        if (head == nullptr)
        {
            newNode->next = head;
            head = newNode;
            cout << "Pushed element is: " << value << endl;
        }
        else
        {

            newNode->next = head;
            head = newNode;
            cout << "Pushed element is: " << value << endl;
        }
    }

    void pop() {

        if (isEmpty())
        {
            cout << "Stack underflow: Cannot pop from an empty stack.\n";
        }
        else
        {
            Node* temp;
            cout << "Popped element is: " << head->data << endl;
            temp = head;
            head = head->next;
            delete temp;

        }

    }

    bool isEmpty() const {
        return head == nullptr;
    }
    bool top(T& num) {
        if (isEmpty())
        {
            return false;
        }
        else
        {
            num = head->data;
            return true;
        }
    }
};

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

string infixToPostfix(string infix) {
    string postfix = "";
    string operators = "";

    for (int i = 0; i < infix.length(); i++) {
        char c = infix[i];

        if (isalnum(c)) {
            postfix = postfix + c;
        }
        else {
            while (!operators.empty() && precedence(operators.back()) >= precedence(c)) {
                postfix = postfix + operators.back();
                operators.pop_back();
            }
            operators = operators + c;
        }
    }

    while (!operators.empty()) {
        postfix = postfix + operators.back();
        operators.pop_back();
    }

    return postfix;
}

template <typename T>
class Queue {
    class Node {
    public:
        T data;
        Node* next;

        Node(T value) : data(value), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    
public:
    Queue() {
        head = nullptr;
        tail = nullptr;
    }
    ~Queue() {
        while (head != tail) {
            Node* del = head;
            head = head->next;
            delete del;
        }
        delete tail;
        head = nullptr;
        tail = nullptr;
    }
    void enqueue(T d) {
        Node* newNode = new Node(d);
        if (isFull()) {
            cout << "Queue is full" << endl;
        }
        else if(head==nullptr){
            head = newNode;
            tail = head;
            cout << "Enqueued the element: " << d << endl;
        }
        else{
            tail->next = newNode;
            tail = newNode;
            cout << "Enqueued the element: " << d << endl;
        }
    }
    void dequeue() {
        if (isEmpty()) {
            cout << "The Queue is empty!!" << endl;
        }
        else {
            Node* del = head;
            head = head->next;
            cout << "Dequeued the element: " << del->data << endl;
            delete del;
        }
    }
    bool isFull() {
        return false;  // you can add infinite number of nodes
    }
    int size() {
        Node* move = head;
        int s = 0;
        while (move != nullptr) {
            s++;
            move = move->next;
        }
        return s;
    }
    bool isEmpty() {
        return head == nullptr;
    }
    int front() {
        return head->data;
    }
    int rear() {
        return tail->data;
    }

    void reverseQueue() {
        LinkedList<T> s;
        while (!isEmpty()) {
            s.push(front());
            dequeue();
        }
        while (!s.isEmpty()) {
            T val;
            s.top(val);
            s.pop();
            enqueue(val);
        }
    }
};

bool isPalin(const char* str) {
    int len = strlen(str);
    int half;
    LinkedList<char> stack;
    half = len / 2;
    for (int i = 0; i < len; i++) {
        stack.push(str[i]);
    }
    for (int i = 0; i <= half; i++) {
        char top;
        stack.top(top);
        if (str[i] != top) {
            return false;
        }
        stack.pop();
    }
    return true;
}

template <typename T>
bool isPalindromeQueue(Queue<T>& q) {
    vector<T> elements;
    Queue<T> temp = q;

    while (!temp.isEmpty()) {
        elements.push_back(temp.front());
        temp.dequeue();
    }

    int n = elements.size();
    for (int i = 0; i < n / 2; ++i)
        if (elements[i] != elements[n - 1 - i]) 
            return false;

    return true;
}

int main() {
    LinkedList<int> list;

    list.push(10);
    list.push(20);
    list.push(30);
    list.push(5);
    list.push(2);
    list.pop();
    list.pop();

    string infix = "A+B*C-D";
    cout << endl<<"Infix: " << infix << endl;
    cout << "Postfix: " << infixToPostfix(infix) << endl;

    cout << "Palindrome String" << endl;
    const char* str = "sasas";
    bool result = isPalin(str);
    if (result) {
        cout << "\nIt is a Palindrome\n"<<endl;
    }
    else {
        cout << "It is not a Palindrome"<<endl;
    }

    Queue<int> q1;
    q1.enqueue(10);
    q1.enqueue(5);
    q1.enqueue(50);
    q1.enqueue(15);
    cout << "Size of Queue: " << q1.size() << endl;
    q1.dequeue();
    q1.dequeue();
    cout << endl << endl;
    q1.reverseQueue();
    return 0;
}
