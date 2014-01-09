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

#ifndef ACPULanguage_CodeBuilder_h
#define ACPULanguage_CodeBuilder_h

#include "CodeBlock.h"

namespace acpul {
    class Name;
    class Processor;
    class Compiler;
    
    class CodeBuilder
    {
        Processor *_processor;
    public:
        CodeBuilder() {}
        
        std::string build(Compiler *compiler, outtree &tree, outtree::iterator node);

//        void processExpressionMultipleNodes(stree &tree, stree::iterator node);
        void processExpressionNode(std::wstringstream &stream, stree &tree, stree::iterator node);

        void processFunctionNode(std::wstringstream &stream, stree &tree, stree::iterator node);
        
        void processName(std::wstringstream &stream, Name *name);

        void info(const wchar_t *s, ...);
        void error(const wchar_t *s, ...);

        std::string ToNarrow( const wchar_t *s, char dfault = '?', 
                                          const std::locale& loc = std::locale() );
    };
}

#endif
