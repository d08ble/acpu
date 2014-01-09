#include <iostream>
#include "../Parser.h"
#include "../Scanner.h"
//#include "../../../core/Processor.h"
#include "../../../core/Core.h"
#include <sys/stat.h>
#include "../../../core/Interpreter.h"
#include "../../../core/CoreTest.h"

using namespace std;

using namespace acpul;

#define TEST_1 0

#if TEST_1
static Scanner *scanner = NULL;
static Parser *parser;

int main()
{
    cout << "Hello World!" << endl;

    wchar_t *fileName = coco_string_create("../test.acpul");
    scanner = new Scanner(fileName);
    parser = new Parser(scanner);
//    parser->tab = new Taste::SymbolTable(parser);
//    parser->gen = new Taste::CodeGenerator();
    parser->Parse();
    parser->dumpTree();

    Processor *p = new Processor();
    p->processSyntaxTree(parser->_tree);

//    parser->gen->Decode();

    return 0;
}
#endif

#if 0
// TEST_2
int main()
{
//    wchar_t *fileName = coco_string_create("../test.acpul");

    const char *filename = "../test.acpul";
    FILE *file = fopen(filename, "r");

    struct stat st;
    stat(filename, &st);

    int size = st.st_size;
    char *data = new char[size];

    fread(data, 1, size, file);
    fclose(file);

    Core core;

    wchar_t *s;

    printf("\n----FILE: %s\n\n", filename);
    printf("%s", data);
    printf("\n\n----FILE END\n\n");

    s = coco_string_create(data);

    int formulaId = 0;
    bool res = core.registerCode(formulaId, s);
    Interpreter *interpreter = core.allocateInterpreter(formulaId);
    interpreter->execute();
    core.releaseInterpreter(interpreter);
//    float n = core.compile(s);
//    core.execute(n);

    coco_string_delete(s);
    delete data;

    return 0;
}
#endif

// TEST 3

//#include "CoreTest.h"
#if 0
int main()
{
    CoreTest test;
    
    test.testQT();

    return 0;
}
#endif

// TEST 4
// loop test

int main()
{
    for (int i = 0; i < 1; i++)
    {
        CoreTest *test = new CoreTest();
        
        test->testQT();
        
        delete test;
    }
}
