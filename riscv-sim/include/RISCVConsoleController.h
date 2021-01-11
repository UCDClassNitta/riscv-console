#ifndef RISCVCCONSOLECONTROLLER_H
#define RISCVCCONSOLECONTROLLER_H

#include "HardwareRegister.h"
#include "GUIToggleButton.h"
#include <vector>

class CRISCVConsoleController : public CHardwareRegister< uint32_t >{
    protected:
        
        std::vector< std::shared_ptr< CGUIToggleButton > > DControllerButtons;

        CRISCVConsoleController(std::vector< std::shared_ptr< CGUIToggleButton > > &buttons);

        virtual uint32_t operator= (uint32_t val) noexcept{
            return load();
        };

        virtual operator uint32_t() const noexcept{
            return 0;
        };

        virtual uint32_t load() noexcept;

        virtual uint32_t exchange(uint32_t val) noexcept{
            return load();
        };

        virtual uint32_t fetch_add(uint32_t val) noexcept{
            return load();
        };

        virtual uint32_t fetch_sub(uint32_t val) noexcept{
            return load();
        };

        virtual uint32_t fetch_and(uint32_t val) noexcept{
            return load();
        };

        virtual uint32_t fetch_or(uint32_t val) noexcept{
            return load();
        };

        virtual uint32_t fetch_xor(uint32_t val) noexcept{
            return load();
        };

        virtual uint32_t operator++() noexcept{
            return load();
        };

        virtual uint32_t operator++(int) noexcept{
            return load();
        };

        virtual uint32_t operator--() noexcept{
            return load();
        };

        virtual uint32_t operator--(int) noexcept{
            return load();
        };
        
};

#endif
