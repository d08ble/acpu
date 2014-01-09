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
#include "AErrors.h"
#include "CoreData.h"
#include "Error.h"

using namespace acpul;

extern CoreData *acpulCoreData;

AErrors::AErrors()
: Errors()
{
    
}

//
// Override error methods
//

void AErrors::Error(int line, int col, const wchar_t *s) {
	acpulCoreData->error(EN_SYNTAX_ERR, L"-- line %d col %d: %ls\n", line, col, s);
	count++;
}

void AErrors::Warning(int line, int col, const wchar_t *s) {
	acpulCoreData->error(EN_SYNTAX_WARN, L"-- line %d col %d: %ls\n", line, col, s);
}

void AErrors::Warning(const wchar_t *s) {
	acpulCoreData->error(EN_SYNTAX_WARN, L"%ls\n", s);
}

// exception is never used
