#ifndef SLIM_NET_FCGI_REQUEST_H
#define SLIM_NET_FCGI_REQUEST_H

#include <Poco/Net/StreamSocket.h>
#include <Slim/Net/FCGI/Message.hpp>
using Poco::Net::StreamSocket;
using std::hash_map;
using std::pair;
using std::string;
namespace Slim {
    namespace Net {
        namespace FCGI {

            class Request {
                public :
                    Request(StreamSocket& socket) : _socket(socket) {
                    }

                    void recv() {
                        CGIHeader header;
                        CGIBeginRequestBody begReqBody;
                        _receiveMessage(header);
                        _message.processHeaderBytes(&header);
                        switch (_message.type) {
                            case FCGI_BEGIN_REQUEST :
                                _receiveMessage(begReqBody);
                                _message.processBeginRecord(&begReqBody);
                                /*
                                 * get environment parameters
                                 */
                                _setRoleParam();
                                _readParams();
                                break;

                            /*
                             * The only type of management message this library 
                             * understands is FCGIGetValues.
                             */
                            case FCGI_GET_VALUES :
                                _message.processManagementRecord();
                                _readParams();
                                break;
                            default :
                                break;
                        }

                        /*
                         * at last , read the rest 
                         */
                        _readPadding();
                    }

                    const hash_map<string, string>& getParams() {
                        return _params;
                    }
                    
                    const int getType() {
                        return _message.type;
                    }

                private :
                    void clear() {
                        _params.clear();
                    }

                private :
                    void _receiveMessage(BYTE* buffer, int length) {
                        for (;;) {
                            int count = _socket.receiveBytes(buffer, length);
                            length -= count;
                            if (length == 0) {
                                break;
                            }
                            if (count == 0 && length != 0) {
                                throw Exception();
                            }
                        }
                    }

                    template <typename M>
                    void _receiveMessage(M) {
                        BYTE* buffer = (BYTE*)(&M);
                        int length = (int) sizeof (M);
                        _receiveMessage(buffer, length);
                    }

                    void _setRoleParam() {
                        switch(_message.role) {
                            case FCGI_RESPONDER:
                                _params.insert(pair<"ROLE","RESPONDER">);
                                break;
                            case FCGI_AUTHORIZER:
                                _params.insert(pair<"ROLE", "AUTHORIZER">);
                                break;
                            case FCGI_FILTER:
                                _params.insert(pair<"ROLE", "FILTER">);
                                break;
                            default :
                                throw Exception(FCGI_ROLE_ERROR);
                        }
                    }
                    /*
                     * Read FCGI name-value pairs from a stream until EOF. Put them
                     * into a map object, storing both as strings.
                     */
                    int _readParams() {
                        int nameLen,valueLen;
                        BYTE lenBuff[3];
                        BYTE* name = NULL;
                        BYTE* value = NULL;
                        for (;;) {
                            try {
                                /*
                                 * receive name length
                                 */
                                _receiveMessage(lenBuff[0]);
                                nameLen = lenBuff[0];
                                if ((lenBuff[0] & 0x80) != 0) {
                                    _receiveMessage(lenBuff, sizeof (lenBuff));
                                    nameLen = ((nameLen & 0x7F) << 24)
                                        | ((lenBuff[0] & 0xFF) << 16)
                                        | ((lenBuff[1] & 0xFF) << 8)
                                        | (lenBuff[2] & 0xFF);
                                }

                                /*
                                 * receive value length
                                 */
                                _receiveMessage(lenBuff[0]);
                                valueLen = lenBuff[0];
                                if ((valueLen & 0x80) != 0) {
                                    _receiveMessage(lenBuff, sizeof (lenBuff));
                                    valueLen = ((valueLen & 0x7f) << 24)
                                        | ((lenBuff[0] & 0xFF) << 16)
                                        | ((lenBuff[1] & 0xFF) << 8)
                                        | (lenBuff[2] & 0xFF);
                                }

                                /*
                                 * nameLen and valueLen are now valid; read the name
                                 * and the value from the stream and construct a standard
                                 * environmental entity
                                 */
                                name  = new BYTE[nameLen];
                                value = new BYTE[valueLen];
                                    _receiveMessage(name, nameLen);
                                    _receiveMessage(value, valueLen);
                                _message.params.insert(
                                        pair<string(name, nameLen), string(value, valueLen)>);
                                delete name;
                                name = NULL;
                                delete value;
                                value = NULL;
                            } catch (Exception& e) {
                                /*
                                 * release resource
                                 */
                                delete name;
                                delete value;
                                break;
                            }
                        } // end for
                    }
                    
                    void _readPadding() {
                        BYTE buffer = new BYTE[8];
                        int length = _message.paddingLength;
                        /*
                         * in case that paddingLength is bigger than 8.
                         */
                        try {
                            for (;;) {
                                _receiveMessage(buffer, _message.paddingLength);
                            }
                        } catch (Exception& e) {
                            throw Exception(FCGI_PROTOCOL_ERROR);
                        }
                    }
                    Message _message;
                    StreamSocket& _socket;
                    hash_map<string, string> _params;
            };
        }
    }
}
#endif
