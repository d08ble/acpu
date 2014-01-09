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
#include "ObjectsMap.h"
#include "Processor.h"
#include "CoreData.h"
#include "Utils.h"

using namespace acpul;

extern CoreData *acpulCoreData;

Object *ObjectsMap::createMainObject()
{
    const wchar_t *main = L"_main";
    Object *object = acpulCoreData->newObject();
    object->setMap(this);
    object->setIdent(main);
    
    otree::iterator top;
    top = _objects.begin();
    otree::iterator i = _objects.insert(top, object);
    object->setMapIterator(i);
    
    return object;
}

Object *ObjectsMap::newObjectForObject(Object *object)
{
    Object *object1 = acpulCoreData->newObject();
    object1->setMap(this);
//    object1->setIdent(object->ident());
    
    otree::iterator i = _objects.append_child(object->mapIterator(), object1);
    object1->setMapIterator(i);
    
    return object1;
}


Object *ObjectsMap::createObject(Object *current, Name *name)
{
/*    Object *object; -- old code
    object = newObject();
    object->setMap(this);*/
    
    Object *o = current;
    Object *found = NULL;
    for (int i = 0; i < name->count(); i++)
    {
        const wchar_t *ident = (*name)[i];
        found = o->objectForIdent(ident);
        if (!found)
        {
            found = createObject(o, ident);
        }
        o = found;
    }
    
    return o;
}

Object *ObjectsMap::followObject(Object *object)
{
    Object *object1;
    object1 = newObjectForObject(object->parent());
    
    object1->follow(object);
    
    return object1;
}

Object *ObjectsMap::createObject(Object *object, const wchar_t *ident)
{
    Object *object1 = newObjectForObject(object);
    object1->setIdent(ident);
    object1->setParent(object);
    object->setObjectForIdent(ident, object1);
    return object1;
}

void ObjectsMap::fixObjects()
{
    otree::iterator i;
    for (i = _objects.begin(); i != _objects.end(); i++)
    {
        Object *o = *i;
        Block &b = o->block();
        for (int j = 0; j < b.expressionsCount(); j++)
        {
            Expression *exp = b.expressionAtIndex(j);
            exp->fix();
        }
    }    
}


