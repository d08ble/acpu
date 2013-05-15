//
//  CoreData.h
//  ACPULanguage
//
//  Copyright (c) 2013 d08ble. All rights reserved.
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

        void error(const wchar_t *s, ...);
        void error(int number, const wchar_t *s, ...);
        void error(int number, const wchar_t *s, va_list ap);
        void releaseErrors();

        Name *readname(stree &tree, stree::iterator node);
        bool readnumber(stree &tree, stree::iterator node, float &v);

        Name *newName();

        Name *nameForVarI(int j);
        Name *tempName();
        
        Object *newObject();

        Expression *newExpression();
        
        CodeBlock *newCodeBlock();
        
        ExecutionBlock *newExecutionBlock();
        
        acpul::Token *createToken();
        
        snode *newSnode(acpul::Token *la_, stype type_);

        void dumpNode(stree::iterator node);
        void dumpTree(stree &tree, stree::iterator start, stree::iterator end);
        void _print_depth(int i);
    };
}

#endif
