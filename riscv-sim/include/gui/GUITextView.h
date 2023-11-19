#ifndef GUITEXTVIEW_H
#define GUITEXTVIEW_H

#include <memory>
#include "GUIContainer.h"
#include "GUITextBuffer.h"

class CGUITextView : public virtual CGUIContainer{
    public: 
        enum class EWrapModeType{None = 0, Character, Word, WordCharacter};
        virtual ~CGUITextView() = default;

        virtual void PlaceCursorOnscreen() = 0;
        virtual void SetEditable(bool edit) = 0;
        virtual void SetCursorVisible(bool visibility) = 0;
        virtual void SetMonospace(bool monospace) = 0;
        virtual void SetWrapMode(EWrapModeType wrap) = 0;
        virtual std::shared_ptr<CGUITextBuffer> GetBuffer() = 0;
        virtual int GetRequiredCharactersWidth(int chars) = 0;
        virtual int GetRequiredLinesHeight(int lines) = 0;
        virtual int GetLineNumberAtY(int y) = 0;
        virtual int GetLineOffsetAtPosition(int x, int y) = 0;
};

#endif
