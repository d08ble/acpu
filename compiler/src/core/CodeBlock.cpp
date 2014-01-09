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
#include "ErrorNumbers.h"

using namespace acpul;

extern CoreData *acpulCoreData;

//
// 1. COMPILE - remove single parent-child nodes (duplicates after compilation)
// 2. BUILD - build blocks from tree, create callblock in tree. remove nodes
//

CodeBlock::CodeBlock()
: _blockId(-1)
{
    acpulCoreData->snodeRetain(*_tree.main());
}

CodeBlock::~CodeBlock()
{
    
}

void CodeBlock::processRemoveNodes()
{
    std::vector<outtree::iterator>::iterator i;
    
    for (i = _removeNodes.begin(); i != _removeNodes.end(); i++)
    {
        outtree::iterator node = *i;
        outtree::iterator child;
        
        child = _tree.begin(node);
        
        (*child)->sign ^= (*node)->sign;
        _tree.move_after(node, child);
        _tree.erase(node);
    }
    _removeNodes.clear();
}

void CodeBlock::buildBlocks()
{
    std::vector<outtree::iterator>::iterator i;

    for (i = _blockNodes.begin(); i != _blockNodes.end(); i++)
    {
        outtree::iterator node = *i;
        CodeBlock *block = createCodeBlock(node);
        
        int n = block->build();
        
        _tree.erase_children(node);
        _compiler->outTreeCreateCallBlock(_tree, node, n);
    }
}

CodeBlock *CodeBlock::createCodeBlock(outtree::iterator node)
{
    CodeBlock *block = acpulCoreData->newCodeBlock();
    
    block->loadNode(_tree, node);
    block->setCompiler(_compiler);
    
    return block;
}

void CodeBlock::loadNode(outtree &tree, outtree::iterator node)
{
    outtree::iterator i1 = _tree.main();
    outtree::sibling_iterator i2 = _tree.begin(i1);

    _tree.merge(i2, _tree.end(i1), tree.begin(node), tree.end(node));
}

void CodeBlock::error1(const wchar_t *s, outtree::iterator node)
{
    error(L"%S:%p:%i\n", s, *node, (*node)->type);
}

void CodeBlock::error(const wchar_t *s, ...)
{
    va_list ap;
    va_start(ap, s);
    acpulCoreData->error(EN_CODE_BLOCK_ERR, s, ap);
    va_end(ap);
}


