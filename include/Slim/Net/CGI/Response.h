#ifndef SLIM_NET_CGI_RESPONSE_H 
#define SLIM_NET_CGI_RESPONSE_H 

#include "Slim/Net/CGI/Request.h"
#include <Poco/Net/StreamSocket.h>

using Poco::Net::StreamSocket;
using std::map;
using std::string;

namespace Slim {
namespace Net {
namespace CGI {

class Response {
    public :
        Response(StreamSocket& socket, Request& request);
        void sendEndRequst(int protocolStatus);
        void sendStdOut(string out);
        void sendStdErr(string err);

        /**
         *  The only variables that this library's FCGIGetValues 
         *  understands are def_FCGIMaxConns, def_FCGIMaxReqs, 
         *  and def_FCGIMpxsConns. Ignore the other management 
         *  variables, and repsond to other management messages 
         *  with FCGIUnknownType.  
        void sendNameValue(const map<string, string>& params);
        void sendUnknownRecord(int unknownType);
        void sendEOF();
         */
    private :
        StreamSocket& _socket;
        Request& _request;
        OutputStreamBuffer _buf;
        OutputStream _out;
        Message _message;
        int _requestID;
};

} // end namespace CGI
} // end namespace Net
} // end namespace Slim

#endif 
