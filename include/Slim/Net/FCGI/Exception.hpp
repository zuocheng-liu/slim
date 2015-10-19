#ifndef SLIM_NET_FCGI_EXCEPTION_H 
#define SLIM_NET_FCGI_EXCEPTION_H 
#include <Poco/Exception.h>
namespace Slim {
namespace Net {
namespace FCGI {
    /*
     * Error Codes 
     */
    enum ErrorCode {
        UnsupportedVersion = -2,
        FCGI_PROTOCOL_ERROR = -3,
        ParamsError = -4,
        CallSeqError = -5,
        FCGI_ROLE_ERROR = -6
    };

    class Exception : Poco::Exception {
        public :
            Exception(int code = 0) 
                    : Poco::Exception(code) {
            }
            Exception(const std::string& msg, int code = 0) 
                : Poco::Exception(msg, code) {
            }


            const std::string& message() {
                return Poco::Exception::message();
            }

            int code() {
                return Poco::Exception::code();
            }
    };
}
}
}
#endif
