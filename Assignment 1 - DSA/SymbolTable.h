#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

//SYMBOL TABLE (SINGLE LINKED LIST STRUCTURE)
class SymbolTable {
public:
    class Symbol {
    public:
        string id;
        string type;
        int block;
        string value;
        Symbol* next;
    public:
        Symbol() {
            this->id = "";
            this->type = "";
            this->value = "";
            this->block = 0;
            this->next = NULL;
        }
        Symbol(string id, string type, int block = 0, string value = "", Symbol* next = NULL) {
            this->id = id;
            this->type = type;
            this->block = block;
            this->value = value;
            this->next = next;
        }
    };
private:
    Symbol* head;
    Symbol* tail;
    int size{ 0 };
    int cBlock{ 0 };
public:
    //******************************************************
    //CONSTRUCTOR AND SUPPORT FUNCTIONS*********************
    //******************************************************
    SymbolTable() {
        this->head = NULL;
        this->tail = NULL;
        this->size = 0;
        this->cBlock = 0;
    }
    void run(string filename);
    string Word(string& s) {
        if (s.size() == 0) {
            return "";
        }
        if (s[0] == ' ') {
            s.erase(0, 1);
            return "";
        }
        string sav = "1";
        sav[0] = s[0];
        return sav + Word(s.erase(0, 1));
    }
    string re(string s) {
        string sav = Word(s);
        if (s.size() == 0) return sav;
        return re(s) + " " + sav;
    }
    int countSpace(string s) {
        int ans = 0;
        for (unsigned int i = 0; i < s.size(); i++)
            if (s[i] == ' ') ans++;
        return ans;
    }
    void divideString(string s, string& code, string& name, string& typ) {
        int pos = 0, space = countSpace(s);
        while (s[pos] != ' ' && s[pos] != '\n') {
            code += s[pos];
            pos++;
            if ((unsigned int)pos > s.size() - 1) break;
        }
        if (space >= 1) {
            pos++;
            while (s[pos] != ' ' && s[pos] != '\n') {
                name += s[pos];
                pos++;
                if ((unsigned int)pos > s.size() - 1) break;
            }
        }
        if (space >= 2) {
            pos++;
            if (s[pos] == char(39)) {
                typ += s[pos];
                pos++;
                while (s[pos] != char(39)) {
                    typ += s[pos];
                    pos++;
                    if ((unsigned int)pos > s.size() - 1) break;
                }
                typ += char(39);
            }
            else {
                while (s[pos] != ' ' && s[pos] != '\n') {
                    typ += s[pos];
                    pos++;
                    if ((unsigned int)pos > s.size() - 1) break;
                }
            }
        }
    }
    //******************************************************
    //BASIC METHODS OF LINKED LIST**************************
    //******************************************************
    Symbol* jump(Symbol* run, int pos) {
        for (int i = 0; i < pos; i++)
            run = run->next;
        return run;
    }
    void add(string id, string type) {
        int block = this->cBlock;
        Symbol* tmp = new Symbol(id, type, block);
        if (this->size == 0) {
            this->head = tmp;
            this->tail = tmp;
            this->size++;
        }
        else {
            this->tail->next = tmp;
            this->tail = tmp;
            this->size++;
        }
    }
    //CHECK THE EXISTENCE OF A NAME IN A BLOCK
    bool containName(string ids, int blocks = 0) {
        Symbol* run = this->head;
        while (run) {
            if (run->id == ids && run->block == blocks) return true;
            run = run->next;
        }
        return false;
    }
    //CHECK THE EXISTENCE OF A NAME IN BIGGER BLOCKS
    bool containInLarge(string ids, int block = 0) {
        if (block == this->cBlock) return false;
        block++;
        bool done = containName(ids, block);
        if (done == true) return true;
        while (done == false && block <= this->cBlock) {
            block++;
            if (block > this->cBlock) break;
            done = containName(ids, block);
            if (done == true) return true;
        }
        return false;
    }
    //RETURN INDEX OF A NAME IN A BLOCK
    int indexOf(string name, int vblock = 0) {
        if (containName(name, vblock) == false) 
            return -1;
        Symbol* run = this->head;
        int ans = 0;
        while (run) {
            if (run->id == name && run->block == vblock) break;
            ans++;
            run = run->next;
        }
        return ans;
    }
    void removeAt(int pos) {
        if (this->size == 0) return;
        if (pos < 0 || pos > this->size - 1) throw std::out_of_range("Out of Range");
        if (this->size == 1) {
            Symbol* sav = this->head;
            this->head = NULL;
            this->tail = NULL;
            delete sav;
            this->size = 0;
        }
        else if (pos == 0) {
            Symbol* sav = this->head;
            this->head = sav->next;
            delete sav;
            this->size--;
        }
        else if (pos == this->size - 1) {
            Symbol* run = this->head;
            run = jump(run, this->size - 2);
            Symbol* sav = run->next;
            this->tail = run;
            this->tail->next = NULL;
            delete sav;
            this->size--;
        }
        else {
            Symbol* run = this->head;
            run = jump(run, pos - 1);
            Symbol* sav = run->next;
            run->next = sav->next;
            delete sav;
            this->size--;
        }
    }
    //REMOVE ALL ITEMS IN BLOCK
    void removeBlock(int fBlock) {
        Symbol* run = this->head;
        int count = 0;
        while (run) {
            if (run->block == fBlock) break;
            run = run->next;
            count++;
        }
        while (run != NULL) {
            if (run->block != fBlock) break;
            run = run->next;
            removeAt(count);
        }
    }
    void clear() {
        Symbol* run = this->head;
        while (run) {
            Symbol* sav = run->next;
            delete run;
            run = sav;
        }
        this->head = NULL;
        this->tail = NULL;
        this->size = 0;
    }
    //******************************************************
    //REQUIRED FUNCTIONS************************************
    //******************************************************
    //CHECK THE INSTRUCTION OF NAME
    bool checkName(string s) {
        if (s[0] < 'a' || s[0] > 'z') return false;
        for (unsigned int i = 0; i < s.size(); i++)
            if ((s[i] < 'a' || s[i] > 'z') && (s[i] < 'A' || s[i] > 'Z') && (s[i] < '0' || s[i] > '9') && s[i] != '_') return false;
        return true;
    }
    //CHECK THE INSTRUCION OF LINE
    void checkInstruction(string s) {
        int space = countSpace(s);
        string code, name, typ;
        divideString(s, code, name, typ);
        //Check Code
        if (code != "INSERT" && code != "ASSIGN" && code != "BEGIN" && code != "END" && code != "LOOKUP" && code != "PRINT" && code != "RPRINT") 
            throw InvalidInstruction(s);
        //Check Name
        if (space >= 1) {
            if (checkName(name) == false) {
                cout << "Hey Name" << endl;
                throw InvalidInstruction(s);
            }
        }
        return;
    }
    //INSERT
    void SymbolInsert(string s) {
        string code, name, typ;
        divideString(s, code, name, typ);
        if (typ != "number" && typ != "string") throw InvalidInstruction(s);
        if (SymbolTable::containName(name, this->cBlock))
            throw Redeclared(s);
        SymbolTable::add(name, typ);
        //HOÀN THÀNH
    }
    bool checkValue(string s) {
        if (s[0] >= '0' && s[0] <= '9') {
            for (unsigned int i = 0; i < s.size(); i++)
                if (s[i] < '0' || s[i] >'9') return false;
        }
        else if (s[0] == char(39)) {
            if (s[s.size() - 1] != char(39)) return false;
            for (unsigned int i = 1; i < s.size() - 1; i++) {
                if ((s[i] < '0' || s[i] > '9') && (s[i] < 'a' || s[i] > 'z') && (s[i] < 'A' || s[i] > 'Z') && s[i] != ' ') return false;
            }
        }
        else return checkName(s);
        return true;
    }
    //ASSIGN
    void SymbolAssign(string s) {
        int fBlock = this->cBlock;
        string code, name, val;
        divideString(s, code, name, val);
        if (checkValue(val) == false) {
            throw InvalidInstruction(s);
        }
        while (containName(name, fBlock) == false) {
            fBlock--;
            if (fBlock < 0) throw Undeclared(s);
            if (containName(name, fBlock) == true) break;
        }
        Symbol* cap = this->head;
        cap = jump(cap, indexOf(name, fBlock)); 
        if (cap->type == "number") {
            if (val[0] == char(39)) throw TypeMismatch(s);
            else if (val[0] < '0' || val[0] > '9') {
                int gBlock = this->cBlock;
                while (containName(val, gBlock) == false) {
                    gBlock--;
                    if (gBlock < 0) throw Undeclared(s);
                    if (containName(val, gBlock) == true) break;
                }
                if (containName(val, gBlock) == true) {
                    Symbol* ex = this->head;
                    ex = jump(ex, indexOf(val, gBlock));
                    if (ex->type == "string") throw TypeMismatch(s);
                    else cap->value = ex->value;
                }
            }
            else if (val[0] >= '0' && val[0] <= '9') {
                cap->value = val;
            }
        }
        else if (cap->type == "string") {
            if (val[0] >= '0' && val[0] <= '9') {
                throw TypeMismatch(s);
            }
            else if (val[0] == char(39)) {
                if (val[val.size() - 1] != char(39)) throw InvalidInstruction(s);
                else
                    cap->value = val;
            }
            else {
                int gBlock = this->cBlock;
                while (containName(val, gBlock) == false) {
                    gBlock--;
                    if (gBlock < 0) {
                        throw Undeclared(s);
                    }
                    if (containName(val, gBlock) == true) break;
                }
                if (containName(val, gBlock) == true) {
                    Symbol* ex = this->head;
                    ex = jump(ex, indexOf(val, gBlock));
                    if (ex->type == "number") throw TypeMismatch(s);
                    else cap->value = ex->value;
                }
            }
        }
    }
    //OPEN
    void SymbolOpen(string s) {
        this->cBlock++;
    }
    //CLOSE
    void SymbolClose(string s) {
        if (this->cBlock == 0) throw UnknownBlock();
        removeBlock(this->cBlock);
        this->cBlock--;
    }
    //LOOKUP
    int SymbolLookup(string s) {
        int fBlock = this->cBlock;
        string code, name, none;
        divideString(s, code, name, none);
        while (containName(name, fBlock) == false && fBlock >= 0) {
            fBlock--;
        }
        if (fBlock >= 0) return fBlock;
        else throw Undeclared(s);
    }
    //PRINT
    void SymbolPrint(string s, bool& done) {
        if (this->size == 0) {
            done = true;
            return;
        }
        else {
            Symbol* run = this->head;
            while (run) {
                if (containInLarge(run->id, run->block) == false) {
                    cout << run->id << "//" << to_string(run->block);
                    if (run->next != NULL) cout << " ";
                }
                run = run->next;
            }
        }
    }
    //RPRINT
    void SymbolRprint(string s, bool& done) {
        if (this->size == 0) {
            done = true;
            return;
        }
        else {
            string ans = "";
            Symbol* run = this->head;
            while (run) {
                if (containInLarge(run->id, run->block) == false) {
                    ans = ans + run->id + "//" + to_string(run->block);
                    if (run->next != NULL) ans += " ";
                }
                run = run->next;
            }
            ans = re(ans); 
            cout << ans;
        }
    }
    void CheckClose() {
        if (this->cBlock != 0) throw UnclosedBlock(cBlock);
        return;
    }
    //******************************************************
    //DESTRUCTOR********************************************
    //******************************************************
    ~SymbolTable() {
        SymbolTable::clear();
    }
};

#endif