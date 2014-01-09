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

#ifndef ACPULanguage_CoreData_h
#define ACPULanguage_CoreData_h

#include "../coco/language/Parser.h"
#include <map>

namespace acpul {

    struct GCObject
    {
        int _references;
        int _type;
//        void *obj;
        
        GCObject()
        {
            _references = 0;
            _type = -1;
        }
        
        GCObject(void *obj, int type_)
        {
            _references = 1;
            _type = type_;
        }
        
        void use()
        {
            _references++;
        }
        
        void unuse()
        {
            _references--;
        }
    };

    typedef std::map<void *, GCObject> gcmap;
    
    class Error;
    class Name;
    class Object;
    class Block;
    class Expression;
    class CodeBlock;
    class ExecutionBlock;
    
//    typedef std::map<void *, int> gcmap;
    typedef std::vector<Error *> aerrors;
    
    class CoreData
    {
        gcmap _map;
        aerrors _errors;
        
        // config
        bool _errorsEnabled;
        bool _errorsConsole;
    public:
        CoreData();
        ~CoreData();
        
        void releaseGCObjects();

        void infoGcObjectForceRelease(gcmap::iterator i);

        void snodeRetain(snode *node);
        void snodeRelease(snode *node);

        void streeRetain(stree *tree);
        void streeRelease(stree *tree);
        
        void nameRetain(Name *name);
        void nameRelease(Name *name);
        
        void identRetain(const wchar_t *ident);
        void identRelease(const wchar_t *ident);
        
        void objectRetain(Object *object);
        void objectRelease(Object *object);
        
        void blockRetain(Block *block);
        void blockRelease(Block *block);
        
        void expressionRetain(Expression *expression);
        void expressionRelease(Expression *expression);
        
        void codeBlockRetain(CodeBlock *codeBlock);
        void codeBlockRelease(CodeBlock *codeBlock);
        
        void executionBlockRetain(ExecutionBlock *executionBlock);
        void executionBlockRelease(ExecutionBlock *executionBlock);

        void tokenRetain(acpul::Token *la);
        void tokenRelease(acpul::Token *la);

        void retain(void *obj, int type);
        void release(void *obj);
        void del(gcmap::iterator i);

        // Errors

        void setErrorsEnabled(bool errorsEnabled_)  { _errorsEnabled = errorsEnabled_; }
        bool errorsEnabled()                        { return _errorsEnabled; }
        void setErrorsConsole(bool errorsConsole_)  { _errorsConsole = errorsConsole_; }
        bool errorsConsole()                        { return _errorsConsole; }
        
        aerrors &errors()           { return _errors; }
        void error(const wchar_t *s, ...);
        void error(int number, const wchar_t *s, ...);
        void error(int number, const wchar_t *s, va_list ap);
        void releaseErrors();

        Name *readname(stree &tree, stree::iterator node);
        bool readnumber(stree &tree, stree::iterator node, float &v);

        Name *newName();

        Name *nameForVarI(int j);
        Name *tempName();
        Name *nameForIdent(const wchar_t *ident);

        const wchar_t *identForVarI(int j);

        Object *newObject();

        Expression *newExpression();
        
        CodeBlock *newCodeBlock();
        
        ExecutionBlock *newExecutionBlock();
        
        acpul::Token *createToken();
        
        snode *newSnode(acpul::Token *la_, stype type_);

        // DUMP HELPER
        static void dumpNode(stree::iterator node);
        static void dumpTree(stree &tree, stree::iterator start, stree::iterator end);

        static void dumpExpression(Expression *exp);
        static void dumpExpressionMultipleNodes(stree &tree, stree::iterator node);
        static void dumpExpressionNode(stree &tree, stree::iterator node);
        static void dumpFunctionNode(stree &tree, stree::iterator node);
        static void dumpExpressionMultipleNodesA(stree &tree, stree::iterator node);
        static void dumpExpressionMultipleNodesA1(stree &tree, stree::iterator node, int prefix);
        static void dumpExpressionNodeA(stree &tree, stree::iterator node, int prefix);
        static void dumpFunctionNodeA(stree &tree, stree::iterator node, int prefix);
    };
}

#endif
