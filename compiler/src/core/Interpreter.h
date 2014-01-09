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

#ifndef ACPULanguage_Interpreter_h
#define ACPULanguage_Interpreter_h

#include <iostream>
#include <map>

#include "VVCalculatorCPU.h"

namespace acpul {
    class Executor;
    
    class Interpreter
    {
    public:
        typedef std::map<int, Interpreter *> imap;
        
        vv::cpu _cpu;

        imap _callMap;
        
        Executor *_executor;
        
        int _formulaId;
        int _blockId;
        
        void *_owner;
        
    public:
        Interpreter();
        virtual ~Interpreter();

        Executor *executor()        { return _executor; }
        vv::cpu &cpu()              { return _cpu; }
        
        void setOwner(void *owner_) { _owner = owner_; }
        void *owner()               { return _owner; }
        
        void setFormulaId(int formulaId_)   { _formulaId = formulaId_; }
        int formulaId()                     { return _formulaId; }
        
        void setBlockId(int blockId_)       { _blockId = blockId_; }
        int blockId()                       { return _blockId; }
        
        void clearCallMap();

        void initCpu();
        void resetCpu();
        void end();

        float execute();

    };
}

#endif
