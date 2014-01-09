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

//
// EXPRESSION
//

void Expression::dumpLocals()
{
    printf("EMAP[%p]:\n", this);
    for (onmap::iterator it = _locals.begin(); it != _locals.end(); it++)
    {
        printf("  ");
        it->first->dump();
        printf(":'%S' (%p)\n", it->second ? it->second->ident() : L"<NULL>", it->second);
    }
}

#if 0
Block _block;

CodeBlock *_codeBlock;

#endif


//
// OBJECT
//

void Object::dumpInfo()
{
    printf("OBJ '%S' (%p)\n", _ident, this);
}

void Object::dumpParams()
{
    printf("%S (%p) params:\n", _ident, this);
    if (_params)
    {
        for (omap::iterator i = _params->begin(); i != _params->end(); i++)
        {
            printf("%S: %S (%p)\n", i->first, i->second ? i->second->ident() : L"<NULL>", i->second);
        }
    }
}

void Object::dumpObjects()
{
    printf("MAP[%p]:\n", this);
    for (omap::iterator it = _objects.begin(); it != _objects.end(); it++)
    {
        //const wchar_t *a
        printf("  %S %p:%p\n", it->first, it->first, it->second);
        printf("  %.2x\n", *it->first);
    }
}

//
// OBJECTS MAP
//

void ObjectsMap::dumpObjects()
{
    dumpObjectsFollowTable();
    dumpObjectsDetails();
}

void ObjectsMap::dumpObjectsFollowTable()
{
    otree::iterator i;
    for (i = _objects.begin(); i != _objects.end(); i++)
    {
        for (int j = 0; j < _objects.depth(i); j++)
            printf(" ");
        
        Object *o = *i;
        printf("%S: %p", o->ident(), o);
        for ( ; o->followObject() ; o = o->followObject())
        {
            if (o == *i)
                printf(" -> ");
            printf("%p ", o);
        }
        printf("\n");
    }
}

void ObjectsMap::dumpObjectsDetails()
{
    otree::iterator i;
    for (i = _objects.begin(); i != _objects.end(); i++)
    {
        int depth = _objects.depth(i);
        
        printDepth(depth);
        
        Object *o = *i;
        printf("%S: %p\n", o->ident(), o);
        omap &objects = o->objects();
        for (omap::iterator it = objects.begin(); it != objects.end(); it++)
        {
            printDepth(depth);
            printf(" %S:%p\n", it->first, it->second);
        }
        //        o->dumpObjects();
        Block &block = o->block();
        for (int j = 0; j < block.expressionsCount(); j++)
        {
            Expression *exp = block.expressionAtIndex(j);
            printDepth(depth+2);
            printf("<");
            CoreData::dumpExpression(exp);
            printf(">\n");
            onmap &locals = exp->locals();
            for (onmap::iterator it = locals.begin(); it != locals.end(); it++)
            {
                printDepth(depth+2);
                it->first->dump();
                printf(":%p\n", it->second);
            }
            //            exp->dumpLocals();
        }
        
        printf("\n");
    }
}


//
// CODE BLOCK
//


void CodeBlock::dump()
{
    printf("BLOCK %i:\n", _blockId);
    acpulCoreData->dumpTree(_tree, _tree.begin(), _tree.end());
    CoreData::dumpExpressionMultipleNodesA(_tree, _tree.begin());
    printf("------------\n\n");
}

//
// NAME
//

void Name::dump() const
{
    if (!this)
    {
        printf("NULLNAME");
        return;
    }
    for (size_t i = 0; i < idents.size(); i++)
    {
        printf("%S.", idents[i]);
    }
}

//
// CORE DATA
//

void CoreData::dumpTree(stree &tree, stree::iterator start, stree::iterator end)
{
    while (start!=end)
    {
        printDepth(tree.depth(start));
        dumpNode(start);
        start++;
    }
}

void CoreData::dumpNode(stree::iterator node)
{
    static const char *s[] = {
        "main",
        "statement",
        "name",
        "ident",
        "number",
        "object",
        "object_declaration",
        "expressions",
        "expression_simple",
        "expression_assign",
        "term",
        "operator",
        "function",
        "block",
        "file",
        "compound",
    };
    
    snode *o = *node;
    if (!o->_la)
    {
        printf("[%s]\n", s[o->type]);
    }
    else
    {
        //        printf("[%s, %S, %i,%i %i,%i]\n", s[o->type], o->val(), o->_la->line, o->_la->col, o->_la->pos, o->_la->charPos);
        printf("[%s, %S, %p]\n", s[o->type], o->val(), o);
    }
    /*    static stype t = stype_main;
     
     if (t == o->type && o->val() && !wcscmp(o->val(), L"+"))
     {
     printf("!");
     }
     ot = o->type;*/
}

