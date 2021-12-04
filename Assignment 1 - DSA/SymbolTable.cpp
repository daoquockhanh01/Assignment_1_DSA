#include "SymbolTable.h"

void SymbolTable::run(string filename) {
    cout << "Hello Bitch\n";
    cout << "This is Assignment 1 - DSA, used Linked List\n";
    ifstream ip;
    ip.open(filename, ios::in);
    while (!ip.eof()) {
        string s, code, name, type;
        getline(ip, s);
        checkInstruction(s);
        divideString(s, code, name, type);
        if (code == "INSERT") {
            SymbolTable::SymbolInsert(s);
            cout << "success" << endl;
        }
        else if (code == "ASSIGN") {
            SymbolTable::SymbolAssign(s);
            cout << "success" << endl;
        }
        else if (code == "BEGIN") {
            SymbolTable::SymbolOpen(s);
        }
        else if (code == "END") {
            SymbolTable::SymbolClose(s);
        }
        else if (code == "LOOKUP") {
            int ans = SymbolTable::SymbolLookup(s);
            cout << to_string(ans) << endl;
        }
        else if (code == "PRINT") {
            bool done = false;
            SymbolTable::SymbolPrint(s, done);
            if (done == false) cout << endl;
        }
        else if (code == "RPRINT") {
            bool done = false;
            SymbolTable::SymbolRprint(s, done);
            if (done == false) cout << endl;
        }
    }
    SymbolTable::CheckClose();
    ip.close();
}
