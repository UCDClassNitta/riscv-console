
#ifndef GUIDISPLAY_H
#define GUIDISPLAY_H
#include "GUICursor.h"
#include <memory>

class CGUIDisplay{
    public:
        virtual ~CGUIDisplay() = default;
                       
        virtual void Sync() = 0;
        virtual void Flush() = 0;
        
        virtual std::shared_ptr< CGUICursor > NewCursor(CGUICursor::ECursorType type) = 0;
};

#endif

