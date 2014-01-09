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
#include "CoreData.h"
#include "Error.h"
#include "Name.h"
#include "Object.h"
#include "CodeBlock.h"
#include "ExecutionBlock.h"
#include "Utils.h"

enum GCType
{
    gct_snode,
    gct_stree,
    gct_name,
    gct_ident,
    gct_object,
    gct_block,
    gct_expression,
    gct_code_block,
    gct_execution_block,
    gct_token

};

using namespace acpul;

CoreData *acpulCoreData = NULL;

CoreData::CoreData()
: _errorsConsole(true)
, _errorsEnabled(true)
{
    assert(!acpulCoreData);
    acpulCoreData = this;
    
    
}

CoreData::~CoreData()
{
    releaseGCObjects();
    releaseErrors();

    acpulCoreData = NULL;
}

//
// GC
//

void CoreData::releaseGCObjects()
{
    gcmap::iterator i;
    for (i = _map.begin(); i != _map.end(); i++)
    {
        infoGcObjectForceRelease(i);
        
        del(i);
    }
    _map.clear();
}

void CoreData::infoGcObjectForceRelease(gcmap::iterator i)
{
    error(L"Force release %p (%i) references %i\n", i->first, i->second._type, i->second._references);
}

void CoreData::snodeRetain(snode *node)
{
    retain(node, gct_snode);
}

void CoreData::snodeRelease(snode *node)
{
    release(node);
}

void CoreData::retain(void *obj, int type)
{
    if (!obj)
    {
        error(L"Retain object is NULL");
        return;
    }
    
    gcmap::iterator i = _map.find(obj);
    if (i == _map.end())
    {
        _map[obj] = GCObject(obj, type);
    }
    else
    {
        int use = i->second._references;
        use++;
        i->second._references = use;
    }
}

void CoreData::release(void *obj)
{
    gcmap::iterator i = _map.find(obj);
    if (i == _map.end())
    {
        error(L"Unregistred object %p", obj);
        return;
    }
    
    int use = i->second._references;
    
    if (use <= 0)
    {
        error(L"Invalid use count %i for %p", obj, use);
        return;
    }
    
    use--;

    if (use == 0)
    {
        del(i);
        _map.erase(i);
        return;
    }
    
    i->second._references = use;
}

void CoreData::del(gcmap::iterator i)
{
    void *obj = i->first;
    switch (i->second._type) {
        case gct_snode:
        {
            delete (snode *)obj;
            break;
        }   
        case gct_stree:
        {
            delete (stree *)obj;
            break;
        }   
        case gct_name:
        {
            delete (Name *)obj;
            break;
        }   
        case gct_ident:
        {
            delete (const wchar_t *)obj;
            break;
        }
        case gct_object:
        {
            delete (Object *)obj;
            break;
        }
        case gct_block:
        {
            delete (Block *)obj;
            break;
        }
        case gct_expression:
        {
            delete (Expression *)obj;
            break;
        }
        case gct_code_block:
        {
            delete (CodeBlock *)obj;
            break;
        }
        case gct_execution_block:
        {
            delete (ExecutionBlock *)obj;
            break;
        }
        case gct_token:
        {
            acpul::Token *la = (acpul::Token *)obj;
            la->val = NULL;
            delete la;
            break;
        }
        default:
            error(L"Undefined type %i", i->second._type);
            break;
    }
}

void CoreData::streeRetain(stree *tree)
{
    retain(tree, gct_stree);
    
    if (tree)
    {
        // retain nodes
        
        stree::iterator i;
        for (i = tree->begin(); i != tree->end(); i++)
        {
            snodeRetain(*i);
        }
    }
}

void CoreData::streeRelease(stree *tree)
{
    if (tree)
    {
        gcmap::iterator i = _map.find(tree);
        
        if (i != _map.end())
        {
            int use = i->second._references;
            
            if (use == 1)
            {
                // release nodes
                
                stree::iterator i;
                for (i = tree->begin(); i != tree->end(); i++)
                {
                    snodeRelease(*i);
                }
            }
            
        }
    }

    release(tree);
}

void CoreData::nameRetain(Name *name)
{
    retain(name, gct_name);
}

void CoreData::nameRelease(Name *name)
{
    release(name);
}

void CoreData::identRetain(const wchar_t *ident)
{
    retain((void *)ident, gct_ident);
}

void CoreData::identRelease(const wchar_t *ident)
{
    release((void *)ident);
}

void CoreData::objectRetain(Object *object)
{
    retain(object, gct_object);
}

void CoreData::objectRelease(Object *object)
{
    release(object);
}

void CoreData::blockRetain(Block *block)
{
    retain(block, gct_block);
}

void CoreData::blockRelease(Block *block)
{
    release(block);
}

void CoreData::expressionRetain(Expression *expression)
{
    retain(expression, gct_expression);
}

void CoreData::expressionRelease(Expression *expression)
{
    release(expression);
}

void CoreData::codeBlockRetain(CodeBlock *codeBlock)
{
    retain(codeBlock, gct_code_block);
}

void CoreData::codeBlockRelease(CodeBlock *codeBlock)
{
    release(codeBlock);
}

void CoreData::executionBlockRetain(ExecutionBlock *executionBlock)
{
    retain(executionBlock, gct_execution_block);
}

void CoreData::executionBlockRelease(ExecutionBlock *executionBlock)
{
    release(executionBlock);
}

void CoreData::tokenRetain(acpul::Token *la)
{
    retain(la, gct_token);
}

void CoreData::tokenRelease(acpul::Token *la)
{
    release(la);
}

//
// ERROR
//

void CoreData::error(const wchar_t *s, ...)
{
    va_list ap;
    va_start(ap, s);
    error(EN_DATA_ERR, s, ap);
    va_end(ap);
}

void CoreData::error(int number, const wchar_t *s, ...)
{
    va_list ap;
    va_start(ap, s);
    error(number, s, ap);
    va_end(ap);
}

void CoreData::error(int number, const wchar_t *s, va_list ap)
{
    if (!_errorsEnabled)
        return;
    
    Error *error = new Error();
    
    error->format(s, ap);
    error->setNumber(number);
    
    _errors.push_back(error);
    
    if (_errorsConsole)
        printf("(%i) %S", error->number(), error->message());
}

void CoreData::releaseErrors()
{
    aerrors::iterator i;
    for (i = _errors.begin(); i != _errors.end(); i++)
    {
        delete *i;
    }
    _errors.clear();
}

//
// UTILS
//

Name *CoreData::readname(stree &tree, stree::iterator node)
{
    stype type = (*node)->type;
    if (type != stype_name)
    {
        error(L"Can't read Name. Wrong type %i\n", type);
        return NULL;
    }
    Name *name = newName();
    stree::sibling_iterator child;
    for (child = tree.begin(node) ; child != tree.end(node); child++)
    {
        //        printf("%S.", (*child)->val());
        name->addIdent((*child)->val());
    }
    //    printf("\n");
    return name;
}

bool CoreData::readnumber(stree &tree, stree::iterator node, float &v)
{
    const wchar_t *s = (*node)->val();
    int c = swscanf(s, L"%f", &v);
    return !!c;
}

Name *CoreData::newName()
{
    Name *name = new Name();
    
    nameRetain(name);

    return name;
}

const wchar_t *CoreData::identForVarI(int j)
{
    const wchar_t *s[] = {
        L"_0", L"_1", L"_2", L"_3", L"_4", L"_5", L"_6", L"_7",
        L"_8", L"_9", L"_10", L"_11", L"_12", L"_13", L"_14", L"_15",
        L"_16", L"_17", L"_18", L"_19", L"_20", L"_21", L"_22", L"_23",
    };
    if (j >= sizeof(s)/sizeof(s[0]))
        return L"_TOO_MANY_PARAMS";
    
    return s[j];
}
/*
Name *CoreData::nameForVarI(int j)
{
    Name *varName = newName();
    const wchar_t *s[] = {
        L"_0", L"_1", L"_2", L"_3", L"_4", L"_5", L"_6", L"_7",
        L"_8", L"_9", L"_10", L"_11", L"_12", L"_13", L"_14", L"_15",
    };
    varName->addIdent(s[j]);
    
    return varName;
}
*/

Name *CoreData::tempName()
{
    Name *name = newName();
    const wchar_t *s = L"r0";
    name->addIdent(s);
    return name;
}

Name *CoreData::nameForIdent(const wchar_t *ident)
{
    Name *name = newName();
    name->addIdent(ident);
    return name;
}

Object *CoreData::newObject()
{
    Object *object = new Object();
    
    objectRetain(object);
    
    return object;
}

Expression *CoreData::newExpression()
{
    Expression *expression = new Expression();
    
    expressionRetain(expression);
    
    return expression;
}

CodeBlock *CoreData::newCodeBlock()
{
    CodeBlock *codeBlock = new CodeBlock();
    
    codeBlockRetain(codeBlock);
    
    return codeBlock;
}

ExecutionBlock *CoreData::newExecutionBlock()
{
    ExecutionBlock *executionBlock = new ExecutionBlock();
    
    executionBlockRetain(executionBlock);
    
    return executionBlock;
}

acpul::Token *CoreData::createToken()
{
    acpul::Token *la = new Token();

    tokenRetain(la);
    
    return la;
}

snode *CoreData::newSnode(acpul::Token *la_, stype type_)
{
    snode *node = new snode(la_, type_);
    
    snodeRetain(node);
    
    return node;
}

