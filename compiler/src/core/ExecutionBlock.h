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

#ifndef ACPULanguage_ExecutionBlock_h
#define ACPULanguage_ExecutionBlock_h

#include <iostream>
#include "../coco/language/Parser.h"
#include "exprtk.hpp"
#include "CodeBlock.h"

namespace acpul {
    
    class CodeBlock;
    
    typedef enum {
        cexprt_expression   = 0,
        cexprt_condition    = 1,
    } cexpr_type;
    
    template <typename T>
    class control_expression
    {
    private:
        cexpr_type _type;
        exprtk::expression<T> _expression;
    public:
        
        control_expression()
        : _type(cexprt_expression)
        {}
        
        inline cexpr_type type()
        {
            return _type;
        }
        
        inline void setType(cexpr_type type)
        {
            _type = type;
        }
        
        inline exprtk::expression<T>& expression()
        {
            return _expression;
        }
    };
    
    typedef control_expression<float> ctrlexp;

    class ExecutionBlock
    {
        std::vector<ctrlexp *> code;
    public:
        ExecutionBlock();
        ~ExecutionBlock();

        void load(CodeBlock *codeBlock);
        void loadExpressionNode(CodeBlock *codeBlock, outtree &tree, outtree::iterator node);

        float execute();

        void info(const wchar_t *s, ...);
        void error(const wchar_t *s, ...);
    };
}

#endif
