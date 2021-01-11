
#ifndef GUICURSOR_H
#define GUICURSOR_H

class CGUICursor{
    public:
        enum class ECursorType{ LeftPointer, RightPointer, CenterPointer, Crosshair, Blank};
        
        CGUICursor(){};
        virtual ~CGUICursor(){};
        
        virtual ECursorType Type() = 0;
};

#endif

