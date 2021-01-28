
#ifndef GUISCROLLABLEMEMORYLABELBOX_H
#define GUISCROLLABLEMEMORYLABELBOX_H
#include "GUIScrollableLabelBox.h"
#include "MemoryDevice.h"
#include <unordered_map>

class CGUIScrollableMemoryLabelBox : public CGUIScrollableLabelBox{
    protected:
        std::shared_ptr< CMemoryDevice > DMemoryDevice;
        std::vector< uint32_t > DMemoryBases;
        std::vector< uint32_t > DMemorySizes;
        std::vector< uint32_t > DMemoryLines;
        size_t DBufferedLineCount;
        uint32_t DBaseAddress;

        bool DAscending;
        static const uint32_t DBytesPerLine;

        virtual bool LineIndexToBaseAddressMemoryIndex(size_t index, uint32_t &baseaddr, size_t &memindex) const;
        virtual void UpdateBaseLine();
        virtual void RefreshLabels() override;
        virtual void UpdateMemoryLine(size_t index, uint32_t addr, uint32_t bytes);
        static std::string FormatMemoryLine(const uint8_t *buffer, uint32_t addr, uint32_t bytes);

    public:
        CGUIScrollableMemoryLabelBox(std::shared_ptr< CMemoryDevice > device, const std::unordered_map< uint32_t, uint32_t > &regions, size_t initsize=8);
        virtual ~CGUIScrollableMemoryLabelBox();

        virtual uint32_t GetBaseAddress();
        virtual void SetBaseAddress(uint32_t addr, bool ascending);

        virtual bool GetAddressAscending() const;

        virtual void SetLineCount(size_t count) override;

        virtual size_t GetBufferedLineCount() const override;
        virtual std::string GetBufferedLine(size_t index) const override;
        virtual void SetBufferedLines(const std::vector< std::string > &lines) override;

        virtual void UpdateBufferedLine(size_t index, const std::string &line) override;
        virtual void AppendBufferedLine(const std::string &line) override;

        virtual void SetWidthCharacters(int chars) override;
        virtual void SetMaxWidthCharacters(int chars) override;

        virtual void Refresh();

};

#endif

