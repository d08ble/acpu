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

#include <iostream>
#include "Compiler.h"
#include "Processor.h"
#include "Executor.h"
#include "CoreData.h"
#include "ErrorNumbers.h"
#include "DumpHelper.h"

using namespace acpul;

extern CoreData *acpulCoreData;

int Compiler::compile(ObjectsMap &map, Executor *executor)
{
    _blocks.clear(); // warning for gc?
    
    _map = &map;
    _executor = executor;
    
    otree &objects = map.objects();
    otree::sibling_iterator i;
    i = objects.begin();
    Object *object = *i;
    CodeBlock *code = compileObject(object);
    
    int mainBlock = code->compile(this);
    
    executor->link(*this);
    
    return mainBlock;
}

CodeBlock *Compiler::compileObject(Object *object)
{
//    object->dumpFull(1, 0);
    _currentObject = object;
    
    // special objects or static
    CodeBlock *code = object->codeBlock();
    if (code)
        return code;
    
//    printf("Compile Object: %S (%p) START\n", object->ident(), object);
    
    code = compileBlock(object->block());
    
//    printf("Compile Object: %S (%p) END\n", object->ident(), object);
    
    return code;
}

CodeBlock *Compiler::compileBlock(Block &block)
{
    CodeBlock *code = acpulCoreData->newCodeBlock();
    
    for (int i = 0; i < block.expressionsCount(); i++)
    {
        Expression *exp = block.expressionAtIndex(i);
        
        CodeBlock *code1 = compileExpression(exp);
        
        codeBlockAttach(code, code1);
    }

//    outtree &otree = code->tree();
//    printf("!!!Code Block compiled (%p)\n", &block);
//    acpulCoreData->dumpTree(otree, otree.begin(), otree.end());
//    CoreData::dumpExpressionMultipleNodes(otree, otree.begin());
//    printf("\n");
    
    return code;
}

CodeBlock *Compiler::compileExpression(Expression *exp)
{
    CodeBlock *code = acpulCoreData->newCodeBlock();

    stree &tree = *exp->tree();
    stree::iterator node = exp->node();

//    printf("Compile Expression: ");
//    _map->dumpExpressionNode(tree, node);
//    printf("; START\n\n");
    
    Expression *exp1 = _exp;
    _exp = exp;
    
    outtree &otree = code->tree();
    outtree::iterator oexp = compileExpressionNode(tree, node, otree, otree.end());
    
    _exp = exp1;

//    printf("Compile Expression: ");
//    _map->dumpExpressionNode(tree, node);
//    printf("; END\n\n");
    
//    printf("CodeBlock: %p\n", code);
////    _map->processor()->dumpTree(otree, oexp, otree.end(oexp));
//    acpulCoreData->dumpTree(otree, otree.begin(), otree.end());
//    _map->dumpExpressionMultipleNodes(otree, otree.begin());
//    printf("\n");

    return code;
}

outtree::iterator Compiler::compileExpressionNode(stree &tree, stree::iterator node, outtree &otree, outtree::iterator onode)
{

//    printf("Processing node: \n");
//    acpulCoreData->dumpTree(tree, node, tree.end(node));
//    printf("\n");

    stype type = (*node)->type;
    
    stree::sibling_iterator i;
    i = tree.begin(node);
    
    outtree::iterator oexp = outTreeCreateExpression(otree, onode);
    
    int a = (*node)->sign;
    (*oexp)->sign = (*node)->sign;
    (*oexp)->type = type;
    
    // ASSIGN
    
    if (type == stype_expression_assign)
    {
        Name *name = acpulCoreData->readname(tree, i);
        i++;
  
        //Object *obj = _exp->block()->object();

        Object *obj = _exp->objectForName(name);
//        printf("EXP <name>:=...; name='");
//        name->dump();
//        printf("' obj=%p\n", obj);
        
        
        if (obj)
        {
            CodeBlock *code = compileObject(obj);
            outtree::iterator iblock = outTreeInsertBlock(otree, oexp, code, false);
        }
        else
        {
            // Not found. Default - insert 'name' in out expression
            
            outtree::iterator iname = outTreeCreateName(otree, oexp, name);
        }
    }
    
    // COMPOUND

    if (type == stype_expression_compound)
    {
        // logic for fix names 'lo while;lo(r0){};'
        Name *name = acpulCoreData->readname(tree, i);
        i++;

        Object *obj = _exp->objectForName(name);
        if (obj)
        {
            Name *name1 = obj->getExpressionValueAsName();
            if (name1)
            {
                // ok
                outTreeCreateName(otree, oexp, name1);
            }
            else
            {
                // fail
                outTreeCreateNOPName(otree, oexp);
            }
        }
        else
        {
            outtree::iterator iname = outTreeCreateName(otree, oexp, name);
        }
        
//        outtree::iterator oexpc = outTreeCreateExpression(otree, oexp);
        outtree::iterator iexp = compileExpressionNode(tree, i, otree, oexp);
        i++;

//        outtree::iterator oexpc1 = outTreeCreateExpression(otree, oexp);
        outtree::iterator iexp1 = compileExpressionNode(tree, i, otree, oexp);
        i++;
    }

    outtree::sibling_iterator j = i;
    j++;
    bool multiple = j != tree.end(node);

    // NAME
    // NUMBER 
    // FUNCTION 
    // EXP_SIMPLE
    // OPERATOR
    for (; i != tree.end(node); i++)
    {
        stype t = (*i)->type;
        
        if (t == stype_name)
        {
            Name *name = acpulCoreData->readname(tree, i);
//            name->dump();
//            printf("\n");
            Object *obj = _exp->objectForName(name);
            if (obj)
            {
//                _exp->dumpLocals();
//                obj->dumpParams();
//                _currentObject->dumpParams();
                omap *oldParams = obj->setParams(_currentObject->params());
//                obj->dumpFull(1, 0);
                CodeBlock *code = compileObject(obj);
                obj->unsetParams(oldParams);
                outtree::iterator iblock = outTreeInsertBlock(otree, oexp, code, multiple);
            }
            else
            {
                outtree::iterator iname = outTreeCreateName(otree, oexp, name);
            }
        }
        else if (t == stype_number)
        {
            outTreeCreateNumber(otree, oexp, (*i)->val());
        }
        else if (t == stype_operator)
        {
            outTreeCreateOperator(otree, oexp, (*i)->val());
        }
        else if (t == stype_expression_simple)
        {
            outtree::iterator iexp = compileExpressionNode(tree, i, otree, oexp);
        }
        else if (t == stype_function)
        {
            outtree::iterator ifnexp = compileFunctionNode(tree, i, otree, oexp, multiple);
        }
        else
        {
            error(L"UNKNOWN OPERAND");
        }
    }
    
    return oexp;
}


outtree::iterator Compiler::compileFunctionNode(stree &tree, stree::iterator node, outtree &otree, outtree::iterator onode, bool multiple)
{
    outtree::iterator out;
    
    stree::sibling_iterator i;
    i = tree.begin(node);
    
    Name *name = acpulCoreData->readname(tree, i);
    
    Object *obj = _exp->objectForName(name);

    if (obj)
    {
        // HAVE OBJECT -> process compilation of template
  
//        _exp->dumpLocals();

        omap &oldParams = obj->saveParams();
        
        omap &params = *obj->params();

        int j = 0;
        for (i++; i != tree.end(node); i++, j++)
        {
//            Name *varName = acpulCoreData->nameForVarI(j);
            const wchar_t *varName = acpulCoreData->identForVarI(j);
            // _exp->save varName
            
            stype type = (*i)->type;
            int count = i.number_of_children();
            
            if (count == 0)
            {
                error(L"Internal error. stree node child count can't be 0");
                continue;
            }
            
            stree::sibling_iterator cnode = tree.begin(i);
            stype ctype = (*cnode)->type;

            
            if (type == stype_expression_simple && count == 1 && ctype == stype_name)
            {
                Name *cname = acpulCoreData->readname(tree, cnode);
                
//                cname->dump();
//                printf(" -> ");

                Object *target = _exp->objectForName(cname);

//                if (target)
//                    target->dumpInfo();
//                else
//                    printf("<NULL>\n");

                if (target)
                {
                    params[varName] = target;
                }
                else
                {
                    params[varName] = objectWithStaticName(cname);//objectWithStatic(tree, cnode);
                }
            }
            else
            {
                params[varName] = objectWithExpressionNode(tree, i);
            }
        }
        
//        obj->dumpParams();
        
        CodeBlock *code = compileObject(obj);
        outTreeInsertBlock(otree, onode, code, multiple);

        // Restore
        
        obj->restoreParams(oldParams);
    }
    else
    {
        // DON'T HAVE OBJECT -> compile inlines

        outtree::iterator ofn = outTreeCreateFunction(otree, onode);
        outtree::iterator ofnname = outTreeCreateName(otree, ofn, name);
        int j = 0;
        for (i++; i != tree.end(node); i++, j++)
        {
            outtree::iterator ofnarg = compileExpressionNode(tree, i, otree, ofn);
        }
        
        out = ofn;
    }
    
    return out;
}

Object *Compiler::objectWithStaticName(Name *name)
{
    CodeBlock *code = acpulCoreData->newCodeBlock();
    Object *obj = acpulCoreData->newObject();
    obj->setCodeBlock(code);

    outTreeCreateName(code->tree(), code->tree().begin(), name);
    
    return obj;
}

Object *Compiler::objectWithExpressionNode(stree &tree, stree::iterator node)
{
    CodeBlock *code = acpulCoreData->newCodeBlock();
    
    outtree &otree = code->tree();
    outtree::iterator oexp = compileExpressionNode(tree, node, otree, otree.end());

    Object *obj = acpulCoreData->newObject();
    obj->setCodeBlock(code);
    
    return obj;
}

outtree::iterator Compiler::outTreeInsertBlock(outtree &otree, outtree::iterator onode, CodeBlock *code, bool multiple)
{
    outtree::iterator i = onode;
    
    if (multiple)
        i = outTreeCreateExpression(otree, onode);

    outTreeInsertBlockA(otree, i, code);
}

outtree::iterator Compiler::outTreeInsertBlockA(outtree &otree, outtree::iterator onode, CodeBlock *code)
{
    outtree::iterator i;

    outtree &ctree = code->tree();
    outtree::iterator from;
    from = ctree.begin();
    ctree.merge(otree.begin(onode), otree.end(onode), ctree.begin(from), ctree.end(from));
    i = onode;

    return i;
}

outtree::iterator Compiler::outTreeCreateTemporraryName(outtree &otree, outtree::iterator onode)
{
    Name *name = acpulCoreData->tempName();
    
    outtree::iterator oname = outTreeCreateName(otree, onode, name);
    
    return oname;
}

outtree::iterator Compiler::outTreeCreateFunction(outtree &otree, outtree::iterator onode)
{
    acpul::Token *la = acpulCoreData->createToken();
    stype type = stype_function;
    la->val = (wchar_t *)L"FN";
    snode *obj = acpulCoreData->newSnode(la, type);
    outtree::iterator i = otree.append_child(onode, obj);
    return i;
}

outtree::iterator Compiler::outTreeCreateCallBlock(outtree &otree, outtree::iterator onode, int n)
{
    outtree::iterator ofn = outTreeCreateFunction(otree, onode);
    
    //0 fnname 'block'
    Name *name = acpulCoreData->newName();
    const wchar_t *s = L"block";
    name->addIdent(s);
    outtree::iterator oname = outTreeCreateName(otree, ofn, name);
    
    //1 arg0 'r0'
    Name *name1 = acpulCoreData->newName();
    const wchar_t *s1 = L"u0";
    name1->addIdent(s1);
    outtree::iterator op0 = outTreeCreateName(otree, ofn, name1);

    //2 arg1 block_number
//    outtree::iterator oexp = outTreeCreateExpression(otree, ofn);
    wchar_t *sn = new wchar_t[32];
    swprintf(sn, 32, L"%i", n);

    outtree::iterator obn = outTreeCreateNumber(otree, ofn, sn);
    
    return ofn;
}

outtree::iterator Compiler::outTreeCreateExpression(outtree &otree, outtree::iterator onode)
{
    outtree::iterator top;
    acpul::Token *la = acpulCoreData->createToken();
    stype type = stype_expression_simple; // default is simple
    snode *obj = acpulCoreData->newSnode(la, type);
    outtree::iterator i;
    if (onode == otree.end())
    {
        top = otree.begin();
//        i = otree.insert(top, obj);
        i = otree.append_child(top, obj);
    }
    else
    {
        top = onode;
        i = otree.append_child(top, obj);
    }
    
    return i;
}

outtree::iterator Compiler::outTreeCreateName(outtree &otree, outtree::iterator onode, Name *name)
{
    acpul::Token *la = acpulCoreData->createToken();
    stype type = stype_name;
    snode *obj = acpulCoreData->newSnode(la, type);
    outtree::iterator oname = otree.append_child(onode, obj);
    
    const wchar_t *first = NULL;
    
    for (int i = 0; i < name->count(); i++)
    {
        acpul::Token *la1 = acpulCoreData->createToken();
        
        const wchar_t *s = (*name)[i];
        la1->val = (wchar_t *)s;
        
        if (!first)
        {
            first = s;
        }
        
        snode *obj1 = acpulCoreData->newSnode(la1, stype_ident);

        otree.append_child(oname, obj1);
    }
    la->val = (wchar_t *)first; // TODO FIX 'const'!!!
    return oname;
}

outtree::iterator Compiler::outTreeCreateNOPName(outtree &otree, outtree::iterator onode)
{
    acpul::Token *la = acpulCoreData->createToken();
    stype type = stype_name;
    snode *obj = acpulCoreData->newSnode(la, type);
    outtree::iterator oname = otree.append_child(onode, obj);
    acpul::Token *la1 = acpulCoreData->createToken();
    la1->val = L"NOP";
    snode *obj1 = acpulCoreData->newSnode(la1, stype_ident);
    otree.append_child(oname, obj1);
    return oname;
}
/* -- old api
outtree::iterator outTreeCreateIdent(outtree &otree, outtree::iterator onode, const wchar_t *ident)
{
    acpul::Token *la = acpulCoreData->createToken();
    la->val = ident;                                    // TODO FIX 'const'!!!???
    snode *obj = acpulCoreData->newSnode(la, stype_ident);
    return otree.append_child(onode, obj);
}*/


outtree::iterator Compiler::outTreeCreateNumber(outtree &otree, outtree::iterator onode, const wchar_t *s)
{
    acpul::Token *la = acpulCoreData->createToken();
    stype type = stype_number;
    la->val = (wchar_t *)s;
    snode *obj = acpulCoreData->newSnode(la, type);
    outtree::iterator i = otree.append_child(onode, obj);
    return i;
}

outtree::iterator Compiler::outTreeCreateOperator(outtree &otree, outtree::iterator onode, const wchar_t *s)
{
    acpul::Token *la = acpulCoreData->createToken();
    stype type = stype_operator;
    la->val = (wchar_t *)s;
    snode *obj = acpulCoreData->newSnode(la, type);
    outtree::iterator i = otree.append_child(onode, obj);
    return i;
}

void Compiler::codeBlockAttach(CodeBlock *code, CodeBlock *code1)
{
    outTreeInsertBlock(code->tree(), code->tree().begin(), code1, false);
}

int Compiler::registerCodeBlock(CodeBlock *code)
{
    int n = _executor->allocateBlock(); //_blocks.size();
    _blocks.push_back(code);
    code->setBlockId(n);
    return n;
}

void Compiler::error(const wchar_t *s, ...)
{
    va_list ap;
    va_start(ap, s);
    acpulCoreData->error(EN_COMPILE_ERR, s, ap);
    va_end(ap);
}

#if 0
// CALL FUNCTION
Name *name;
void *arguments;
Object *object;
if (object)
{
    
}
else
{
    // default
    for (arguments;;)
    {
        
    }
}
#endif

