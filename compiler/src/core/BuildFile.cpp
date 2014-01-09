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
#include "BuildFile.h"
#include "Processor.h"
#include "Compiler.h"
#include "CoreData.h"

#define INFO__processSource \
printf("AAAAAAAAAAAAAAAAAAAAAA\n"); \
printf("AAAAAAAAAAAAAAAAAAAAAA\n"); \
printf("AAAAAAAAAAAAAAAAAAAAAA\n"); \
printf("AAAAAAAAAAAAAAAAAAAAAA\n"); \
_parser->dumpTree(*_tree);

using namespace acpul;

extern CoreData *acpulCoreData;

BuildFile::BuildFile()
: _tree(NULL)
, _source(NULL)
, _fail(true)
{
    _processor = new Processor();
    _compiler = new Compiler();
}

BuildFile::~BuildFile()
{
    releaseTree();
    releaseSource();
    releaseBuildContext();
    
    delete _processor;
    delete _compiler;
}

void BuildFile::setSource(const wchar_t *source)
{
    assert(source);

    releaseSource();
    
    int sz = (wcslen(source) + 1) * sizeof(*source);
    _source = (wchar_t *)malloc(sz);
    memcpy(_source, source, sz);
}

void BuildFile::setTree(stree &tree)
{
    releaseTree();
    _tree = new stree(false);
    *_tree = tree;
    
    acpulCoreData->streeRetain(_tree);
}

void BuildFile::releaseTree()
{
    if (_tree)
    {
        acpulCoreData->streeRelease(_tree);
        _tree = NULL;
    }
}

void BuildFile::releaseSource()
{
    if (_source)
    {
        delete _source;
        _source = NULL;
    }
}

void BuildFile::processSource()
{
    int len = wcslen(_source);
    _scanner = new Scanner(_source, len);
    _parser = new AParser(_scanner);
    
    _parser->Parse();
//    _parser->dumpTree();
    
    setTree(_parser->_tree);

    //INFO__processSource;
    
    // WARNING - SHOULD RETURN PARSE RESULT
}

void BuildFile::releaseBuildContext()
{
    if (_parser)
    {
        delete _parser;
        _parser = NULL;
    }
    if (_scanner)
    {
        delete _scanner;
        _scanner = NULL;
    }
}

int BuildFile::compile(Executor *executor)
{
    _processor->processSyntaxTree(_parser->_tree);                      // should debug call twice

    int blockId = _compiler->compile(_processor->getMap(), executor);   // should debug call twice
    
    return blockId;
}
