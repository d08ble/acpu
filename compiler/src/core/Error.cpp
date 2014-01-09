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
#include "Error.h"

using namespace acpul;

Error::Error()
: _number(0)
, _message(NULL)
, _line(0)
, _col(0)
, _val(NULL)
, _obj(NULL)
{
    
}

Error::~Error()
{
    if (_message)
    {
        free(_message);
    }
}

void Error::format(const wchar_t *format, va_list ap)
{
    wchar_t dest[1024];
    int n = vswprintf(dest, 1024, format, ap);
    
    if (n > 0)
    {
        int size = (n + 1) * sizeof(dest[0]);

        _message = (wchar_t *)malloc(size);
        
        memcpy(_message, dest, size);
    }
    
//    printf("ERROR: %S\n", _message);
}
