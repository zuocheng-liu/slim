#include "Slim/Net/CGI/Request.h"

using Poco::Net::StreamSocket;
using std::map;
using std::string;
using std::cout;
namespace Slim {
namespace Net {
namespace CGI {

Request::Request(StreamSocket& socket) :
    _requestID(0),
    _buf(socket), 
    _in(_buf) {
}

Request::~Request() {
}

void Request::recv() {
    unsigned int paramRecv = INIT;
    unsigned int stdInRecv = INIT;
    try {
        for (;;) {
            CGIHeader header;
            cout<<"-------Recieve Header-------"<<std::endl;
            _in >> header;
            cout<<"-------Recieve Header Done-------"<<std::endl;
            _message.processHeader(header);
            cout<<"-------Process Header Done-------"<<std::endl;
            cout<<"type:"<<_message.type<<std::endl;
            cout<<"REQID:"<<_message.requestID<<std::endl;
            cout<<"conLen:"<<_message.contentLength<<std::endl;
            cout<<"version:"<<_message.version<<std::endl;
            cout<<"padding:"<<_message.paddingLength<<std::endl;
            switch (_message.type) {
                case FCGI_BEGIN_REQUEST :
                    if (_message.contentLength > 0) {
                        beginRequest();
                        recvPadding();
                        cout<<"-------Begin Request End-------"<<std::endl;
                    } else {
                        throw Exception(FCGI_PROTOCOL_ERROR);
                    }
                    recvPadding();
                    continue;
                case FCGI_PARAMS :
                    /*
                     * get environment parameters
                     */
                    if (_message.contentLength > 0) {
                        recvParams();
                        paramRecv = RECV;
                    } else if (_message.contentLength == 0) {
                        paramRecv = DONE;
                    } else {
                        throw Exception(FCGI_PROTOCOL_ERROR);
                    }
                    cout<<"-------Param END-------"<<std::endl;
                    recvPadding();
                    continue;
                case FCGI_STDIN :
                    if (_message.contentLength > 0) {
                        recvStdIn();
                        stdInRecv = RECV;
                    } else if (_message.contentLength == 0) {
                        stdInRecv = DONE;
                    } else {
                        throw Exception(FCGI_PROTOCOL_ERROR);
                    }
                    cout<<"-------Stdin END-------"<<std::endl;
                    recvPadding();
                    continue;
                /*
                 * The only type of management message this library 
                 * understands is FCGIGetValues.
                 */
                case FCGI_GET_VALUES :
                    recvPadding();
                    continue;
                default :
                    continue;
            }
            if (paramRecv == DONE && stdInRecv == DONE) {
                break;
            }
        }
    } catch (Exception) {
    }
}

void Request::beginRequest() {
    CGIBeginRequestBody begReqBody;
    _in >> begReqBody;
    cout<<"roleB1:\t"<<begReqBody.roleB1<<std::endl;
    cout<<"roleB0:\t"<<begReqBody.roleB0<<std::endl;
    _message.processBeginRecord(begReqBody);
    cout<<"role:\t"<<_message.role<<std::endl;
    cout<<"flags:\t"<<_message.flags<<std::endl;
}

void Request::recvParams() {
    _message.data.resize(_message.contentLength);
    _in.getline(_message.data, _message.contentLength);
    _message.processParams(_message.data, _params);
} 

void Request::recvStdIn() {
    _message.data.resize(_message.contentLength);
    _in.getline(_message.data, _message.contentLength);
    _message.processStdIn(_message.data, _stdIn);
} 


void Request::recvPadding() {
    BYTE buffer[8];
    int len = _message.paddingLength;
    while (len > 0) {
        int min = len < 8 ? len : 8;
        _in.getline(buffer, (std::streamsize)min);
        len -= min;
    }
}

const map<string, string>& Request::getParams() {
    return _params;
}

const int Request::getRequestID() {
    return _requestID;
}

/*
const int Request::getType() {
    return _requestID;
}
*/

InputStream& Request::getStdin() {
    return _in;
}

void Request::clear() {
    _requestID = 0;
    _params.clear();
}

} // end namespace CGI
} // end namespace Net
} // end namespace Slim

