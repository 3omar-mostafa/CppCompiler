#ifndef FUNCTION_DECLARATION_NODE
#define FUNCTION_DECLARATION_NODE

#include <iostream>
#include <vector>

#include "../../utils/enums.h"
#include "../statements/VariableDeclarationNode.h"

typedef vector<VariableDeclarationNode *> VarDecList;
class FunctionDeclarationNode : public Node
{
    DataType type;
    IdentifierNode *identifier;
    VarDecList params;
    Node *body;
    EntryType entryType;

public:
    FunctionDeclarationNode(yy::location loc, DataType type, IdentifierNode *identifier, vector<VariableDeclarationNode *> params, Node *body)
        : Node(loc)
    {
        this->type = type;
        this->identifier = identifier;
        this->params = params;
        this->body = body;
        this->entryType = TYPE_FUNC;
    }

    bool analyzeSemantic(AnalysisHelper *analysisHelper, bool used = false) override
    {
        if (!analysisHelper->isGlobalScope())
        {
            analysisHelper->log("function declaration is allowed only in global scope", identifier->loc, "error");
            return false;
        }

        bool check = true;

        if (!analysisHelper->addSymbol(identifier->loc, identifier->name, type, entryType, {}, 0, true))
        {
            analysisHelper->log("'" + identifier->name + "' is already declared", identifier->loc, "error");
            check = false;
        }

        analysisHelper->pushScope(SCOPE_FUNCTION, this);

        analysisHelper->declareParamas = true;

        for (int i = 0; i < params.size(); ++i)
        {
            check &= params[i]->analyzeSemantic(analysisHelper);
        }

        analysisHelper->declareParamas = false;

        check &= body->analyzeSemantic(analysisHelper);

        analysisHelper->popScope();

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
};

#endif // FUNCTION_DECLARATION_NODE
