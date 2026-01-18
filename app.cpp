#include <iostream>
#include <string>
using namespace std;


class Transaction {
public:
    int tid;
    int amount;
    int borrower;
    int lender;
    int borrowDay;
    int returnDays;
    float finePercent;
    bool isReturned;

    Transaction* next;

    Transaction(int id, int amt, int b, int l, int bd, int rd, float f) {
        tid = id;
        amount = amt;
        borrower = b;
        lender = l;
        borrowDay = bd;
        returnDays = rd;
        finePercent = f;
        isReturned = false;
        next = NULL;
    }
};


class TransactionList {
public:
    Transaction* head;

    TransactionList() { head = NULL; }

    void add(Transaction* t) {
        t->next = head;
        head = t;
    }
};


class User {
public:
    int id;
    string name;
    bool active;

    TransactionList loans;
    TransactionList payments;

    User* left;
    User* right;

    User(int i, string n) {
        id = i;
        name = n;
        active = true;
        left = right = NULL;
    }
};

class UserBST {
public:
    User* root;

    UserBST() { root = NULL; }

    User* insert(User* node, int id, string name) {
        if (!node) return new User(id, name);
        if (id < node->id)
            node->left = insert(node->left, id, name);
        else if (id > node->id)
            node->right = insert(node->right, id, name);
        return node;
    }

    void addUser(int id, string name) {
        root = insert(root, id, name);
    }

    User* search(User* node, int id) {
        if (!node || node->id == id) return node;
        if (id < node->id) return search(node->left, id);
        return search(node->right, id);
    }

    User* getUser(int id) {
        return search(root, id);
    }

    void inorder(User* node) {
        if (!node) return;
        inorder(node->left);
        if (node->active)
            cout << node->id << " - " << node->name << endl;
        inorder(node->right);
    }

    void listUsers() {
        inorder(root);
    }
};


class LoanSystem {
public:
    UserBST users;
    TransactionList allTransactions;

    void addUser(int id, string name) {
        users.addUser(id, name);
    }

    void personSummary(int id) {
        User* u = users.getUser(id);
        if (!u) return;

        cout << "Loans:" << endl;
        u->loans.showAll();
        cout << "Payments:" << endl;
        u->payments.showAll();
    }

    void systemSummary() {
        allTransactions.showAll();
    }
};

int main() {
    return 0;
}
