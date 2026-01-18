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

    Transaction* find(int id) {
        Transaction* temp = head;
        while (temp) {
            if (temp->tid == id) return temp;
            temp = temp->next;
        }
        return NULL;
    }

    void showAll() {
        Transaction* temp = head;
        while (temp) {
            cout << "Transaction ID: " << temp->tid
                 << " Amount: " << temp->amount << endl;
            temp = temp->next;
        }
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

    void borrow(int tid, int amt, int b, int l, int day, int r, float f) {
        User* borrower = users.getUser(b);
        User* lender = users.getUser(l);

        if (!borrower || !lender || !borrower->active || !lender->active) {
            cout << "Invalid users" << endl;
            return;
        }

        Transaction* t = new Transaction(tid, amt, b, l, day, r, f);
        borrower->loans.add(t);
        lender->payments.add(t);
        allTransactions.add(t);
    }

    void returnLoan(int tid, int day) {
        Transaction* t = allTransactions.find(tid);
        if (!t || t->isReturned) {
            cout << "Invalid transaction" << endl;
            return;
        }

        int total = t->amount;
        if (day > t->borrowDay + t->returnDays) {
            total += total * (t->finePercent / 100);
        }

        t->isReturned = true;
        cout << "Returned Amount: " << total << endl;
    }

    void transferLoan(int tid, int newBorrower) {
        Transaction* t = allTransactions.find(tid);
        User* u = users.getUser(newBorrower);
        if (!t || !u) return;

        t->borrower = newBorrower;
        u->loans.add(t);
    }

    void personSummary(int id) {
        User* u = users.getUser(id);
        if (!u) return;

        cout << "Loans:" << endl;
        u->loans.showAll();
        cout << "Payments:" << endl;
        u->payments.showAll();
    }

    void transactionBetween(int p1, int p2) {
        Transaction* t = allTransactions.head;
        while (t) {
            if ((t->borrower == p1 && t->lender == p2) ||
                (t->borrower == p2 && t->lender == p1)) {
                cout << "Transaction ID: " << t->tid << endl;
            }
            t = t->next;
        }
    }

    void systemSummary() {
        allTransactions.showAll();
    }
};

int main() {
    LoanSystem sys;

    // USERS
    sys.addUser(10, "Ali");
    sys.addUser(20, "Ahmed");
    sys.addUser(30, "Sara");
    sys.addUser(40, "Usman");

    cout << endl << "Users Added " << endl;
    sys.users.listUsers();

    // BORROW TRANSACTIONS
    // Ali borrows from Ahmed
    sys.borrow(1001, 5000, 10, 20, 1, 10, 5);

    // Sara borrows from Ali
    sys.borrow(1002, 3000, 30, 10, 2, 5, 3);

    // Usman borrows from Ahmed
    sys.borrow(1003, 8000, 40, 20, 3, 7, 4);

    cout << endl << "Transactions After Borrowing " << endl;
    sys.systemSummary();

    // RETURN LOANS
    cout << endl << "Returning Loans " << endl;

    // On-time return (no fine)
    sys.returnLoan(1001, 8);

    // Late return (fine applied)
    sys.returnLoan(1002, 15);

    // TRANSFER LOAN
    cout << endl << "Loan Transfer " << endl;

    // Ali takes responsibility of Usman's loan
    sys.transferLoan(1003, 10);

    // PERSON ACCOUNT SUMMARY
    cout << endl << "Account Summary: Ali " << endl;
    sys.personSummary(10);

    cout << endl << "Account Summary: Ahmed " << endl;
    sys.personSummary(20);

    cout << endl << "Account Summary: Sara " << endl;
    sys.personSummary(30);

    // TRANSACTIONS BETWEEN TWO USERS
    cout << endl << "Transactions Between Ali and Ahmed " << endl;
    sys.transactionBetween(10, 20);

    // SYSTEM SUMMARY
    cout << endl << "Complete System Transaction Summary " << endl;
    sys.systemSummary();


    return 0;
}
