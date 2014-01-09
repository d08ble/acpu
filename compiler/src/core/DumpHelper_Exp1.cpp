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
#include "Utils.h"
#include "CoreData.h"
#include "ObjectsMap.h"

using namespace acpul;

extern CoreData *acpulCoreData;

// old code, should use DumpHelper::

//
// CORE DATA
//

void CoreData::dumpExpressionMultipleNodesA(stree &tree, stree::iterator node)
{
    dumpExpressionMultipleNodesA1(tree, node, 0);
}

void CoreData::dumpExpressionMultipleNodesA1(stree &tree, stree::iterator node, int prefix)
{
    stree::sibling_iterator i;
    i = tree.begin(node);
    for (; i != tree.end(node); i++)
    {
        printDepth(prefix);
        dumpExpressionNodeA(tree, i, prefix);
        printf(";\n");
    }
}

void CoreData::dumpExpressionNodeA(stree &tree, stree::iterator node, int prefix)
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
            dumpExpressionNodeA(tree, tree.begin(node), prefix);
            printf(")");
            return;
        }
        else if (type == stype_function)
        {
            dumpFunctionNodeA(tree, node, prefix);
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
            dumpExpressionNodeA(tree, i, prefix);
            i++;
            
            //            name->dump();
            
            printf(":=");
            
            single = true;
        }
        if (type == stype_expression_compound)
        {
            dumpExpressionNodeA(tree, i, prefix);
            //            Name *name = acpulCoreData->readname(tree, i);
            i++;
            
            //            name->dump();
            
            printf("(");
            dumpExpressionNodeA(tree, i, prefix);
            printf(")");
            i++;
            
            printf("\n");
            printDepth(prefix);
            printf("{\n");
            
            dumpExpressionMultipleNodesA1(tree, i, prefix+1);
            i++;
            
            printDepth(prefix);
            printf("}\n");
        }
        
        
        for (; i != tree.end(node); i++)
        {
            stype t = (*i)->type;
            if (t == stype_expression_simple)
            {
                printf("(");
            }
            
            dumpExpressionNodeA(tree, i, prefix);
            
            if (t == stype_expression_simple)
            {
                printf(")");
            }
            
            if (!single)
                printf(";");
        }
    }
}

void CoreData::dumpFunctionNodeA(stree &tree, stree::iterator node, int prefix)
{
    stree::sibling_iterator i;
    
    i = tree.begin(node);
    Name *name = acpulCoreData->readname(tree, i);
    name->dump();
    printf("(");
    for (i++; i != tree.end(node); i++)
    {
        dumpExpressionNodeA(tree, i, prefix);
        printf(",");
    }
    printf(")");
}
