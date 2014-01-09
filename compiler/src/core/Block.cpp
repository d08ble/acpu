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

#include "Block.h"
#include "Object.h"
#include "ObjectsMap.h"
#include "Processor.h"
#include "Expression.h"
#include "CoreData.h"

using namespace acpul;

extern CoreData *acpulCoreData;

void Block::addExpression(stree &tree, stree::iterator node)
{
    Expression *exp = acpulCoreData->newExpression();
    
    exp->setBlock(this);
    exp->setNode(tree, node);
    
    _expressions.push_back(exp);
    
    updateCodeType();
}

void Block::updateCodeType()
{
    if (_expressions.size() == 1)
    {
        std::vector<Expression *>::iterator i = _expressions.begin();
        
        if ((*i)->type() == stype_expression_simple)
        {
            stree &tree = *(*i)->tree();
            stree::iterator node = (*i)->node();
            
            if (tree.number_of_children(node) == 1)
            {
                node = tree.begin(node);
                
                stype type = (*node)->type;
                
                if (type == stype_name || type == stype_function)
                {
                    _codeType = false;
                    return;
                }
            }
        }
    }
    _codeType = true;
}

bool Block::isLink()
{
    int n = _expressions.size();
    if (_expressions.size() == 1)
    {
        Expression *exp = _expressions[0];
        stree *tree = exp->tree();
        stree::iterator node = exp->node();
        n = tree->number_of_children(node);
        if (n == 1)
        {
            if ((*node)->type == stype_expression_simple)
            {
                stree::iterator node1 = tree->begin(node);
                if ((*node1)->type == stype_name)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

Object *Block::getLinkObject()
{
    // should debug
    
    if (_expressions.size() == 1)
    {
        Expression *exp = _expressions[0];
        stree *tree = exp->tree();
        stree::iterator node = exp->node();
        int n = tree->number_of_children(node);
        if (n == 1)
        {
            if ((*node)->type == stype_expression_simple)
            {
                stree::iterator node1 = tree->begin(node);
                if ((*node1)->type == stype_name)
                {
                    Name *name = acpulCoreData->readname(*tree, node1);
                    if (name)
                    {
                        exp->fix(); // may slow & may bug in link/follow logic
                        return exp->objectForName(name);
                    }
                }
            }
        }
    }
    return NULL;
}

bool Block::getNumber(float &v)
{
    stree *tree = _expressions[0]->tree();
    stree::iterator node = _expressions[0]->node();
    
    while (tree->number_of_children(node) == 1)
    {
        if ((*node)->type == stype_expression_simple)
        {
            node = tree->begin(node);
            continue;
        }
        break;
    }
    if ((*node)->type == stype_number)
    {
        return acpulCoreData->readnumber(*tree, node, v);
    }
    return false;
}

bool Block::isFollowing()
{
    Name name;
    name.addIdent(L"_");
    for (int i = 0; i < _expressions.size(); i++)
    {
//        _expressions[i]->dumpLocals();
        onmap &locals = _expressions[i]->locals();
        onmap::iterator n = locals.find(&name);
        if (n != locals.end())
        {
            return true;
        }
    }
    return false;
}
