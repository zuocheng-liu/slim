#ifndef SLIM_UTIL_REFLECTION_CLASSFACTORY_HPP
#define SLIM_UTIL_REFLECTION_CLASSFACTORY_HPP
#include <hash_map>
#include <string>
#include <cstring>
#include <cstdlib>
#include "Slim/Util/Reflection/Object.hpp"
#include "Slim/Exception.hpp"
namespace Slim {
    namespace Util {
        namespace Reflection {
            class ClassFactory {
                public :
                    inline Object* create(const std::string& className) {
                        Object *prototype = getPrototype(className);
                        Object *obj = (Object*)malloc(prototype->getClassSize());
                        memcpy(prototype, obj , prototype->getClassSize());
                        return obj;
                    }

                    inline Object* getPrototype(const std::string& className) {
                        std::hash_map<std::string, Object*> ::iterator it = reflectionMap.find(className);
                        if (it == reflectionMap.end()) {
                            throw new Slim::Exception("Class " + className + " is not in Reflection Map.");
                        } 
                        Object *prototype = it->second;
                        return prototype;
                    }
                    static inline void registerClass(const Object*& p) {
                       reflectionMap.insert(pair<std::string, Object*>(p->getClassName(), p));
                    }
                protected :
                    static std::hash_map<std::string, Object*> reflectionMap; 
            }
        }
    }
}
#endif
