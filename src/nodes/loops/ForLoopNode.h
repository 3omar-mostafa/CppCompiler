#ifndef FOR_LOOP_NODE
#define FOR_LOOP_NODE
#include "../Node.h"
#include "../statements/VariableDeclarationNode.h"

class ForLoopNode : public Node
{
    VariableDeclarationNode *varDec;
    ExpressionNode *cond;
    ExpressionNode *step;
    Node *body;

public:
    ForLoopNode(yy::location loc, VariableDeclarationNode *varDec, ExpressionNode *cond, ExpressionNode *step, Node *body) : Node(loc)
    {
        this->varDec = varDec;
        this->cond = cond;
        this->step = step;
        this->body = body;
    }

    bool analyzeSemantic(bool used = false) override
    {
        auto scopeHelper = ScopeHelper::getInstance();

        if (scopeHelper->isInsideGlobalScope())
        {
            Utils::log("for loop is not allowed in global scope", loc, "error");
            return false;
        }

        bool check = true;
        scopeHelper->pushScope(SCOPE_LOOP, this);

        if (!(varDec->analyzeSemantic() & cond->analyzeSemantic(true)
              & step->analyzeSemantic() & body->analyzeSemantic()))
        {
            check &= false;
        }

        if (cond->type == DTYPE_VOID)
        {
            Utils::log("invalid conversion from '" + Utils::typeToQuad(cond->type) + "' to 'bool'", cond->loc, "error");
            check &= false;
        }

        scopeHelper->popScope();

        return check;
    }

    string generateCode() override
    {
        /*
        --------INIT-------
        L1:
        --------COND-------
        JMPZ L3
        --------BODY-------

        L2: (FOR CONTINUE)
        --------STEP-------
        JMP L1

        L3: (FOR BREAK)
        --------EXIT-------

        */

        auto genHelper = CodeGenerationHelper::getInstance();

        string quad;
        string l1 = genHelper->getNewLabel();
        string l2 = genHelper->getNewLabel();
        string l3 = genHelper->getNewLabel();

        quad = varDec->generateCode();
        quad += l1 + ":\n";
        quad += cond->generateCode();
        quad += Utils::opToQuad(OPR_JMPZ, cond->type) + " " + l3 + "\n";

        genHelper->addContinueLabel(l2);
        genHelper->addBreakLabel(l3);

        quad += body->generateCode();

        genHelper->removeContinueLabel();
        genHelper->removeBreakLabel();

        quad += l2 + ":\n";
        quad += step->generateCode();
        quad += Utils::opToQuad(OPR_JMP, cond->type) + " " + l1 + "\n";
        quad += l3 + ":\n";

        return quad;
    }

    ~ForLoopNode() override
    {
        delete varDec;
        delete cond;
        delete step;
        delete body;

        varDec = nullptr;
        cond = nullptr;
        step = nullptr;
        body = nullptr;
    }
};
#endif