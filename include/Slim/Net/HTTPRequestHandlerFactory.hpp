#ifndef SLIM_NET_HTTP_REQUEST_HANDLER_FACTORY
#define SLIM_NET_HTTP_REQUEST_HANDLER_FACTORY

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Slim/Net/HTTPRequestHandler.hpp"
#include <iostream>
using Slim::Net::HTTPRequestHandler;
namespace Slim {
    namespace Net {
        class HTTPRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory {
            public:
                HTTPRequestHandlerFactory(const std::string& format):_format(format) {
                }

                HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) {
                    return new HTTPRequestHandler(_format);
                }

            private:
                std::string _format;
        };
    }
}
#endif
