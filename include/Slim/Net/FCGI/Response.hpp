#ifndef SLIM_NET_FCGI_RESPONSE_H 
#define SLIM_NET_FCGI_RESPONSE_H 

#include <Poco/Net/TCPServerConnectionFactory.h> 
#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/StreamSocket.h>
#include <Slim/Net/FCGI/TCPServerConnection.hpp>
namespace Slim {
    namespace Net {
        namespace FCGI {
            class Response : public Message { 
            };
        }
    }
}
#endif 
