
#ifndef GUICURSOR_H
#define GUICURSOR_H

class CGUICursor{
    public:
        enum class ECursorType{ LeftPointer, RightPointer, CenterPointer, Crosshair, Blank};
        
        virtual ~CGUICursor() = default;
        
        virtual ECursorType Type() = 0;
};

#endif

