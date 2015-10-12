#ifndef SLIM_NET_FCGI_REQUEST_H 
#define SLIM_NET_FCGI_REQUEST_H 

#include <Poco/Net/StreamSocket.h>
#include <Slim/Net/FCGI/Message.hpp>
namespace Slim {
    namespace Net {
        namespace FCGI {
           
            const unsigned int BUFFER_SIZE = 8192;
            class Request : public Message {
                public :
                    
                    Request(Poco::Net::StreamSocket& socket) : socket_(socket) {
                        BYTE buffer[BUFFER_SIZE];
                        BYTE *bufferStop = buffer;
                        for (;;) {
                            int total = 0;
                            int count = socket_.receiveBytes(bufferStop, BUFFER_SIZE + buffer - bufferStop);
                            if (count == 0) {
                                break;
                            }
                            total += count;
                            bufferStop += count;
                        }
                        protocol.parse(buffer);
                    }

                    void recv() {
                    }

                private :
                    Poco::Net::StreamSocket& socket_;
            };
        }
    }
}
#endif 
