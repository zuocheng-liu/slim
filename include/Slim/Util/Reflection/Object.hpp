#ifndef SLIM_UTIL_REFLECTION_OBJECT_HPP
#define SLIM_UTIL_REFLECTION_OBJECT_HPP
#include <string>
#include <Slim/Util/Reflection/ClassFactory.hpp>
namespace Slim {
    namespace Util {
        namespace Reflection {
            class Object {
                public :
                    virtual size_t  getClassSize () {
                        return classSize;
                    }
                    virtual std::string getClassName {
                        return className;
                    }
                    virtual void initialize () = 0;

                protected :
                    virtual void setClassSize (const size_t size) {
                        classSize = size;
                    }
                    virtual void setClassName (const std::string name){
                        className = name;
                    }
                    void registerClass(Object*& p) {
                        ClassFactory::registerClass(p);
                    }
                    std::string className;
                    size_t classSize;
            }
        }
    }
}
#endif
