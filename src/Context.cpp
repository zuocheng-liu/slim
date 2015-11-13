#include "Slim/Net/CGI/Context.h"
using std::string;

namespace Slim {
namespace Net {
namespace CGI {

Context::Context() : requestID(0) {
}

void Context::clear() {
    requestID = 0;
}

} // end namespace CGI
} // end namespace Net
} // end namespace Slim
