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
#include "Interpreter.h"
#include "Executor.h"
#include "Core.h"

using namespace acpul;

extern Core *acpulSharedCore;

Interpreter *acpulSharedInterpreter = NULL;

Interpreter::Interpreter()
    : _executor(NULL)
    , _owner(NULL)
    , _blockId(-1)
{
    initCpu();
    
    _executor = acpulSharedCore->executor();
}

Interpreter::~Interpreter()
{
    clearCallMap();
}

void Interpreter::clearCallMap()
{
    imap::iterator i;
    for (i = _callMap.begin(); i != _callMap.end(); i++) {
        acpulSharedCore->releaseInterpreter(i->second);
    }
    _callMap.clear();
}

void Interpreter::initCpu()
{
    memset(&_cpu, 0, sizeof(_cpu));
}

void Interpreter::resetCpu()
{
    _cpu.globals.tcnt = 0;
    _cpu.globals.lifeTime = 0;
    memset(&_cpu.locals, 0, sizeof(_cpu.locals));
    
    imap::iterator i;
    for (i = _callMap.begin(); i != _callMap.end(); i++)
    {
        i->second->resetCpu();
    }
}

void Interpreter::end()
{
    clearCallMap();
}

float Interpreter::execute()
{
    vv::cpu *cpu = &_executor->CPU();
    vv::cpu cpuPrev = *cpu;
    
    *cpu = _cpu;

    Interpreter *i = acpulSharedInterpreter;
    acpulSharedInterpreter = this;
    
    float ret = _executor->execute(_blockId);
    
    acpulSharedInterpreter = i;
    _cpu = *cpu;
    *cpu = cpuPrev;
    
    return ret;
}

// call

float vv_cpu_owner_call(float index, float formulaId, float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7)
{
    acpul::Interpreter::imap &map = acpulSharedInterpreter->_callMap;
    
    Interpreter *interpreter = map[formulaId];
    
    if (!interpreter)
    {
        interpreter = acpulSharedCore->allocateInterpreter(formulaId);
        if (!interpreter)
        {
            // lowlevel error can't allocate interpreter
            return -1;
        }
        interpreter->setOwner(acpulSharedInterpreter->owner()); // update owner

        map[formulaId] = interpreter;
    }
    
    vv::cpu *cpuA = &acpulSharedInterpreter->_cpu;
    vv::cpu *cpu = &interpreter->_cpu;

    cpu->globals = cpuA->globals;
    cpu->params.p0 = p0;
    cpu->params.p1 = p1;
    cpu->params.p2 = p2;
    cpu->params.p3 = p3;
    cpu->params.p4 = p4;
    cpu->params.p5 = p5;
    cpu->params.p6 = p6;
    cpu->params.p7 = p7;
    
    float ret = interpreter->execute();
    
    cpuA->globals = cpu->globals;
    
    return ret;
}

// block

float vv_cpu_owner_block(float n)
{
    float ret = acpulSharedInterpreter->_executor->execute(n);
    return ret;
}
