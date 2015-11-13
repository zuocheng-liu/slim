#ifndef SLIM_NET_CGI_HANDLER_H
#define SLIM_NET_CGI_HANDLER_H

#include "Slim/Net/CGI/Response.h"
using Poco::Net::StreamSocket;

namespace Slim {
namespace Net {
namespace CGI {

class Handler {
    public :
        Handler(StreamSocket& socket);
        ~Handler();
        /*
         * Accepts a new request from the HTTP server and creates
         * a conventional execution environment for the request.
         * If the application was invoked as a FastCGI server,
         * the first call to FCGIaccept indicates that the application
         * has completed its initialization and is ready to accept
         * a request.  Subsequent calls to FCGI_accept indicate that
         * the application has completed its processing of the
         * current request and is ready to accept a new request.
         * If the application was invoked as a CGI program, the first
         * call to FCGIaccept is essentially a no-op and the second
         * call returns EOF (-1) as does an error. Application should exit.
         *
         * If the application was invoked as a FastCGI server,
         * and this is not the first call to this procedure,
         * FCGIaccept first flushes any buffered output to the HTTP server.
         *
         * On every call, FCGIaccept accepts the new request and
         * reads the FCGI_PARAMS stream into System.props. It also creates
         * streams that understand FastCGI protocol and take input from
         * the HTTP server send output and error output to the HTTP server,
         * and assigns these new streams to System.in, System.out and
         * System.err respectively.
         *
         * For now, we will just return an int to the caller, which is why
         * this method catches, but doen't throw Exceptions.
         *
         */
        void accept();
        void handle();
        const map<string, string>& getParams();
        void send();
        void sendError();
        void complete();
    private :
        void _clear();
        bool _isBeginProcessed;
        StreamSocket& _socket;
        Request _request;
        Response _response;
        Message _message;
        //Context _context;

}; // end class Handler

class AbstractRequestHandler {
    public :
        virtual void handle() = 0;
    private :
        void checkHeader();
};

} // end namespace CGI
} // end namespace Net
} // end namespace Slim
#endif
