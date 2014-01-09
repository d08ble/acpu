

#if !defined(acpul_COCO_PARSER_H__)
#define acpul_COCO_PARSER_H__

#include "tree.hh"
#include "Scanner.h"

typedef enum {
  stype_main,
  stype_statement,
  stype_name,
  stype_ident,
  stype_number,
  stype_object,
  stype_object_declaration,
  stype_expressions,
  stype_expression_simple,
  stype_expression_assign,
  stype_term,
  stype_operator,
  stype_function,
  stype_block,
  stype_file,
  stype_expression_compound
} stype;

class snode {
public:
    int uid;
    acpul::Token *_la;
    stype type;
    bool sign;
    snode(acpul::Token *la_, stype type_, int uid_ = 0)
    {
        _la = la_;
        type = type_;
        uid = uid_;
        sign = false;
    }

    const wchar_t *val()
    {
        return _la ? _la->val : L"";
    }
};

class stree : public tree<snode *>
{
    stree::iterator _main;
    int _uid;
public:
    stree() : tree<snode *>()
    {
      _uid = 0;
      stree::iterator top;
      top = begin();
      _main = insert(top, new snode(NULL, stype_main, _uid));
      _uid++;
    }

    stree(bool a) : tree<snode *>()
    {
      _uid = 0;
    }

    stree::iterator main() {return _main;}

    stree::iterator add(stree::iterator node, acpul::Token *la, stype type)
    {
        stree::iterator n1 = append_child(node, new snode(la, type, _uid));
        _uid++;
        //printf("add %p<-%p\n", *node, *n1);
        return n1;
    }

    stree::iterator createUniqueObject(stree::iterator node, acpul::Token *la)
    {
        acpul::Token *la1 = uniqueToken();
        stree::iterator object = add(node, la1, stype_object); 
        stree::iterator name = add(object, la1, stype_name);
        stree::iterator ident = add(name, la1, stype_ident);
        ident = ident;
        return object;
    }

    acpul::Token *uniqueToken()
    {
        static int n = 0;             // todo fix unique for all objects
        acpul::Token *la = new acpul::Token();
        wchar_t *s = new wchar_t[32]; // todo fix cleanup
        swprintf(s, 32, L"~%i", n++);
        la->val = s;
        la->charPos = -1;
        return la;
    }

    stree::iterator createEmptyExpression(stree::iterator node, acpul::Token *la)
    {
        acpul::Token *la1 = new acpul::Token();
        la1->val = (wchar_t *)L"0";
        la1->charPos = -1;
        stree::iterator exp = add(node, la1, stype_expression_simple); 
        stree::iterator number = add(exp, la1, stype_number); 
        number = number;
        return exp;
    }
};


#include "Scanner.h"

namespace acpul {


class Errors {
public:
	int count;			// number of errors detected

	Errors();
	void SynErr(int line, int col, int n);
	virtual void Error(int line, int col, const wchar_t *s);
	virtual void Warning(int line, int col, const wchar_t *s);
	virtual void Warning(const wchar_t *s);
	virtual void Exception(const wchar_t *s);

}; // Errors

class Parser {
private:
	enum {
		_EOF=0,
		_ident=1,
		_number=2,
		_testpragma=30
	};
	int maxT;

	Token *dummyToken;
	int errDist;
	int minErrDist;

	void SynErr(int n);
	void Get();
	void Expect(int n);
	bool StartOf(int s);
	void ExpectWeak(int n, int follow);
	bool WeakSeparator(int n, int syFol, int repFol);

public:
	Scanner *scanner;
	Errors  *errors;

	Token *t;			// last recognized token
	Token *la;			// lookahead token

stree _tree;

typedef stree::iterator STI;

void dumpTree(stree &tree)
{
//  return; // ADEBUG_1

    stree::iterator sib2=tree.begin();
    stree::iterator end2=tree.end();

//    Buffer *buffer = scanner->buffer;

    const char *s[] = {
        "main",
        "statement",
        "name",
        "ident",
        "number",
        "object",
        "object_declaration",
        "expressions",
        "expression_simple",
        "expression_assign",
        "term",
        "operator",
        "function",
        "block",
        "file",
        "compound"
    };

    while (sib2!=end2)
    {
        for(int i=0; i<_tree.depth(sib2); ++i) //>
            printf(" ");
//        printf("%s '%S'\n", s[(*sib2)->type], buffer->GetString((*sib2)->start, (*sib2)->end));
        printf("%s %s%S\n", s[(*sib2)->type], (*sib2)->sign ? "SIGN" : "", (*sib2)->val());
        ++sib2;
    }
}

void dumpTree()
{
    dumpTree(_tree);
}

Token *NextAfterComplexIdent()
{
    scanner->ResetPeek();
    Token *x = scanner->Peek();

    int a = 0;
//    printf(">>");
    while (1)
    {
//        printf("%S", x->val);
        if (a)
        {
            if (x->kind != _ident)
                break;
        }
        else
            if (wcscmp(x->val, L"."))
                break;
        a ^= 1;
        x = scanner->Peek();
    }
//    printf("\n");
    return x;
}

bool IsObject()
{
    if (errDist==0)
      return false;
    Token *next = NextAfterComplexIdent();
    bool a = next->kind == _ident 
        || next->kind ==_number 
        || !wcscmp(next->val, L"{") 
        || !wcscmp(next->val, L"@");
//    printf("IsObject %S %S --- %i %i %s\n", la->val, next->val, la->kind, next->kind, a ? "+" : "-");
    scanner->ResetPeek();
    return a;
}

bool IsAssignExpression()
{
    if (errDist==0)
      return false;
    Token *next = NextAfterComplexIdent();
    bool a = !wcscmp(next->val, L":=")
             || !wcscmp(next->val, L"+=")
             || !wcscmp(next->val, L"-=")
             || !wcscmp(next->val, L"*=")
             || !wcscmp(next->val, L"/=")
             || !wcscmp(next->val, L"%=");
//    printf("IsAssignExpression %S %S --- %i %i %s\n", la->val, next->val, la->kind, next->kind, a ? "+" : "-");
    scanner->ResetPeek();
    return a;
}

bool IsFunction()
{
    if (errDist==0)
      return false;
    if (la->kind != _ident)
      return false;
    Token *next = NextAfterComplexIdent();
    bool a = !wcscmp(next->val, L"(");
//    printf("IsFunction %S %S --- %i %i %s\n", la->val, next->val, la->kind, next->kind, a ? "+" : "-");
    scanner->ResetPeek();
    return a;
}

bool IsCompoundExpression()
{
    if (errDist==0)
      return false;
    Token *next = NextAfterComplexIdent();
    if (!next)
      return false;
    bool res = false;
    if (!wcscmp(next->val, L"("))
    {
      int braces = 1;
      while(1)
      {
        next = scanner->Peek();
        //printf("%S\t%i\n", next->val, next->kind);
        if (!next)
        {
          // why this happened?
          break;
        }
        if (next->kind == 0)
        {
          // EOF
          break;
        }
        if (!wcscmp(next->val, L"("))
        {
          braces++;
        }
        if (!wcscmp(next->val, L")"))
        {
          braces--;
          if (braces == 0)
          {
            next = scanner->Peek();
            //printf("%S\t%i\n", next->val, next->kind);
            bool a = !wcscmp(next->val, L"{");
            res = a;
            break;
          }
        }
        // not this chars:
        if (!wcscmp(next->val, L"{") ||
            !wcscmp(next->val, L"}") ||
            !wcscmp(next->val, L";"))
          break;
      }
    }
    scanner->ResetPeek();
    return res;
}



	Parser(Scanner *scanner);
    Parser(Scanner *scanner, bool a);
	~Parser();
	void SemErr(const wchar_t* msg);

	void ACPU();
	void Statements(STI parent);
	void Statement(STI parent);
	void Object(STI parent);
	void Expression(STI parent);
	void ComplexIdent(STI parent);
	void ObjectDeclaration(STI parent);
	void ODBlockSimple(STI parent);
	void ODBlockComplex(STI parent);
	void ODBlockFile(STI parent);
	void Number(STI node);
	void AssignExpression(STI parent);
	void CompoundExpression(STI parent);
	void SimpleExpression(STI parent);
	void AOp(STI parent);
	void Term(STI parent);
	void OpAdd(STI parent);
	void Factor(STI parent);
	void OpMul(STI parent);
	void Function(STI parent);
	void Ident(STI node);
	void Arguments(STI parent);

	void Parse();

}; // end Parser

} // namespace


#endif

