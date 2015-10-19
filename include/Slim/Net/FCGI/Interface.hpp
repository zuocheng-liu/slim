#ifndef SLIM_NET_FCGI_MESSAGE_H
#define SLIM_NET_FCGI_MESSAGE_H

#include <ostream>
#include <istream>
#include "Request.hpp"
#include "Response.hpp"
#include <Poco/Net/StreamSocket.h>
using Poco::Net::StreamSocket;
using std::hash_map;
using std::string;
namespace Slim {
namespace Net {
namespace FCGI {
    class Interface {
        public :
            Interface(StreamSocket& socket) :
                _isBeginProcessed(false), 
                _socket(socket),
                _request(socket),
                _response(socket) {
                }

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
            int accept() {
                for (;;) {
                    _clear();
                    _request.recv();
                    int requestType = _request.getType();
                    if (!_isBeginProcessed && 
                            requestType != FCGI_BEGIN_REQUEST){
                        continue;
                    } 
                    if (!_isBeginProcessed && 
                            requestType != FCGI_BEGIN_REQUEST){
                        /*
                         * If the webserver is multiplexing the connection,
                         * this library can't deal with it, so repond with
                         * FCGIEndReq message with protocolStatus FCGICantMpxConn
                         */
                        _response.sendEndRequst(_request.requestID, 
                                FCGI_CANT_MPX_CONN);
                        _isBeginProcessed = true;
                        continue;
                    }
                    switch (requestType) {
                        case FCGI_BEGIN_REQUEST :
                            return 1;
                        case FCGI_GET_VALUES :
                            _response.sendNameValue(_request.getParams());
                            return 0;
                        case FCGI_ABORT_REQUEST :
                            _response.sendEndRequst(_request.requestID, FCGI_REQUEST_COMPLETE);
                        default :
                            _response.sendUnknownRecord(requestType);
                            return -2;
                    }
                }
            }

            const hash_map<string, string>& getParams() {
                return _request.getParams();
            }
        private :

            void _clear() {
                _request.clear();
                _response.clear();
            }

            bool _isBeginProcessed;
            StreamSocket& _socket;
            Request _request;
            Response _response;
    }
}
}
}
#endif
