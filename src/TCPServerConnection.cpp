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
        InputStreamBuffer isBuf(socket());
        InputStream is(isBuf);
        OutputStreamBuffer osBuf(socket());
        OutputStream os(osBuf);
        
        string str;
        /*
        int a = isBuf.sbumpc();
        s += a;
        app.logger().information("one byte:" + s); 
        */
        //char buffer[512];
        long buffer[512];
        is>>buffer;
        str += (char*)buffer;
        /*
        BYTE b;
        while (is.get(b)) {
            str += (char)b;
        }
        */
        app.logger().information("one byte:" + str); 
        //is>>buffer;
        //socket().receiveBytes(buffer, 512);
        //app.logger().information(string(buffer)); 
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
