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

#ifndef ACPULanguage_Compiler_h
#define ACPULanguage_Compiler_h

#include "ObjectsMap.h"
#include "CodeBlock.h"

namespace acpul {
    class Executor;
    
    class Compiler
    {
//        outtree outTree;
        ObjectsMap *_map;
        Expression *_exp;
        
        Executor *_executor;
        std::vector<CodeBlock *> _blocks;
        
        Object *_currentObject;
    public:
        Compiler() {}
        
        ObjectsMap *map()                                   { return _map; }
        
        std::vector<CodeBlock *> &blocks()                  { return _blocks; }
        
        Executor *executor()                                { return _executor; }
        
        int compile(ObjectsMap &map, Executor *executor);
        
        CodeBlock *compileObject(Object *object);
        CodeBlock *compileBlock(Block &block);
        CodeBlock *compileExpression(Expression *exp);

        outtree::iterator compileExpressionNode(stree &tree, stree::iterator node, outtree &otree, outtree::iterator onode);
        outtree::iterator compileFunctionNode(stree &tree, stree::iterator node, outtree &otree, outtree::iterator onode, bool multiple);

        Object *objectWithStaticName(Name *name);
        Object *objectWithExpressionNode(stree &tree, stree::iterator node);

        outtree::iterator outTreeInsertBlock(outtree &otree, outtree::iterator onode, CodeBlock *code, bool multiple);
        outtree::iterator outTreeInsertBlockA(outtree &otree, outtree::iterator onode, CodeBlock *code);

        outtree::iterator outTreeCreateTemporraryName(outtree &otree, outtree::iterator onode);
        outtree::iterator outTreeCreateFunction(outtree &otree, outtree::iterator onode);
        outtree::iterator outTreeCreateCallBlock(outtree &otree, outtree::iterator onode, int n);
        outtree::iterator outTreeCreateExpression(outtree &otree, outtree::iterator onode);
        outtree::iterator outTreeCreateName(outtree &otree, outtree::iterator onode, Name *name);
        outtree::iterator outTreeCreateNOPName(outtree &otree, outtree::iterator onode);
        outtree::iterator outTreeCreateIdent(outtree &otree, outtree::iterator onode, const wchar_t *ident);
        outtree::iterator outTreeCreateNumber(outtree &otree, outtree::iterator onode, const wchar_t *s);
        outtree::iterator outTreeCreateOperator(outtree &otree, outtree::iterator onode, const wchar_t *s);

        void codeBlockAttach(CodeBlock *code, CodeBlock *code1);
        
        int registerCodeBlock(CodeBlock *code);

        void error(const wchar_t *s, ...);
    };
}

#endif
