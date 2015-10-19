#ifndef SLIM_NET_FCGI_RESPONSE_H 
#define SLIM_NET_FCGI_RESPONSE_H 

#include <Poco/Net/TCPServerConnectionFactory.h> 
#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/StreamSocket.h>
#include <Slim/Net/FCGI/TCPServerConnection.hpp>
#include <Slim/Net/FCGI/Message.hpp>
using Poco::Net::StreamSocket;
using std::hash_map;
using std::pair;
using std::string;

namespace Slim {
namespace Net {
namespace FCGI {
    class Response {
        public :
            Response(StreamSocket& socket) : _socket(socket) {
            }

            void sendEndRequst(int requestID, int protocolStatus) {

                CGIEndRequestRecord record;
                /*
                 * build end request record
                 */
                _message.requestID = requestID;
                _message.type = FCGI_END_REQUEST;
                _message.contentLength = FCGI_END_REQ_BODY_LEN;
                _message.paddingLength = 0;
                _message.protocolStatus = protocolStatus;
                _message.makeHeader(record.header);
                _message.makeEndRequestBody(record.body);
                try {
                    _sendMessage(record);
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
            void sendNameValue(const hash_map<string, string>& params) {
                BYTE buffer[48];
                CGIHeader* header = (CGIHeader*)buffer;
                int pos = FCGI_HEADER_LEN;
                /*
                 * build response name-value
                 */
                if (params.find(FCGI_MAX_CONNS) != params.end()) {
                    _message.makeNameValue(FCGI_MAX_CONNS, "1", buffer, pos);
                } else if (params.find(FCGI_MAX_REQS) != params.end()) {
                    _message.makeNameValue(FCGI_MAX_REQS, "1", buffer, pos);
                } else if (params.find(FCGI_MPXS_CONNS) != params.end()) {
                    _message.makeNameValue(FCGI_MPXS_CONNS, "0", buffer, pos);
                }

                /*
                 * build end request record
                 */
                _message.requestID = FCGI_NULL_REQUEST_ID;
                _message.type = FCGI_GET_VALUE_RESULT;
                _message.contentLength = pos - FCGI_HEADER_LEN;
                _message.paddingLength = ((pos + 7) & 0xFFF8) - pos;
                _message.makeHeader(*header);

                try {
                    _sendMessage(buffer, _message.contentLength + _message.paddingLength);
                } catch (Exception &e) {
                }
            }

            void sendUnknownRecord(int unknownType) {
                CGIUnknownTypeRecord record;
                /*
                 * build end request record
                 */
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

        private :
            template <typename M>
                void _sendMessage(M& m, int length = 0) {
                    if (0 == length) {
                        length = sizeof(M);
                    }
                    void* buffer = (void *)(&m);

                    for (;;) {
                        int count = _soceket.sendBytes(buffer, length);
                        length -= count;
                        if (length == 0) {
                            break;
                        }
                        if (count == 0 && length !=0 ) {
                            continue;
                        }
                    } 
                }

            Message _message;
            StreamSocket& _socket;
            hash_map<string, string> _nameValues;
    };
}
}
}
#endif 
