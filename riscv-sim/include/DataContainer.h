
#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <string>
#include <memory>

class CDataSource;
class CDataSink;

class CDataContainerIterator{
    public:  
        virtual ~CDataContainerIterator(){};
        virtual std::string Name() = 0;
        virtual bool IsContainer() = 0;
        virtual bool IsValid() = 0;
        virtual void Next() = 0;
};

class CDataContainer{
    public:
        virtual ~CDataContainer(){};
        virtual std::shared_ptr< CDataContainerIterator > First() = 0;
        virtual std::shared_ptr< CDataSource > DataSource(const std::string &name) = 0;
        virtual std::shared_ptr< CDataSink > DataSink(const std::string &name) = 0;
        virtual std::shared_ptr< CDataContainer > Container(){
            return nullptr;
        };
        virtual std::shared_ptr< CDataContainer > DataContainer(const std::string &name) = 0;
};

#endif
