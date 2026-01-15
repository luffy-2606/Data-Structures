#include <iostream>
#include <string>
using namespace std;

const int MAX = 256;

struct Node {
   char ch;
   int freq;
   Node* left, * right;
};

Node* createNode(char c, int f) {
   Node* n = new Node();
   n->ch = c;
   n->freq = f;
   n->left = NULL;
   n->right = NULL;
   return n;
}

void insertNode(Node* arr[], int& size, Node* n) {
   arr[size] = n;
   size++;

   for (int i = 0; i < size - 1; i++) {
       for (int j = 0; j < size - i - 1; j++) {
           if (arr[j]->freq > arr[j + 1]->freq) {
               Node* temp = arr[j];
               arr[j] = arr[j + 1];
               arr[j + 1] = temp;
           }
       }
   }
}

Node* extractMin(Node* arr[], int& size) {
   Node* minNode = arr[0];
   for (int i = 1; i < size; i++) {
       arr[i - 1] = arr[i];
   }
   size--;
   return minNode;
}

void generateCodes(Node* root, string code, string codes[]) {
   if (!root) return;

   if (!root->left && !root->right) {
       codes[(int)root->ch] = code;
   }

   generateCodes(root->left, code + "0", codes);
   generateCodes(root->right, code + "1", codes);
}

string encodeText(string S, string codes[]) {
   string encoded = "";
   for (int i = 0; i < S.length(); i++) {
       encoded += codes[(int)S[i]];
   }
   return encoded;
}

string decodeText(Node* root, string encoded) {
   string decoded = "";
   Node* curr = root;

   for (int i = 0; i < encoded.length(); i++) {
       if (encoded[i] == '0') curr = curr->left;
       else curr = curr->right;

       if (!curr->left && !curr->right) {
           decoded += curr->ch;
           curr = root;
       }
   }
   return decoded;
}

int main() {
   string S;
	cout << "Enter text to encode: ";
   getline(cin, S);

   int freq[MAX] = { 0 };

   // Frequency 
   for (int i = 0; i < S.length(); i++) {
       freq[(int)S[i]]++;
   }

   cout << "Frequencies:" << endl;
   for (int i = 0; i < MAX; i++) {
       if (freq[i] > 0) {
           cout << (char)i << " : " << freq[i] << endl;
       }
   }

   Node* arr[MAX];
   int size = 0;

   for (int i = 0; i < MAX; i++) {
       if (freq[i] > 0) {
           insertNode(arr, size, createNode((char)i, freq[i]));
       }
   }

   // Huffman Tree
   while (size > 1) {
       Node* left = extractMin(arr, size);
       Node* right = extractMin(arr, size);

       Node* newNode = createNode('\0', left->freq + right->freq);
       newNode->left = left;
       newNode->right = right;

       insertNode(arr, size, newNode);
   }

   Node* root = arr[0];


   string codes[MAX];
   generateCodes(root, "", codes);

   cout << "\nHuffman Codes:" << endl;
   for (int i = 0; i < MAX; i++) {
       if (freq[i] > 0) {
           cout << (char)i << " : " << codes[i] << endl;
       }
   }

   // Encode 
   string encoded = encodeText(S, codes);
   cout << "\nEncoded: " << encoded << endl;

   // Decode 
   string decoded = decodeText(root, encoded);
   cout << "Decoded: " << decoded << endl;

   return 0;
}
