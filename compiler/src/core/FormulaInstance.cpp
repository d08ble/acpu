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

#include "FormulaInstance.h"
#include "CoreData.h"
#include "ErrorNumbers.h"
#include "Core.h"

using namespace acpul;

extern Core *acpulSharedCore;
extern CoreData *acpulCoreData;

FormulaInstance::FormulaInstance()
: _formulaId(INT_MIN)
, _blockId(-1)
, _file(NULL)
{
    
}

FormulaInstance::~FormulaInstance()
{
    // release file
    
    if (_file)
    {
        delete _file;
    }
    
    // release interpreters
    
    itpmap::iterator i;
    for (i = _interprters.begin(); i != _interprters.end(); i++)
    {
        error(L"Can't release allocated intepreter %p formulaId=%i", i->first, _formulaId);
    }
}

bool FormulaInstance::registerCode(const wchar_t *sourceCode)
{
    if (!_file)
    {
        _file = new BuildFile();
//        setlocale(LC_ALL, "rus_rus.866");
//        setlocale (LC_ALL, "utf-16"); 
//        std::wcout.imbue(std::locale("rus_rus.866"));
//        std::wcout.imbue(std::locale("rus_rus.866"));

//        std::cout << '\n' << '\n' <<"*** FORMULA=" << _formulaId << '\n';

        /*        char *aaa = "123ЭЭЭ";
        char *a = aaa;
        int l = strlen(aaa);
        for (int i = 0; i < l; i+=sizeof(char))
        {
            printf("%i", a[i]);
        }
        printf("%s %s\n", "acaaassssфыва", aaa);*/
//        printf("%S\n", L"a");
//        printf("%S\n", L"ффф");
//        std::wcout << L"ц";
//        std::wstring text(L"汉语");//sourceCode);
//        std::wcout << text;
//        std::string text("汉语");
//        std::cout << text;
//        std::cout << 
//        std::wcout <<L"ЭЭ" << '\n';

//        std::wcout << sourceCode << '\n';
//        std::wcout << flush();
        
        _file->setSource(sourceCode);
        _file->processSource();
        
        // update deps/interpreters
        checkBuild2();
    }
    else
    {
        error(L"fi file is not null");
    }
    
    return true;
}

void FormulaInstance::unregisterCode()
{
    if (_file)
    {
        delete _file;
        _file = NULL;
        
        // update deps/interpreters
    }
    _blockId = -1;
    updateInterpreters(_blockId);
}

Interpreter *FormulaInstance::allocateInterpreter()
{
    checkBuild();
    
    Interpreter *interpreter = new Interpreter();
    
    interpreter->setBlockId(_blockId);
    interpreter->setFormulaId(_formulaId);

    _interprters[interpreter] = _formulaId;
    
    return interpreter;

}

void FormulaInstance::releaseInterpreter(Interpreter *interpreter)
{
    if (interpreter)
    {
        itpmap::iterator i = _interprters.find(interpreter);
        
        if (i != _interprters.end())
        {
            _interprters.erase(i);
        
            delete interpreter;
        }
        else
        {
            error(L"interpreter not in map");
        }
    }
}

void FormulaInstance::checkBuild()
{
    if (_blockId == -1)
    {
        compile();
    }
}

void FormulaInstance::checkBuild2()
{
    if (_interprters.size() > 0)
    {
        compile();
    }
    // SHOULD PROCESSS DEPS
}

int FormulaInstance::compile()
{
    if (_file)
    {
        int prevFormulaId = acpulSharedCore->processingFormulaId();
        acpulSharedCore->setProcessingFormulaId(_formulaId);
        
        _blockId = _file->compile(acpulSharedCore->executor());
        
        updateInterpreters(_blockId);
        updateDeps();

        acpulSharedCore->setProcessingFormulaId(prevFormulaId);
    }
    
    return _blockId;
}

void FormulaInstance::updateInterpreters(int blockId)
{
    printf("formulaId=%i\n", formulaId());
    printf("updateInterpreters\n");
    
    itpmap::iterator i;
    for (i = _interprters.begin(); i != _interprters.end(); i++)
    {
        printf("%p: %i\n", i->first, blockId);
        i->first->setBlockId(blockId);
    }
}

void FormulaInstance::updateDeps()
{
    printf("updateDeps\n");
    
    fidmap deps = _deps;
    _deps.clear();
    
    fidmap::iterator i;
    for (i = deps.begin(); i != deps.end(); i++)
    {
        printf("%i: %i\n", _formulaId, i->first);
        acpulSharedCore->compileFormulaId(i->first);
    }
}

void FormulaInstance::addTargetDepence(int formulaId)
{
    _deps[formulaId] = formulaId;
}

void FormulaInstance::error(const wchar_t *s, ...)
{
    va_list ap;
    va_start(ap, s);
    acpulCoreData->error(EN_CORE_ERR, s, ap);
    va_end(ap);
}

