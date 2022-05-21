#ifndef SYMBOLTABLE
#define SYMBOLTABLE

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include "../utils/enums.h"


struct EntryInfo
{
    DataType type;
    EntryType entryType;
    std::vector<DataType> paramsTypes;

    EntryInfo() = default;

    explicit EntryInfo(DataType type, EntryType entryType = TYPE_VAR, const std::vector<DataType>& paramsTypes = {})
    {
        this->type = type;
        this->entryType = entryType;
        this->paramsTypes = paramsTypes;
    }
};

class SymbolTable
{
private:
    std::unordered_map<std::string, EntryInfo> table;
    static SymbolTable* symbolTable;

    SymbolTable() = default;

public:
    static SymbolTable* getInstance();

    bool insert(const std::string& name, DataType type, EntryType entryType = TYPE_VAR,
                const std::vector<DataType>& paramsTypes = {});

    bool update();

    bool lookup(const std::string& name, EntryInfo& info);
};

SymbolTable* SymbolTable::symbolTable = nullptr;

#endif