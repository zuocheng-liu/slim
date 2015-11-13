#ifndef SLIM_NET_CGI_REQUEST_H
#define SLIM_NET_CGI_REQUEST_H

#include "Slim/Net/CGI/Message.h"
#include "Slim/Net/CGI/Stream.h"
#include <Poco/Net/StreamSocket.h>
#include <map>
#include <cstring>
using Poco::Net::StreamSocket;
using std::map;
using std::string;
namespace Slim {
namespace Net {
namespace CGI {

class Request {
    public :
        
        enum RECV_STATUS {
            INIT = 0x00,
            RECV = 0x01,
            END  = 0x10,
            DONE = 0x11
        };

        Request(StreamSocket& socket);
        ~Request();
        void recv();
        const map<string, string>& getParams();
        const int getType();
        const int getRequestID();
        InputStream& getStdin();
        void clear();
    
    private :
        void beginRequest();
        void recvParams();
        void recvStdIn();
        void recvPadding();
        int _requestID;
        map<string, string> _params;
        string _stdIn;
        string _stdErr;
        
        InputStreamBuffer _buf;
        InputStream _in;
        Message _message;
}; // end class Request

} // end namespace CGI
} // end namespace Net
} // end namespace Slim
#endif
