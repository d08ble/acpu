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

#ifndef ACPULanguage_Core_h
#define ACPULanguage_Core_h

#include "Executor.h"
#include "Interpreter.h"
#include "BuildFile.h"
#include "FormulaInstance.h"

namespace acpul {
    class CoreData;
    
    typedef std::map<int, BuildFile *> bfmap;
    typedef std::map<int, FormulaInstance *> fimap;

    class Core
    {
        Executor *_executor;
        
        CoreData *_coreData;
        
        fimap _formulaInstance;
        bfmap _fileMap; // this is double in instance
        
        int _processingFormulaId;
    public:
        Core();
        ~Core();
        
        bfmap &fileMap()                { return _fileMap; }
        Executor *executor()            { return _executor; }
        
        void setProcessingFormulaId(int formulaId)  { _processingFormulaId = formulaId; }
        int processingFormulaId()                   { return _processingFormulaId; }
        
        bool registerCode(int formulaId, const wchar_t *sourceCode);
        void unregisterCode(int formulaId);
        void releaseInstances();

        FormulaInstance *formulaInstanceForId(int formulaId);

        Interpreter *allocateInterpreter(int formulaId);
        void releaseInterpreter(Interpreter *interpreter);
        
        int compileFormulaId(int formulaId);
        
        stree *getSourceTreeForProcess(int formulaId);

        stree *getSourceTree(int formulaId);

        void error(const wchar_t *s, ...);
    };
}

#endif
