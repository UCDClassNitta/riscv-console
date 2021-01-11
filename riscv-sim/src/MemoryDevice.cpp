#include "MemoryDevice.h"
#include <sstream>
#include <iomanip>
#include <cctype>

void CMemoryDevice::DumpData(std::ostream &out, uint32_t saddr, uint32_t eaddr){
    uint64_t AnchorAddress = BaseAddress() & 0xFFFFFFF0;
    uint64_t EndAddress = BaseAddress() + MemorySize();
    uint64_t AnchorAddress2 = saddr & 0xFFFFFFF0;
    uint64_t EndAddress2 = eaddr ? eaddr : 0x100000000ULL;
    std::stringstream TempStream;
    std::string TempEnd;

    if(!MemorySize()){
        EndAddress += 0x100000000ULL;
    }
    AnchorAddress = AnchorAddress > AnchorAddress2 ? AnchorAddress : AnchorAddress2;
    EndAddress = EndAddress < EndAddress2 ? EndAddress : EndAddress2;

    while(AnchorAddress < EndAddress){
        bool AllZero = true;
        TempStream.str("");
        TempEnd.clear();
        TempStream<<std::setfill('0')<<std::setw(8)<<std::right<<std::hex<<AnchorAddress<<":";
        for(uint32_t Index = 0; Index < 16; Index++){
            uint32_t CurrentAddr = AnchorAddress + Index;
            if((BaseAddress() <= CurrentAddr)&&(CurrentAddr < EndAddress)){
                uint8_t Char = LoadUINT8(CurrentAddr);
                TempStream<<" "<<std::setfill('0')<<std::setw(2)<<std::right<<std::hex<<(uint32_t)Char;
                if(std::isprint(Char)){
                    TempEnd += (char)Char;
                }
                else{
                    TempEnd += ".";
                }
                
                if(Char){
                    AllZero = false;
                }
            }
            else{
                TempStream<<"   ";
                TempEnd += " ";
            }
        }
        if(!AllZero){
            out<<TempStream.str()<<" "<<TempEnd<<std::endl;
        }
        AnchorAddress += 16;
    }

}