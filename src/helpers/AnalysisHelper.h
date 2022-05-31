#ifndef ANALYSIS_HELPER
#define ANALYSIS_HELPER

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "../nodes/Node.h"
#include "../utils/enums.h"
using namespace std;

class Scope
{
public:
    ScopeType type;    // The type of the scope
    Node *ptr;         // A pointer to the node of the scope
    SymbolTable table; // The symbol table of this scope

    Scope(ScopeType type, Node *ptr = NULL)
    {
        this->type = type;
        this->ptr = ptr;
    }
};

class AnalysisHelper
{
    string srcCodeFile;
    vector<string> srcCode;
    vector<Scope *> scopes;
    string symbTableStr;

    void readSourceCode()
    {
        ifstream fin(srcCodeFile);

        if (!fin.is_open())
        {
            return;
        }

        string line;
        while (getline(fin, line))
        {
            srcCode.push_back(Utils::replaceTabsWithSpaces(line));
        }

        fin.close();
    }

    void initSymbolTableString()
    {
        string tmp;
        tmp += "+---------+---------------------+----------------+-------+\n";
        tmp += "| scope   | identifier          | type           | used  |\n";
        tmp += "+---------+---------------------+----------------+-------+\n";
        symbTableStr = tmp + symbTableStr;
    }

    void updateSymbolTableString(SymbolTable t, int scope)
    {
        string scopeStr;
        switch (scope)
        {
        case SCOPE_LOOP:
            scopeStr = "loop";
            break;
        case SCOPE_IF:
            scopeStr = "if";
            break;
        case SCOPE_SWITCH:
            scopeStr = "switch";
            break;
        case SCOPE_FUNCTION:
            scopeStr = "func";
            break;
        case SCOPE_BLOCK:
            scopeStr = "block";
            break;
        default:
            scopeStr = "global";
            break;
        }

        stringstream ss;
        for (auto &it : t.getTable())
        {
            EntryInfo symbol = it.second;

            ss << "| " << left << setw(8) << scopeStr;
            ss << "| " << left << setw(20) << it.first;
            ss << "| " << left << setw(15) << Utils::typeToQuad(symbol.type);
            ss << "| " << left << setw(6) << symbol.used << "|\n";
            ss << "+---------+---------------------+----------------+-------+\n";
        }
        symbTableStr = ss.str() + symbTableStr;
    }

public:
    AnalysisHelper(const string &srcCodeFile)
    {
        this->srcCodeFile = srcCodeFile;
        readSourceCode();
    }

    void pushScope(ScopeType type, Node *scopePtr = NULL)
    {
        scopes.push_back(new Scope(type, scopePtr));
    }

    void popScope()
    {
        Scope *scope = scopes.back();
        scopes.pop_back();

        for (auto &it : scope->table.getTable())
        {
            EntryInfo symbol = it.second;

            if (symbol.used <= 0)
            {
                log("the value of variable '" + it.first + "' is never used", symbol.loc, "warning");
            }
        }

        updateSymbolTableString(scope->table, scopes.size() == 0 ? 0 : scope->type);

        delete scope;
    }

    bool isGlobalScope()
    {
        return scopes.size() == 1;
    }

    bool hasSwitchScope()
    {
        for (int i = (int)scopes.size() - 1; i >= 0; --i)
        {
            if (scopes[i]->type == SCOPE_SWITCH)
            {
                return true;
            }
        }

        return false;
    }

    bool hasBreakScope()
    {
        for (int i = (int)scopes.size() - 1; i >= 0; --i)
        {
            if (scopes[i]->type == SCOPE_LOOP || scopes[i]->type == SCOPE_SWITCH)
            {
                return true;
            }
        }

        return false;
    }

    bool hasLoopScope()
    {
        for (int i = (int)scopes.size() - 1; i >= 0; --i)
        {
            if (scopes[i]->type == SCOPE_LOOP)
            {
                return true;
            }
        }

        return false;
    }

    bool addSymbol(yy::location loc, const string &name, DataType type, EntryType entryType = TYPE_VAR, const vector<DataType> &paramsTypes = {}, int used = 0, bool initialized = false)
    {
        SymbolTable &symbolTable = scopes.back()->table;

        if (!symbolTable.insert(loc, name, type, entryType, paramsTypes, used, initialized))
        {
            return false;
        }

        return true;
    }

    bool lookup(const string &name, EntryInfo *&info)
    {
        for (int i = (int)scopes.size() - 1; i >= 0; --i)
        {
            if (scopes[i]->table.lookup(name, info))
            {
                return true;
            }
        }

        return false;
    }

    void log(const string &message, yy::location loc, const string &logType)
    {

        fprintf(stdout, "%s:%d:%d: %s: %s\n", srcCodeFile.c_str(), loc.begin.line, loc.begin.column, logType.c_str(), message.c_str());
        fprintf(stdout, "%s\n", srcCode[loc.begin.line - 1].c_str());
        fprintf(stdout, "%*s", loc.begin.column, "^");

        if (loc.begin.line > 1)
        {
            fprintf(stdout, "%s", string(loc.begin.line - 1, '~').c_str());
        }
        fprintf(stdout, "\n");
    }

    string getSymbolTableString()
    {
        initSymbolTableString();
        return symbTableStr;
    }
};

#endif