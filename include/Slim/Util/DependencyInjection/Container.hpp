#ifndef SLIM_UTIL_DEPENDENCYINJECTION_HPP
#define SLIM_UTIL_DEPENDENCYINJECTION_HPP
#include <string>
#include <hash_map>
namespace Slim {
    namespace Util {
        namespace DependencyInjection {
            class Container {
                public :
                    typedef void* service_type;
                    
                    template <typename T>
                    T get(const std::string& id) {
                        return *((T*)services.find(id));
                    }
                    
                    template <typename T>
                    void set(const std::string& id, const T &service) {
                       services[id] = (service_type)(&service);
                    }

                    bool has(const std::string& id) {
                        return (services.find(id) != namemap.end());
                    }
                protected :
                    std::hash_map<std::string, service_type> services;

            }
        }
    }
}
#endif
