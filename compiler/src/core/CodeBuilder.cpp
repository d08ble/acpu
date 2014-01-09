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

#include <iostream>
#include <string>
#include <sstream>
#include "CodeBuilder.h"
#include "Name.h"
#include "Processor.h"
#include "Compiler.h"
#include "CoreData.h"
#include "ErrorNumbers.h"

using namespace acpul;

extern CoreData *acpulCoreData;

std::string CodeBuilder::build(Compiler *compiler, outtree &tree, outtree::iterator node)
{
    _processor = compiler->map()->processor();
    
    std::wstringstream stream(L"");
    
//    stream.seekp(0, std::ios::end);
//    stream << 12345;

    processExpressionNode(stream, tree, node);
                          
//    std::wcout << L"L:" << stream.str() << L'\n';
    
    std::string s = ToNarrow(stream.str().c_str());
    
//    std::cout << "s:" << s << '\n';
    info(L"> %s\n", s.c_str());

    return s;//stream.str();
}

/*
void CodeBuilder::processExpressionMultipleNodes(stree &tree, stree::iterator node)
{
    stree::sibling_iterator i;
    i = tree.begin(node);
    for (; i != tree.end(node); i++)
    {
        processExpressionNode(tree, i);
        printf(";\n");
    }
}
*/
void CodeBuilder::processExpressionNode(std::wstringstream &stream, stree &tree, stree::iterator node)
{
    stype type = (*node)->type;

    if ((*node)->sign)
        stream << "-";
    
    int childCount = node.number_of_children();
    
    if (type == stype_function)
    {
        processFunctionNode(stream, tree, node);
        return;
    }
    else if (type == stype_expressions)
    {
        stree::sibling_iterator i = tree.begin(node);
        for (; i != tree.end(node); i++)
        {
            processExpressionNode(stream, tree, i);
        }

        return;
    }
    if (childCount <= 1)
    {
        if (type == stype_name)
        {
            Name *name = acpulCoreData->readname(tree, node);
            processName(stream, name);
        }
        else if (type == stype_number)
        {
            stream << (*node)->val();
        }
        else if (type == stype_operator)
        {
            const wchar_t *s = (*node)->val();      // can be '+'
            if (!wcscmp(s, L"+="))
            {
                stream << L"+";
            }
            else if (!wcscmp(s, L"-="))
            {
                stream << L"-";
            }
            else if (!wcscmp(s, L"*="))
            {
                stream << L"*";
            }
            else if (!wcscmp(s, L"/="))
            {
                stream << L"/";
            }
            else if (!wcscmp(s, L"%="))
            {
                stream << L"%";
            }
            else
            {
                stream << s; //s[0] 
            }
        }
        else if (type == stype_expression_simple)
        {
//            stream << "(";
            processExpressionNode(stream, tree, tree.begin(node));
//            stream << ")";
            return;
        }
        else
        {
            error(L"UNKNOWN %i", type);
        }
    }
    else
    {

//        bool single = false;
/*        
        stree::sibling_iterator i;
        i = tree.begin(node);
        
        for (; i != tree.end(node); i++)
        {
            stype t = (*i)->type;
            if (t == stype_operator)
            {
//                single = true;
                break;
            }
        }
*/        
        stree::sibling_iterator i;
        i = tree.begin(node);
        
        if (type == stype_expression_assign)
        {
            Name *name = acpulCoreData->readname(tree, i);
            i++;
            
            processName(stream, name);
            
            stream << ":=";
            
//            single = true;
        }
        else if (type == stype_expression_compound)
        {
            Name *name = acpulCoreData->readname(tree, i);
            i++;
            
            processName(stream, name);
  
            Name *name1 = acpulCoreData->newName();
            name1->addIdent(L"if");
            if (name->isEqual(*name1))
            {
                // statment: IF(a, b, c)
                stream << "(";
                
                processExpressionNode(stream, tree, i);
                i++;

                stream << ",";

                processExpressionNode(stream, tree, i);
                i++;
                
                stream << ", 0";
                stream << ")";
            }
            else
            {
                // statment: WHILE(a){b}
                stream << "(";
                
                processExpressionNode(stream, tree, i);
                i++;
                
                stream << ")";

                stream << "{";
                
                processExpressionNode(stream, tree, i);
                i++;
                
                stream << "}";
            }
            return;
        }
        for (; i != tree.end(node); i++)
        {
            stype t = (*i)->type;
            if (t == stype_expression_simple)
            {
                stream << "(";
            }
            
            processExpressionNode(stream, tree, i);
            
            if (t == stype_expression_simple)
            {
                stream << ")";
            }
            
//            if (!single)
//                printf(";");
        }
    }
}

void CodeBuilder::processFunctionNode(std::wstringstream &stream, stree &tree, stree::iterator node)
{
    stree::sibling_iterator i;
    
    i = tree.begin(node);
    Name *name = acpulCoreData->readname(tree, i);
    processName(stream, name);
    stream << "(";
    for (i++; i != tree.end(node); i++)
    {
        processExpressionNode(stream, tree, i);
        outtree::sibling_iterator i1 = i;
        i1++;
        if (i1 != tree.end(node))
            stream << ",";
    }
    stream << ")";
}

void CodeBuilder::processName(std::wstringstream &stream, Name *name)
{
    if (name)
    {
        if (name->count() == 1)
        {
            const wchar_t *s = (*name)[0];
            
            stream << s;
            
            return;
        }
    }
    stream << L"NULLNAME";
    error(L"Name is undefined");
}

void CodeBuilder::info(const wchar_t *s, ...)
{
    va_list ap;
    va_start(ap, s);
    acpulCoreData->error(EN_EXE_INFO, s, ap);
    va_end(ap);
}

void CodeBuilder::error(const wchar_t *s, ...)
{
    va_list ap;
    va_start(ap, s);
    acpulCoreData->error(EN_EXE_ERR, s, ap);
    va_end(ap);
}


#include <locale>
#include <sstream>
#include <string>

std::string CodeBuilder::ToNarrow(const wchar_t *s, char dfault, 
                     const std::locale& loc)
{
    std::ostringstream stm;
    
    while( *s != L'\0' ) {
        stm << std::use_facet< std::ctype<wchar_t> >( loc ).narrow( *s++, dfault );
    }
    return stm.str();
}
