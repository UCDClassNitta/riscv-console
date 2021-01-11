
#include "Path.h"
#include <unistd.h>


const char CPath::DDelimiter = '/';
const std::string CPath::DDelimiterString = "/";

CPath::CPath(){
    DIsRelative = true;
    DIsValid = false;
}

CPath::CPath(const CPath &path){
    DDecomposedPath = path.DDecomposedPath;
    DIsRelative = path.DIsRelative;
    DIsValid = path.DIsValid;
}

CPath::CPath(const std::string &path){
    DIsValid = DecomposePath(DDecomposedPath, path);
    DIsRelative = true;
    if(path.length()){
        if(path[0] == DDelimiter){
            DIsRelative = false;   
        }
    }
}

CPath &CPath::operator=(const CPath &path){
    if(&path != this){
        DDecomposedPath = path.DDecomposedPath;
        DIsRelative = path.DIsRelative;
        DIsValid = path.DIsValid;
    }
    return *this;
}

CPath CPath::Containing() const{
    CPath ReturnPath(*this);
    
    if(ReturnPath.DDecomposedPath.size()){
        ReturnPath.DDecomposedPath.pop_back();
    }
    else if(ReturnPath.DIsRelative){
        ReturnPath.DDecomposedPath.push_back("..");
    }
    else{
        ReturnPath.DIsValid = false;   
    }
    
    return ReturnPath;
}

std::string CPath::ToString() const{
    std::string TempString;
    size_t StartIndex = 0;
    
    if(!DIsValid){
        return "";    
    }
    if(DIsRelative){
        if(DDecomposedPath.size()){
            if(DDecomposedPath[0] != ".."){
                TempString = ".";   
            }
            else{
                TempString = "..";
                StartIndex = 1;
            }
        }
    }
    while(StartIndex < DDecomposedPath.size()){
        TempString += DDelimiterString + DDecomposedPath[StartIndex];
        StartIndex++;
    }
    return TempString;
}

CPath::operator std::string() const{
    return this->ToString();
}

CPath CPath::Simplify(const CPath &destpath) const{
    return SimplifyPath(*this, destpath);
}

CPath CPath::Relative(const CPath &destpath) const{
    return RelativePath(*this, destpath);
}

bool CPath::DecomposePath(std::vector< std::string > &decomp, const std::string &path){
    size_t Anchor, DelimiterLocation;
    bool PathIsRelative = true;
    size_t Index = 0;
    
    if(0 == path.length()){
        return false;
    }
    if(path[0] == DDelimiter){
        PathIsRelative = false;   
    }
    decomp.clear();
    Anchor = 0;
    while((Anchor + 1 < path.length()) && (std::string::npos != (DelimiterLocation = path.find(DDelimiterString, Anchor)))){
        decomp.push_back(path.substr(Anchor, DelimiterLocation - Anchor));
        Anchor = DelimiterLocation + 1;
    }
    if(Anchor < path.length()){
        std::string TempString = path.substr(Anchor, path.length() - Anchor);
        if(TempString != DDelimiterString){
            decomp.push_back(TempString);
        }
    }
    if(decomp.size()){
        if(!decomp[0].length()){
            decomp.erase(decomp.begin());
        }
    }
    while(Index  < decomp.size()){
        if(decomp[Index] == "."){
            decomp.erase(decomp.begin() + Index);
        }
        else if(decomp[Index] == ".."){
            if(Index){
                Index--;
                decomp.erase(decomp.begin() + Index);
                decomp.erase(decomp.begin() + Index);
            }
            else if(!PathIsRelative){
                return false;
            }
            else{
                Index++;
            }
        }
        else{
            Index++;
        }
    }    

    return true;
}

CPath CPath::SimplifyPath(const CPath &srcpath, const CPath &destpath){
    CPath ReturnPath;
    std::vector< std::string > BasePath;
    std::vector< std::string > RelPath;
    size_t Index = 0;

    ReturnPath.DIsValid = true;
    if(destpath.IsAbsolute()){
       ReturnPath.DIsRelative = false;
    }
    else{
        ReturnPath.DIsRelative = srcpath.DIsRelative;
        ReturnPath.DDecomposedPath = srcpath.DDecomposedPath;
    }
    RelPath = destpath.DDecomposedPath;
    while(Index  < RelPath.size()){
        if(RelPath[Index] == "."){
            RelPath.erase(RelPath.begin());
        }
        else if(RelPath[Index] == ".."){
            if(Index){
                Index--;
                RelPath.erase(RelPath.begin() + Index);
                RelPath.erase(RelPath.begin() + Index);
            }
            else if(ReturnPath.DDecomposedPath.size()){
                ReturnPath.DDecomposedPath.erase(ReturnPath.DDecomposedPath.end() - 1);
                RelPath.erase(RelPath.begin());
            }
            else{
                ReturnPath.DIsValid = false;
                return ReturnPath;
            }
        }
        else{
            Index++;
        }
    }    
    ReturnPath.DDecomposedPath.insert(ReturnPath.DDecomposedPath.end(), RelPath.begin(), RelPath.end());
    return ReturnPath;
}

CPath CPath::RelativePath(const CPath &srcpath, const CPath &destpath){
    CPath ReturnPath(destpath);
    std::vector< std::string > BasePath;

    ReturnPath.DIsRelative = true;
    if(srcpath.DIsRelative){
        if(destpath.DIsRelative){
            return SimplifyPath(srcpath,destpath);
        }
        else{
            return RelativePath(CurrentPath().Simplify(srcpath), destpath);    
        }
    }
    else{
        BasePath = srcpath.DDecomposedPath;    
        while(ReturnPath.DDecomposedPath.size() && BasePath.size()){
            if(ReturnPath.DDecomposedPath[0] == BasePath[0]){
                ReturnPath.DDecomposedPath.erase(ReturnPath.DDecomposedPath.begin());
                BasePath.erase(BasePath.begin());
            }
            else{
                break;
            }
        }
        while(BasePath.size()){
            ReturnPath.DDecomposedPath.insert(ReturnPath.DDecomposedPath.begin(), "..");
            BasePath.erase(BasePath.begin());
        }  
    }
    return ReturnPath;
}

CPath CPath::CurrentPath(){
    std::vector< char > CurrentPathName;

    CurrentPathName.resize(1024);
    while(NULL == getcwd(CurrentPathName.data(), CurrentPathName.size())){
        CurrentPathName.resize(CurrentPathName.size() * 2);
    }
    return CPath(CurrentPathName.data());
}

CPath CPath::CurrentPath(const CPath &path){
    CPath NewPath = SimplifyPath(CurrentPath(), path);
    
    chdir(NewPath.ToString().c_str());
    return CurrentPath();
}
