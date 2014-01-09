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
#include "AScanner.h"

using namespace acpul;

//
// WCBuffer
//

class WCBuffer : public Buffer {
    wchar_t *_wcbuf;
    int _pos;
    int _len;
public:
	WCBuffer(const wchar_t* s);
	virtual ~WCBuffer();
	
	virtual void Close();
	virtual int Read();
	virtual int Peek();
	virtual wchar_t* GetString(int beg, int end);
	virtual int GetPos();
	virtual void SetPos(int value);
};


//
// Scanner extension
//

Scanner::Scanner(const wchar_t* buf, int len) {
	buffer = new WCBuffer(buf);
	Init();
}


//
// Buffer extension
//

Buffer::Buffer()
: buf(NULL)
, isUserStream(true)
{
}

WCBuffer::WCBuffer(const wchar_t* s)
: Buffer()
{
    int len = wcslen(s);
    _len = len;
    _pos = 0;
    _wcbuf = new wchar_t[len];
    memcpy(_wcbuf, s, len*sizeof(*_wcbuf));
}

WCBuffer::~WCBuffer()
{
    if (_wcbuf)
    {
        delete _wcbuf;
        _wcbuf = NULL;
    }
}

void WCBuffer::Close()
{
    // do nothing, because no file
}

int WCBuffer::Read()
{
    if (_pos < _len) {
		return _wcbuf[_pos++];
	} else {
		return EoF;
	}
    
}

int WCBuffer::Peek()
{
	int curPos = GetPos();
	int ch = Read();
	SetPos(curPos);
	return ch;
}

wchar_t* WCBuffer::GetString(int beg, int end) {
	int len = 0;
	wchar_t *buf = new wchar_t[end - beg];
	int oldPos = GetPos();
	SetPos(beg);
	while (GetPos() < end) buf[len++] = (wchar_t) Read();
	SetPos(oldPos);
	wchar_t *res = coco_string_create(buf, 0, len);
	coco_string_delete(buf);
	return res;
}

int WCBuffer::GetPos() {
	return _pos;
}

void WCBuffer::SetPos(int value)
{
    if ((value < 0) || (value > _len)) {
		wprintf(L"--- buffer out of bounds access, position: %d\n", value);
		exit(1);
	}
    _len = value;
}

