#ifndef SLIM_NET_FCGI_EXCEPTION_H 
#define SLIM_NET_FCGI_EXCEPTION_H 
#include <Poco/Excpetion.h>
namespace Slim {
    namespace Net {
        namespace FCGI {
            class Exception : Poco::Exception {
                public :
                    Exception(int code = 0) : Poco::Exception(code){
                    }
            };
        }
    }
}
#endif
