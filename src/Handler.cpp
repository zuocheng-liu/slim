#include "Slim/Net/CGI/Handler.h"
using Poco::Net::StreamSocket;
using std::map;
using std::string;

namespace Slim {
namespace Net {
namespace CGI {

Handler::Handler(StreamSocket& socket) :
    _socket(socket),
    _request(socket),
    _response(socket, _request) {
}

Handler::~Handler() {
    complete();
}

void Handler::accept() {
        _clear();
        _request.recv();
        handle();
        complete();
        
                /*
        //int requestType = _request.getType();
        switch (requestType) {
            case FCGI_BEGIN_REQUEST :
                break;
            case FCGI_GET_VALUES :
                 * send FCGI_GET_VALUES_RESULT
                _response.sendNameValue(_request.getParams());
                break;
            case FCGI_ABORT_REQUEST :
                _response.sendEndRequst(_request.getRequestID(), FCGI_REQUEST_COMPLETE);
                break;
            default :
                _response.sendUnknownRecord(requestType);
                break;
            default :
                throw Exception(-123);
        }
                 */
}

void Handler::complete() {
    _response.sendEndRequst(FCGI_REQUEST_COMPLETE);
}

void Handler::handle() {
    accept();
    string out = "Content-type: text/html\r\n";
    out += "\r\n";
    out += "<h1>FastCGI Hello!</h1>";
    _response.sendStdOut(out);
    string err = "";
    _response.sendStdErr(err);
}


const map<string, string>& Handler::getParams() {
    return _request.getParams();
}

void Handler::_clear() {
    _request.clear();
    //    _response.clear();
}

} // end namespace CGI
} // end namespace Net
} // end namespace Slim
