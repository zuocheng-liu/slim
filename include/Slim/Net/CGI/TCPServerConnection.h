#ifndef SLIM_NET_CGI_TCPSERVERCONNECTION_H 
#define SLIM_NET_CGI_TCPSERVERCONNECTION_H 

#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/StreamSocket.h>

using Poco::Net::StreamSocket;
using namespace std;

namespace Slim {
namespace Net {
namespace CGI {

class TCPServerConnection: public Poco::Net::TCPServerConnection { 
    public:
        TCPServerConnection(const StreamSocket& socket);
        void run();
}; 

} // end namespace CGI
} // end namespace Net
} // end namespace Slim
#endif 
