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

#include "Expression.h"
#include "Block.h"
#include "Processor.h"
#include "CoreData.h"

using namespace acpul;

extern CoreData *acpulCoreData;

void Expression::setNode(stree &tree, stree::iterator node_)
{
    _tree = &tree;
    _node = node_;
    _type = (*_node)->type;
    
    parseNode(tree, node_);
}

void Expression::parseNode(stree &tree, stree::iterator node_)
{
    stree::iterator i = node_;
    for (i++; i !=tree.end(node_);)
    {
        snode *o = *i;
        stype type = o->type;
        
        if (type == stype_name)
        {
            Name *name = acpulCoreData->readname(tree, i);
            requestObject(name, tree, i);
            i = tree.end(i);
        }
        else
        {
            i++;
        }
    }
}

void Expression::requestObject(Name *name, stree &tree, stree::iterator node)
{
    onmap::iterator i = _locals.find(name);
    Object *object = (i == _locals.end()) ? NULL : i->second;
    if (!object)
    {
        object = _block->object()->queryObjectWithParents(name);
        _locals[name] = object;
    }
}

Object *Expression::objectForName(Name *name)
{
/*    onmap::iterator i; -- what this do is unknown...
    onmap *params = _block->object()->params();
    if (params)
    {
        i = params->find(name);
        if (i != params->end())
            return i->second;
    }
    
    i = _locals.find(name);
    Object *object = (i == _locals.end()) ? NULL : i->second;
    return object;
 */
    onmap::iterator i;
    i = _locals.find(name);
    Object *object = (i == _locals.end()) ? NULL : i->second;
    if (object)
        return object;
    
    object = _block->object()->queryObjectWithParents(name);
    return object;
}

void Expression::fix()
{
    for (onmap::iterator it = _locals.begin(); it != _locals.end(); it++)
    {
        if (!it->second)
        {
            const Name *name = it->first;
//            printf("FIX "); // -- todo
//            it->first->dump();
            it->second = _block->object()->queryObjectWithParents(name);
//            if (it->second)
//                printf(" SUCCESS %p", it->second);
//            printf("\n");
        }
    }
}

