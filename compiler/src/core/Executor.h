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

#ifndef ACPULanguage_Executor_h
#define ACPULanguage_Executor_h

#include "ExecutionBlock.h"
#include "VVCalculatorCPU.h"

namespace acpul {
    class Compiler;
    
    class Executor
    {
        std::vector<ExecutionBlock *> _blocks;
        
        vv::cpu cpu;
        vv::cpu_functions fn;
        vv::cpu_common_regs cregs;
        exprtk::symbol_table<float> _symbol_table;
    public:
        Executor();
        ~Executor();
        
        exprtk::symbol_table<float> &symbol_table()  { return _symbol_table; }
        vv::cpu &CPU()                               { return cpu; }
        
        void link(Compiler &compiler);
        void linkCodeBlock(CodeBlock *codeBlock);

        float execute(float block);

        int allocateBlock();
        void releaseBlock(int block);

        void initCpu();

        void error(const wchar_t *s, ...);
    };
}

#endif
