#include <iostream>
#include <string>
using namespace std;


class Graph {
private:
    string users[100];      
    int adj[100][100];      
    int userCount;          
    int maxUsers = 100;   // maximum number of users

public:
    Graph() {
        userCount = 0;
        for (int i = 0; i < maxUsers; i++)
            for (int j = 0; j < maxUsers; j++)
                adj[i][j] = 0;
    }

    // 1. Add User
    void addUser(string name) {
        if (userCount == maxUsers) {
            cout << "Limit reached!\n";
            return;
        }
        users[userCount] = name;
        cout << "User Added with ID: " << userCount << endl;
        userCount++;
    }

    // 2. Add Friendship
    void addFriendship(int a, int b) {
        if (a >= userCount || b >= userCount || a < 0 || b < 0) {
            cout << "Invalid IDs!\n";
            return;
        }
        adj[a][b] = adj[b][a] = 1;
        cout << "Friendship added!\n";
    }

    // 3. Display Friends
    void displayFriends(int id) {
        if (id >= userCount || id < 0) {
            cout << "Invalid user ID!\n";
            return;
        }

        cout << users[id] << "'s Friends: ";
        bool hasFriends = false;

        for (int i = 0; i < userCount; i++) {
            if (adj[id][i] == 1) {
                cout << users[i] << " ";
                hasFriends = true;
            }
        }

        if (!hasFriends) 
            cout << "No friends.";
        cout << endl;
    }

    // DFS helper
    void dfsUtil(int id, bool visited[]) {
        visited[id] = true;
        cout << users[id] << " ";

        for (int i = 0; i < userCount; i++) {
            if (adj[id][i] == 1 && !visited[i]) {
                dfsUtil(i, visited);
            }
        }
    }

    // 4. DFS Traversal
    void DFS(int start) {
        if (start >= userCount || start < 0) {
            cout << "Invalid ID!\n";
            return;
        }

        bool visited[100] = { false };
        cout << "DFS Traversal: ";
        dfsUtil(start, visited);
        cout << endl;
    }

    // 5. Count Friend Circles
    int countFriendCircles() {
        bool visited[100] = { false };
        int count = 0;

        for (int i = 0; i < userCount; i++) {
            if (!visited[i]) {
                dfsUtil(i, visited);
                count++;
            }
        }
        return count;
    }

    // 6. Popular Users (max degree)
    void popularUsers() {
        int maxFriends = 0;

        // find max
        for (int i = 0; i < userCount; i++) {
            int degree = 0;
            for (int j = 0; j < userCount; j++)
                degree += adj[i][j];

            if (degree > maxFriends)
                maxFriends = degree;
        }

        cout << "Most Popular Users (Friends = " << maxFriends << "): ";
        for (int i = 0; i < userCount; i++) {
            int degree = 0;
            for (int j = 0; j < userCount; j++)
                degree += adj[i][j];

            if (degree == maxFriends)
                cout << users[i] << " ";
        }
        cout << endl;
    }

    // 7. Isolated Users
    void isolatedUsers() {
        cout << "Isolated Users: ";
        bool found = false;

        for (int i = 0; i < userCount; i++) {
            bool hasFriend = false;
            for (int j = 0; j < userCount; j++)
                if (adj[i][j] == 1)
                    hasFriend = true;

            if (!hasFriend) {
                cout << users[i] << " ";
                found = true;
            }
        }
        if (!found) cout << "None";
        cout << endl;
    }

    // 8. Mutual Friends
    void mutualFriends(int a, int b) {
        if (a >= userCount || b >= userCount || a < 0 || b < 0) {
            cout << "Invalid IDs!\n";
            return;
        }

        cout << "Mutual Friends: ";
        bool found = false;

        for (int i = 0; i < userCount; i++) {
            if (adj[a][i] == 1 && adj[b][i] == 1) {
                cout << users[i] << " ";
                found = true;
            }
        }

        if (!found) cout << "None";
        cout << endl;
    }

    // 9. Suggest Friends
    void suggestFriends(int id) {
        cout << "Suggested Friends for " << users[id] << ": ";
        bool suggested = false;

        for (int x = 0; x < userCount; x++) {
            if (x == id) continue;

            if (adj[id][x] == 0) { // not already friend
                for (int f = 0; f < userCount; f++) {
                    if (adj[id][f] == 1 && adj[f][x] == 1) {
                        cout << users[x] << " ";
                        suggested = true;
                        break;
                    }
                }
            }
        }

        if (!suggested) 
            cout << "No suggestions";
        cout << endl;
    }

    // 10. Remove User
    void removeUser(int id) {
        if (id < 0 || id >= userCount) {
            cout << "Invalid ID!\n";
            return;
        }

        // shift rows upward
        for (int i = id; i < userCount - 1; i++) {
            users[i] = users[i + 1];
            for (int j = 0; j < userCount; j++)
                adj[i][j] = adj[i + 1][j];
        }

        // shift columns leftward
        for (int i = 0; i < userCount; i++) {
            for (int j = id; j < userCount - 1; j++)
                adj[i][j] = adj[i][j + 1];
        }

        userCount--;
        cout << "User removed.\n";
    }
};


int main() {
    Graph g;
    int choice;

    while (true) {
        cout << "\n--- Social Network Menu ---\n";
        cout << "1. Add User\n2. Add Friendship\n3. Display Friends\n4. DFS Traversal";
        cout << "\n5. Count Friend Circles\n6. Popular Users\n7. Isolated Users";
        cout << "\n8. Mutual Friends\n9. Suggest Friends\n10. Remove User\n11. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            cout << "Enter name: ";
            cin >> name;
            g.addUser(name);
        }
        else if (choice == 2) {
            int a, b;
            cout << "Enter two user IDs: ";
            cin >> a >> b;
            g.addFriendship(a, b);
        }
        else if (choice == 3) {
            int id;
            cout << "Enter user ID: ";
            cin >> id;
            g.displayFriends(id);
        }
        else if (choice == 4) {
            int start;
            cout << "Enter start ID: ";
            cin >> start;
            g.DFS(start);
        }
        else if (choice == 5) {
            cout << "Friend Circles: " << g.countFriendCircles() << endl;
        }
        else if (choice == 6) {
            g.popularUsers();
        }
        else if (choice == 7) {
            g.isolatedUsers();
        }
        else if (choice == 8) {
            int a, b;
            cout << "Enter two IDs: ";
            cin >> a >> b;
            g.mutualFriends(a, b);
        }
        else if (choice == 9) {
            int id;
            cout << "Enter ID: ";
            cin >> id;
            g.suggestFriends(id);
        }
        else if (choice == 10) {
            int id;
            cout << "Enter ID to remove: ";
            cin >> id;
            g.removeUser(id);
        }
        else if (choice == 11) {
            break;
        }
    }

    return 0;
}
