#include "Slim/Net/CGI/Exception.h"
using std::string;

namespace Slim {
namespace Net {
namespace CGI {

Exception::Exception(int code) : Poco::Exception(code) {
}

Exception::Exception(const std::string& msg, int code) : Poco::Exception(msg, code) {
}

const std::string& Exception::message() {
    return Poco::Exception::message();
}

int Exception::code() {
    return Poco::Exception::code();
}

} // end namespace CGI
} // end namespace Net
} // end namespace Slim
