#ifndef SLIM_NET_REQUEST_HANDLER_HPP
#define SLIM_NET_REQUEST_HANDLER_HPP

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include <iostream>

namespace Slim {
    namespace Net {
        class HTTPRequestHandler: public Poco::Net::HTTPRequestHandler {
            public:
                TimeRequestHandler(const std::string& format):  _format(format) {
                }
                
                void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
                    Application& app = Application::instance();
                    app.logger().information("Request from " + request.clientAddress().toString());

                    Poco::Timestamp now;
                    std::string dt(Poco::DateTimeFormatter::format(now, _format));

                    response.setChunkedTransferEncoding(true);
                    response.setContentType("text/html");

                    std::ostream& ostr = response.send();
                    ostr << "<html><head><title>HTTPTimeServer powered by POCO C++ Libraries</title>";
                    ostr << "<meta http-equiv=\"refresh\" content=\"1\"></head>";
                    ostr << "<body><p style=\"text-align: center; font-size: 48px;\">";
                    ostr << dt;
                    ostr << "</p></body></html>";
                }
            private:
                std::string _format;
        };
    }
}
#endif
