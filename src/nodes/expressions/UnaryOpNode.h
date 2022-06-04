#ifndef UNARY_OP_NODE
#define UNARY_OP_NODE

#include "../Node.h"

class UnaryOpNode : public ExpressionNode
{
    Operator op;
    ExpressionNode* operand;
    bool isUsed = false;

public:
    UnaryOpNode(yy::location loc, Operator op, ExpressionNode* operand) : ExpressionNode(loc)
    {
        this->op = op;
        this->operand = operand;
    }

    bool analyzeSemantic(bool used = false) override
    {
        this->isUsed = used;

        auto scopeHelper = ScopeHelper::getInstance();

        if (!(operand->analyzeSemantic(true)))
            return false;

        if (operand->type == DTYPE_VOID)
        {
            Utils::log("invalid operand of type '" + Utils::typeToQuad(operand->type), loc, "error");
            return false;
        }

        if (op == OPR_PRE_INC || op == OPR_PRE_DEC || op == OPR_POST_INC || op == OPR_POST_DEC)
        {
            auto* identifier = dynamic_cast<IdentifierNode*>(operand);
            if (identifier == nullptr)
            {
                Utils::log("++/-- can only be applied to variables", loc, "error");
                return false;
            }

            EntryInfo* info = scopeHelper->lookup(identifier->name);

            if (info && info->entryType == TYPE_FUNC)
            {
                Utils::log("++/-- cannot be applied to functions", loc, "error");
                return false;
            }

            if (info && info->entryType == TYPE_CONST)
            {
                Utils::log("can not use ++/-- on read-only (const) variable '" + identifier->name + "'", loc, "error");
                return false;
            }
        }

        if (Utils::isLogical(op))
            type = DTYPE_BOOL;
        else
            type = operand->type;

        entryType = operand->entryType; // check if constant
        return true;
    }

    string generateCode() override
    {
        string quad;

        quad = operand->generateCode();
        quad += Utils::convTypeToQuad(operand->type, type);

        auto* identifier = dynamic_cast<IdentifierNode*>(operand);

        if (op == OPR_PRE_INC || op == OPR_PRE_DEC)
        {
            quad += Utils::opToQuad(op, type) + "\n";
            quad += Utils::opToQuad(OPR_POP, type) + " " + identifier->name + "\n";
            if (isUsed)
                quad += Utils::opToQuad(OPR_PUSH, type) + " " + identifier->name + "\n";
        } else if (op == OPR_POST_INC || op == OPR_POST_DEC)
        {
            if (isUsed)
                quad += Utils::opToQuad(OPR_PUSH, type) + " " + identifier->name + "\n";
            quad += Utils::opToQuad(op, type) + "\n";
            quad += Utils::opToQuad(OPR_POP, type) + " " + identifier->name + "\n";
        } else
        {
            quad += Utils::opToQuad(op, type) + "\n";
        }

        return quad;
    }

    ~UnaryOpNode() override
    {
        delete operand;
        operand = nullptr;
    }
};

#endif