#ifndef JSONELEMENT_H
#define JSONELEMENT_H

#include <cstdint>
#include <string>
#include <vector> 
#include <memory>
#include <map>

class CJSONElement{
    public:
        enum class EType{Number, String, Boolean, Array, Object};
    protected:
        EType DType;

        std::map< std::string, std::shared_ptr< CJSONElement > > DMembers;

    public:
        EType Type() const;
        void Type(EType type);

        std::string PrimitiveText() const;
        void PrimitiveText(const std::string &text);

        void AssignMember(const std::string &key, std::shared_ptr< CJSONElement > element);
        void AssignMember(const std::string &key, const std::string &str);
        void AssignMember(const std::string &key, double val);
        void AssignMember(const std::string &key, uint64_t val);
        void AssignMember(const std::string &key, int64_t val);
        void AssignMember(const std::string &key, uint32_t val);
        void AssignMember(const std::string &key, int32_t val);
        void AssignMember(const std::string &key, uint16_t val);
        void AssignMember(const std::string &key, int16_t val);
        void AssignMember(const std::string &key, bool tf);
        void PushBack(std::shared_ptr< CJSONElement > element);

        size_t ChildCount() const;
        void MemberKeys(std::vector< std::string > &keys) const;
        std::shared_ptr< const CJSONElement > Member(const std::string &member) const;
        std::shared_ptr< const CJSONElement > Index(size_t index) const;
};

#endif
