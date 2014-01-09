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

#ifndef ACPULanguage_Expression_h
#define ACPULanguage_Expression_h

#include <iostream>
#include <tr1/unordered_map>

#include "../coco/language/Parser.h"

#include "Name.h"

namespace acpul {
    class Object;
    class Block;
    
    struct eqname {
        inline size_t operator()(const Name *name) const {
            return name->hash();
        }
        inline bool operator()(const Name *n1, const Name *n2) const {
            return n1->isEqual(*n2);
        }
    };
    
    typedef std::tr1::unordered_map<const Name *, Object *, eqname, eqname > onmap;
    

    class Expression
    {
        stree *_tree;
        stree::iterator _node;
        Block *_block;
        stype _type;

        onmap _locals;
        
    public:
        Expression() {}

        void setBlock(Block *block_)                { _block = block_; }
        Block *block()                              { return _block; }
        stype type()                                { return _type; }
        
        onmap &locals()                             { return _locals; }
        
        void setNode(stree &tree, stree::iterator node_);
        stree::iterator node()                      { return _node; }
        stree *tree()                               { return _tree; }

        Object *objectForName(Name *name);
        
        void parseNode(stree &tree, stree::iterator node_);
        
        void requestObject(Name *name, stree &tree, stree::iterator node);

        void fix();
        
        // DUMP HELPER
        void dumpLocals();
        void dumpFull(int prefix, int flags);
    };
}

#endif
