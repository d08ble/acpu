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
#include <sys/stat.h>

#include "CoreTest.h"

#include <iostream>
//#include "Parser.h"
//#include "Scanner.h"
#include "../coco/language/Parser.h"
#include "Core.h"
#include "Interpreter.h"
#include "Dumper.h"

using namespace acpul;

CoreTest::CoreTest()
{
   core = new Core();
}

CoreTest::~CoreTest()
{
    if (core)
    {
        delete core;
        core = NULL;
    }
}

void CoreTest::testQT()
{
    struct
    {
        int formulaId;
        const char *filename;
    } files[] = {
        0, "../test-function-bug-b.acpul",
//        0, "../test-function-bug-a.acpul",
//        0, "../test-operator-pow.acpul",
//        0, "../test-compound-bug-a.acpul",
//        0, "../test-object-def-b.acpul",
//        0, "../test-object-def-a.acpul",
//        0, "../test-_params.acpul",
//        0, "../test-compound.acpul",
//        1, "../stack.acpul",
//        0, "../test.acpul",
//        0, "../test-follow-link.acpul",
    };
    
    int executeId[] = {
        // minimal 3 items
        0, 
        0, 
        1,
        -1739203 // undefined test
    };
    
    for (int i = 0; i < sizeof(files)/sizeof(files[0]); i++)
    {
        int formulaId = files[i].formulaId;
        const char *filename = files[i].filename;

        int res = loadFile(formulaId, filename);

        printf("TEST_QT: loadFile(%i, %s) result is %i\n", formulaId, filename, res);
    }

    Interpreter *interpreter0 = core->allocateInterpreter(executeId[0]);
    interpreter0->execute();

    Interpreter *interpreter1 = core->allocateInterpreter(executeId[1]);
    interpreter1->execute();

    Interpreter *interpreter2 = core->allocateInterpreter(executeId[2]);
    interpreter2->execute();
    
    Interpreter *interpreterNULL = core->allocateInterpreter(executeId[3]);

 //    float n = core.compile(s);
    //    core.execute(n);
/*
    Dumper *dumper = new Dumper();
    dumper->exportJsonCore(*core);
*/    
    
    core->releaseInterpreter(interpreter0);
    core->releaseInterpreter(interpreter1);
    core->releaseInterpreter(interpreter2);
    core->releaseInterpreter(interpreterNULL);
    
//    delete dumper;
}

int CoreTest::loadFile(int formulaId, const char *filename)
{
    FILE *file = fopen(filename, "r");
    
    if (!file)
        return -1;
    
    struct stat st;
    stat(filename, &st);
    
    int size = st.st_size;
    char *data = new char[size+1];
    data[size] = 0;
    fread(data, 1, size, file);
    fclose(file);
    
    wchar_t *s;
    
//    printf("\n----FILE %i: %s\n\n", formulaId, filename);
//    printf("%s", data);
//    printf("\n\n----FILE END\n\n");
    
    s = coco_string_create(data);
    
    int res = core->registerCode(formulaId, s);

    coco_string_delete(s);
    delete data;
    
    return res;
}
