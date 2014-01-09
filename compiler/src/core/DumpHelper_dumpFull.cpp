//
// Another CPU Language - ACPUL - a{};
//
// THIS SOFTWARE IS PROVIDED BY THE FREEBSD PROJECT ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
// SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.
//
// Made by d08ble, thanks for watching.
//

#include "DumpHelper.h"
#include "Expression.h"
#include "Object.h"
#include "Utils.h"
#include "Name.h"
#include "ObjectsMap.h"
#include "CoreData.h"
#include "CodeBlock.h"
#include "Compiler.h"

using namespace acpul;

extern CoreData *acpulCoreData;

#define E printf("\n");
#define P printDepth(prefix);
#define P0 printDepth(prefix-1);
#define P1 P0
#define E0 E
#define E1 E
#define E2 E

//
// EXPRESSION
//

void Expression::dumpFull(int prefix, int flags)
{
    P printf("expression: [");E
    prefix++;
    P printf("locals: [");E
    prefix++;
    for (onmap::iterator it = _locals.begin(); it != _locals.end(); it++)
    {
        P it->first->dump();
        printf(": %S (uid=%i)\n", it->second ? it->second->ident() : L"<NULL>", it->second ? it->second->uid() : 0);
    }
    prefix--;
    P printf("]");E
    
    P printf("instructions: [");E
    prefix++;
    DumpHelper::dumpExpression(prefix, flags, this);
    prefix--;
    P printf("]");E
    prefix--;
    P printf("]");E
}

//
// BLOCK
//

void Block::dumpFull(int prefix, int flags)
{
    std::vector<Expression *>::iterator i;
    
    if (_expressions.size())
    {
        P printf("block: [");E
        prefix++;
        for (i = _expressions.begin(); i != _expressions.end(); i++)
        {
            Expression *e = *i;
            e->dumpFull(prefix, flags);
        }
        prefix--;
        P printf("]");E
    }
}

//
// CODE BLOCK
//

void CodeBlock::dumpFull(int prefix, int flags)
{
    P printf("codeBlock: [");E
    prefix++;
//    acpulCoreData->dumpTree(_tree, _tree.begin(), _tree.end());
//    DumpHelper::dumpExpressionMultipleNodesA(_tree, _tree.begin());
  
    DumpHelper::dumpExpressionMultipleNodesA1(prefix, flags, _tree, _tree.begin());

    prefix--;
    P printf("]");E
}

//
// OBJECT
//

void Object::dumpFull(int prefix, int flags)
{
    P0 printf("OBJECT [");E0
    P printf("uid: %i", _uid);E
    P printf("ident: %S", _ident);E
    if (_parent)
    {
        P printf("parent: %i", _parent->uid());E
    }
    if (_followObject)
    {
        P printf("follow: %i", _followObject->uid());E
    }
    if (_link)
    {
        P printf("link: %i", _link->uid());E
    }
    
    if (_params)
    {
        P printf("params:");E
        prefix++;
        for (omap::iterator i = _params->begin(); i != _params->end(); i++)
        {
            P printf("%S: %S (uid: %i)\n", i->first, i->second ? i->second->ident() : L"<NULL>", i->second ? i->second->uid() : 0);
        }
        prefix--;
    }
    
    if (_objects.size())
    {
        P printf("objects:");E
        prefix++;
        for (omap::iterator i = _objects.begin(); i != _objects.end(); i++)
        {
            P printf("%S: %S (uid: %i)\n", i->first, i->second ? i->second->ident() : L"<NULL>", i->second ? i->second->uid() : 0);
        }
        prefix--;
    }
    
    _block.dumpFull(prefix, flags);
    
    if (_codeBlock)
    {
        _codeBlock->dumpFull(prefix, flags);
    }
    
    P1 printf("]");E2
}

//
// DUMP HELPER
//

void DumpHelper::dumpExpression(int prefix, int flags, Expression *exp)
{
    stree &tree = *exp->tree();
    stree::iterator node = exp->node();
    P dumpExpressionNode(prefix, flags, tree, node);
    printf(";");E
}

void DumpHelper::dumpExpressionMultipleNodes(stree &tree, stree::iterator node)
{
    dumpExpressionMultipleNodes1(0, 0, tree, node);
}

void DumpHelper::dumpExpressionMultipleNodes1(int prefix, int flags, stree &tree, stree::iterator node)
{
    stree::sibling_iterator i;
    i = tree.begin(node);
    for (; i != tree.end(node); i++)
    {
        P dumpExpressionNode(prefix, flags, tree, i);
        printf(";");E
    }
}

void DumpHelper::dumpExpressionNode(int prefix, int flags, stree &tree, stree::iterator node)
{
    stype type = (*node)->type;
    
    stree::sibling_iterator i;
    i = tree.begin(node);
    
    if (type == stype_expression_assign)
    {
        if ((*i)->type == stype_name)
        {
            Name *name = acpulCoreData->readname(tree, i);
            
            name->dump();
        }
        else
        {
            dumpExpressionNode(prefix, flags, tree, i);
        }
        i++;
        printf(":=");
    }
    
    if (type == stype_expression_compound)
    {
        Name *name = acpulCoreData->readname(tree, i);
        i++;
        
        name->dump();
        
        printf("(");
        dumpExpressionNode(prefix, flags, tree, i);
        printf(")");
        i++;
        
        printf("{"); E
        
        dumpExpressionMultipleNodes1(prefix+1, flags, tree, i);
        i++;
        
        P printf("}");
    }
    
    for (; i != tree.end(node); i++)
    {
        stype t = (*i)->type;
        if (t == stype_name)
        {
            Name *name = acpulCoreData->readname(tree, i);
            name->dump();
        }
        else if (t == stype_number)
        {
            //            float n = _processor->readnumber(tree, i);
            printf("%S", (*i)->val());
        }
        else if (t == stype_operator)
        {
            //            wchar_t op = _processor->readoperator(tree, i);
            const wchar_t *s = (*i)->val();
            printf("%S", s);
        }
        else if (t == stype_expression_simple)
        {
            printf("(");
            dumpExpressionNode(prefix, flags, tree, i);
            printf(")");
        }
        else if (t == stype_function)
        {
            dumpFunctionNode(prefix, flags, tree, i);
        }
        else if (t == stype_expression_assign
                 || t == stype_expression_compound)
        {
            dumpExpressionNode(prefix, flags, tree, i);
        }
        else
        {
            printf("UNKNOWN %i", t);
        }
    }
}

void DumpHelper::dumpFunctionNode(int prefix, int flags, stree &tree, stree::iterator node)
{
    stree::sibling_iterator i;
    
    i = tree.begin(node);
    Name *name = acpulCoreData->readname(tree, i);
    name->dump();
    printf("(");
    for (i++; i != tree.end(node); i++)
    {
        dumpExpressionNode(prefix, flags, tree, i);
        printf(",");
    }
    printf(")");
}

//
// DUMP HELPER A
//

/*void DumpHelper::dumpExpressionMultipleNodesA(stree &tree, stree::iterator node)
 {
 dumpExpressionMultipleNodesA1(tree, node, 0);
 }*/

void DumpHelper::dumpExpressionMultipleNodesA1(int prefix, int flags, stree &tree, stree::iterator node)
{
    stree::sibling_iterator i;
    i = tree.begin(node);
    for (; i != tree.end(node); i++)
    {
        P dumpExpressionNodeA(prefix, flags, tree, i);
        printf(";");E
    }
}

void DumpHelper::dumpExpressionNodeA(int prefix, int flags, stree &tree, stree::iterator node)
{
    stype type = (*node)->type;
    
    int childCount = node.number_of_children();
    
    if (childCount <= 1
        || (type == stype_function))
        //        || (type == stype_expressions)
    {
        if (type == stype_name)
        {
            Name *name = acpulCoreData->readname(tree, node);
            name->dump();
        }
        else if (type == stype_number)
        {
            printf("%S", (*node)->val());
        }
        else if (type == stype_operator)
        {
            const wchar_t *s = (*node)->val();
            printf("%S", s);
        }
        else if (type == stype_expression_simple)
        {
            printf("(");
            dumpExpressionNodeA(prefix, flags, tree, tree.begin(node));
            printf(")");
            return;
        }
        else if (type == stype_function)
        {
            dumpFunctionNodeA(prefix, flags, tree, node);
            return;
        }
        //        else if (type == stype_expressions)
        // {
        // printf("{");
        // dumpExpressionMultipleNodesA(tree, node);
        // printf("}");
        // return;
        // }
        else
        {
            printf("UNKNOWN %i", type);
        }
    }
    else
    {
        bool single = false;
        
        stree::sibling_iterator i;
        i = tree.begin(node);
        
        for (; i != tree.end(node); i++)
        {
            stype t = (*i)->type;
            if (t == stype_operator)
            {
                single = true;
                break;
            }
        }
        
        //        stree::sibling_iterator i;
        i = tree.begin(node);
        
        if (type == stype_expression_assign)
        {
            //            Name *name = acpulCoreData->readname(tree, i);
            dumpExpressionNodeA(prefix, flags, tree, i);
            i++;
            
            //            name->dump();
            
            printf(":=");
            
            single = true;
        }
        if (type == stype_expression_compound)
        {
            dumpExpressionNodeA(prefix, flags, tree, i);
            //            Name *name = acpulCoreData->readname(tree, i);
            i++;
            
            //            name->dump();
            
            printf("(");
            dumpExpressionNodeA(prefix, flags, tree, i);
            printf(")");
            i++;
            
            printf("{");E
            
            dumpExpressionMultipleNodesA1(prefix+1, flags, tree, i);
            i++;
            
            P printf("}");E
        }
        
        
        for (; i != tree.end(node); i++)
        {
            stype t = (*i)->type;
            if (t == stype_expression_simple)
            {
                printf("(");
            }
            
            dumpExpressionNodeA(prefix, flags, tree, i);
            
            if (t == stype_expression_simple)
            {
                printf(")");
            }
            
            if (!single)
                printf(";");
        }
    }
}

void DumpHelper::dumpFunctionNodeA(int prefix, int flags, stree &tree, stree::iterator node)
{
    stree::sibling_iterator i;
    
    i = tree.begin(node);
    Name *name = acpulCoreData->readname(tree, i);
    name->dump();
    printf("(");
    for (i++; i != tree.end(node); i++)
    {
        dumpExpressionNodeA(prefix, flags, tree, i);
        printf(",");
    }
    printf(")");
}

#undef E
#undef P

