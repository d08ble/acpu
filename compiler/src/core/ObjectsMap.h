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

#ifndef ACPULanguage_ObjectsMap_h
#define ACPULanguage_ObjectsMap_h

//#include "tree.hh"
#include "../coco/language/Parser.h"
#include "Object.h"

namespace acpul {
    
    class Processor;
    
    class ObjectsMap
    {
        otree _objects;
        Processor *_processor;
    public:
        ObjectsMap() {}
        
        void setProcessor(Processor *processor_)    { _processor = processor_; }
        Processor *processor()                      { return _processor; }
        
        otree &objects()                            { return _objects; }
        
        Object *createMainObject();

        Object *newObjectForObject(Object *object);
        
        Object *createObject(Object *current, Name *name);
        Object *followObject(Object *object);
        Object *createObject(Object *object, const wchar_t *ident);

        void fixObjects();

        // DUMP HELPER
        void dumpObjects();
        void dumpObjectsFollowTable();
        void dumpObjectsDetails();
    };
}

#endif
