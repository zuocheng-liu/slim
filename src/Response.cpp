#include "Slim/Net/CGI/Response.h"
using Poco::Net::StreamSocket;
using std::map;
using std::string;
namespace Slim {
namespace Net {
namespace CGI {

Response::Response(StreamSocket& socket, Request& request) : 
    _socket(socket),
    _request(request),
    _buf(socket),
    _out(_buf) {
    _requestID = request.getRequestID();
}

void Response::sendEndRequst(int protocolStatus) {
    try {
        CGIEndRequestRecord record;
        /*
         * build end request record
         */
        _message.requestID = _requestID;
        _message.type = FCGI_END_REQUEST;
        _message.contentLength = FCGI_END_REQ_BODY_LEN;
        _message.paddingLength = 0;
        _message.protocolStatus = protocolStatus;
        _message.makeHeader(record.header);
        _message.makeEndRequestBody(record.body);
        _out<<record;
    } catch (Exception &e) {
    }
}

/**
 *  The only variables that this library's FCGIGetValues 
 *  understands are def_FCGIMaxConns, def_FCGIMaxReqs, 
 *  and def_FCGIMpxsConns. Ignore the other management 
 *  variables, and repsond to other management messages 
 *  with FCGIUnknownType.  
 */
    /*
void Response::sendNameValue(const map<string, string>& params) {
    BYTE buffer[48];
    CGIHeader* header = (CGIHeader*)buffer;
    int pos = FCGI_HEADER_LEN;
     * build response name-value
    if (params.find(FCGI_MAX_CONNS) != params.end()) {
        _message.makeNameValue(FCGI_MAX_CONNS, "1", buffer, pos);
    } else if (params.find(FCGI_MAX_REQS) != params.end()) {
        _message.makeNameValue(FCGI_MAX_REQS, "1", buffer, pos);
    } else if (params.find(FCGI_MPXS_CONNS) != params.end()) {
        _message.makeNameValue(FCGI_MPXS_CONNS, "0", buffer, pos);
    }
     */

    /*
     * build end request record
    _message.requestID = FCGI_NULL_REQUEST_ID;
    _message.type = FCGI_GET_VALUES_RESULT;
    _message.contentLength = pos - FCGI_HEADER_LEN;
    _message.paddingLength = ((pos + 7) & 0xFFF8) - pos;
    _message.makeHeader(*header);

    try {
        _sendMessage(buffer, _message.contentLength + _message.paddingLength);
    } catch (Exception &e) {
    }
}
     */

    /*
void Response::sendUnknownRecord(int unknownType) {
    CGIUnknownTypeRecord record;
     * build unknown record
    _message.requestID = FCGI_NULL_REQUEST_ID;
    _message.type = FCGI_UNKNOWN_TYPE;
    _message.contentLength = FCGI_UNKOWN_BODY_TYPE_BODY_LEN;
    _message.paddingLength = 0;
    _message.makeHeader(record.header);
    _message.makeUnknownTypeBody(record.body, unknownType);
    try {
        _sendMessage(record);
    } catch (Exception &e) {
    }
}
     */

void Response::sendStdOut(string out) {
    ByteArray ba; 
    _message.requestID = _requestID;
    _message.type = FCGI_STDOUT;
    _message.contentLength = out.length();
    int& len = _message.contentLength;
    _message.paddingLength = ((len + 7) & 0xFFFFFFF8) - len;
    _message.data.assign((BYTE*)const_cast<char*>(out.c_str()), len);
    _message.makeStdOutBody(ba);
    _out<<ba;
}

void Response::sendStdErr(string err) {
    ByteArray ba; 
    _message.requestID = _requestID;
    _message.type = FCGI_STDERR;
    _message.contentLength = err.length();
    int& len = _message.contentLength;
    _message.paddingLength = ((len + 7) & 0xFFFFFFF8) - len;
    _message.data.assign((BYTE*)const_cast<char*>(err.c_str()), len);
    
    _message.makeStdErrBody(ba);
    _out<<ba;
}

} // end namespace CGI
} // end namespace Net
} // end namespace Slim
