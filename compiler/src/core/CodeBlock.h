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

#ifndef ACPULanguage_CodeBlock_h
#define ACPULanguage_CodeBlock_h

#include "../coco/language/Parser.h"

namespace acpul {
    class Compiler;
    
    typedef stree outtree;

    class CodeBlock
    {
        int _blockId;
        
        outtree _tree;
        
        Compiler *_compiler;

        std::vector<outtree::iterator> _removeNodes;
        std::vector<outtree::iterator> _blockNodes;
    public:
        CodeBlock();
        virtual ~CodeBlock();
        
        outtree &tree()                             { return _tree; }
        
        void setCompiler(Compiler *compiler_)       { _compiler = compiler_; }
        Compiler *compiler()                        { return _compiler; }
        
        void setBlockId(int blockId_)               { _blockId = blockId_; }
        int blockId()                               { return _blockId; }
        
        int compile(Compiler *compiler);
        outtree::iterator compileExpressionNode(outtree::iterator node);

        void addCmdNode(outtree::iterator node, int n);
        void processRemoveNodes();

        int build();
        bool buildExpressionNode(outtree::iterator node);
        void buildBlocks();
        CodeBlock *createCodeBlock(outtree::iterator node);
        
        void loadNode(outtree &tree, outtree::iterator node);

        void error1(const wchar_t *s, outtree::iterator node);
        void error(const wchar_t *s, ...);

        // DUMP HELPER
        void dump();
        // DUMP HELPER
        void dumpFull(int prefix, int flags);
    };
}

#endif
