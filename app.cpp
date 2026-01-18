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
