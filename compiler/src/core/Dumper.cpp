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

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include "Core.h"
#include "Dumper.h"
#include "Processor.h"

std::stringstream s;

using namespace acpul;

#define DUMPER_ROOT_DIR         "/Users/vv/web/iPixelSDK/code/"
#define DUMPER_CORE_FILE        DUMPER_ROOT_DIR "core.json"
#define DUMPER_CORE_SOURCES     DUMPER_ROOT_DIR "sources/"

//#define DUMPER_STREAM_CORE_FILE     DUMPER_STREAM_CORE_DIR "/stree.json"
//#define DUMPER_STREAM_CORE_SOURCE   DUMPER_STREAM_CORE_DIR "/sources"
//#define DUMPER_STREAM_CORE_FAIL     DUMPER_STREAM_CORE_DIR "/fail.json"

#define SPX outPrefix(s, prefix); s

Dumper::Dumper()
{
//    test();
}

Dumper::~Dumper()
{
    
}

void Dumper::exportJsonCore(Core &core)
{
    //
    // PROCESSOR
    // - OBJECTS
    //
    // COMPILER
    // - CODE BLOCKS
    //
    // EXECUTOR
    // - BLOCKS
    // -? CPU
    //
    // CORE
    // + FILE MAP
    // ++ ID : FILE
    // +++ SYNTAX TREE
    // --- FILE TEXT
    // - INTERPRETER
    // - ALLOCATIONS
    //
    
    std::wstringstream s;
    int prefix = 0;
    jsonCore(s, prefix, core);
//    printf("%S\n", s.str().c_str());
    
    std::wofstream f(DUMPER_CORE_FILE);
    f << s.str().c_str();
}

void Dumper::test()
{
    std::wstringstream s;
    
    s << "TEST:\n";

    printf("%s\n", s.str().c_str());
    
}

void Dumper::jsonCore(std::wostream &s, int prefix, Core &core)
{
    bfmap fileMap = core.fileMap();
    
    jsonFileMap(s, prefix, fileMap);
//    jsonInterpreters(stream);
//    jsonAllocations(stream);
}

void Dumper::jsonFileMap(std::wostream &s, int prefix, bfmap &fileMap)
{
    SPX << "{\n";
    bfmap::iterator i;
    int n = 0;
    prefix++;
    for (i = fileMap.begin(); i != fileMap.end(); i++)
    {
        if (n++ > 0)
        {
            SPX << ",\n";
        }
        else
        {
            s << "\n";
        }

        SPX << "\"" << (int)i->first << "\":";

        BuildFile *file = i->second;

        if (file)
        {
            jsonBuildFile(s, prefix, file);
            jsonBuildFileSource(i->first, file->source());
        }
        else
        {
//            s << "\"NULL\"\n";
            s << "null\n";
        }
    }
    prefix--;
    SPX << "}\n";
}

void Dumper::jsonBuildFile(std::wostream &s, int prefix, BuildFile *file)
{
    // STREE
    
    SPX << "{\n";
    prefix++;
    
    SPX << "\"stree\":";
    jsonStree(s, prefix, file->tree());
    
    SPX << ",\n";
    SPX << "\"objects\":";
    jsonObjects(s, prefix, file->processor()->getMap());
    
    prefix--;
    SPX << "}\n";
    // FAIL
//    s = requireStream(DUMPER_STREAM_CORE_FAIL, s);
}

//
// STREE
//

void Dumper::jsonStree(std::wostream &s, int prefix, stree *tree)
{
    stree::iterator node = tree->begin();
    jsonStreeNode(s, prefix, tree, node);
}

void Dumper::jsonStreeNode(std::wostream &s, int prefix, stree *tree, stree::iterator node)
{
    SPX << "{\n";
    prefix++;
    jsonStreeNodeData(s, prefix, node);
    jsonStreeNodeChilds(s, prefix, tree, node);
    prefix--;
    SPX << "}\n";
}

void Dumper::jsonStreeNodeChilds(std::wostream &s, int prefix, stree *tree, stree::iterator node)
{
    SPX << "\"nodes\":[";
    prefix++;
    stree::sibling_iterator i = tree->begin(node);
    int n = 0;
    for (;i != tree->end(node); i++)
    {
        if (n++ > 0)
        {
            SPX << ",\n";
        }
        else
        {
            s << "\n";
        }
        jsonStreeNode(s, prefix, tree, i);
    }
    prefix--;
    SPX << "]\n";
}

void Dumper::jsonStreeNodeData(std::wostream &s, int prefix, stree::iterator node)
{
    static const char *st[] = {
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
    
    snode *o = *node;
    
    SPX << "\"id\":\"" << o->uid << "\",\n";
    SPX << "\"type\":\"" << st[o->type] << "\",\n";
    SPX << "\"val\":\"" << o->val() << "\",\n";

    if (o->_la)
    {
        SPX << "\"line\":\"" << o->_la->line << "\",\n";
        SPX << "\"col\":\"" << o->_la->col << "\",\n";
        SPX << "\"pos\":\"" << o->_la->pos << "\",\n";
        SPX << "\"charPos\":\"" << o->_la->charPos << "\",\n";
    }
}

void Dumper::outPrefix(std::wostream &o, int prefix)
{
    for (int i = 0; i < prefix; i++)
        o << "  ";
}

//
// SOURCE
//

void Dumper::jsonBuildFileSource(int i, const wchar_t *source)
{
    std::stringstream s;
    s << DUMPER_CORE_SOURCES << i << ".acpul";
    std::string ss = s.str();
    const char *filename = ss.c_str();
    
//    int a;
    
//    printf("%i\n", a);
    printf("%s\n", filename);
    
    std::wofstream f;
    f.open(filename);
    f << source;
    f.close();
}

//
// Objects
//

void Dumper::jsonObjects(std::wostream &s, int prefix, ObjectsMap &map)
{
    jsonObjectsTree(s, prefix, map.objects());
}

void Dumper::jsonObjectsTree(std::wostream &s, int prefix, otree &tree)
{
    jsonObjectsNode(s, prefix, tree, tree.begin());
}

void Dumper::jsonObjectsNode(std::wostream &s, int prefix, otree &tree, otree::iterator node)
{
    SPX << "{\n";
    prefix++;

    jsonObjectsNodeDumpProps(s, prefix, node);

    SPX << "\"childs\":[\n";
    prefix++;
    otree::sibling_iterator i;
    for (i = tree.begin(node); i != tree.end(node); i++)
    {
        if (i != tree.begin(node))
        {
            SPX << ",\n";
        }
        jsonObjectsNode(s, prefix, tree, i);
        
    }
    prefix--;
    SPX << "]\n";

    prefix--;
    SPX << "}\n";
}

void Dumper::jsonObjectsNodeDumpProps(std::wostream &s, int prefix, otree::iterator node)
{
    Object *o = *node;

    if (!o)
    {
        SPX << "\"id\":null\n";
        return;
    }
    
    // ID
    
    SPX << "\"id\":" << o->uid() << ",\n";

    // PARENT
    
    SPX << "\"parent\":";
    if (!o->parent())
    {
        s << "null";
    }
    else
    {
        Object *parent = o->parent();
        s << parent->uid();
    }
    s << ",\n";
    
    // FOLLOW
    
    SPX << "\"follow\":";
    if (!o->followObject())
    {
        s << "null";
    }
    else
    {
        Object *follow = o->followObject();
        s << follow->uid();
    }
    s << ",\n";
    
    // LINK
    
    SPX << "\"link\":";
    if (!o->link())
    {
        s << "null";
    }
    else
    {
        Object *link = o->link();
        s << link->uid();
    }
    s << ",\n";
    
    // IDENT
    
    SPX << "\"ident\":\"" << o->ident() << "\",\n";
    
    // OBJECTS
    
    SPX << "\"objects\": {\n";
    prefix++;
    
    omap &objects = o->objects();
    omap::iterator oi;
    for (oi = objects.begin(); oi != objects.end(); oi++)
    {
        if (oi != objects.begin())
        {
           s << ",\n";
        }
        const wchar_t *ident = oi->first;
        Object *o = oi->second;
        
        SPX << "\"" << ident << "\":" << o->uid();
    }
    if (objects.size() > 0)
    {
        s << "\n";
    }
    
    prefix--;
    SPX << "},\n";
}


#if 0

"core" : {
    
}

std::ostream &Dumper::requireStream(const char *path, std::ostream &s)
{
    // write to file if path ?
}
#endif
