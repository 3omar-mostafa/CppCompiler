#ifndef __SYMBOL_TABLE__
#define __SYMBOL_TABLE__

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include "../utils/enums.h"

struct EntryInfo
{
    DataType type;
    EntryType entryType;
    std::vector<DataType> paramsTypes; // params of function

    EntryInfo() = default;

    explicit EntryInfo(DataType type, EntryType entryType = TYPE_VAR, const std::vector<DataType> &paramsTypes = {})
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
    // static SymbolTable *symbolTable;

public:
    // static SymbolTable *getInstance();
    SymbolTable(){};

    bool insert(const std::string &name, DataType type, EntryType entryType = TYPE_VAR,
                const std::vector<DataType> &paramsTypes = {});

    bool update();

    bool lookup(const std::string &name, EntryInfo &info);
};

#endif //__SYMBOL_TABLE__