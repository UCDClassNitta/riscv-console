#ifndef GUITEXTITER_H
#define GUITEXTITER_H

class CGUITextIter{
    public:
        virtual ~CGUITextIter() = default;

        virtual bool MoveBackwardLines(int count) = 0;
        virtual bool MoveForwardLines(int count) = 0; 
        virtual bool MoveBackwardChars(int count) = 0;
        virtual bool MoveForwardChars(int count) = 0;
};

#endif
