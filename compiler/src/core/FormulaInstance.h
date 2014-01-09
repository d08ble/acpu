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

#ifndef __ACPULanguage__FormulaInstance__
#define __ACPULanguage__FormulaInstance__

#include <iostream>
#include <map.h>
#include "BuildFile.h"
#include "Interpreter.h"
#include "ExecutionBlock.h"

namespace acpul {
    class FormulaInstance {
    public:
        typedef std::map<Interpreter *, int> itpmap;
        typedef std::map<int, int> fidmap;

    private:
        int _formulaId;
        int _blockId;
        BuildFile *_file;
        fidmap _deps;
        itpmap _interprters;
    
        typedef std::vector<ExecutionBlock *> _blocks;
        
    public:
        FormulaInstance();
        virtual ~FormulaInstance();

        BuildFile *file()                       { return _file; }
        
        void setFormulaId(int formulaId_)       { _formulaId = formulaId_; }
        int formulaId()                         { return _formulaId; }

        bool registerCode(const wchar_t *sourceCode);
        void unregisterCode();

        Interpreter *allocateInterpreter();
        void releaseInterpreter(Interpreter *interpreter);

        void checkBuild();
        void checkBuild2();
        int compile();
        void updateInterpreters(int blockId);
        void updateDeps();
        
        void addTargetDepence(int formulaId);

        void error(const wchar_t *s, ...);
    };
}

#endif
