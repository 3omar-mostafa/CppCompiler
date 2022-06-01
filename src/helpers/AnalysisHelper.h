#ifndef ANALYSIS_HELPER
#define ANALYSIS_HELPER

#include <vector>
#include <string>
#include <fstream>

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
    std::string symbolTableRepresentation;
    static inline int indent = 0;


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

public:
    AnalysisHelper(const string &srcCodeFile)
    {
        this->srcCodeFile = srcCodeFile;
        readSourceCode();
    }

    const string& getSymbolTableRepresentation() const
    {
        return symbolTableRepresentation;
    }

    void pushScope(ScopeType type, Node* scopePtr = nullptr)
    {
        scopes.push_back(new Scope(type, scopePtr));

        std::string scope_str = (type == SCOPE_BLOCK) ? "{" : "====== Begin " + Utils::scopeToString(type) + " ======";
        symbolTableRepresentation += std::string(indent, ' ') + scope_str + "\n";
        indent += 4;
    }

    void popScope()
    {
        Scope* scope = scopes.back();
        scopes.pop_back();

        for (auto &it : scope->table.getTable())
        {
            EntryInfo symbol = it.second;

            if (symbol.used <= 0)
            {
                log("the value of variable '" + it.first + "' is never used", symbol.loc, "warning");
            }
        }

        indent -= 4;
        std::string scope_str = (scope->type == SCOPE_BLOCK) ? "}" : "====== End " + Utils::scopeToString(scope->type) + " ======";
        symbolTableRepresentation += std::string(indent, ' ') + scope_str + "\n";

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

    bool addSymbol(yy::location loc, const string& name, DataType type, EntryType entryType = TYPE_VAR,
                   const vector<DataType>& paramsTypes = {}, int used = 0, bool initialized = false)
    {
        SymbolTable& symbolTable = scopes.back()->table;

        if (!symbolTable.insert(loc, name, type, entryType, paramsTypes, used, initialized))
        {
            return false;
        }

        EntryInfo info = EntryInfo(loc, type, entryType, paramsTypes, used, initialized);
        symbolTableRepresentation += std::string(indent, ' ') + name + ": " + info.to_string() + "\n";
//        str += std::string(indent, ' ') + identifier + ": " + entryInfo.to_string(std::string(indent + 4, ' ')) + "\n";
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
    }
};

#endif