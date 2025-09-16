#include <iostream>
#include <vector>
using namespace std;

class DynamicArray {
	int* arr;
	int size;
	int maxSize;
public:
	DynamicArray(int s = 0) {
		maxSize = s;
		arr = new int[maxSize];
		size = 0;
	}
	~DynamicArray() {
		delete arr;
	}
	DynamicArray(DynamicArray& obj) {
		maxSize = obj.maxSize;
		size = obj.size;
		arr = new int[maxSize];
		for (int i = 0; i < size; i++){
			arr[i] = obj.arr[i];
		}
	}
	void push_back(int value) {
		if (size == maxSize) {
			maxSize=maxSize*2;
			int* newArr = new int[maxSize];
			for (int i = 0; i < size; i++){
				newArr[i] = arr[i];
			}
			int* temp = arr;
			arr = newArr;
			delete temp;
			arr[size] = value;
			size++;
		}
		else {
			arr[size] = value;
			size++;
		}
	}

	void pop_back() {
		if(size>0)
			size--;
	}

	int getSize() {
		return size;
	}

	int capacity() {
		return maxSize;
	}

	void remove_duplicates() {
		for (int i= 0; i < size; i++){
			int curElem = arr[i];
			for (int j = i+1; j < size; j++){
				if (curElem == arr[j]) {
					size--;
					for (int k = j; k < size; k++){
						arr[k] = arr[k + 1];
					}
				}
			}
		}
	}

	void rotate_right(int k) {
		k = k % size;
		int* newArr = new int[maxSize];

		for (int i = 0; i < size; i++){
			if (i + k < size)
				newArr[i + k] = arr[i];
			else
				newArr[i + k- size] = arr[i];
		}
		int* temp = arr;
		arr = newArr;
		delete temp;
	}

	void print() {
		for (int i = 0; i < size; i++) {
			cout << arr[i] << " ";
		}
		cout << endl;
	}

	vector<vector<int>> find_subarrays_with_sum(int target) {
		vector<vector<int>> result;

		for (int i = 0; i < size; i++) {
			int sum = 0;
			for (int j = i; j < size; j++) {
				sum = sum + arr[j];

				if (sum == target) {
					vector<int> subarray;
					for (int k = i; k <= j; k++) {
						subarray.push_back(arr[k]);
					}
					result.push_back(subarray);
				}
			}
		}
		return result;
	}
};

void swapVector(vector<int> obj) {
	int sz = obj.size();
	for (int i = 0; i < sz / 2; i++) {
		swap(obj[i], obj[sz - i - 1]);
	}

	cout << "Swapped Vector: ";
	for (int i = 0; i < sz; i++) {
		cout << obj[i] << " ";
	}
	cout << endl;
}

void productVector(vector<int> obj) {
	int total = 1;
	for (int i = 0; i < obj.size(); i++) {
		total = total*obj[i];
	}
	cout << "Product of all elements: " << total << endl;
}

void maxProductVector(vector<int> obj) {
	int maxProd = 0, minProd = 0, result = 0;
	int start = 0, end = 0, tempStart = 0;

	for (int i = 1; i < obj.size(); i++) {
		if (obj[i] < 0) {  //sawp for negative
			int temp = maxProd;
			maxProd = minProd;
			minProd = temp;
		}
		if (obj[i] > (maxProd * obj[i])) {
			maxProd = obj[i];
			tempStart = i;
		}
		else {
			maxProd = maxProd * obj[i];
		}

		minProd = min(obj[i], minProd * obj[i]);  //this would return the minimum of the 2 nums

		if (maxProd > result) {
			result = maxProd;
			start = tempStart;
			end = i;
		}
	}

	cout << "Maximum Product Subarray: The subarray [";
	for (int i = start; i <= end; i++) {
		cout << obj[i];
		if (i < end) cout << ", ";
	}
	cout << "] has the maximum product " << result << endl;
}


template <typename T>
class LinkedList {
	class Node {
	public:
		T data;
		Node* next;
		Node() {
			next = nullptr;
		}
		Node(T d = 0) {
			data = d;
			next = nullptr;
		}
	};
	Node* head;
	Node* tail;
public:
	LinkedList() {
		head = nullptr;
		tail = nullptr;
	}
	void insertAtHead(T const& element) {
		Node* newNode = new Node(element);
		if (head == nullptr) {
			head = newNode;
			tail = head;
		}
		else {
			newNode->next = head;
			head = newNode;
		}
	}
	void insertAtTail(T const& element) {
		Node* newNode = new Node(element);
		if (tail == nullptr) {
			head = newNode;
			tail = head;
		}
		else {
			tail->next = newNode;
			tail = newNode;
		}
	}
	void update(int Kth_position, T const& val) { // i have added value to be added
		if (Kth_position < 0) {
			cout << "Index can not be negative\n";
			return;
		}
		int pos = 0;
		Node* move = head;
		while (move != tail && pos!=Kth_position) {
			pos++;
			move = move->next;
		}
		if (pos == Kth_position)
			move->data = val;
		else
			cout << "Index is not in List!!!!!!!!"<<endl;
	}
	void print() {
		Node* move = head;
		while (move != tail) {
			cout << move->data << ", ";
			move = move->next;
		}
		cout << tail->data;
		cout << endl;
	}
};

int main() {
	DynamicArray arr(10); // i have added capacity here as well
	arr.push_back(10);
	arr.push_back(20);
	arr.push_back(30);
	arr.push_back(20);
	arr.push_back(10);
	arr.push_back(50);
	cout << "Initial array: ";
	arr.print();
	arr.remove_duplicates();
	cout << "Array after removing duplicates: ";
	arr.print();
	arr.rotate_right(21);
	cout << "Array after rotating right by 2 positions: ";
	arr.print();
	cout << "Array size: " << arr.getSize() << endl;
	cout << "Array capacity: " << arr.capacity() << endl;
	int target = 30;
	auto subarrays = arr.find_subarrays_with_sum(target); 
	cout << "Subarrays with sum " << target << ": " << endl; 
	for(const auto& subarray : subarrays) {
		for (int num : subarray) {
			cout << num << " ";
		}
		cout << endl;
	}

	vector<int> vec = { 2, 3, -2, 4, 6};
	cout << "Input vector: 2 3 -2 4 6" << endl;
	swapVector(vec);
	productVector(vec);
	maxProductVector(vec);

	cout << endl;
	LinkedList<int> L1;
	L1.insertAtHead(10);
	L1.insertAtHead(1);
	L1.insertAtTail(100);
	L1.insertAtTail(130);
	L1.insertAtTail(0);
	L1.print();
	return 0;
}
