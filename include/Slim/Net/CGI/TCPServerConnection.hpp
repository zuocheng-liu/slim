#ifndef SLIM_NET_CGI_TCPSERVERCONNECTION_HPP 
#define SLIM_NET_CGI_TCPSERVERCONNECTION_HPP 

#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/StreamSocket.h>
#include "Slim/Net/CGI/Handler.h"
#include <string> 
#include <sstream> 

using Poco::Util::Application;
using Poco::Net::StreamSocket;
using namespace std;

namespace Slim {
namespace Net {
namespace CGI {

class TCPServerConnection: public Poco::Net::TCPServerConnection { 
    public:
        TCPServerConnection(const StreamSocket& socket) : 
            Poco::Net::TCPServerConnection(socket) {
            }
        void run() {
            Application& app = Application::instance(); 
            // 日志输出连接的TCP用户的地址IP和端口 
            stringstream ss;
            try {
                Handler cgi(socket());
                cgi.accept();
            } catch (Exception& e) {
                ss << "\tmessage:\t" << e.message() <<std::endl;
                ss << "\tcode:\t" << e.code() <<std::endl;
                app.logger().information(ss.str());
            }
        }
}; 

} // end namespace CGI
} // end namespace Net
} // end namespace Slim
#endif 
