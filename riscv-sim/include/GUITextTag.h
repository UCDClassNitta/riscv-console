#ifndef GUITEXTTAG_H
#define GUITEXTTAG_H

class CGUITextTag{
    public:
        virtual ~CGUITextTag() = default;

        virtual int GetPriority() = 0;
        virtual void SetPriority(int prio) = 0;
};

#endif
