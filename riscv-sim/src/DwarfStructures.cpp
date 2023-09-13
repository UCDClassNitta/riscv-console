#include "DwarfStructures.h"
#include <cstdio>
#include <cinttypes>

void CDwarfStructures::SValue::SetDataFromString(const std::string &str){
    DData.clear();
    for(auto Ch : str){
        DData.push_back(Ch);
    }
    DData.push_back('\0');
}

std::string CDwarfStructures::SValue::GetString() const{
    switch(DForm){
        case DW_FORM::string:
        case DW_FORM::strp:
        case DW_FORM::line_strp:    return (const char *)DData.data();
        default:                    return std::string();
    }
}

int64_t CDwarfStructures::SValue::GetINT64() const{
    return -1;
}

uint64_t CDwarfStructures::SValue::GetUINT64() const{
    switch(DForm){
        case DW_FORM::addr            :
        case DW_FORM::ref_addr        :
        case DW_FORM::sec_offset      :
        case DW_FORM::strp_sup        :

        case DW_FORM::data1           :
        case DW_FORM::data2           :
        case DW_FORM::data4           :
        case DW_FORM::data8           :

        case DW_FORM::ref1            :
        case DW_FORM::ref2            :
        case DW_FORM::ref4            :
        case DW_FORM::ref8            :

        case DW_FORM::flag            :
        
        case DW_FORM::strx1           :
        case DW_FORM::strx2           :
        case DW_FORM::strx3           :
        case DW_FORM::strx4           :

        case DW_FORM::addrx1          : 
        case DW_FORM::addrx2          : 
        case DW_FORM::addrx3          : 
        case DW_FORM::addrx4          :

        case DW_FORM::ref_sup4        : 
        case DW_FORM::ref_sup8        :

        case DW_FORM::ref_sig8        : 
        case DW_FORM::implicit_const  : {
                                            uint64_t ReturnValue = 0;
                                            for(auto ByteIter = DData.rbegin(); ByteIter != DData.rend(); ByteIter++){
                                                ReturnValue <<= 8;
                                                ReturnValue |= *ByteIter;
                                            }
                                            return ReturnValue;
                                        }
                                        break;
        case DW_FORM::sdata           :
        case DW_FORM::udata           :
        case DW_FORM::ref_udata       :
        case DW_FORM::strx            :
        case DW_FORM::addrx           :
        case DW_FORM::loclistx        :
        case DW_FORM::rnglistx        : {
                                            uint64_t ReturnValue = 0;
                                            for(auto ByteIter = DData.rbegin(); ByteIter != DData.rend(); ByteIter++){
                                                ReturnValue <<= 7;
                                                ReturnValue |= (*ByteIter) & 0x7F;
                                            }
                                            return ReturnValue;
                                        }
                                        break;
        default:                        break;
    }
    return 0;
}

void CDwarfStructures::SLineNumberStateMachine::Reset(){
    DRegs.address = DRegs.op_index = DRegs.column = DRegs.isa = DRegs.discriminator = 0;
    DRegs.file = DRegs.line = 1;
    DRegs.is_stmt = DDefaultIsStmt;
    DRegs.basic_block = DRegs.end_sequence = DRegs.prologue_end = DRegs.epilogue_begin = false;
}

uint64_t CDwarfStructures::SProgram::ReadCompilationUnit(std::shared_ptr<CSeekableDataSource> source){
    std::shared_ptr< CDwarfStructures::SCompilationUnit > NewCompUnit;
    std::shared_ptr< CSeekableDataSourceConverter > SourceConverter = std::make_shared< CSeekableDataSourceConverter >(source);
    uint64_t CUSize;
    uint64_t LengthSize;
    CUSize = SourceConverter->ReadUINT32();
    if(D32Bit){
        if(CUSize >= 0xFFFFFFF0){
            return 0;
        }
        LengthSize = 4;
    }
    else{
        if(CUSize != 0xFFFFFFFF){
            return 0;
        }
        CUSize = SourceConverter->ReadUINT64();
        LengthSize = 12;
    }
    NewCompUnit = std::make_shared< CDwarfStructures::SCompilationUnit >();
    NewCompUnit->D32Bit = D32Bit;
    NewCompUnit->DDebugStrings = DDebugStrings;
    NewCompUnit->DDebugLineStrings = DDebugLineStrings;
    NewCompUnit->DLength = CUSize;
    NewCompUnit->DVersion = SourceConverter->ReadUINT16();
    NewCompUnit->DType = DW_UT(SourceConverter->ReadUINT8());
    if((NewCompUnit->DType != DW_UT::compile)&&(NewCompUnit->DType != DW_UT::partial)){
        return CUSize + LengthSize;
    }
    NewCompUnit->DAddressSize = SourceConverter->ReadUINT8();
    NewCompUnit->DAbbreviationOffset = D32Bit ? SourceConverter->ReadUINT32() : SourceConverter->ReadUINT64();
    
    ReadAbbreviationTable(NewCompUnit);
    NewCompUnit->DRoot = std::make_shared<CDwarfStructures::SDie>();
    NewCompUnit->DRoot->DCompilationUnit = NewCompUnit;
    NewCompUnit->ReadDebugInformationEntry(NewCompUnit->DRoot,SourceConverter);
    if(NewCompUnit->DRoot->HasAttribute(DW_AT::stmt_list)){
        ReadLineNumbers(NewCompUnit);
    }
    
    DCompilaitonUnits.push_back(NewCompUnit);
    return CUSize + LengthSize;
}

bool CDwarfStructures::SProgram::ReadAbbreviationTable(std::shared_ptr< SCompilationUnit > cu){
    DAbbreviationsSourceConverter->Seek(cu->DAbbreviationOffset);
    while(DAbbreviationsSourceConverter->Tell() < DAbbreviationsSize){
        uint64_t Code = DAbbreviationsSourceConverter->ReadULEB128(); 
        if(!Code){
            return true;
        }
        uint64_t Tag = DAbbreviationsSourceConverter->ReadULEB128();
        uint8_t Children = DAbbreviationsSourceConverter->ReadUINT8();
        std::shared_ptr< CDwarfStructures::SAbbreviationEntry > NewEntry = std::make_shared< CDwarfStructures::SAbbreviationEntry >();
        NewEntry->DCode = Code;
        NewEntry->DTag = DW_TAG(Tag);
        NewEntry->DChildren = Children;
        while(DAbbreviationsSourceConverter->Tell() < DAbbreviationsSize){
            uint64_t Name = DAbbreviationsSourceConverter->ReadULEB128();
            uint64_t Form = DAbbreviationsSourceConverter->ReadULEB128();
            int64_t Extra = 0;
            if(!Name && !Form){
                break;
            }
            if(DW_FORM(Form) == DW_FORM::implicit_const){
                Extra = DAbbreviationsSourceConverter->ReadSLEB128();
            }
            NewEntry->DAttributes.push_back({DW_AT(Name),DW_FORM(Form),Extra});
        }
        cu->DAbbreviationTable[Code] = NewEntry;
    }
    return true;
}

bool CDwarfStructures::SProgram::ReadLineNumbers(std::shared_ptr< SCompilationUnit > cu){
    uint64_t LNSize;
    DLineNumberSourceConverter->Seek(cu->DRoot->GetAttribute(DW_AT::stmt_list).GetUINT64());
    LNSize = DLineNumberSourceConverter->ReadUINT32();
    if(D32Bit){
        if(LNSize >= 0xFFFFFFF0){
            return false;
        }
    }
    else{
        if(LNSize != 0xFFFFFFFF){
            return false;
        }
        LNSize = DLineNumberSourceConverter->ReadUINT64();
    }
    uint64_t ProgramEnd = DLineNumberSourceConverter->Tell() + LNSize;
    CDwarfStructures::SLineNumberStateMachine LineNumberStateMachine; 
    auto Version = DLineNumberSourceConverter->ReadUINT16();
    if(Version != 5){
        return false;
    }
    LineNumberStateMachine.DAddressSize = DLineNumberSourceConverter->ReadUINT8();
    LineNumberStateMachine.DSegmentSelectorSize = DLineNumberSourceConverter->ReadUINT8();
    uint64_t HeaderLength = D32Bit ? DLineNumberSourceConverter->ReadUINT32() : DLineNumberSourceConverter->ReadUINT64();
    auto HeaderEnd = DLineNumberSourceConverter->Tell() + HeaderLength;
    if(HeaderEnd > DLineNumberSize){
        return false;
    }
    LineNumberStateMachine.DMinimumInstructionLength = DLineNumberSourceConverter->ReadUINT8();
    LineNumberStateMachine.DMaximumOperandsPerInstruction = DLineNumberSourceConverter->ReadUINT8();
    LineNumberStateMachine.DDefaultIsStmt = DLineNumberSourceConverter->ReadUINT8();
    LineNumberStateMachine.DLineBase = int8_t(DLineNumberSourceConverter->ReadUINT8());
    LineNumberStateMachine.DLineRange = DLineNumberSourceConverter->ReadUINT8();
    LineNumberStateMachine.DOpcodeBase = DLineNumberSourceConverter->ReadUINT8();
    for(uint8_t OpcodeIndex = 1; OpcodeIndex < LineNumberStateMachine.DOpcodeBase; OpcodeIndex++){
        LineNumberStateMachine.DOpcodeLengths.push_back(DLineNumberSourceConverter->ReadUINT8());
    }
    auto DirectoryEntryFormatCount = DLineNumberSourceConverter->ReadUINT8();
    std::vector<std::pair<DW_LNCT,DW_FORM>> DirectoryEntryFormat;
    for(uint8_t DirectoryEntryFormatIndex = 0; DirectoryEntryFormatIndex < DirectoryEntryFormatCount; DirectoryEntryFormatIndex++){
        auto ContentType = DLineNumberSourceConverter->ReadULEB128();
        auto Attribute = DLineNumberSourceConverter->ReadULEB128();
        DirectoryEntryFormat.push_back({DW_LNCT(ContentType),DW_FORM(Attribute)});
    }
    auto DirectoriesCount = DLineNumberSourceConverter->ReadULEB128();
    std::vector< std::unordered_map<DW_LNCT,CDwarfStructures::SValue> > DirectoryEntries;
    for(uint64_t DirectoryIndex = 0; DirectoryIndex < DirectoriesCount; DirectoryIndex++){
        std::unordered_map<DW_LNCT,CDwarfStructures::SValue> Entry;
        for(auto Format : DirectoryEntryFormat){
            //Entry[Format.first] = ReadDebugValue(DLineNumberSourceConverter,Format.second,cu,0);
            Entry[Format.first] = cu->ReadValue(DLineNumberSourceConverter,Format.second,0);

        }
        DirectoryEntries.push_back(Entry);
    }
    auto FileNameEntryFormatCount = DLineNumberSourceConverter->ReadUINT8();
    std::vector<std::pair<DW_LNCT,DW_FORM>> FileNameEntryFormat;
    for(uint8_t FileNameEntryFormatIndex = 0; FileNameEntryFormatIndex < FileNameEntryFormatCount; FileNameEntryFormatIndex++){
        auto ContentType = DLineNumberSourceConverter->ReadULEB128();
        auto Attribute = DLineNumberSourceConverter->ReadULEB128();
        FileNameEntryFormat.push_back({DW_LNCT(ContentType),DW_FORM(Attribute)});
    }
    auto FileNamesCount = DLineNumberSourceConverter->ReadULEB128();
    std::vector< std::unordered_map<DW_LNCT,CDwarfStructures::SValue> > FileNameEntries;
    for(uint64_t FileNamesIndex = 0; FileNamesIndex < FileNamesCount; FileNamesIndex++){
        std::unordered_map<DW_LNCT,CDwarfStructures::SValue> Entry;
        for(auto Format : FileNameEntryFormat){
            //Entry[Format.first] = ReadDebugValue(DLineNumberSourceConverter,Format.second,cu,0);
            Entry[Format.first] = cu->ReadValue(DLineNumberSourceConverter,Format.second,0);
        }
        FileNameEntries.push_back(Entry);
    }
    cu->ResolvePaths(DirectoryEntries,FileNameEntries);
    LineNumberStateMachine.Reset();
    while(DLineNumberSourceConverter->Tell() < ProgramEnd){
        if(ComputeLineNumberStep(LineNumberStateMachine,DLineNumberSourceConverter)){
            // Add line number data
            CDwarfStructures::SLineNumberEntry LineNumberEntry = {LineNumberStateMachine.DRegs.file, LineNumberStateMachine.DRegs.line, LineNumberStateMachine.DRegs.column, uint32_t(LineNumberStateMachine.DRegs.address)};
            cu->DLineNumberData.DLineNumberEntries.push_back(LineNumberEntry);
        }
        if(LineNumberStateMachine.DRegs.end_sequence){
            LineNumberStateMachine.Reset();
        }
    }
    return true;
}


bool CDwarfStructures::SProgram::ComputeLineNumberStep(CDwarfStructures::SLineNumberStateMachine &machine, std::shared_ptr< CSeekableDataSourceConverter > source){
    auto Opcode = source->ReadUINT8();
    if(!Opcode){
        // Extended opcode
        uint64_t Length = source->ReadULEB128();
        uint64_t InstructionEnd = source->Tell() + Length;
        Opcode = source->ReadUINT8();
        switch(DW_LNE(Opcode)){
            case DW_LNE::end_sequence:      machine.DRegs.end_sequence = true;
                                            if(Length != 1){
                                                // Error
                                            }
                                            return true;
            case DW_LNE::set_address:       machine.DRegs.address = D32Bit ? source->ReadUINT32() : source->ReadUINT64();
                                            machine.DRegs.op_index = 0;
                                            break;
            case DW_LNE::set_discriminator: machine.DRegs.discriminator = source->ReadULEB128();
                                            break;
            default:                        //ERROR
                                            break;
        }
        if(InstructionEnd != source->Tell()){
            // Error
        }
    }
    else if(Opcode < machine.DOpcodeBase){
        // Standard opcode
        uint32_t OperationAdvance = (255 - machine.DOpcodeBase) / machine.DLineRange;
        switch(DW_LNS(Opcode)){
            case DW_LNS::copy                :  machine.DRegs.basic_block = machine.DRegs.prologue_end = machine.DRegs.epilogue_begin = false;
                                                machine.DRegs.discriminator = 0;
                                                return true;
            case DW_LNS::advance_pc          :  OperationAdvance = source->ReadULEB128();
            case DW_LNS::const_add_pc        :  machine.DRegs.address += machine.DMinimumInstructionLength * ((machine.DRegs.op_index + OperationAdvance) / machine.DMaximumOperandsPerInstruction);
                                                machine.DRegs.op_index = (machine.DRegs.op_index + OperationAdvance) % machine.DMaximumOperandsPerInstruction;
                                                break;
            case DW_LNS::advance_line        :  machine.DRegs.line += source->ReadSLEB128();
                                                break;
            case DW_LNS::set_file            :  machine.DRegs.file = source->ReadULEB128();
                                                break;
            case DW_LNS::set_column          :  machine.DRegs.column = source->ReadULEB128();
                                                break;
            case DW_LNS::negate_stmt         :  machine.DRegs.is_stmt = !machine.DRegs.is_stmt;
                                                break;
            case DW_LNS::set_basic_block     :  machine.DRegs.basic_block = true;
                                                break;
            case DW_LNS::fixed_advance_pc    :  machine.DRegs.address += source->ReadUINT16();
                                                break;
            case DW_LNS::set_prologue_end    :  machine.DRegs.prologue_end = true;
                                                break;
            case DW_LNS::set_epilogue_begin  :  machine.DRegs.epilogue_begin = true;
                                                break;
            case DW_LNS::set_isa             :  machine.DRegs.isa = source->ReadULEB128();
                                                break;
        }
    }
    else{
        // Special opcode
        uint8_t AdjustedOpcode = Opcode - machine.DOpcodeBase;
        uint8_t OperationAdvance = AdjustedOpcode / machine.DLineRange;
        int8_t LineIncrement = machine.DLineBase + (AdjustedOpcode % machine.DLineRange);
        machine.DRegs.line = machine.DRegs.line + LineIncrement;
        machine.DRegs.address += machine.DMinimumInstructionLength * ((machine.DRegs.op_index + OperationAdvance) / machine.DMaximumOperandsPerInstruction);
        machine.DRegs.op_index = (machine.DRegs.op_index + OperationAdvance) % machine.DMaximumOperandsPerInstruction;
        machine.DRegs.basic_block = machine.DRegs.prologue_end = machine.DRegs.epilogue_begin = false;
        machine.DRegs.discriminator = 0;
        return true;
    }
    return false;
}

bool CDwarfStructures::SProgram::ConsolidateLineNumbers(){
    std::unordered_map< std::string, uint32_t > FileIndices;

    for(auto CompilationUnit : DCompilaitonUnits){
        std::unordered_map< uint32_t, uint32_t > FileTranslations;

        for(auto LineNumberEntry : CompilationUnit->DLineNumberData.DLineNumberEntries){
            if(FileTranslations.find(LineNumberEntry.DFileIndex) == FileTranslations.end()){
                 auto FilePath = CompilationUnit->DLineNumberData.DFilePaths[LineNumberEntry.DFileIndex];
                if(FileIndices.find(FilePath) == FileIndices.end()){
                    FileIndices[FilePath] = DLineNumberData.DFilePaths.size();
                    FileTranslations[LineNumberEntry.DFileIndex] = DLineNumberData.DFilePaths.size();
                    DLineNumberData.DFilePaths.push_back(FilePath);
                }
                else{
                    FileTranslations[LineNumberEntry.DFileIndex] = FileIndices[FilePath];
                }
            }

            auto FileIndex = FileTranslations[LineNumberEntry.DFileIndex];
            DLineNumberData.DLineNumberEntries.push_back({FileIndex,LineNumberEntry.DLineNumber,LineNumberEntry.DColumn,LineNumberEntry.DAddress});
        }
    }
    std::sort(DLineNumberData.DLineNumberEntries.begin(),DLineNumberData.DLineNumberEntries.end(),[](const CDwarfStructures::SLineNumberEntry &a, const CDwarfStructures::SLineNumberEntry &b) -> bool{
        if(a.DAddress < b.DAddress){
            return true;
        }
        if(a.DAddress > b.DAddress){
            return false;
        }
        if(a.DFileIndex < b.DFileIndex){
            return true;
        }
        if(a.DFileIndex > b.DFileIndex){
            return false;
        }
        if(a.DLineNumber < b.DLineNumber){
            return true;
        }
        if(a.DLineNumber > b.DLineNumber){
            return false;
        }
        return a.DColumn < b.DColumn;
    });
    return true;
}

CDwarfStructures::SValue CDwarfStructures::SCompilationUnit::ReadValue(std::shared_ptr< CSeekableDataSourceConverter > source, DW_FORM form, int64_t implicit){
    CDwarfStructures::SValue ReturnValue;
    size_t ReadSize = 0;
    DW_FORM ReturnForm = form;
    switch(form){
        case DW_FORM::addr            : ReadSize = DAddressSize;
                                        break;
        case DW_FORM::block1          : ReadSize = source->ReadUINT8();
                                        break;
        case DW_FORM::block2          : ReadSize = source->ReadUINT16();
                                        break;
        case DW_FORM::block4          : ReadSize = source->ReadUINT32();
                                        break;
        case DW_FORM::block           :
        case DW_FORM::exprloc         : ReadSize = source->ReadULEB128();
                                        break;
        case DW_FORM::data1           :
        case DW_FORM::flag            :
        case DW_FORM::ref1            :
        case DW_FORM::strx1           :
        case DW_FORM::addrx1          : ReadSize = 1;
                                        break;
        case DW_FORM::data2           :
        case DW_FORM::ref2            :
        case DW_FORM::strx2           :
        case DW_FORM::addrx2          : ReadSize = 2;
                                        break;
        case DW_FORM::strx3           :
        case DW_FORM::addrx3          : ReadSize = 3;
                                        break;
        case DW_FORM::data4           :
        case DW_FORM::ref4            :
        case DW_FORM::strx4           :
        case DW_FORM::addrx4          :
        case DW_FORM::ref_sup4        : ReadSize = 4;
                                        break;
        case DW_FORM::data8           :
        case DW_FORM::ref8            :
        case DW_FORM::ref_sup8        :
        case DW_FORM::ref_sig8        : ReadSize = 8;
                                        break;
        case DW_FORM::data16          : ReadSize  = 16;
                                        break;
        case DW_FORM::string          : while(1){
                                            auto NextChar = source->ReadUINT8();
                                            ReturnValue.DData.push_back(NextChar);
                                            if(!NextChar){
                                                break;
                                            }
                                        }
                                        break;
        case DW_FORM::ref_addr        :
        case DW_FORM::sec_offset      :
        case DW_FORM::strp_sup        : ReadSize = D32Bit ? 4 : 8;
                                        break;
        case DW_FORM::sdata           :
        case DW_FORM::udata           :
        case DW_FORM::ref_udata       :
        case DW_FORM::strx            :
        case DW_FORM::addrx           :
        case DW_FORM::loclistx        :
        case DW_FORM::rnglistx        : while(1){
                                            auto NextChar = source->ReadUINT8();
                                            ReturnValue.DData.push_back(NextChar);
                                            if(0 == (NextChar & 0x80)){
                                                break;
                                            }
                                        }
                                        break;
        case DW_FORM::strp            : {
                                            uint64_t Offset = D32Bit ? source->ReadUINT32() : source->ReadUINT64();
                                            ReturnValue.SetDataFromString(DDebugStrings->GetString(Offset));
                                        }
                                        break;
        case DW_FORM::line_strp       : {
                                            uint64_t Offset = D32Bit ? source->ReadUINT32() : source->ReadUINT64();
                                            ReturnValue.SetDataFromString(DDebugLineStrings->GetString(Offset));
                                        }
                                        break;
        case DW_FORM::indirect        : return ReadValue(source, DW_FORM(source->ReadULEB128()), 0);
        
        case DW_FORM::flag_present    : break;        
        case DW_FORM::implicit_const  : for(size_t Index = 0; Index < sizeof(int64_t); Index++){
                                            ReturnValue.DData.push_back(implicit & 0xFF);
                                            implicit >>= 8;
                                        }
                                        break;
    }
    ReturnValue.DForm = ReturnForm;
    if(ReadSize){
        ReturnValue.DData.resize(ReadSize);
        source->Read(ReturnValue.DData.data(),ReadSize);
    }
    return ReturnValue;
}


bool CDwarfStructures::SCompilationUnit::ReadDebugInformationEntry(std::shared_ptr< CDwarfStructures::SDie > &die, std::shared_ptr< CSeekableDataSourceConverter > source){
    auto Code = source->ReadULEB128();
    if(!Code){
        die.reset();
        return true;
    }
    auto Search = DAbbreviationTable.find(Code);
    if(Search == DAbbreviationTable.end()){
        // Error 
        printf("Unknown abbreviation code 0x%08" PRIX64 "!\n", Code);
        return false;
    }
    auto Abbreviation = Search->second;
    die->DTag = Abbreviation->DTag;
    die->DAttributeEntry = Abbreviation;
    for(auto &Attribute : Abbreviation->DAttributes){
        //CDwarfStructures::SValue Value = ReadDebugValue(source, Attribute.DForm, CompilationUnit, Attribute.DExtra);
        CDwarfStructures::SValue Value = ReadValue(source, Attribute.DForm, Attribute.DExtra);
        if((Value.DForm != Attribute.DForm)&&(DW_FORM::indirect != Attribute.DForm)){
            // Error
            return false;
        }
        die->DAttributes[Attribute.DName] = Value;
    }
    if(Abbreviation->DChildren){
        while(1){
            auto NewChild = std::make_shared<CDwarfStructures::SDie>();
            NewChild->DCompilationUnit = shared_from_this();// CompilationUnit;
            if(!ReadDebugInformationEntry(NewChild,source)){
                return false;
            }
            if(!NewChild){
                break;
            }
            die->DChildren.push_back(NewChild);
        }
    }
    return true;
}

bool CDwarfStructures::SCompilationUnit::ResolvePaths(std::vector< std::unordered_map<DW_LNCT,CDwarfStructures::SValue> > &directories, std::vector< std::unordered_map<DW_LNCT,CDwarfStructures::SValue> > &files){
    std::string CompilationDirectory;
    std::vector<std::string> ResolvedDirectories;
    if(DRoot->HasAttribute(DW_AT::comp_dir)){
        CompilationDirectory = DRoot->GetAttribute(DW_AT::comp_dir).GetString();
        if(CompilationDirectory.back() != '/'){
            CompilationDirectory += "/";
        }
    }
    else{
        CompilationDirectory = "./";
    }
    for(auto &DirectoryEntry : directories){
        auto Path = DirectoryEntry[DW_LNCT::path].GetString();
        if(Path.front() != '/'){
            Path = CompilationDirectory + Path;
        }
        if(Path.back() != '/'){
            Path += "/";
        }
        ResolvedDirectories.push_back(Path);
    }
    for(auto &FileEntry : files){
        auto Path = FileEntry[DW_LNCT::path].GetString();
        if(FileEntry.find(DW_LNCT::directory_index) != FileEntry.end()){
            auto DirIndex = FileEntry[DW_LNCT::directory_index].GetUINT64();
            Path = ResolvedDirectories[DirIndex] + Path;
        }
        DLineNumberData.DFilePaths.push_back(Path);
    }
    return true;
}
