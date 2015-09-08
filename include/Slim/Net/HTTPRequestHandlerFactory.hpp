#ifndef SLIM_NET_HTTPREQUESTHANDLER_FACTORY
#define SLIM_NET_HTTPREQUESTHANDLER_FACTORY

#include <iostream>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Slim/Net/HTTPRequestHandler.hpp>
using Slim::Net::HTTPRequestHandler;
namespace Slim {
    namespace Net {
        class HTTPRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory {
            public:
                HTTPRequestHandlerFactory() {
                }

                HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) {
                    return new HTTPRequestHandler();
                }
        };
    }
}
#endif
