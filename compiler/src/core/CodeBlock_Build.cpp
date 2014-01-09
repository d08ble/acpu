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
#include "CodeBlock.h"
#include "Compiler.h"
#include "Processor.h"
#include "CoreData.h"

using namespace acpul;

extern CoreData *acpulCoreData;

//
// BUILD
//

int CodeBlock::build()
{
    int n = _compiler->registerCodeBlock(this);
    
    
//    printf("BUILD BLOCK:\n");
//    dump();
    
    while (buildExpressionNode(_tree.begin()))
    {
        processRemoveNodes();
        buildBlocks();
        
//        dump();
    }
    
    return n;
}

bool CodeBlock::buildExpressionNode(outtree::iterator node)
{
    stype type = (*node)->type;
    int childCount = node.number_of_children();
    
    if (type == stype_expression_simple)
    {
        outtree::iterator parent = _tree.parent(node);
        bool inline_ = (*parent)->type == stype_main;
        
        outtree::sibling_iterator child;
        
        if (childCount == 1)
        {
            child = _tree.begin(node);
            stype type1 = (*child)->type;
            if (type1 == stype_expression_assign ||
                type1 == stype_expression_compound)
            {
                if (inline_)
                {
                    addCmdNode(node, 0); // remove
                }
                else
                {
                    addCmdNode(node, 1); // block childs
                }
                return true;
            }
        }
        else
        {
            bool single = false; // true if statment is expressions
            
            // check for normal expression for multiple nodes <a> <op> <b>
            stree::sibling_iterator i;
            i = _tree.begin(node);
            
            for (; i != _tree.end(node); i++)
            {
                stype t = (*i)->type;
                if (t == stype_operator)
                {
                    single = true;
                    break;
                }
            }
            
            if (!single)
            {
                // child "exp;exp;"
                addCmdNode(node, 1); // block childs
                return true;
            }
            else
            {
                // child "exp;"
                stree::sibling_iterator i;
                i = _tree.begin(node);
                bool res = false;
                
                for (; i != _tree.end(node); i++)
                {
                    res |= buildExpressionNode(i);
                }
                
                return res;
            }
        }
    }    
    else if (type == stype_expression_assign)
    {
        if (childCount != 2)
        {
            error1(L"Wrong assign expression", node);
            return false;
        }
        
        outtree::sibling_iterator child = _tree.begin(node);
        
/*        if ((*child)->type != stype_name) // -- maybe too many errors while debug by this line
        {
            error(L"Assign expression lvalue should be Name", child);
            return false;
        }*/
        bool res;
        res = buildExpressionNode(child);
        
        child++;
        
        res |= buildExpressionNode(child);
        return res;
    }
    else if (type == stype_expression_compound)
    {
        if (childCount != 3)
        {
            error1(L"Wrong compound expression", node);
            return false;
        }
        
        outtree::sibling_iterator child = _tree.begin(node);
        
/*        if ((*child)->type != stype_name) // -- same as previous
        {
            error(L"Assign expression lvalue should be Name", child);
            return false;
        }*/
        bool res;
        res = buildExpressionNode(child);
        
        child++;
        
        res |= buildExpressionNode(child);
        
        child++;
        
        if ((*child)->type == stype_expressions)
        {
            int childCount = child.number_of_children();
            if (childCount != 1)
            {
                addCmdNode(child, 1);
                return true;
            }
            outtree::sibling_iterator i = _tree.begin(child);
            stype t = (*i)->type;
            if (t != stype_expression_simple
                && t != stype_function
                && t != stype_name
                && t != stype_number)
            {
                addCmdNode(child, 1);
                return true;
            }
            res |= buildExpressionNode(child); // res=false
        }
        return res;
    }
    else if (type == stype_expressions)
    {
        stree::sibling_iterator i;
        i = _tree.begin(node);
        bool res = false;
        
        for (; i != _tree.end(node); i++)
        {
            res |= buildExpressionNode(i);
        }
        
        return res;
    }
    else if (type == stype_function)
    {
        if (childCount < 1)
        {
            error1(L"Wrong function", node);
            return false;
        }
        
        outtree::sibling_iterator child = _tree.begin(node);
        
/*        if ((*child)->type != stype_name) // -- again
        {
            error(L"Assign expression lvalue should be Name", child);
            return false;
        }*/
        
//        child++;
        
        bool res = false;
        
        for (; child != _tree.end(node); child++)
        {
            res |= buildExpressionNode(child);
        }
        
        return res;
    }
    else if (type == stype_main)
    {
        outtree::sibling_iterator child = _tree.begin(node);
        
        bool res = false;
        
        for (; child != _tree.end(node); child++)
        {
            res |= buildExpressionNode(child);
        }
        
        return res;
    }
    else if (type == stype_number)
    {
    }
    else if (type == stype_name)
    {
    }
    else if (type == stype_operator)
    {
    }
    else
    {
        error1(L"Undefined type", node);
    }
    
    return false;
}

