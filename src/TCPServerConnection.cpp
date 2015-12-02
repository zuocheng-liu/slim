#include "Slim/Net/CGI/TCPServerConnection.h"
#include "Poco/Util/Application.h"
#include "Slim/Net/CGI/Handler.h"
#include <string> 
#include <sstream> 

using Poco::Util::Application;
using Poco::Net::StreamSocket;
using namespace std;

namespace Slim {
namespace Net {
namespace CGI {

TCPServerConnection::TCPServerConnection(const StreamSocket& socket) 
    : Poco::Net::TCPServerConnection(socket) {
}

void TCPServerConnection::run() {
    Application& app = Application::instance(); 
    app.logger().information("Request from " + this->socket().peerAddress().toString());
    
    // 日志输出连接的TCP用户的地址IP和端口 
    try {
        Handler handler(socket());
        handler.accept();
        //app.logger().information("one byte:" + str); 
    } catch (Exception& e) {
        stringstream ss;
        ss << "\tmessage:\t" << e.message() <<std::endl;
        ss << "\tcode:\t" << e.code() <<std::endl;
        app.logger().information(ss.str());
    }
}

} // end namespace CGI
} // end namespace Net
} // end namespace Slim
