#ifndef SLIM_NET_CGI_TCPSERVERCONNECTION_HPP 
#define SLIM_NET_CGI_TCPSERVERCONNECTION_HPP 

#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/NetException.h>
#include "Interface.hpp"
#include <Poco/Exception.h>
#include <string> 
#include <sstream> 
#include <cstdlib> 
#include <iostream> 
using Poco::Util::Application;
using std::string;
using std::stringstream;
namespace Slim {
namespace Net {
namespace FCGI {
    class TCPServerConnection: public Poco::Net::TCPServerConnection { 
        public:
            TCPServerConnection(const Poco::Net::StreamSocket& socket) : 
                Poco::Net::TCPServerConnection(socket) {
            }
            void run() {
                Application& app = Application::instance(); 
                // 日志输出连接的TCP用户的地址IP和端口 
                //Interface interface;
                stringstream ss;
                try {
                    Interface interface();
                } catch (Exception& e) {
                    ss << "\tmessage:\t" << e.message() <<std::endl;
                    ss << "\tcode:\t" << e.code() <<std::endl;
                    app.logger().information(ss.str());
                }
            }
    }; 
}
    }
}
#endif 
