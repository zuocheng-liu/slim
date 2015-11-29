#ifndef SLIM_NET_CGI_TCPCONNECTIONFACTORY_H 
#define SLIM_NET_CGI_TCPCONNECTIONFACTORY_H 

#include <Poco/Net/TCPServerConnectionFactory.h> 
#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/StreamSocket.h>

namespace Slim {
namespace Net {
namespace CGI {

class TCPServerConnectionFactory: public Poco::Net::TCPServerConnectionFactory { 
    public:
        Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket); 
};

} // end namespace CGI
} // end namespace Net
} // end namespace Slim
#endif 
