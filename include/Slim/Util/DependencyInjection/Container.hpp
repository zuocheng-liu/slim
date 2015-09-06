#ifndef SLIM_UTIL_DEPENDENCYINJECTION_HPP
#define SLIM_UTIL_DEPENDENCYINJECTION_HPP
#include <hash_map>
namespace Slim {
    namespace Util {
        namespace DependencyInjection {
            class Container {
                public :
                    typedef void *service_type;
                    inline service_type get(const std::string& id) {
                       return *services.find(id);
                    }
                    
                    template <typename V>
                    inline void set(const std::string& id, const V &service) {
                       services[id] = (service_type)(&service);
                    }

                    inline bool has(const std::string& id) {
                        return (services.find(id) != namemap.end());
                    }
                protected :
                    std::hash_map<std::string, service_type> services;

            }
        }
    }
}
#endif
