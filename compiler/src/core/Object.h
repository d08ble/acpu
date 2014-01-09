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

#ifndef ACPULanguage_Object_h
#define ACPULanguage_Object_h

#include <tr1/unordered_map>

#include "../coco/language/Parser.h"

#include "Name.h"
#include "Block.h"

namespace acpul
{
    class ObjectsMap;
    class Object;
    class CodeBlock;
    class Name;

    struct eqwstr {
        inline size_t operator()(const wchar_t *s) const {
            size_t hash = 1;
            for (; *s; ++s) hash = hash * 5 + *s;
            return hash;
        }
        inline bool operator()(const wchar_t *s1, const wchar_t *s2) const {
            return wcscmp(s1, s2) == 0;
        }
    };
    
    typedef std::tr1::unordered_map<const wchar_t *, Object *, eqwstr, eqwstr > omap;
    
    typedef tree<Object *> otree;

    class Object
    {
        int _uid;
        ObjectsMap *_map;
        otree::iterator _mapIterator;

        omap *_params;
        omap _objects;
        
        Object *_parent;
        Object *_followObject;
        Object *_link;
        
        const wchar_t *_ident;
        
        bool _blockUpdated;
        Block _block;
        
        CodeBlock *_codeBlock;
        
    public:
        Object();
        ~Object();
        
        int uid()                           { return _uid; }

        void setIdent(const wchar_t *ident) { _ident = ident; }
        const wchar_t *ident()              { return _ident; }
        
        void setParent(Object *parent_)     { _parent = parent_; }
        Object *parent()                    { return _parent; }
        
        Object *followObject()              { return _followObject; }
        
        Object *link()                      { return _link; }
        
        void setMap(ObjectsMap *map_)       { _map = map_; }
        ObjectsMap *map()                   { return _map; }
        
        void addExpressionNode(stree &tree, stree::iterator node);
        
        Object *queryObject2(const Name *name, int i);
        Object *queryObject(const Name *name);
        Object *queryObjectWithParents(const Name *name);

        void mergeObjects(Object *target);
        void mergeObjectsToMap(omap &map);

        bool getExpressionValueAsNumber(float &v);
//        bool Object::getExpressionValueAsIdent(const wchar_t &*v); -- old api
        Name *getExpressionValueAsName();

        void setObjectForIdent(const wchar_t *ident, Object *object);
        Object *objectForIdent(const wchar_t *ident);

        bool isLink();
        bool isFollowing();

        void follow(Object *object);
        
        void setMapIterator(otree::iterator mapIterator_)   { _mapIterator = mapIterator_; }
        otree::iterator mapIterator()                       { return _mapIterator; }

        Block &block()                      { return _block; }
        omap &objects()                     { return _objects; }
        
        void setCodeBlock(CodeBlock *codeBlock_)            { _codeBlock = codeBlock_; }
        CodeBlock *codeBlock()                              { return _codeBlock; }
        
        omap *params()                     { return _params; }
        void releaseParams();
        omap &saveParams();
        void restoreParams(omap &params);
        omap *setParams(omap *params);
        void unsetParams(omap *params);
        
        void error(const wchar_t *s, ...);
        
        // DUMP HELPER
        void dumpInfo();
        void dumpParams();
        void dumpObjects();
        void dumpFull(int prefix, int flags);
    };
}

#endif
