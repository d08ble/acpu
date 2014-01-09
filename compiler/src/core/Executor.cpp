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
#include "Executor.h"
#include "Compiler.h"
#include "CoreData.h"
#include "ErrorNumbers.h"

using namespace acpul;

extern CoreData *acpulCoreData;

Executor::Executor()
{
    initCpu();
}

Executor::~Executor()
{
    // cleanup
    _symbol_table.clear();
}


void Executor::link(Compiler &compiler)
{
    std::vector<CodeBlock *> &blocks = compiler.blocks();
    
//    std::vector<CodeBlock *>::iterator i;
    int i;

    for (i = 0; i < blocks.size(); i++)
    {
        CodeBlock *codeBlock = blocks[i];
        
        linkCodeBlock(codeBlock);
    }
}

void Executor::linkCodeBlock(CodeBlock *codeBlock)
{
    int i = codeBlock->blockId();
    
    ExecutionBlock *block = _blocks[i];
    
    if (!block)
    {
        error(L"Block %i not found", i);
    }
    else
    {
        block->load(codeBlock);
    }
}

float Executor::execute(float blockId)
{
    if (blockId >= 0 && blockId < _blocks.size())
    {
        ExecutionBlock *block = _blocks[blockId];
        return block->execute();
    }
    else
    {
        error(L"Block %i not found", (int)blockId);
    }
    return -1;
}

int Executor::allocateBlock()
{
    int n = _blocks.size();
    ExecutionBlock *block = acpulCoreData->newExecutionBlock();
    _blocks.push_back(block);
    return n;
}

void Executor::releaseBlock(int block)
{
    
}

void Executor::initCpu()
{
    _symbol_table.clear();
    _symbol_table.add_constants();
    _symbol_table.add_variable("x", cpu.globals.x);
    _symbol_table.add_variable("y", cpu.globals.y);
    _symbol_table.add_variable("s", cpu.globals.scale);
    //    _symbol_table.add_variable("sxy", cpu.globals.scaleXY);
    _symbol_table.add_variable("sx", cpu.globals.scaleX);
    _symbol_table.add_variable("sy", cpu.globals.scaleY);
    _symbol_table.add_variable("a", cpu.globals.alpha);
    _symbol_table.add_variable("r", cpu.globals.rotation);
    _symbol_table.add_variable("dt", cpu.globals.dt);
    _symbol_table.add_variable("lt", cpu.globals.lifeTime);
    _symbol_table.add_variable("p", cpu.globals.progress);
    _symbol_table.add_variable("t", cpu.globals.tcnt);
//    _symbol_table.add_variable("crgb", cpu.globals.crgb);
//    _symbol_table.add_variable("cr", cpu.globals.cr);
//    _symbol_table.add_variable("cb", cpu.globals.cb);
//    _symbol_table.add_variable("cg", cpu.globals.cg);
    _symbol_table.add_variable("w", cpu.globals.w);
    _symbol_table.add_variable("h", cpu.globals.h);
    
    
    _symbol_table.add_variable("r0", cpu.globals.r0);
    _symbol_table.add_variable("r1", cpu.globals.r1);
    _symbol_table.add_variable("r2", cpu.globals.r2);
    _symbol_table.add_variable("r3", cpu.globals.r3);
    _symbol_table.add_variable("r4", cpu.globals.r4);
    _symbol_table.add_variable("r5", cpu.globals.r5);
    _symbol_table.add_variable("r6", cpu.globals.r6);
    _symbol_table.add_variable("r7", cpu.globals.r7);
    _symbol_table.add_variable("o0", cpu.globals.shared.o0);
    _symbol_table.add_variable("o1", cpu.globals.shared.o1);
    _symbol_table.add_variable("o2", cpu.globals.shared.o2);
    _symbol_table.add_variable("o3", cpu.globals.shared.o3);
    _symbol_table.add_variable("o4", cpu.globals.shared.o4);
    _symbol_table.add_variable("o5", cpu.globals.shared.o5);
    _symbol_table.add_variable("o6", cpu.globals.shared.o6);
    _symbol_table.add_variable("o7", cpu.globals.shared.o7);
    _symbol_table.add_variable("zero", cpu.globals.zero);
    _symbol_table.add_variable("one", cpu.globals.one);
    
    _symbol_table.add_variable("p0", cpu.params.p0);
    _symbol_table.add_variable("p1", cpu.params.p1);
    _symbol_table.add_variable("p2", cpu.params.p2);
    _symbol_table.add_variable("p3", cpu.params.p3);
    _symbol_table.add_variable("p4", cpu.params.p4);
    _symbol_table.add_variable("p5", cpu.params.p5);
    _symbol_table.add_variable("p6", cpu.params.p6);
    _symbol_table.add_variable("p7", cpu.params.p7);
    
    _symbol_table.add_variable("ret", cpu.locals.ret);
    _symbol_table.add_variable("l0", cpu.locals.l0);
    _symbol_table.add_variable("l1", cpu.locals.l1);
    _symbol_table.add_variable("l2", cpu.locals.l2);
    _symbol_table.add_variable("l3", cpu.locals.l3);
    _symbol_table.add_variable("l4", cpu.locals.l4);
    _symbol_table.add_variable("l5", cpu.locals.l5);
    _symbol_table.add_variable("l6", cpu.locals.l6);
    _symbol_table.add_variable("l7", cpu.locals.l7);
    _symbol_table.add_variable("l8", cpu.locals.l8);
    _symbol_table.add_variable("l9", cpu.locals.l9);
    _symbol_table.add_variable("l10", cpu.locals.l10);
    _symbol_table.add_variable("l11", cpu.locals.l11);
    _symbol_table.add_variable("l12", cpu.locals.l12);
    _symbol_table.add_variable("l13", cpu.locals.l13);
    _symbol_table.add_variable("l14", cpu.locals.l14);
    _symbol_table.add_variable("l15", cpu.locals.l15);
    _symbol_table.add_variable("n", cpu.locals.n);
    _symbol_table.add_variable("i", cpu.locals.i);
    _symbol_table.add_variable("j", cpu.locals.j);
    _symbol_table.add_variable("k", cpu.locals.k);
    
    _symbol_table.add_function("rnd", fn.rnd);
    _symbol_table.add_function("create", fn.create);
    _symbol_table.add_function("call", fn.call);
    _symbol_table.add_function("watch", fn.watch);
    
    _symbol_table.add_function("shader", fn.shader);
    _symbol_table.add_function("uniform", fn.uniform);
    _symbol_table.add_function("uniform2", fn.uniform2);
    _symbol_table.add_function("uniform4", fn.uniform4);
    
    _symbol_table.add_function("io", fn.io);
    _symbol_table.add_function("mem", fn.mem);
    _symbol_table.add_function("set", fn.set);
    _symbol_table.add_function("set2", fn.set2);
    _symbol_table.add_function("set4", fn.set4);
    _symbol_table.add_function("get", fn.get);
    _symbol_table.add_function("get2", fn.get2);
    _symbol_table.add_function("get4", fn.get4);
    
    _symbol_table.add_function("draw", fn.draw);
    _symbol_table.add_function("point", fn.point);
    _symbol_table.add_function("line", fn.line);
    _symbol_table.add_function("rect", fn.rect);
    _symbol_table.add_function("circle", fn.circle);
    _symbol_table.add_function("qbezier", fn.qbezier);
    _symbol_table.add_function("cbezier", fn.cbezier);
    _symbol_table.add_function("triangle", fn.triangle);
    _symbol_table.add_function("point4", fn.point4);
    _symbol_table.add_function("color", fn.color);
    _symbol_table.add_function("psize", fn.psize);
    
    _symbol_table.add_function("block", fn.block);

    _symbol_table.add_variable("u0", cregs.u[0]);
    _symbol_table.add_variable("u1", cregs.u[1]);
    _symbol_table.add_variable("u2", cregs.u[2]);
    _symbol_table.add_variable("u3", cregs.u[3]);
    _symbol_table.add_variable("u4", cregs.u[4]);
    _symbol_table.add_variable("u5", cregs.u[5]);
    _symbol_table.add_variable("u6", cregs.u[6]);
    _symbol_table.add_variable("u7", cregs.u[7]);
    _symbol_table.add_variable("u8", cregs.u[8]);
    _symbol_table.add_variable("u9", cregs.u[9]);
    _symbol_table.add_variable("u10", cregs.u[10]);
    _symbol_table.add_variable("u11", cregs.u[11]);
    _symbol_table.add_variable("u12", cregs.u[12]);
    _symbol_table.add_variable("u13", cregs.u[13]);
    _symbol_table.add_variable("u14", cregs.u[14]);
    _symbol_table.add_variable("u15", cregs.u[15]);
    _symbol_table.add_variable("u16", cregs.u[16]);
    _symbol_table.add_variable("u17", cregs.u[17]);
    _symbol_table.add_variable("u18", cregs.u[18]);
    _symbol_table.add_variable("u19", cregs.u[19]);
    _symbol_table.add_variable("u20", cregs.u[20]);
    _symbol_table.add_variable("u21", cregs.u[21]);
    _symbol_table.add_variable("u22", cregs.u[22]);
    _symbol_table.add_variable("u23", cregs.u[23]);
    _symbol_table.add_variable("u24", cregs.u[24]);
    _symbol_table.add_variable("u25", cregs.u[25]);
    _symbol_table.add_variable("u26", cregs.u[26]);
    _symbol_table.add_variable("u27", cregs.u[27]);
    _symbol_table.add_variable("u28", cregs.u[28]);
    _symbol_table.add_variable("u29", cregs.u[29]);
    _symbol_table.add_variable("u30", cregs.u[30]);
    _symbol_table.add_variable("u31", cregs.u[31]);
    
    // CLEAR CPU
    
    memset(&cpu, 0, sizeof(cpu));
    memset(&cregs, 0, sizeof(cregs));
    cregs.u[0] = 1; // u0 = always one. used for block calling block(_u0...)
}

void Executor::error(const wchar_t *s, ...)
{
    va_list ap;
    va_start(ap, s);
    acpulCoreData->error(EN_EXE_ERR, s, ap);
    va_end(ap);
}

