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

    typedef struct {
        BYTE data[BUFFER_SIZE];
        int length;
        int pos;
    } Buffer;

    class Request {
        public :
            Request(StreamSocket& socket) : 
                _socket(socket) {
                    _buffer = new Buffer;
                    _buffer->length = 0;
                    _buffer->pos = 0;
                }
            ~Request() {
                delete _buffer;
            }
            void recv() {
                try {
                    CGIHeader* header;
                    CGIBeginRequestBody* begReqBody;
                    _receiveMessage(header);
                    _message.processHeaderBytes(header);
                    switch (_message.type) {
                        case FCGI_BEGIN_REQUEST :
                            if (_isBeginProcessed) {
                                _receiveMessage(begReqBody);
                                _message.processBeginRecord(begReqBody);
                                /*
                                 * get environment parameters
                                 */
                                _setRoleParam();
                                _receiveAll();
                                _message.processParams(
                                        _buffer->data + _buffer->pos,
                                        _buffer->length - _buffer->pos);
                                break;
                            } else {
                            }
                            /*
                             * The only type of management message this library 
                             * understands is FCGIGetValues.
                             */
                        case FCGI_GET_VALUES :
                            _message.processManagementRecord();
                            _receiveAll();
                            _readParams();
                            break;
                        case FCGI_ABORT_REQUEST :
                            break;
                        default :
                            break;
                    }
                    //_readPadding();

                } catch (exception) {
                    delete _buffer;
                }
            }

            const hash_map<string, string>& getParams() {
                return _message.params;
            }

            const int getType() {
                return _message.type;
            }

                private :
                void clear() {
                    _buffer->length = 0;
                    _buffer->pos = 0;
                }

                private :
                /*
                 * read fcgi message from buffer
                 */
                void _receiveMessage(BYTE*& msg, int msgLen) {
                    /*
                     * if bytes are not enough, receive bytes
                     */
                    while (_buffer->length - _buffer->pos < msgLen) {
                        int count = _socket.receiveBytes(
                                _buffer->data + pos,
                                BUFFER_SIZE - _buffer->length);
                        if (count == 0) {
                            throw Exception(FCGI_PROTOCOL_ERROR);
                        }
                        _buffer->length += count;
                    };

                    msg = _buffer->data + _buffer->pos;
                    _buffer->pos += msgLen;
                }

                template <typename M>
                    void _receiveMessage(M*& msg) {
                        _receiveMessage(msg, sizeof(M));
                    }

                void _receiveAll() {
                    int count;
                    do {
                        count = _socket.receiveBytes(
                                _buffer->data + pos,
                                BUFFER_SIZE - _buffer->length);
                    } while (count > 0);
                }              
                void _readPadding() {
                    BYTE* padding;
                    _receiveMessage(padding, _message.paddingLength);
                }

                StreamSocket& _socket;
                Buffer* _buffer;
                Message _message;
            };
    }
}
}
#endif
