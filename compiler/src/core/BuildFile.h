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

#ifndef ACPULanguage_BuildFile_h
#define ACPULanguage_BuildFile_h

#include "AParser.h"

namespace acpul {
    class Executor;
    class Processor;
    class Compiler;
    
    class BuildFile
    {
        stree *_tree;
        wchar_t *_source;
        bool _fail;
        
        Scanner *_scanner;
        AParser *_parser;
        
        Processor *_processor;
        Compiler *_compiler;

    public:
        BuildFile();
        virtual ~BuildFile();
        
        void setTree(stree &tree);
        stree *tree()                       { return _tree; }
        
        void setSource(const wchar_t *source);
        const wchar_t *source()             { return _source; }
        
        Processor *processor()              { return _processor; }
        Compiler *compiler()                { return _compiler; }

        void releaseTree();
        void releaseSource();

        // PROCESS
        
        void processSource();
        
        void releaseBuildContext();

        int compile(Executor *executor);

    };
}

#endif
