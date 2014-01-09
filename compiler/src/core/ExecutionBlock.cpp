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
#include "ExecutionBlock.h"
#include "CodeBuilder.h"
#include "CodeBlock.h"
#include "Compiler.h"
#include "Executor.h"
#include "ErrorNumbers.h"
#include "CoreData.h"

using namespace acpul;

extern CoreData *acpulCoreData;

ExecutionBlock::ExecutionBlock()
{
    
}

ExecutionBlock::~ExecutionBlock()
{
    for (int i = 0; i < code.size(); i++)
    {
        delete code[i];
    }
    code.clear();
}

void ExecutionBlock::load(CodeBlock *codeBlock)
{
    info(L"LOAD BLOCK: %i\n", codeBlock->blockId());

    outtree &tree = codeBlock->tree();
    outtree::iterator node = tree.begin();

    outtree::sibling_iterator i;
    i = tree.begin(node);
    for (; i != tree.end(node); i++)
    {
        loadExpressionNode(codeBlock, tree, i);
    }
}

void ExecutionBlock::loadExpressionNode(CodeBlock *codeBlock, outtree &tree, outtree::iterator node)
{
    CodeBuilder builder;
    
//    loadMultipleBlocks
    std::string s = builder.build(codeBlock->compiler(), tree, node);
    
    ctrlexp *ctrl = new ctrlexp();
    
    exprtk::parser<float>::optimization_level opt_level = exprtk::parser<float>::e_all;
    exprtk::parser<float> parser;

    exprtk::symbol_table<float> &symbol_table = codeBlock->compiler()->executor()->symbol_table();
    exprtk::expression<float>& expression = ctrl->expression();
    expression.register_symbol_table(symbol_table);

//    s = "r0:=block(r0, 0)";
//    s = "r0";
    if (!parser.compile(s, expression, opt_level))
    {
        error(L"FAIL EXPRESSION: %s", s.c_str());
    }

    code.push_back(ctrl);
}

float ExecutionBlock::execute()
{
    float ret = -1;
    for (std::size_t i = 0; i < code.size(); ++i)
    {
        ctrlexp *ctrl = code[i];
        if (ctrl->type() == cexprt_expression)
        {
            ret = ctrl->expression().value();
        }
    }
    return ret;
}

void ExecutionBlock::info(const wchar_t *s, ...)
{
    va_list ap;
    va_start(ap, s);
    acpulCoreData->error(EN_EXE_INFO, s, ap);
    va_end(ap);
}

void ExecutionBlock::error(const wchar_t *s, ...)
{
    va_list ap;
    va_start(ap, s);
    acpulCoreData->error(EN_EXE_ERR, s, ap);
    va_end(ap);
}

