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
#include "Core.h"
#include "Processor.h"
#include <assert.h>
#include "CoreData.h"
#include "ErrorNumbers.h"

using namespace acpul;

extern CoreData *acpulCoreData;

Core *acpulSharedCore = NULL;

Core::Core()
{
    assert(!acpulSharedCore);
    acpulSharedCore = this;
    
    _executor = new Executor();
    
    _coreData = new CoreData();
    
    _processingFormulaId = INT_MIN;
}

Core::~Core()
{
    releaseInstances();
    
    delete _executor;
    _executor = NULL;
    
    delete _coreData;
    _coreData = NULL;
    
    acpulSharedCore = NULL;
}

void Core::releaseInstances()
{
    fimap::iterator i;
    for (i = _formulaInstance.begin(); i != _formulaInstance.end(); i++)
    {
        delete i->second;
    }
    _formulaInstance.clear();

    // also release file map internally
    _fileMap.clear();
}

FormulaInstance *Core::formulaInstanceForId(int formulaId)
{
    FormulaInstance *fi = _formulaInstance[formulaId];
    if (!fi)
    {
        fi = new FormulaInstance();
        fi->setFormulaId(formulaId);
        _formulaInstance[formulaId] = fi;
    }
    return fi;
}

bool Core::registerCode(int formulaId, const wchar_t *sourceCode)
{
    bool res;
    
    FormulaInstance *fi = formulaInstanceForId(formulaId);

    fi->unregisterCode();
    
    res = fi->registerCode(sourceCode);
    
    _fileMap[formulaId] = fi->file();

    return res;
}

void Core::unregisterCode(int formulaId)
{
    FormulaInstance *fi = formulaInstanceForId(formulaId);
    
    fi->unregisterCode();

    _fileMap.erase(_fileMap.find(formulaId));
}

Interpreter *Core::allocateInterpreter(int formulaId)
{
    FormulaInstance *fi = formulaInstanceForId(formulaId);

    Interpreter *interpreter = fi->allocateInterpreter();
    
    return interpreter;
}

void Core::releaseInterpreter(Interpreter *interpreter)
{
    if (interpreter)
    {
        FormulaInstance *fi = formulaInstanceForId(interpreter->formulaId());
        
        fi->releaseInterpreter(interpreter);
    }
}

stree *Core::getSourceTreeForProcess(int formulaId)
{
    if (_processingFormulaId == formulaId)
    {
        error(L"Can't recursive include");
        return NULL;
    }
  
    FormulaInstance *fi = formulaInstanceForId(formulaId);
    
    fi->addTargetDepence(_processingFormulaId);
    
    BuildFile *file = fi->file();
    
    if (!file)
    {
        error(L"File not found %i", formulaId);
        return NULL;
    }
    
    return file->tree();
}

stree *Core::getSourceTree(int formulaId)
{
    FormulaInstance *fi = formulaInstanceForId(formulaId);
    
    BuildFile *file = fi->file();
    
    if (!file)
    {
        error(L"File not found %i", formulaId);
        return NULL;
    }
    
    return file->tree();
}

int Core::compileFormulaId(int formulaId)
{
    FormulaInstance *fi = formulaInstanceForId(formulaId);
    
    return fi->compile();
}

void Core::error(const wchar_t *s, ...)
{
    va_list ap;
    va_start(ap, s);
    acpulCoreData->error(EN_CORE_ERR, s, ap);
    va_end(ap);
}


#if 0

#include "FORMULA-ID"

_ @1234567
a @100500

c 100500@;
e 100500$; // '$' is object!
d 100500?;

d 'this text shoudld be <div>HTML</div>????;

#endif
