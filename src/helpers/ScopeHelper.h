#ifndef ANALYSIS_HELPER
#define ANALYSIS_HELPER

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "../nodes/Node.h"
#include "../symbolTable/SymbolTable.h"
#include "../utils/enums.h"
#include "../utils/utils.h"

class FunctionDeclarationNode;

using namespace std;

class Scope
{
public:
    ScopeType type;    // The type of the scope
    Node *ptr;         // A pointer to the node of the scope
    SymbolTable table; // The symbol table of this scope

    explicit Scope(ScopeType type, Node* ptr = nullptr)
    {
        this->type = type;
        this->ptr = ptr;
    }
};

class ScopeHelper
{
    vector<Scope *> scopes;
    string symbolTableStr;

    void initSymbolTableString()
    {
        string tmp;
        tmp += "+---------+---------------------+----------------+-------+\n";
        tmp += "| scope   | identifier          | type           | used  |\n";
        tmp += "+---------+---------------------+----------------+-------+\n";
        symbolTableStr = tmp + symbolTableStr;
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
        for (auto& [name, info] : t.getTable())
        {
            ss << "| " << left << setw(8) << scopeStr;
            ss << "| " << left << setw(20) << name;
            ss << "| " << left << setw(15) << Utils::typeToQuad(info.type);
            ss << "| " << left << setw(6) << info.used << "|\n";
            ss << "+---------+---------------------+----------------+-------+\n";
        }
        symbolTableStr = ss.str() + symbolTableStr;
    }

    ScopeHelper() = default;

public:
    bool declareParams = false;

    static ScopeHelper* getInstance()
    {
        static ScopeHelper instance;
        return &instance;
    }

    void pushScope(ScopeType type, Node* scopePtr = nullptr)
    {
        scopes.push_back(new Scope(type, scopePtr));
    }

    void popScope()
    {
        Scope *scope = scopes.back();
        scopes.pop_back();

        for (auto& [name, info]: scope->table.getTable())
        {
            if (info.used <= 0)
            {
                if (info.entryType == TYPE_VAR)
                {
                    Utils::log("the value of variable '" + name + "' is never used", info.loc, "warning");
                } else if (info.entryType == TYPE_FUNC and name != "main")
                {
                    Utils::log("the function '" + name + "' is never called", info.loc, "warning");
                }
            }
        }

        updateSymbolTableString(scope->table, scopes.empty() ? 0 : scope->type);

        delete scope;
    }

    bool isInsideGlobalScope()
    {
        return scopes.size() == 1;
    }

    bool isInsideSwitchScope()
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

    FunctionDeclarationNode* isInsideFunctionScope()
    {
        for (int i = (int)scopes.size() - 1; i >= 0; --i)
        {
            if (scopes[i]->type == SCOPE_FUNCTION)
            {
                return reinterpret_cast<FunctionDeclarationNode*>(scopes[i]->ptr);
            }
        }

        return nullptr;
    }

    bool isInsideLoopScope()
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

    EntryInfo* addSymbol(yy::location loc, const string& name, DataType type, EntryType entryType = TYPE_VAR,
              const vector<DataType>& paramsTypes = {}, int used = 0, bool initialized = false)
    {
        SymbolTable& symbolTable = scopes.back()->table;

        auto info = symbolTable.insert(loc, name, type, entryType, paramsTypes, used, initialized);
        if (!info)
        {
            return nullptr;
        }

        return info;
    }

    EntryInfo* lookup(const string &name)
    {
        for (int i = (int)scopes.size() - 1; i >= 0; --i)
        {
            if (auto info = scopes[i]->table.lookup(name))
            {
                return info;
            }
        }

        return nullptr;
    }

    string getSymbolTableString()
    {
        initSymbolTableString();
        return symbolTableStr;
    }
};

#endif