#ifndef SLIM_NET_CGI_EXCEPTION_H 
#define SLIM_NET_CGI_EXCEPTION_H 
#include <Poco/Exception.h>

namespace Slim {
namespace Net {
namespace CGI {

/*
 * Error Codes 
 */
enum ErrorCode {
    UNSUPPORTED_VERSION = -2,
    FCGI_PROTOCOL_ERROR = -3,
    PARAMS_ERROR = -4,
    CALL_SEQ_ERROR = -5,
    FCGI_ROLE_ERROR = -6
};

class Exception : Poco::Exception {
    public :
        Exception(int code = 0);
        Exception(const std::string& msg, int code = 0); 

        const std::string& message();
        int code();  
};

} // end namespace CGI
} // end namespace Net
} // end namespace Slim
#endif
