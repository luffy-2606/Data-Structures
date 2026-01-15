#include <iostream>
using namespace std;


class StudentMaxHeap;

class Student {
	friend class StudentMaxHeap;
private: 
	int rollNo; 
	double cgpa; 
public:
	Student(int r = 0, double c = 0) {
		rollNo = r;
		cgpa = c;
	}
};

class StudentMaxHeap {
private: 
	Student* st; 
	int currSize; 
	int maxSize; 
public:
	StudentMaxHeap(int size) {
		maxSize = size;
		currSize = 0;
		st = new Student[maxSize];
	} 

	~StudentMaxHeap() {
		//delete[] st;
		//delete st;
	} 

	bool isEmpty() {
		return currSize == 0;
	} 

	bool isFull() {
		return currSize == maxSize;
	} 

	void ReheapDown(int root, int bottom){
		int maxChild, rightChild, leftChild;
		leftChild = 2 * root + 1;
		rightChild = 2 * root + 2;

		if (st[leftChild].cgpa > st[rightChild].cgpa)
			maxChild = leftChild;
		else if (st[leftChild].cgpa < st[rightChild].cgpa)
			maxChild = rightChild;
		else {
			if (st[leftChild].rollNo > st[rightChild].rollNo)
				maxChild = leftChild;
			else
				maxChild = rightChild;
		}

		if (st[root].cgpa < st[maxChild].cgpa ||
			(st[root].cgpa == st[maxChild].cgpa && st[root].rollNo < st[maxChild].rollNo)) {
			swap(st[root], st[maxChild]);
			ReheapDown(maxChild, bottom);
		}

	}

	void ReheapUp(int root, int bottom)
	{
		int parent;
		if (bottom > root) { // tree is not empty
			parent = (bottom - 1) / 2;
			if (st[parent].cgpa < st[bottom].cgpa ||
				(st[parent].cgpa == st[bottom].cgpa && st[parent].rollNo < st[bottom].rollNo)) {
				swap(st[parent], st[bottom]);
				ReheapUp(root, parent);
			}
		}
	}

	bool insert(int rollNo, double cgpa) {
		if (!isFull()) {
			Student newSt(rollNo, cgpa);
			currSize++;
			st[currSize - 1] = newSt;
			ReheapUp(0, currSize - 1); 
			return true;
		}
		else {
			return false;
		}
	}

	bool removeBestStudent(int& rollNo, double& cgpa) {
		if (!isEmpty()) {
			rollNo = st[0].rollNo;
			cgpa = st[0].cgpa;
			st[0] =st[currSize - 1];
			currSize--;
			ReheapDown(0, currSize - 1);
			return true;
		}
		else {
			return false;
		}
	}
};

int main() {
	StudentMaxHeap heap(10);

	heap.insert(101, 3.5);
	heap.insert(102, 3.8);
	heap.insert(103, 3.8); // Same CGPA, higher rollNo
	heap.insert(104, 3.2);

	cout << "Removing students in order of best CGPA:\n";
	int roll;
	double cgpa;
	while (!heap.isEmpty()) {
		heap.removeBestStudent(roll, cgpa);
		cout << "Roll No: " << roll << ", CGPA: " << cgpa << endl;
	}

	return 0;
}
