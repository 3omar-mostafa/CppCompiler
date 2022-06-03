#ifndef FUNCTION_DECLARATION_NODE
#define FUNCTION_DECLARATION_NODE

#include <iostream>
#include <vector>

#include "../../utils/enums.h"
#include "../../utils/utils.h"
#include "../statements/VariableDeclarationNode.h"

typedef vector<VariableDeclarationNode *> VarDecList;

class FunctionDeclarationNode : public Node
{
    IdentifierNode* identifier;
    Node* body;
    EntryType entryType;

public:
    DataType type;
    VarDecList params;

    FunctionDeclarationNode(yy::location loc, DataType type, IdentifierNode* identifier,
                            vector<VariableDeclarationNode*> params, Node* body) : Node(loc)
    {
        this->type = type;
        this->identifier = identifier;
        this->params = std::move(params);
        this->body = body;
        this->entryType = TYPE_FUNC;
    }

    bool analyzeSemantic(bool used = false) override
    {
        auto scopeHelper = ScopeHelper::getInstance();

        if (!scopeHelper->isInsideGlobalScope())
        {
            Utils::log("function declaration is allowed only in global scope", identifier->loc, "error");
            return false;
        }

        bool check = true;

        std::vector<DataType> paramsTypes;
        for (auto& param: params)
        {
            paramsTypes.push_back(param->type);
        }

        if (!scopeHelper->addSymbol(identifier->loc, identifier->name, type, entryType, paramsTypes, 0, true))
        {
            Utils::log("'" + identifier->name + "' is already declared", identifier->loc, "error");
            check = false;
        }

        scopeHelper->pushScope(SCOPE_FUNCTION, this);

        scopeHelper->declareParams = true;

        for (auto& param: params)
        {
            check &= param->analyzeSemantic();
        }

        scopeHelper->declareParams = false;

        check &= body->analyzeSemantic();

        scopeHelper->popScope();

        return check;
    }

    string generateCode(CodeGenerationHelper *genHelper) override
    {
        string quad;

        quad += "PROC " + identifier->name + " \n";
        for (auto &param : params)
        {
            quad += Utils::opToQuad(OPR_POP, param->type) + " " + param->identifier->name + "\n";
        }

        quad += body->generateCode(genHelper);
        quad += "END " + identifier->name + " \n";
        return quad;
    }

    ~FunctionDeclarationNode() override
    {
        delete identifier;
        delete body;
        identifier = nullptr;
        body = nullptr;

        for (auto &param : params)
        {
            delete param;
            param = nullptr;
        }
    }
};

#endif // FUNCTION_DECLARATION_NODE
