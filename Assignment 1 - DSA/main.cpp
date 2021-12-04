#include "SymbolTable.h"
#include "SymbolTable.cpp"
using namespace std;

void test(string filename)
{
    SymbolTable* st = new SymbolTable();
    try
    {
        st->run(filename);
    }
    catch (exception& e)
    {
        cout << e.what();
    }
    delete st;
}

void validSubmittedFiles(string filename, string* allowedIncludingFiles, int numOfAllowedIncludingFiles = 1)
{
    ifstream infile(filename);
    string line;
    while (getline(infile, line))
    {
        if (line.find("#include") != string::npos)
        {
            for (int i = 0; i < numOfAllowedIncludingFiles; i++)
            {
                if (line.find(allowedIncludingFiles[i]) != string::npos)
                    continue;
                cout << "Use disallowed file: " + line << endl;
                exit(1);
            }
        }
    }
    infile.close();
}

int main(int argc, char** argv)
{
    test("test2.txt");
    return 0;
}