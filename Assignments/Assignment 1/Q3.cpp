#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Maze {
	class Node {
	public:
		int data;
		Node* up;
		Node* down;
		Node* left;
		Node* right;
		Node(int d) {
			data = d;
			up = nullptr;
			down = nullptr;
			left = nullptr;
			right = nullptr;
		}
	};
	Node* start;
public:
	Maze() {
		start = nullptr;
	}
	void Read(string filename) {
		ifstream fin(filename);
		if (!fin) {
			cout << "File not found!\n";
		}
		else {
			string line;
			Node* prevRowHead = nullptr;

			while (getline(fin, line)) {
				if (!line.empty()) {
					Node* rowHead = nullptr;
					Node* tail = nullptr;
					Node* prevRowNode = prevRowHead;
					string num = "";
					int c = 0;

					for (int i = 0; i <= line.size(); i++) {
						if (i == line.size() || line[i] == ',') {
							if (!num.empty()) {
								int d = stoi(num); //string to integer  
								Node* newNode = new Node(d);

								if (start == nullptr)
									start = newNode;

								if (rowHead==nullptr) {
									rowHead = newNode;
									tail = newNode;
								}
								else {
									tail->right = newNode;
									newNode->left = tail;
									tail = newNode;
								}
								
								
								if (prevRowNode != nullptr) {
									prevRowNode->down = newNode;
									newNode->up = prevRowNode;
									prevRowNode = prevRowNode->right;
								}

								num.clear();
								c++;
							}
						}
						else {
							num = num + line[i];
						}
					}

					prevRowHead = rowHead; 
				}
			}
		}
		fin.close();
	}

	void Print() {
		Node* moveD = start;
		while (moveD != nullptr) {
			Node* moveR = moveD; 
			while (moveR != nullptr) {
				cout << moveR->data << " ";
				moveR = moveR->right;
			}
			cout << endl;
			moveD = moveD->down;
		}
	}


	int ClueRow(int data) {
		int noRows = 0;
		Node* moveD = start;
		while (moveD != nullptr) {
			moveD = moveD->down;
			noRows++;
		}

		int temp = data;
		int sum = 0;
		while (temp != 0) {
			sum = sum + temp % 10;
			temp = temp / 10;
		}

		return (sum % noRows + 1);
	}

	int ClueColumn(int data) {
		if (data == 0) {
			return 0;
		}
		else {
			int temp = data;
			int digits = 0;

			while (temp != 0) {
				digits++;
				temp = temp / 10;
			}
			return digits;
		}
	}

	void Visited(Node* Current) {
		cout << Current->data<<"->";
	}

	void EliteNode(Node* elite) {
		if (elite == nullptr) {
			cout << "\n\nTHERE IS NO ELITE NODE IN MAZE" << endl;
		}
		else {
			cout << "\n\nElite Node = " << elite->data << endl<<endl;
		}
	}

	void Traverse() {
		if (start == nullptr) 
			return;

		vector<Node*> visited; //to store+check the visited nodws

		Node* current = start;
		while (current != nullptr) {
			Visited(current);
			int r = ClueRow(current->data);
			int c = ClueColumn(current->data);

			Node* temp = start;
			for (int i = 0; i < r-1; i++) {
				temp = temp->down;
			}
			for (int j = 0; j < c-1; j++) {
				temp = temp->right;
			}

			if (temp == current) {
				EliteNode(current);
				return;
			}

			for (int i = 0; i < visited.size(); i++) {
				if (visited[i] == temp) {
					cout << "\nNode " << temp->data << " visited twice. ending traversal.\n";
					return;
				}
			}

			visited.push_back(temp);

			current = temp;
		}
	}
};

int main() {
	Maze M;
	M.Read("maze.txt");
	cout << "Maze:\n";
	M.Print();

	cout << "\nTraversal Path:\n";
	M.Traverse();

	return 0;
}
