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

#ifndef ACPULanguage_Block_h
#define ACPULanguage_Block_h

#include <iostream>

#include "Expression.h"

namespace acpul {
    class Block
    {
        Object *_object;
        std::vector<Expression *> _expressions;
        
        bool _codeType; // 0 object "o;"  "o();"
                        // 1 code   "o+1; "o:=1;" "o;o;"
        
    public:
        Block() : _codeType(false) {}
        
        void setObject(Object *object_)                 { _object = object_; }
        Object *object()                                { return _object; }
        
        size_t expressionsCount()                       { return _expressions.size(); }
        Expression *expressionAtIndex(int i)            { return _expressions[i]; }
        
        bool codeType()                                 { return _codeType; }
        
        void addExpression(stree &tree, stree::iterator node);

        void updateCodeType();

        bool isLink();
        Object *getLinkObject();
        
        bool getNumber(float &v);
        
        bool isFollowing();

        // DUMP HELPER
        void dumpFull(int prefix, int flags);
    };
}

#endif
