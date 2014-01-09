

#include <wchar.h>
#include "Parser.h"
#include "Scanner.h"


namespace acpul {


void Parser::SynErr(int n) {
	if (errDist >= minErrDist) errors->SynErr(la->line, la->col, n);
	errDist = 0;
}

void Parser::SemErr(const wchar_t* msg) {
	if (errDist >= minErrDist) errors->Error(t->line, t->col, msg);
	errDist = 0;
}

void Parser::Get() {
	for (;;) {
		t = la;
		la = scanner->Scan();
		if (la->kind <= maxT) { ++errDist; break; }
		if (la->kind == _testpragma) {
				printf("%p", la->val); 
		}

		if (dummyToken != t) {
			dummyToken->kind = t->kind;
			dummyToken->pos = t->pos;
			dummyToken->col = t->col;
			dummyToken->line = t->line;
			dummyToken->next = NULL;
			coco_string_delete(dummyToken->val);
			dummyToken->val = coco_string_create(t->val);
			t = dummyToken;
		}
		la = t;
	}
}

void Parser::Expect(int n) {
	if (la->kind==n) Get(); else { SynErr(n); }
}

void Parser::ExpectWeak(int n, int follow) {
	if (la->kind == n) Get();
	else {
		SynErr(n);
		while (!StartOf(follow)) Get();
	}
}

bool Parser::WeakSeparator(int n, int syFol, int repFol) {
	if (la->kind == n) {Get(); return true;}
	else if (StartOf(repFol)) {return false;}
	else {
		SynErr(n);
		while (!(StartOf(syFol) || StartOf(repFol) || StartOf(0))) {
			Get();
		}
		return StartOf(syFol);
	}
}

void Parser::ACPU() {
		STI node = _tree.main(); 
		Statements(node);
}

void Parser::Statements(STI parent) {
		int count = 0; 
		while (StartOf(1)) {
			Statement(parent);
			count++; 
		}
		if (count == 0) {
		_tree.createEmptyExpression(parent, la);
		}
		
}

void Parser::Statement(STI parent) {
		if (IsObject()) {
			Object(parent);
		} else if (StartOf(1)) {
			Expression(parent);
		} else SynErr(30);
		Expect(3 /* ";" */);
}

void Parser::Object(STI parent) {
		STI node = _tree.add(parent, la, stype_object); 
		ComplexIdent(node);
		ObjectDeclaration(node);
}

void Parser::Expression(STI parent) {
		STI node = parent; 
		if (IsAssignExpression()) {
			AssignExpression(node);
		} else if (IsCompoundExpression()) {
			CompoundExpression(node);
		} else if (StartOf(1)) {
			SimpleExpression(node);
		} else SynErr(31);
}

void Parser::ComplexIdent(STI parent) {
		STI node = _tree.add(parent, la, stype_name); 
		Ident(node);
		while (la->kind == 11 /* "." */) {
			Get();
			Ident(node);
		}
}

void Parser::ObjectDeclaration(STI parent) {
		if (StartOf(1)) {
			ODBlockSimple(parent);
		} else if (la->kind == 4 /* "{" */) {
			ODBlockComplex(parent);
		} else if (la->kind == 6 /* "@" */) {
			ODBlockFile(parent);
		} else SynErr(32);
}

void Parser::ODBlockSimple(STI parent) {
		Expression(parent);
}

void Parser::ODBlockComplex(STI parent) {
		Expect(4 /* "{" */);
		Statements(parent);
		Expect(5 /* "}" */);
}

void Parser::ODBlockFile(STI parent) {
		STI node = _tree.add(parent, la, stype_file); 
		Expect(6 /* "@" */);
		if (la->kind == _number) {
			Number(node);
		} else if (la->kind == _ident) {
			ComplexIdent(node);
		} else SynErr(33);
}

void Parser::Number(STI node) {
		_tree.add(node, la, stype_number); 
		Expect(_number);
}

void Parser::AssignExpression(STI parent) {
		STI node = _tree.add(parent, la, stype_expression_assign); 
		ComplexIdent(node);
		if (la->kind == 9 /* ":=" */) {
			Get();
			SimpleExpression(node);
		} else if (StartOf(2)) {
			stree::iterator i = _tree.begin(node); 
			STI node1 = _tree.add(node, la, stype_expression_simple); 
			_tree.insert_subtree(_tree.begin(node1), i); 
			AOp(node1);
			SimpleExpression(node1);
		} else SynErr(34);
}

void Parser::CompoundExpression(STI parent) {
		STI object = _tree.createUniqueObject(parent, la); 
		STI name = _tree.begin(object); 
		
		// Create Compound
		STI node = _tree.add(parent, la, stype_expression_compound); 
		ComplexIdent(node);
		Expect(7 /* "(" */);
		SimpleExpression(node);
		Expect(8 /* ")" */);
		STI exps = _tree.add(node, la, stype_expressions);
		STI exp = _tree.add(exps, la, stype_expression_simple); 
		_tree.merge(_tree.begin(exp), _tree.end(exp), _tree.begin(object), _tree.end(name)); 
		Expect(4 /* "{" */);
		Statements(object);
		Expect(5 /* "}" */);
}

void Parser::SimpleExpression(STI parent) {
		STI node = _tree.add(parent, la, stype_expression_simple); 
		Term(node);
		while (la->kind == 10 /* "-" */ || la->kind == 18 /* "+" */) {
			OpAdd(node);
			Term(node);
		}
}

void Parser::AOp(STI parent) {
		STI node = _tree.add(parent, la, stype_operator);
		//printf("OP %S\n", la->val); 
		
		if (la->kind == 13 /* "+=" */) {
			Get();
		} else if (la->kind == 14 /* "-=" */) {
			Get();
		} else if (la->kind == 15 /* "*=" */) {
			Get();
		} else if (la->kind == 16 /* "/=" */) {
			Get();
		} else if (la->kind == 17 /* "%=" */) {
			Get();
		} else SynErr(35);
}

void Parser::Term(STI parent) {
		STI node = parent; 
		Factor(node);
		while (StartOf(3)) {
			OpMul(node);
			Factor(node);
		}
}

void Parser::OpAdd(STI parent) {
		STI node = _tree.add(parent, la, stype_operator);
		//printf("OP %S\n", la->val); 
		
		if (la->kind == 18 /* "+" */) {
			Get();
		} else if (la->kind == 10 /* "-" */) {
			Get();
		} else SynErr(36);
}

void Parser::Factor(STI parent) {
		STI node = parent; 
		if (IsFunction()) {
			Function(node);
		} else if (la->kind == _ident) {
			ComplexIdent(node);
		} else if (la->kind == _number) {
			Number(node);
		} else if (la->kind == 7 /* "(" */) {
			Get();
			SimpleExpression(node);
			Expect(8 /* ")" */);
		} else if (la->kind == 10 /* "-" */) {
			Get();
			node = _tree.add(parent, la, stype_expression_simple);
			(*node)->sign = true; 
			Factor(node);
		} else SynErr(37);
}

void Parser::OpMul(STI parent) {
		STI node = _tree.add(parent, la, stype_operator);
		//printf("OP %S\n", la->val); 
		
		switch (la->kind) {
		case 19 /* "*" */: {
			Get();
			break;
		}
		case 20 /* "/" */: {
			Get();
			break;
		}
		case 21 /* "%" */: {
			Get();
			break;
		}
		case 22 /* "^" */: {
			Get();
			break;
		}
		case 23 /* ">" */: {
			Get();
			break;
		}
		case 24 /* "<" */: {
			Get();
			break;
		}
		case 25 /* "!=" */: {
			Get();
			break;
		}
		case 26 /* "==" */: {
			Get();
			break;
		}
		case 27 /* "<=" */: {
			Get();
			break;
		}
		case 28 /* ">=" */: {
			Get();
			break;
		}
		default: SynErr(38); break;
		}
}

void Parser::Function(STI parent) {
		STI node = _tree.add(parent, la, stype_function); 
		ComplexIdent(node);
		Arguments(node);
}

void Parser::Ident(STI node) {
		_tree.add(node, la, stype_ident); 
		Expect(_ident);
}

void Parser::Arguments(STI parent) {
		Expect(7 /* "(" */);
		if (StartOf(1)) {
			SimpleExpression(parent);
			while (la->kind == 12 /* "," */) {
				Get();
				SimpleExpression(parent);
			}
		}
		Expect(8 /* ")" */);
}




// If the user declared a method Init and a mehtod Destroy they should
// be called in the contructur and the destructor respctively.
//
// The following templates are used to recognize if the user declared
// the methods Init and Destroy.

template<typename T>
struct ParserInitExistsRecognizer {
	template<typename U, void (U::*)() = &U::Init>
	struct ExistsIfInitIsDefinedMarker{};

	struct InitIsMissingType {
		char dummy1;
	};
	
	struct InitExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static InitIsMissingType is_here(...);

	// exist only if ExistsIfInitIsDefinedMarker is defined
	template<typename U>
	static InitExistsType is_here(ExistsIfInitIsDefinedMarker<U>*);

	enum { InitExists = (sizeof(is_here<T>(NULL)) == sizeof(InitExistsType)) };
};

template<typename T>
struct ParserDestroyExistsRecognizer {
	template<typename U, void (U::*)() = &U::Destroy>
	struct ExistsIfDestroyIsDefinedMarker{};

	struct DestroyIsMissingType {
		char dummy1;
	};
	
	struct DestroyExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static DestroyIsMissingType is_here(...);

	// exist only if ExistsIfDestroyIsDefinedMarker is defined
	template<typename U>
	static DestroyExistsType is_here(ExistsIfDestroyIsDefinedMarker<U>*);

	enum { DestroyExists = (sizeof(is_here<T>(NULL)) == sizeof(DestroyExistsType)) };
};

// The folloing templates are used to call the Init and Destroy methods if they exist.

// Generic case of the ParserInitCaller, gets used if the Init method is missing
template<typename T, bool = ParserInitExistsRecognizer<T>::InitExists>
struct ParserInitCaller {
	static void CallInit(T *t) {
		// nothing to do
	}
};

// True case of the ParserInitCaller, gets used if the Init method exists
template<typename T>
struct ParserInitCaller<T, true> {
	static void CallInit(T *t) {
		t->Init();
	}
};

// Generic case of the ParserDestroyCaller, gets used if the Destroy method is missing
template<typename T, bool = ParserDestroyExistsRecognizer<T>::DestroyExists>
struct ParserDestroyCaller {
	static void CallDestroy(T *t) {
		// nothing to do
	}
};

// True case of the ParserDestroyCaller, gets used if the Destroy method exists
template<typename T>
struct ParserDestroyCaller<T, true> {
	static void CallDestroy(T *t) {
		t->Destroy();
	}
};

void Parser::Parse() {
	t = NULL;
	la = dummyToken = new Token();
	la->val = coco_string_create(L"Dummy Token");
	Get();
	ACPU();
	Expect(0);
}

Parser::Parser(Scanner *scanner) {
	maxT = 29;

	ParserInitCaller<Parser>::CallInit(this);
	dummyToken = NULL;
	t = la = NULL;
	minErrDist = 2;
	errDist = minErrDist;
	this->scanner = scanner;
	errors = new Errors();
}

bool Parser::StartOf(int s) {
	const bool T = true;
	const bool x = false;

	static bool set[4][31] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,T,T,x, x,x,x,T, x,x,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,T,T,T, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,T, T,T,T,T, T,T,T,T, T,x,x}
	};



	return set[s][la->kind];
}

Parser::~Parser() {
	ParserDestroyCaller<Parser>::CallDestroy(this);
	delete errors;
	delete dummyToken;
}

Errors::Errors() {
	count = 0;
}

void Errors::SynErr(int line, int col, int n) {
	wchar_t* s;
	switch (n) {
			case 0: s = coco_string_create(L"EOF expected"); break;
			case 1: s = coco_string_create(L"ident expected"); break;
			case 2: s = coco_string_create(L"number expected"); break;
			case 3: s = coco_string_create(L"\";\" expected"); break;
			case 4: s = coco_string_create(L"\"{\" expected"); break;
			case 5: s = coco_string_create(L"\"}\" expected"); break;
			case 6: s = coco_string_create(L"\"@\" expected"); break;
			case 7: s = coco_string_create(L"\"(\" expected"); break;
			case 8: s = coco_string_create(L"\")\" expected"); break;
			case 9: s = coco_string_create(L"\":=\" expected"); break;
			case 10: s = coco_string_create(L"\"-\" expected"); break;
			case 11: s = coco_string_create(L"\".\" expected"); break;
			case 12: s = coco_string_create(L"\",\" expected"); break;
			case 13: s = coco_string_create(L"\"+=\" expected"); break;
			case 14: s = coco_string_create(L"\"-=\" expected"); break;
			case 15: s = coco_string_create(L"\"*=\" expected"); break;
			case 16: s = coco_string_create(L"\"/=\" expected"); break;
			case 17: s = coco_string_create(L"\"%=\" expected"); break;
			case 18: s = coco_string_create(L"\"+\" expected"); break;
			case 19: s = coco_string_create(L"\"*\" expected"); break;
			case 20: s = coco_string_create(L"\"/\" expected"); break;
			case 21: s = coco_string_create(L"\"%\" expected"); break;
			case 22: s = coco_string_create(L"\"^\" expected"); break;
			case 23: s = coco_string_create(L"\">\" expected"); break;
			case 24: s = coco_string_create(L"\"<\" expected"); break;
			case 25: s = coco_string_create(L"\"!=\" expected"); break;
			case 26: s = coco_string_create(L"\"==\" expected"); break;
			case 27: s = coco_string_create(L"\"<=\" expected"); break;
			case 28: s = coco_string_create(L"\">=\" expected"); break;
			case 29: s = coco_string_create(L"??? expected"); break;
			case 30: s = coco_string_create(L"invalid Statement"); break;
			case 31: s = coco_string_create(L"invalid Expression"); break;
			case 32: s = coco_string_create(L"invalid ObjectDeclaration"); break;
			case 33: s = coco_string_create(L"invalid ODBlockFile"); break;
			case 34: s = coco_string_create(L"invalid AssignExpression"); break;
			case 35: s = coco_string_create(L"invalid AOp"); break;
			case 36: s = coco_string_create(L"invalid OpAdd"); break;
			case 37: s = coco_string_create(L"invalid Factor"); break;
			case 38: s = coco_string_create(L"invalid OpMul"); break;

		default:
		{
			wchar_t format[20];
			coco_swprintf(format, 20, L"error %d", n);
			s = coco_string_create(format);
		}
		break;
	}
//	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
    Error(line, col, s);
	coco_string_delete(s);
	count++;
}

void Errors::Error(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	count++;
}

void Errors::Warning(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
}

void Errors::Warning(const wchar_t *s) {
	wprintf(L"%ls\n", s);
}

void Errors::Exception(const wchar_t* s) {
	wprintf(L"%ls", s); 
	exit(1);
}

} // namespace

