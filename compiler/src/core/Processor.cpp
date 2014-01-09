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
#include "Processor.h"
#include "Core.h"
#include "CoreData.h"
#include "ErrorNumbers.h"

using namespace acpul;

extern Core *acpulSharedCore;
extern CoreData *acpulCoreData;

Processor::Processor()
{
    map.setProcessor(this);
    
    recursiveIncludeCountMax = 100;
}

Processor::~Processor()
{
    
}

void Processor::processSyntaxTree(stree &tree)
{
    map.objects().clear(); // warning for gc?
    
    recursiveIncludeCount = 0;
    
    stree::iterator node = tree.begin();
//    stree::iterator end2 = tree.end();
    processNode(tree, node, NULL);

    map.fixObjects();
    
//    map.dumpObjects();
}

Object *Processor::processNodes(stree &tree, stree::iterator node, Object *object)
{
    stree::sibling_iterator child;
    for (child = tree.begin(node) ; child != tree.end(node); child++)
    {
        processNode(tree, child, object);
    }
    return object;
}

Object *Processor::processNode(stree &tree, stree::iterator node, Object *object)
{
    snode *o = *node;
    stype type = o->type;

    if (type == stype_main)
    {
        object = map.createMainObject();
        processNodes(tree, node, object);
    }
    if (type == stype_object)
    {
        stree::sibling_iterator child = tree.begin(node);
        Name *name = acpulCoreData->readname(tree, child);
        if (name)
        {
//            acpulCoreData->dumpNode(child);
            
            Object *object1;
            
            bool underscoreObject = name->isUnderscore();
            
            if (!underscoreObject)
            {
                object1 = objectForName(name, object);
            }
            else
            {
                object1 = object;
            }
            
            child++;
            if (child != tree.end() && (*child)->type == stype_file)
            {
                processFile(tree, child, object1);
            }
            else
            {
                if (!underscoreObject)
                {
                    for (; child != tree.end(node); child++)
                    {
                        processNode(tree, child, object1);
                    }
                }
                else
                {
                    processUnderscoreObject(tree, child, object1);
                }
            }
        }
    }
    else if (type == stype_expression_simple)
    {
        object->addExpressionNode(tree, node);
    }
    else if (type == stype_expression_assign)
    {
        object->addExpressionNode(tree, node);
    }
    else if (type == stype_expression_compound)
    {
        object->addExpressionNode(tree, node);
    }
    
    return object;
}

void Processor::processUnderscoreObject(stree &tree, stree::iterator node, Object *object)
{
    Name *name;
    Object *object1;

    stype type = (*node)->type;
    if (type != stype_expression_simple)
    {
        error(L"Should be expression_simple %i", type);
        return;
    }
    node = tree.begin(node);
    name = acpulCoreData->readname(tree, node);

    printf("Include object: ");
    name->dump();
    printf("\n");

    if (!name)
    {
        error(L"Cann't read name for include object %p", object);
        return;
    }
    object1 = object->queryObjectWithParents(name);
    if (!object1)
    {
        error(L"Include object not found %p", name);
        return;
    }
    
    object1->mergeObjects(object);
}

void Processor::processFile(stree &tree, stree::iterator node, Object *object)
{
    stree *filetree;
    int formulaId;
    
    if (recursiveIncludeCount >= recursiveIncludeCountMax)
    {
        error(L"File include level limit %i reached", recursiveIncludeCount);
        return;
    }

    node = tree.begin(node);
    stype type = (*node)->type;
    
    if (type == stype_number)
    {
        float v;
        if (!acpulCoreData->readnumber(tree, node, v))
        {
            error(L"Can't read number for file from source tree");
            return;
        }
        formulaId = v;
    }
    else if (type == stype_name)
    {
        Name *name = acpulCoreData->readname(tree, node);
        name->dump();
        printf("\n");
        if (name)
        {
            float v;
            Object *object1 = object->queryObjectWithParents(name);
            if (!object1)
            {
                error(L"Object not found '%p' for file", name);
                return;
            }
            if (!object1->getExpressionValueAsNumber(v))
            {
                error(L"Can't read number from object name '%p' for file", name);
                return;
            }
            formulaId = v;
        }
        else
        {
            error(L"Can't read name for file from source tree");
            return;
        }
    }
    else
    {
        error(L"Unexpected type %i for file", type);
        return;
    }
    
    filetree = acpulSharedCore->getSourceTreeForProcess(formulaId);
    
    if (filetree)
    {
        stree::iterator node = filetree->begin();
        if ((*node)->type == stype_main)
        {
            recursiveIncludeCount++;

            processNodes(*filetree, node, object);

            recursiveIncludeCount--;
        }
        else
        {
            error(L"Wrong file node type %i", (*node)->type);
        }
    }
    else
    {
        // error should be in getSourceTree too
        error(L"Source tree not found %i", formulaId);
    }
}


Object *Processor::objectForName(Name *name, Object *current)
{
    Object *object;
    object = current->queryObject(name);
    if (!object)
    {
        object = map.createObject(current, name);
    }
    else
    {
        object = map.followObject(object);
    }
    return object;
}

void Processor::error(const wchar_t *s, ...)
{
    printf("%S\n", s);
    va_list ap;
    va_start(ap, s);
    acpulCoreData->error(EN_PROC_ERR, s, ap);
    va_end(ap);
}

/*
-- very old code
Object *objectForName(name)
{
    Name *
    Object *object = queryObject(name);
    if (!object)
    {
        // undefined object, create new recursivy
        
        Object *object = newObject();
    }
}*/

#if 0
f
{
    main = create object
    
    pointer = tree begin
    parse(pointer, main)
}

parse
    parse item
    if is expression
        block add expression
        continue
    if is object
        create object
        parse(pointer, object);
        continue

//
// Test: ACPUL Google.Glass Interface Prototype
//
"OK, GLASS"
"f body begin"
"main is create object"
"pointer is tree begin"
"run parse with pointer and main arguments"
"parse body begin"
"parse item"
"..."

#endif
