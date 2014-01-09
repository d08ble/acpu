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

#ifndef ACPULanguage_Name_h
#define ACPULanguage_Name_h

#include <iostream>
#include <vector>

namespace acpul {
    class Name
    {
//        std::vector<std::wstring> idents;
        std::vector<const wchar_t *> idents;
    public:
        Name() {};
        void addIdent(const wchar_t *ident_);
        
        const wchar_t *operator[](int n) const
        {
            return idents[n];
        }
        
        int count() const
        {
            return idents.size();
        }
        
        size_t hash() const
        {
            size_t hash = 1;
            for (size_t i = 0; i < idents.size(); i++)
            {
                for (const wchar_t *s = idents[i]; *s; ++s)
                    hash = hash * 5 + *s;
                hash = hash * 5 + L'.'; 
            }
            return hash;
        }
        
//        inline bool operator==(const Name& rhs) // -- todo
        inline bool isEqual(const Name& rhs) const
        {
            if (rhs.count() != count())
            {
                return false;
            }
            
            for (int i = 0; i < count(); i++)
            {
                const wchar_t *s1 = idents[i];
                const wchar_t *s2 = rhs[i];
                if (wcscmp(s1, s2))
                    return false;
            }
            return true;
        }
        
        inline bool isUnderscore()
        {
            if (count() == 1)
            {
                if (!wcscmp(idents[0], L"_"))
                    return true;
            }
            return false;
        }

        // DUMP HELPER
        void dump() const;
    };
}

#endif
