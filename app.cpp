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

    // Separate pointers for separate lists
    Transaction* nextLoan;
    Transaction* nextPayment;
    Transaction* nextAll;

    Transaction(int id, int amt, int b, int l, int bd, int rd, float f) {
        tid = id;
        amount = amt;
        borrower = b;
        lender = l;
        borrowDay = bd;
        returnDays = rd;
        finePercent = f;
        isReturned = false;

        nextLoan = nextPayment = nextAll = NULL;
    }
};

class TransactionList {
public:
    Transaction* head;
    int type; // 0 = loans, 1 = payments, 2 = all

    TransactionList(int t = 2) {
        head = NULL;
        type = t;
    }

    Transaction* next(Transaction* t) {
        if (type == 0) return t->nextLoan;
        if (type == 1) return t->nextPayment;
        return t->nextAll;
    }

    void setNext(Transaction* t, Transaction* n) {
        if (type == 0) t->nextLoan = n;
        else if (type == 1) t->nextPayment = n;
        else t->nextAll = n;
    }

    void add(Transaction* t) {
        setNext(t, head);
        head = t;
    }

    Transaction* find(int id) {
        Transaction* temp = head;
        while (temp) {
            if (temp->tid == id) return temp;
            temp = next(temp);
        }
        return NULL;
    }

    void remove(int id) {
        Transaction* curr = head;
        Transaction* prev = NULL;

        while (curr) {
            if (curr->tid == id) {
                if (!prev)
                    head = next(curr);
                else
                    setNext(prev, next(curr));
                return;
            }
            prev = curr;
            curr = next(curr);
        }
    }

    void showAll() {
        Transaction* temp = head;
        while (temp) {
            if (!temp->isReturned) {
                cout << "Transaction ID: " << temp->tid
                     << " Amount: " << temp->amount << endl;
            }
            temp = next(temp);
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

    User(int i, string n) : loans(0), payments(1) {
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
        if (!getUser(id))
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

    LoanSystem() : allTransactions(2) {}

    void addUser(int id, string name) {
        users.addUser(id, name);
    }

    void borrow(int tid, int amt, int b, int l, int day, int r, float f) {
        if (allTransactions.find(tid)) {
            cout << "Transaction ID already exists" << endl;
            return;
        }

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

        float total = t->amount;
        if (day > t->borrowDay + t->returnDays) {
            total += total * (t->finePercent / 100.0f);
        }

        t->isReturned = true;
        cout << "Returned Amount: " << total << endl;
    }

    void transferLoan(int tid, int newBorrower) {
        Transaction* t = allTransactions.find(tid);
        if (!t || t->isReturned) return;

        User* oldB = users.getUser(t->borrower);
        User* newB = users.getUser(newBorrower);

        if (!newB || !newB->active) return;

        oldB->loans.remove(tid);
        t->borrower = newBorrower;
        newB->loans.add(t);
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
            t = t->nextAll;
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
