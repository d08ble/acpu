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

#ifndef ACPULanguage_Dumper_h
#define ACPULanguage_Dumper_h

#include "../coco/language/Parser.h"
#include "ObjectsMap.h"

namespace acpul {
    class Core;
    
    class Dumper
    {
        std::string outDir;
    public:
        Dumper();
        ~Dumper();
        
        void exportJsonCore(Core &core);

        void jsonCore(std::wostream &stream);

        void jsonCore(std::wostream &s, int prefix, Core &core);
        void jsonFileMap(std::wostream &s, int prefix, bfmap &fileMap);
        void jsonBuildFile(std::wostream &s, int prefix, BuildFile *file);
        void jsonStree(std::wostream &s, int prefix, stree *tree);
        void jsonStreeNode(std::wostream &s, int prefix, stree *tree, stree::iterator node);
        void jsonStreeNodeChilds(std::wostream &s, int prefix, stree *tree, stree::iterator node);
        void jsonStreeNodeData(std::wostream &s, int prefix, stree::iterator node);

        void outPrefix(std::wostream &o, int prefix);

        void jsonBuildFileSource(int i, const wchar_t *source);

        void jsonObjects(std::wostream &s, int prefix, ObjectsMap &map);
        void jsonObjectsTree(std::wostream &s, int prefix, otree &tree);
        void jsonObjectsNode(std::wostream &s, int prefix, otree &tree, otree::iterator node);
        void jsonObjectsNodeDumpProps(std::wostream &s, int prefix, otree::iterator node);

        // export c++
        // export js
        // export as3
        // export java
        
//        std::ostream &requireStream(const char *path, std::ostream &s);
        
        void test();
    };
}

#endif
