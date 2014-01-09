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

#ifndef ACPULanguage_Processor_h
#define ACPULanguage_Processor_h

#include "../coco/language/Parser.h"
#include "ObjectsMap.h"
#include "Name.h"
#include "Compiler.h"

namespace acpul {
    class Processor
    {
        ObjectsMap map;
        
        int recursiveIncludeCount;
        int recursiveIncludeCountMax;
    public:
        Processor();
        ~Processor();
        
        ObjectsMap &getMap() { return map; }
        
        void processSyntaxTree(stree &tree);
        Object *processNodes(stree &tree, stree::iterator node, Object *object);
        Object *processNode(stree &tree, stree::iterator node, Object *object);
        void processUnderscoreObject(stree &tree, stree::iterator node, Object *object);
        void processFile(stree &tree, stree::iterator node, Object *object);

        Object *objectForName(Name *name, Object *current);

        void error(const wchar_t *s, ...);
    };
}

#endif
