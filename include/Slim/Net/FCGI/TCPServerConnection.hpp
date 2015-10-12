#ifndef SLIM_NET_CGI_TCPSERVERCONNECTION_HPP 
#define SLIM_NET_CGI_TCPSERVERCONNECTION_HPP 

#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/NetException.h>
#include <Slim/Net/CGI/TCPRequest.hpp>
#include <Poco/Exception.h>
#include <string> 
#include <sstream> 
#include <cstdlib> 
#include <iostream> 
using std::string;
namespace Slim {
    namespace Net {
        namespace CGI {
            class TCPServerConnection: public Poco::Net::TCPServerConnection { 
                public:
                    TCPServerConnection(const Poco::Net::StreamSocket& socket) : Poco::Net::TCPServerConnection(socket) {
                    }
                    void run() {
                        TCPRequest request(socket());
                        char buff[1024];
                        int i;
                        Poco::Util::Application& app = Poco::Util::Application::instance(); 
                        // 日志输出连接的TCP用户的地址IP和端口 
                        std::stringstream log;
                        std::string logS;
                        log << ("Request from " + socket().peerAddress().toString())<<std::endl;
                        log << "length :"<< request.protocol.header.contentLength<<std::endl;
                        log << "version :"<< request.protocol.header.version<<std::endl;
                        log << "id :"<< request.protocol.header.requestId<<std::endl;
                        log << "type :"<< request.protocol.header.type<<std::endl;
                        std::cout<<log.str();
                        //app.logger().information("Request from " + socket().peerAddress().toString());
                        //app.logger().information();
                        try {
                            Poco::Timestamp now;
                            std::string timeFormat = Poco::DateTimeFormat::SORTABLE_FORMAT;
                            std::string dt(Poco::DateTimeFormatter::format(now, timeFormat));
                            dt.append("\r\n");   
                            //socket().sendBytes(dt.data(), (int) dt.length());
                        } catch (Poco::Exception& e) { 
                            app.logger().log(e); 
                        } 
                    }
            }; 
        }
    }
}
#endif 
