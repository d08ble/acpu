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
#include "ObjectsMap.h"
#include "CoreData.h"

using namespace acpul;

extern CoreData *acpulCoreData;

// old code, should use DumpHelper::

void CoreData::dumpExpression(Expression *exp)
{
    stree &tree = *exp->tree();
    stree::iterator node = exp->node();
    dumpExpressionNode(tree, node);
    printf(";");
}

void CoreData::dumpExpressionMultipleNodes(stree &tree, stree::iterator node)
{
    stree::sibling_iterator i;
    i = tree.begin(node);
    for (; i != tree.end(node); i++)
    {
        dumpExpressionNode(tree, i);
        printf(";\n");
    }
}

void CoreData::dumpExpressionNode(stree &tree, stree::iterator node)
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
            dumpExpressionNode(tree, i);
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
        dumpExpressionNode(tree, i);
        printf(")");
        i++;
        
        printf("{");
        
        dumpExpressionMultipleNodes(tree, i);
        i++;
        
        printf("}");
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
            dumpExpressionNode(tree, i);
            printf(")");
        }
        else if (t == stype_function)
        {
            dumpFunctionNode(tree, i);
        }
        else if (t == stype_expression_assign
                 || t == stype_expression_compound)
        {
            dumpExpressionNode(tree, i);
        }
        else
        {
            printf("UNKNOWN %i", t);
        }
    }
}

void CoreData::dumpFunctionNode(stree &tree, stree::iterator node)
{
    stree::sibling_iterator i;
    
    i = tree.begin(node);
    Name *name = acpulCoreData->readname(tree, i);
    name->dump();
    printf("(");
    for (i++; i != tree.end(node); i++)
    {
        dumpExpressionNode(tree, i);
        printf(",");
    }
    printf(")");
}
