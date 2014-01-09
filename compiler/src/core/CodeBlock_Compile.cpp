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
// COMPILE
//

int CodeBlock::compile(Compiler *compiler)
{
    _compiler = compiler;
    
    // merge count==1 nodes
    compileExpressionNode(_tree.begin());
    
    processRemoveNodes();
    
//    dump();
    
    int n = build();
    
    return n;
}

outtree::iterator CodeBlock::compileExpressionNode(outtree::iterator node)
{
    stype type = (*node)->type;
    int childCount = node.number_of_children();
    
    if (childCount == 1)
    {
        outtree::iterator child;
        child = _tree.begin(node);
        stype ctype = (*child)->type;
        
        if (type == stype_expression_simple)
        {
            if (ctype == stype_expression_simple
                || ctype == stype_function
                || ctype == stype_expression_assign
                || ctype == stype_expression_compound)
            {
                addCmdNode(node, 0);
                compileExpressionNode(child);
            }
            else if (ctype == stype_number
                || ctype == stype_name)
            {
                addCmdNode(node, 0);
            }
        }
        else if (type == stype_expression_assign ||
            type == stype_function ||
            type == stype_expressions ||
            type == stype_expression_compound ||
            type == stype_main)
        {
            compileExpressionNode(child);
        }
        return node;
    }
    else
    {
        stree::sibling_iterator i;
        i = _tree.begin(node);
        
        for (; i != _tree.end(node); i++)
        {
            compileExpressionNode(i);
        }
        
        return i;
    }
}

void CodeBlock::addCmdNode(outtree::iterator node, int n)
{
    if (n == 0)
    {
//        printf("Remove %p ", *node);
//        acpulCoreData->dumpNode(node);
        
        _removeNodes.push_back(node);
    }
    else if (n == 1)
    {
//        printf("Block %p ", *node);
//        acpulCoreData->dumpNode(node);
        
        _blockNodes.push_back(node);
    }
}

