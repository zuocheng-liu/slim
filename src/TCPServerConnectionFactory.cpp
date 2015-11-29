#include "Slim/Net/CGI/TCPServerConnectionFactory.h"
#include "Slim/Net/CGI/TCPServerConnection.h"

using Poco::Net::StreamSocket;

namespace Slim {
namespace Net {
namespace CGI {

Poco::Net::TCPServerConnection* 
TCPServerConnectionFactory::createConnection(const StreamSocket& socket) { 
    return new TCPServerConnection(socket); 
} 

} // end namespace CGI
} // end namespace Net
} // end namespace Slim

