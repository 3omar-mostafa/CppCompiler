#ifndef VARIABLE_DECLARATION_NODE
#define VARIABLE_DECLARATION_NODE

#include "../../utils/enums.h"
#include "../expressions/IdentifierNode.h"
#include <iostream>

class VariableDeclarationNode : public Node
{
    ExpressionNode *value;
    EntryType entryType;

public:
    DataType type;
    IdentifierNode *identifier;

    VariableDeclarationNode(yy::location loc, DataType type, IdentifierNode *identifier, ExpressionNode *value = nullptr, bool constant = false)
        : Node(loc)
    {
        this->type = type;
        this->identifier = identifier;
        this->value = value;
        this->entryType = constant ? EntryType::TYPE_CONST : EntryType::TYPE_VAR;
    }

    bool analyzeSemantic(bool used = false) override
    {
        auto scopeHelper = ScopeHelper::getInstance();

        if (type == DTYPE_VOID)
        {
            Utils::log("Variable '" + identifier->name + "' declared as void", identifier->loc, "error");
            return false;
        }

        bool initialized = false;

        if (value != nullptr)
        {
            initialized = true;
            if (!value->analyzeSemantic(true))
                return false;

            if (value->type == DTYPE_VOID)
            {
                Utils::log("invalid conversion from '" + Utils::typeToQuad(value->type) + "' to '" + Utils::typeToQuad(type) + "'", value->loc, "error");
                return false;
            }
        }

        if (entryType == TYPE_CONST && value == nullptr)
        {
            Utils::log("uninitialized const '" + identifier->name + "'", identifier->loc, "error");
            return false;
        }

        if (scopeHelper->declareParams)
        {
            initialized = true;
        }

        if (!scopeHelper->addSymbol(identifier->loc, identifier->name, type, entryType, {}, 0, initialized))
        {
            Utils::log("Variable '" + identifier->name + "' is already declared in this scope", identifier->loc, "error");
            return false;
        }

        return true;
    }

    string generateCode() override
    {
        string quad;
        if (value != nullptr)
        {
            quad += value->generateCode();
            quad += Utils::convTypeToQuad(value->type, type);
            quad += Utils::opToQuad(OPR_POP, type) + " " + identifier->name + "\n";
        }
        return quad;
    }

    ~VariableDeclarationNode() override
    {
            delete value;
            value = nullptr;
    }
};

#endif // VARIABLE_DECLARATION_NODE
