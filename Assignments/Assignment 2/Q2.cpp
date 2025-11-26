#include <iostream>
#include <string>
#include <cstring>
using namespace std;

const int MAX_AUTHORS = 5;         
const int MAX_USERS = 1000;        
const int MAX_BORROWED = 10;       
const int MAX_REQUESTS = 500;      
const int BORROW_PERIOD = 14;      
const double FINE_PER_DAY = 1.0;   


bool parseDate(const string& s, int& y, int& m, int& d) {
    if (s.size() < 10) 
        return false;
    y = (s[0] - '0') * 1000 + (s[1] - '0') * 100 + (s[2] - '0') * 10 + (s[3] - '0');
    m = (s[5] - '0') * 10 + (s[6] - '0');
    d = (s[8] - '0') * 10 + (s[9] - '0');
    if (m < 1 || m > 12) 
        return false;
    if (d < 1 || d > 31) 
        return false;
    return true;
}

bool isLeap(int y) {
    if (y % 400 == 0) 
        return true;
    if (y % 100 == 0) 
        return false;
    return (y % 4 == 0);
}

int daysInMonth(int y, int m) {
    int mdays[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    if (m == 2 && isLeap(y)) 
        return 29;
    return mdays[m - 1];
}

double dateToDays(int y, int m, int d) {
    double days = 0;
    int year = 1;

    double years = y - 1;
    days += years * 365;
    days += years / 4 - years / 100 + years / 400; // leap days
    for (int mm = 1; mm < m; ++mm) 
        days += daysInMonth(y, mm);
    days += d; 

    return days;
}

double parseDateToDays(const string& s) {
    int y, m, d;
    if (!parseDate(s, y, m, d)) 
        return -1;
    return dateToDays(y, m, d);
}

double daysBetweenDates(const string& fromDate, const string& toDate) {
    double a = parseDateToDays(fromDate);
    double b = parseDateToDays(toDate);
    if (a < 0 || b < 0) 
        return INT_MIN;
    return b - a;
}

string daysToDate(double totalDays) {
    int days = totalDays;
    int year = 1;
    
    int daysIn400Years = 146097; 
    int daysIn100Years = 36524;
    int daysIn4Years = 1461;
    int daysInYear = 365;
    
    int cycles400 = days / daysIn400Years;
    days %= daysIn400Years;
    year += cycles400 * 400;
    
    int cycles100 = days / daysIn100Years;
    if (cycles100 == 4) cycles100 = 3; 
    days -= cycles100 * daysIn100Years;
    year += cycles100 * 100;
    
    int cycles4 = days / daysIn4Years;
    days %= daysIn4Years;
    year += cycles4 * 4;
    
    int remainingYears = days / daysInYear;
    if (remainingYears == 4) remainingYears = 3; 
    days -= remainingYears * daysInYear;
    year += remainingYears;
    
    days++; 
    int month = 1;
    while (days > daysInMonth(year, month)) {
        days -= daysInMonth(year, month);
        month++;
    }

    string result = "";
    result += to_string(year);
    result += "-";
    if (month < 10) result += "0";
    result += to_string(month);
    result += "-";
    if (days < 10) result += "0";
    result += to_string(days);
    
    return result;
}

struct Request {
    int userID;
    double reqDay;
};

class MinHeap {
public:
    Request* arr;
    int size;
    int capacity;
    
    MinHeap(int cap = MAX_REQUESTS) {
        capacity = cap;
        arr = new Request[capacity];
        size = 0;
    }
    
    MinHeap(const MinHeap& other) {
        capacity = other.capacity;
        size = other.size;
        arr = new Request[capacity];
        for (int i = 0; i < size; i++) {
            arr[i] = other.arr[i];
        }
    }
    
    MinHeap& operator=(const MinHeap& other) {
        if (this != &other) {
            delete[] arr;
            capacity = other.capacity;
            size = other.size;
            arr = new Request[capacity];
            for (int i = 0; i < size; i++) {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }
    
    ~MinHeap() {
        delete[] arr;
    }
    
    bool empty() const { return size == 0; }
    int getSize() const { return size; }
    
    bool push(const Request& r) {
        if (size >= capacity) return false;
        arr[size] = r;
        reheapUp(0, size);
        size++;
        return true;
    }
    
    bool top(Request& out) {
        if (size == 0) return false;
        out = arr[0];
        return true;
    }
    
    bool pop() {
        if (size == 0) return false;
        arr[0] = arr[size - 1];
        size--;
        if (size > 0) reheapDown(0, size - 1);
        return true;
    }
    
    void clear() { size = 0; }

private:
    void swapReq(int i, int j) {
        Request tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
    void reheapDown(int root, int bottom) {
        int minChild, leftChild, rightChild;
        leftChild = 2 * root + 1;
        rightChild = 2 * root + 2;
        if (leftChild <= bottom) {
            if (leftChild == bottom)
                minChild = leftChild;
            else {
                if (arr[leftChild].reqDay <= arr[rightChild].reqDay)
                    minChild = leftChild;
                else
                    minChild = rightChild;
            }
            if (arr[root].reqDay > arr[minChild].reqDay) {
                swapReq(root, minChild);
                reheapDown(minChild, bottom);
            }
        }
    }
    void reheapUp(int root, int bottom) {
        if (bottom > root) {
            int parent = (bottom - 1) / 2;
            if (arr[parent].reqDay > arr[bottom].reqDay) {
                swapReq(parent, bottom);
                reheapUp(root, parent);
            }
        }
    }
};

class Book {
public:
    int issn;
    string title;
    string authors; // comma separated
    int referenceCopies;
    int issuableCopies;
    MinHeap* demand;

    Book() {
        issn = 0;
        title = "";
        authors = "";
        referenceCopies = 0;
        issuableCopies = 0;
        demand = new MinHeap();
    }
    
    Book(int i, const string& t, const string& a, int r, int iss) {
        issn = i; 
        title = t; 
        authors = a;
        referenceCopies = r; 
        issuableCopies = iss;
        demand = new MinHeap();
    }
    
    Book(const Book& other) {
        issn = other.issn;
        title = other.title;
        authors = other.authors;
        referenceCopies = other.referenceCopies;
        issuableCopies = other.issuableCopies;
        demand = new MinHeap(*other.demand); 
    }
    
    Book& operator=(const Book& other) {
        if (this != &other) {
            issn = other.issn;
            title = other.title;
            authors = other.authors;
            referenceCopies = other.referenceCopies;
            issuableCopies = other.issuableCopies;
            if (demand) delete demand;
            demand = new MinHeap(*other.demand); 
        }
        return *this;
    }
    
    ~Book() {
        if (demand) 
            delete demand;
    }
};

struct BookNode {
    Book data;
    BookNode* left;
    BookNode* right;
    BookNode(const Book& b) : data(b), left(nullptr), right(nullptr) {}
};

class BookBST {
private:
    BookNode* root;

    BookNode* insertRec(BookNode* node, const Book& b) {
        if (node == nullptr) {
            BookNode* n = new BookNode(b);
            return n;
        }
        if (b.issn < node->data.issn) 
            node->left = insertRec(node->left, b);
        else if (b.issn > node->data.issn) 
            node->right = insertRec(node->right, b);
        else {
            node->data.title = b.title;
            node->data.authors = b.authors;
            node->data.referenceCopies = b.referenceCopies;
            node->data.issuableCopies = b.issuableCopies;
        }
        return node;
    }

    BookNode* searchRec(BookNode* node, int issn) {
        if (!node) 
            return nullptr;
        if (issn == node->data.issn) 
            return node;
        if (issn < node->data.issn) 
            return searchRec(node->left, issn);
        return searchRec(node->right, issn);
    }

    BookNode* findMin(BookNode* node) {
        if (!node) 
            return nullptr;
        while (node->left) 
            node = node->left;
        return node;
    }

    BookNode* removeRec(BookNode* node, int issn) {
        if (!node) 
            return nullptr;
        if (issn < node->data.issn) 
            node->left = removeRec(node->left, issn);
        else if (issn > node->data.issn) 
            node->right = removeRec(node->right, issn);
        else {
            if (!node->left) {
                BookNode* r = node->right;
                delete node;
                return r;
            }
            else if (!node->right) {
                BookNode* l = node->left;
                delete node;
                return l;
            }
            else {
                BookNode* succ = findMin(node->right);

                if (node->data.demand) 
                    delete node->data.demand;
                node->data.issn = succ->data.issn;
                node->data.title = succ->data.title;
                node->data.authors = succ->data.authors;
                node->data.referenceCopies = succ->data.referenceCopies;
                node->data.issuableCopies = succ->data.issuableCopies;
                node->data.demand = new MinHeap(*(succ->data.demand));
                node->right = removeRec(node->right, succ->data.issn);
            }
        }
        return node;
    }

    void inorderRec(BookNode* node) {
        if (!node) return;
        inorderRec(node->left);
        cout << "ISSN: " << node->data.issn << " | Title: " << node->data.title
            << " | Authors: " << node->data.authors
            << " | Ref: " << node->data.referenceCopies
            << " | Issuable: " << node->data.issuableCopies
            << " | PendingRequests: " << node->data.demand->getSize()
            << endl;
        inorderRec(node->right);
    }

    void destroyRec(BookNode* node) {
        if (!node) 
            return;
        destroyRec(node->left);
        destroyRec(node->right);
        delete node;
    }

    void traverseAndFindMax(BookNode* node, BookNode*& best, int& bestSize) {
        if (!node) 
            return;
        traverseAndFindMax(node->left, best, bestSize);
        int s = node->data.demand->getSize();
        if (s > bestSize) {
            bestSize = s;
            best = node;
        }
        traverseAndFindMax(node->right, best, bestSize);
    }

public:
    BookBST() { 
        root = nullptr; 
    }
    ~BookBST() { 
        destroyRec(root); 
    }

    void insertBook(const Book& b) { 
        root = insertRec(root, b); 
    }

    bool removeBook(int issn) {
        if (!searchBook(issn)) 
            return false;
        root = removeRec(root, issn);
        return true;
    }

    BookNode* searchBook(int issn) { 
        return searchRec(root, issn); 
    }

    void displayAll() {
        if (!root) { 
            cout << "No books in library.\n"; 
            return; 
        }
        inorderRec(root);
    }

    BookNode* findMostDemanded() {
        BookNode* best = nullptr;
        int bestSize = -1;
        traverseAndFindMax(root, best, bestSize);
        return best;
    }

    void clearAll() { 
        destroyRec(root); 
        root = nullptr; 
    }
};

struct BorrowRecord {
    int issn;
    double borrowDay; 
};

class User {
public:
    int userID;
    string name;
    BorrowRecord borrowed[MAX_BORROWED];
    int borrowedCount;
    double fine;

    User() {
        userID = 0;
        name = "";
        borrowedCount = 0;
        fine = 0.0;
    }
    User(int id, const string& n) {
        userID = id; 
        name = n;
        borrowedCount = 0; 
        fine = 0.0;
    }

    bool borrowBook(int issn, double day) {
        if (borrowedCount >= MAX_BORROWED) 
            return false;
        borrowed[borrowedCount].issn = issn;
        borrowed[borrowedCount].borrowDay = day;
        borrowedCount++;
        return true;
    }
    bool returnBook(int issn, double& borrowDay) {
        for (int i = 0; i < borrowedCount; i++) {
            if (borrowed[i].issn == issn) {
                borrowDay = borrowed[i].borrowDay;
                for (int j = i; j < borrowedCount - 1; j++) 
                    borrowed[j] = borrowed[j + 1];
                borrowedCount--;
                return true;
            }
        }
        return false;
    }
    void removeBookReference(int issn) {
        for (int i = 0; i < borrowedCount; i++) {
            if (borrowed[i].issn == issn) {
                for (int j = i; j < borrowedCount - 1; j++) 
                    borrowed[j] = borrowed[j + 1];
                borrowedCount--;
                return;
            }
        }
    }
};

struct UserNode {
    User data;
    UserNode* left;
    UserNode* right;
    int height;
    UserNode(const User& u) : data(u), left(nullptr), right(nullptr), height(1) {}
};

int height(UserNode* n) { 
    if (n)
        return n->height;
    else
        return 0;
}

int getBalance(UserNode* n) { 
    if (n)
        return height(n->left) - height(n->right);
    else
        return 0;
}
void updateHeight(UserNode* n) { 
    if (n) {
        if (height(n->left) > height(n->right))
            n->height = 1 + height(n->left);
        else
            n->height = 1 + height(n->right);
    }
}

UserNode* rightRotate(UserNode* y) {
    UserNode* x = y->left;
    UserNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    updateHeight(y);
    updateHeight(x);
    return x;
}
UserNode* leftRotate(UserNode* x) {
    UserNode* y = x->right;
    UserNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    updateHeight(x);
    updateHeight(y);
    return y;
}

class UserAVL {
private:
    UserNode* root;

    UserNode* insertRec(UserNode* node, const User& u) {
        if (!node) 
            return new UserNode(u);
        if (u.userID < node->data.userID) 
            node->left = insertRec(node->left, u);
        else if (u.userID > node->data.userID) 
            node->right = insertRec(node->right, u);
        else { 
            node->data.name = u.name;
            return node;
        }
        updateHeight(node);
        int balance = getBalance(node);
        // LL
        if (balance > 1 && u.userID < node->left->data.userID) return rightRotate(node);
        // RR
        if (balance < -1 && u.userID > node->right->data.userID) return leftRotate(node);
        // LR
        if (balance > 1 && u.userID > node->left->data.userID) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        // RL
        if (balance < -1 && u.userID < node->right->data.userID) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    UserNode* minValueNode(UserNode* node) {
        UserNode* cur = node;
        while (cur->left) cur = cur->left;
        return cur;
    }

    UserNode* removeRec(UserNode* node, int userID) {
        if (!node) 
            return nullptr;
        if (userID < node->data.userID) 
            node->left = removeRec(node->left, userID);
        else if (userID > node->data.userID) 
            node->right = removeRec(node->right, userID);
        else {
            if (!node->left || !node->right) {
                UserNode* tmp = node->left ? node->left : node->right;
                if (!tmp) {
                    delete node;
                    return nullptr;
                }
                else {
                    *node = *tmp;
                    delete tmp;
                }
            }
            else {
                UserNode* tmp = minValueNode(node->right);
                node->data = tmp->data;
                node->right = removeRec(node->right, tmp->data.userID);
            }
        }
        updateHeight(node);
        int balance = getBalance(node);
        // LL
        if (balance > 1 && getBalance(node->left) >= 0) 
            return rightRotate(node);
        // LR
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        // RR
        if (balance < -1 && getBalance(node->right) <= 0) 
            return leftRotate(node);
        // RL
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    UserNode* searchRec(UserNode* node, int userID) {
        if (!node) 
            return nullptr;
        if (userID == node->data.userID) 
            return node;
        if (userID < node->data.userID) 
            return searchRec(node->left, userID);
        return searchRec(node->right, userID);
    }

    void inorderRec(UserNode* node) {
        if (!node) 
            return;
        inorderRec(node->left);
        cout << "UserID: " << node->data.userID << " | Name: " << node->data.name
            << " | Fine: " << node->data.fine << " | Borrowed: ";
        if (node->data.borrowedCount == 0) 
            cout << "None";
        else {
            for (int i = 0; i < node->data.borrowedCount; i++) {
                cout << node->data.borrowed[i].issn;
                if (i + 1 < node->data.borrowedCount) 
                    cout << ", ";
            }
        }
        cout << endl;
        inorderRec(node->right);
    }

    void destroyRec(UserNode* node) {
        if (!node) return;
        destroyRec(node->left);
        destroyRec(node->right);
        delete node;
    }

    void displayFinesRec(UserNode* node, bool& any) {
        if (!node) return;
        displayFinesRec(node->left, any);
        if (node->data.fine > 0.0) {
            cout << "User ID: " << node->data.userID << " | Fine: " << node->data.fine << endl;
            any = true;
        }
        displayFinesRec(node->right, any);
    }

    void removeBookFromAllUsersRec(UserNode* node, int issn) {
        if (!node) return;
        removeBookFromAllUsersRec(node->left, issn);
        node->data.removeBookReference(issn);
        removeBookFromAllUsersRec(node->right, issn);
    }

public:
    UserAVL() { 
        root = nullptr; 
    }
    ~UserAVL() { 
        destroyRec(root); 
    }

    void insertUser(const User& u) { 
        root = insertRec(root, u);
    }

    bool removeUser(int userID) {
        if (!searchUser(userID)) 
            return false;
        root = removeRec(root, userID);
        return true;
    }

    UserNode* searchUser(int userID) { 
        return searchRec(root, userID); 
    }

    void displayAll() {
        if (!root) { 
            cout << "No users registered." << endl; 
            return; 
        }
        inorderRec(root);
    }

    void calculateFinesAndDisplay() {
        bool any = false;
        displayFinesRec(root, any);
        if (!any) 
            cout << "No outstanding fines for any users.\n";
    }

    void removeBookFromAllUsers(int issn) {
        removeBookFromAllUsersRec(root, issn);
    }
};

class Library {
private:
    BookBST books;
    UserAVL users;

public:
    void addBook() {
        int issn, refCopies, issuableCopies;
        string title, authors;

        cout << "Enter ISSN (int): "; 
        cin >> issn; 
        cin.ignore();
        
        // Check if book with this ISSN already exists
        BookNode* existing = books.searchBook(issn);
        if (existing) {
            cout << "Error: A book with ISSN " << issn << " already exists!\n";
            return;
        }
        
        cout << "Enter Title: "; 
        getline(cin, title);
        cout << "Enter Authors (comma separated): "; 
        getline(cin, authors);
        cout << "Enter Reference Copies: "; 
        cin >> refCopies;
        cout << "Enter Issuable Copies: "; 
        cin >> issuableCopies;

        Book b(issn, title, authors, refCopies, issuableCopies);
        books.insertBook(b);
        cout << "Book added successfully: ISSN: " << issn << ", Title: " << title << "\n";
    }

    void registerUser() {
        int uid; string name;
        cout << "Enter User ID (int): "; 
        cin >> uid; 
        cin.ignore();
        cout << "Enter Name: "; 
        getline(cin, name);
        User u(uid, name);
        users.insertUser(u);
        cout << "User registered: User ID: " << uid << ", Name: " << name << "\n";
    }

    void borrowBook() {
        int uid, issn;
        string reqDate;
        cout << "Enter User ID: "; 
        cin >> uid;
        cout << "Enter ISSN: "; 
        cin >> issn; 
        cin.ignore();
        cout << "Enter Request Date (yyyy-mm-dd): "; 
        getline(cin, reqDate);
        double reqDay = parseDateToDays(reqDate);
        if (reqDay < 0) { 
            cout << "Invalid request date format.\n"; 
            return; 
        }
        UserNode* un = users.searchUser(uid);
        if (!un) { 
            cout << "User not found.\n"; 
            return; 
        }
        BookNode* bn = books.searchBook(issn);
        if (!bn) { 
            cout << "Book not found.\n"; 
            return; 
        }

        Request r; r.userID = uid; r.reqDay = reqDay;
        if (!bn->data.demand->push(r)) {
            cout << "Could not place request: heap full.\n"; 
            return;
        }
        cout << "Request placed: User " << uid << " requested ISSN " << issn << " on " << reqDate << "\n";

        if (bn->data.issuableCopies > 0) {
            Request topReq;
            if (bn->data.demand->top(topReq)) {
                bn->data.demand->pop();
                double borrowDay = reqDay;
                if (!un->data.borrowBook(issn, borrowDay)) {
                    cout << "User cannot borrow more books (limit reached). Request remains in queue.\n";
                    bn->data.demand->push(topReq);
                    return;
                }
                bn->data.issuableCopies -= 1;
                cout << "Book borrowed: ISSN: " << issn << ", Title: " << bn->data.title << "\n";
            }
        }
        else {
            cout << "No copies available now. Request queued.\n";
        }
    }

    void returnBook() {
        int uid, issn;
        string returnDate;
        cout << "Enter User ID: "; cin >> uid;
        cout << "Enter ISSN: "; cin >> issn; cin.ignore();
        cout << "Enter Return Date (yyyy-mm-dd): "; getline(cin, returnDate);
        double retDay = parseDateToDays(returnDate);
        if (retDay < 0) { cout << "Invalid return date format.\n"; return; }

        UserNode* un = users.searchUser(uid);
        if (!un) { cout << "User not found.\n"; return; }
        BookNode* bn = books.searchBook(issn);
        if (!bn) { cout << "Book not found.\n"; return; }

        double borrowDay;
        if (!un->data.returnBook(issn, borrowDay)) {
            cout << "This user did not borrow this book.\n"; return;
        }

        bn->data.issuableCopies += 1;

        double dueDay = borrowDay + BORROW_PERIOD;
        if (retDay > dueDay) {
            double overdue = retDay - dueDay;
            un->data.fine += overdue * FINE_PER_DAY;
            cout << "Book returned: ISSN: " << issn << ", Title: " << bn->data.title << "\n";
            cout << "Fine added: User ID: " << uid << ", Fine: " << (overdue * FINE_PER_DAY) << "\n";
        }
        else {
            cout << "Book returned: ISSN: " << issn << ", Title: " << bn->data.title << "\n";
        }

        if (!bn->data.demand->empty() && bn->data.issuableCopies > 0) {
            Request next;
            bn->data.demand->top(next);
            bn->data.demand->pop();
            UserNode* nextUser = users.searchUser(next.userID);
            if (nextUser) {
                if (!nextUser->data.borrowBook(bn->data.issn, next.reqDay)) {
                    cout << "Next user " << next.userID << " cannot borrow (limit). Skipping.\n";
                    while (!bn->data.demand->empty() && bn->data.issuableCopies > 0) {
                        Request nxt; bn->data.demand->top(nxt); bn->data.demand->pop();
                        UserNode* nu = users.searchUser(nxt.userID);
                        if (nu && nu->data.borrowBook(bn->data.issn, nxt.reqDay)) {
                            bn->data.issuableCopies -= 1;
                            cout << "Book auto-issued to next user: " << nu->data.userID << "\n";
                            break;
                        }
                    }
                }
                else {
                    bn->data.issuableCopies -= 1;
                    cout << "Book auto-issued to next user: " << next.userID << "\n";
                }
            }
            else {
                cout << "Next requester not found in user records; skipped.\n";
            }
        }
    }

    void updateBookInfo() {
        int issn;
        cout << "Enter ISSN: "; cin >> issn; cin.ignore();
        BookNode* bn = books.searchBook(issn);
        if (!bn) { 
            cout << "Book not found.\n"; 
            return; 
        }
        cout << "Enter Field to Update (Title/Authors/Reference Copies/Issuable Copies): ";
        string field; getline(cin, field);
        if (field == "Title" || field == "title") {
            string newVal; 
            cout << "Enter New Title: "; 
            getline(cin, newVal);
            bn->data.title = newVal;
            cout << "Book updated: ISSN: " << issn << ", New Title: " << newVal << "\n";
        }
        else if (field == "Authors" || field == "authors") {
            string newVal; 
            cout << "Enter New Authors (comma separated): "; 
            getline(cin, newVal);
            bn->data.authors = newVal;
            cout << "Book updated: ISSN: " << issn << ", New Authors: " << newVal << "\n";
        }
        else if (field == "Reference Copies" || field == "reference copies") {
            int v; 
            cout << "Enter New Value: "; 
            cin >> v;
            bn->data.referenceCopies = v;
            cout << "Book updated: ISSN: " << issn << ", New Reference Copies: " << v << "\n";
        }
        else if (field == "Issuable Copies" || field == "issuable copies") {
            int v; 
            cout << "Enter New Value: "; 
            cin >> v;
            bn->data.issuableCopies = v;
            cout << "Book updated: ISSN: " << issn << ", New Issuable Copies: " << v << "\n";
        }
        else {
            cout << "Unknown field.\n";
        }
    }

    void deleteBook() {
        int issn;
        cout << "Enter ISSN: "; 
        cin >> issn;
        BookNode* bn = books.searchBook(issn);
        if (!bn) { 
            cout << "Book not found.\n"; 
            return; 
        }
        string title = bn->data.title;
        users.removeBookFromAllUsers(issn);
        if (books.removeBook(issn)) {
            cout << "Book deleted: ISSN: " << issn << ", Title: " << title << "\n";
        }
        else {
            cout << "Failed to delete book.\n";
        }
    }

    void findMostDemanded() {
        BookNode* bn = books.findMostDemanded();
        if (!bn) { 
            cout << "No books or no requests.\n"; 
            return; 
        }
        cout << "Most Demanded Book: " << bn->data.title << ", ISSN: " << bn->data.issn << ", Pending Requests: " << bn->data.demand->getSize() << "\n";
    }

    void displayUserRecord() {
        int uid; 
        cout << "Enter User ID: "; 
        cin >> uid;
        UserNode* un = users.searchUser(uid);
        if (!un) { 
            cout << "User not found.\n"; 
            return; 
        }
        cout << "User ID: " << un->data.userID << ", Name: " << un->data.name << "\n";
        cout << "Borrowed Books: ";
        if (un->data.borrowedCount == 0) 
            cout << "None\n";
        else {
            for (int i = 0; i < un->data.borrowedCount; i++) {
                string borrowDate = daysToDate(un->data.borrowed[i].borrowDay);
                cout << "ISSN: " << un->data.borrowed[i].issn << " (Borrowed on: " << borrowDate << ")";
                if (i + 1 < un->data.borrowedCount) 
                    cout << ", ";
            }
            cout << "\n";
        }
        cout << "Outstanding Fine: $" << un->data.fine << "\n";
    }

    void calculateFinesForAllUsers() {
        users.calculateFinesAndDisplay();
    }

    void listAllBooks() { 
        books.displayAll(); 
    }
    void listAllUsers() { 
        users.displayAll(); 
    }
};

void printMenu() {
    cout << "\n--- Library Menu ---\n";
    cout << "1. Add Book\n";
    cout << "2. Register User\n";
    cout << "3. Borrow Book\n";
    cout << "4. Return Book\n";
    cout << "5. Update Book Information\n";
    cout << "6. Delete Book\n";
    cout << "7. Find Most Demanded Book\n";
    cout << "8. Display User Record\n";
    cout << "9. Calculate Fines for All Users\n";
    cout << "10. List All Books\n";
    cout << "11. List All Users\n";
    cout << "0. Exit\n";
    cout << "Choose: ";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Library lib;
    //lib.wireUp(); // placeholder

    bool running = true;
    while (running) {
        printMenu();
        int choice;
        if (!(cin >> choice)) break;
        cin.ignore();
        switch (choice) {
        case 1: lib.addBook(); break;
        case 2: lib.registerUser(); break;
        case 3: lib.borrowBook(); break;
        case 4: lib.returnBook(); break;
        case 5: lib.updateBookInfo(); break;
        case 6: lib.deleteBook(); break;
        case 7: lib.findMostDemanded(); break;
        case 8: lib.displayUserRecord(); break;
        case 9: lib.calculateFinesForAllUsers(); break;
        case 10: lib.listAllBooks(); break;
        case 11: lib.listAllUsers(); break;
        case 0: running = false; break;
        default: cout << "Invalid choice.\n"; break;
        }
    }

    cout << "Exiting Library System. Goodbye.\n";
    return 0;
}
