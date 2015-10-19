#ifndef SLIM_NET_CGI_TCPCONNECTIONFACTORY_H 
#define SLIM_NET_CGI_TCPCONNECTIONFACTORY_H 

#include <Poco/Net/TCPServerConnectionFactory.h> 
#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/StreamSocket.h>
#include "TCPServerConnection.hpp"
namespace Slim {
    namespace Net {
        namespace FCGI {
            class TCPServerConnectionFactory: public Poco::Net::TCPServerConnectionFactory { 
                
                public:
                    Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket) { 
                        return new TCPServerConnection(socket); 
                    } 
            };
        }
    }
}
#endif 
