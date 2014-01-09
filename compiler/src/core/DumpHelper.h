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
//  This file contains dump functions for help discovery ACPUL Core
//  XREFS Classes (Object::, Processor:: Compile::, Executor::). See .cpp file.
//

#ifndef ACPULanguage_DumpHelper_h
#define ACPULanguage_DumpHelper_h

#include "Expression.h"

namespace acpul {
    class DumpHelper
    {
    public:
        static void dumpExpression(int prefix, int flags, Expression *exp);
        static void dumpExpressionMultipleNodes(stree &tree, stree::iterator node);
        static void dumpExpressionMultipleNodes1(int prefix, int flags, stree &tree, stree::iterator node);
        static void dumpExpressionNode(int prefix, int flags, stree &tree, stree::iterator node);
        static void dumpFunctionNode(int prefix, int flags, stree &tree, stree::iterator node);

        static void dumpExpressionMultipleNodesA1(int prefix, int flags, stree &tree, stree::iterator node);
        static void dumpExpressionNodeA(int prefix, int flags, stree &tree, stree::iterator node);
        static void dumpFunctionNodeA(int prefix, int flags, stree &tree, stree::iterator node);
    };
}

#endif
