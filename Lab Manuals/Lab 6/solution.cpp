#include <iostream>
#include <string>
#include <cstring>
using namespace std;

int fibNumber(int k) {
	if (k == 0)
		return 0;
	else if (k == 1)
		return 1;
	return fibNumber(k - 1) + fibNumber(k - 2);
}

void findSubsets(string input, string output, int index) {
    if (index == input.length()) {
        if (output != "")   
            cout << output << " ";
        return;
    }

    findSubsets(input, output + input[index], index + 1);

    findSubsets(input, output, index + 1);
}

int digitSum(int N) {
    if (N == 0)
        return 0;
    int x = N % 10;
    N = N / 10;
    return x + digitSum(N);
}

int fact(int N){
    if (N == 1)
        return 1;
    return N * fact(N - 1);
}

int findGCD(int num1, int num2) {
    if (num2 == 0)
        return num1;  
    int rem = num1 % num2;
    return findGCD(num2, rem);  
}

class List {
    class Node {
    public:
        int data;
        Node* next;
        Node(int d) {
            data = d;
            next = nullptr;
        }
    };
    Node* head;
public:
    List() {
        head = nullptr;
    }
    bool search(Node* h1, int key) {
        if (h1 == nullptr)
            return false;

        if (h1->data == key)
            return true;

        return search(h1->next, key);
    }
};



int main() {
    cout << "Fibonacci of 6 is: " << fibNumber(6);
    cout << endl << endl;

	string str = "abcd";
    cout << "Subsets of " << str << " are:" << endl;
    findSubsets(str, "", 0);
    cout << endl<<endl;

    cout << "Sum of Digits of 12345 is: " << digitSum(12345);
    cout << endl << endl;

    cout << "Factorial of 5 is: " << fact(5);
    cout << endl << endl;

    cout << "GCD of 35 and 6 is: " << findGCD(33, 12);
    cout << endl << endl;


	return 0;
}
