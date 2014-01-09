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
#include "Object.h"
#include "Name.h"
#include "CoreData.h"
#include "ErrorNumbers.h"
#include "Utils.h"

using namespace acpul;

extern CoreData *acpulCoreData;

static int _uid_global;

Object::Object()
: _followObject(NULL)
, _codeBlock(NULL)
, _parent(NULL)
, _blockUpdated(false)
, _link(NULL)
, _params(NULL)
, _ident(L"<NULLIDENT>")
{
    _block.setObject(this);
    
    _uid = ++_uid_global;
}

Object::~Object()
{
    releaseParams();
}

void Object::addExpressionNode(stree &tree, stree::iterator node)
{
    _block.addExpression(tree, node);
    _blockUpdated = true;
}

Object *Object::queryObject2(const Name *name, int i)
{
    Object *o = this;
    Object *found = NULL;
    for (; i < name->count(); i++)
    {
        const wchar_t *ident = (*name)[i];
        found = o->objectForIdent(ident);
        if (!found)
        {
            Object *o1 = NULL;
            if (o->isFollowing())
            {
                o1 = o->followObject()->queryObject2(name, i);
            }
            if (!o1 && o->isLink())
            {
                o1 = o->link()->queryObject2(name, i);
            }
            return o1;
        }
        o = found;
    }
    return found;
}

Object *Object::queryObject(const Name *name)
{
    Object *o = this;
    Object *found = NULL;
    for (int i = 0; i < name->count(); i++)
    {
        const wchar_t *ident = (*name)[i];
        found = o->objectForIdent(ident);
        if (!found)
            return NULL;
        o = found;
    }
    return found;
}

Object *Object::queryObjectWithParents(const Name *name)
{
#if 0
    if (name.isParam)
    {
        return queryObject(name);
    }
#endif
    Object *object = NULL;
    for (Object *o = this; o != NULL; o = o->parent())
    {
        object = o->queryObject2(name, 0);
        if (object)
            break;
    }
    return object;
}

void Object::mergeObjects(Object *target)
{
    omap mergeMap;
    
    mergeObjectsToMap(mergeMap);
    
    printf("mergeObjects replace: map {\n");

    omap &objects = target->objects();
    omap::iterator i;
    for (i = mergeMap.begin(); i != mergeMap.end(); i++)
    {
        // replace object
        printf("%S\n", i->first);
        objects[i->first] = i->second;
    }
    
    printf("mergeObjects replace: map }\n");
}

void Object::mergeObjectsToMap(omap &map)
{
    // copy objects to object
    omap::iterator i;
    for (i = _objects.begin(); i != _objects.end(); i++)
    {
        // skip if present
        if (map[i->first])
            continue;
        
        map[i->first] = i->second;
    }
    
    if (_followObject && _block.isFollowing())
    {
        printf("Follow %p %S\n", _followObject, _followObject->ident());
        _followObject->dumpObjects();
        _followObject->mergeObjectsToMap(map);
    }
    if (isLink())
    {
        printf("Link %p %S\n", _link, _link->ident());
        _link->dumpObjects();
        _link->mergeObjectsToMap(map);
    }
}

bool Object::isLink()
{
    // update block
    if (_blockUpdated)
    {
        _blockUpdated = false;
        
        _link = _block.getLinkObject();
    }
    return !!_link;
}

bool Object::isFollowing()
{
    // update block -- maybe bug
    //if (_blockUpdated)
    //{
    //    _blockUpdated = false;
    //}
    return _followObject && _block.isFollowing();
}

bool Object::getExpressionValueAsNumber(float &v)
{
    if (_block.isLink())
    {
        Object *link = _block.getLinkObject();
        if (link)
        {
            return link->getExpressionValueAsNumber(v);
        }
        else
        {
            error(L"Link object is undefined %p", this);
            return false;
        }
    }
    return _block.getNumber(v);
}

Name *Object::getExpressionValueAsName()
{
    if (_block.isLink())
    {
        Object *link = _block.getLinkObject(); // link will be null for undefined object
        if (link)
        {
            return link->getExpressionValueAsName();
        }
        else
        {
            return acpulCoreData->nameForIdent(_ident);
        }
    }
    error(L"Not link object %p for name query", this);
    return NULL;
}

/* -- old code
bool Object::getExpressionValueAsIdent(const wchar_t &*v)
{
    if (_block.isLink())
    {
        Object *link = _block.getLinkObject(); // link will be null for undefined object
        if (link)
        {
            return link->getExpressionValueAsIdent(v);
        }
        else
        {
            v = _ident;
            return true;
        }
    }
    error(L"Not link object %p for name query", this);
    return false; //_block.getName(v);
}*/

void Object::setObjectForIdent(const wchar_t *ident, Object *object)
{
    _objects[ident] = object;
}

Object *Object::objectForIdent(const wchar_t *ident)
{
    //
    // PARAMS FIRST
    //
    if (_params)
    {
        omap::iterator j = _params->find(ident);
        if (j != _objects.end())
            return j->second;
    }
    //
    // OBJECTS SECOND
    //
    omap::iterator i = _objects.find(ident);
    return (i == _objects.end()) ? NULL : i->second;
}

void Object::follow(Object *object)
{
    object->parent()->setObjectForIdent(object->ident(), this);
    _followObject = object;
    setIdent(object->ident());
    setParent(object->parent());
}

void Object::releaseParams()
{
    if (_params)
    {
        delete _params;
        _params = NULL;
    }
}

omap &Object::saveParams()
{
    omap *params = _params;

    _params = new omap;
    
    return *params;
}

void Object::restoreParams(omap &params)
{
    releaseParams();
    
    _params = &params;
}

omap *Object::setParams(omap *params)
{
    omap *params1 = _params;
    
    _params = params;
    
    return params1;
}

void Object::unsetParams(omap *params)
{
    _params = params;
}

void Object::error(const wchar_t *s, ...)
{
    va_list ap;
    va_start(ap, s);
    acpulCoreData->error(EN_OBJ_ERR, s, ap);
    va_end(ap);
}


