
#ifndef PATH_H
#define PATH_H

#include <string>
#include <vector>

class CPath{
    protected:
        std::vector< std::string > DDecomposedPath;
        bool DIsRelative;
        bool DIsValid;
        
        static bool DecomposePath(std::vector< std::string > &decomp, const std::string &path);
        static const char DDelimiter;
        static const std::string DDelimiterString;
        
    public:
        CPath();
        CPath(const CPath &path);
        CPath(const std::string &path);
        
        CPath &operator=(const CPath &path);
        
        CPath Containing() const;
        
        bool IsRelative() const{
            return DIsRelative;  
        };
        
        bool IsAbsolute() const{
            return !DIsRelative;  
        };
        
        bool IsValid() const{
            return DIsValid;  
        };
        
        size_t ComponentCount() const{
            return DDecomposedPath.size();
        };
        
        std::string Component(size_t index) const{
            if((0 > index)||(index >= DDecomposedPath.size())){
                return "";   
            }
            return DDecomposedPath[index];  
        };
        
        std::string ToString() const;
        operator std::string() const;
        
        CPath Simplify(const CPath &destpath) const;
        CPath Relative(const CPath &destpath) const;
        
        static CPath SimplifyPath(const CPath &srcpath, const CPath &destpath);
        static CPath RelativePath(const CPath &srcpath, const CPath &destpath);
        static CPath CurrentPath();
        static CPath CurrentPath(const CPath &path);
        
};

#endif
