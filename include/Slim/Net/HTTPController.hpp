#ifndef SLIM_NET_HTTPCONTROLLER_HPP
#define SLIM_NET_HTTPCONTROLLER_HPP

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Timestamp.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/Util/ServerApplication.h>
#include <iostream>

namespace Slim {
    namespace Net {
        class HTTPController {
            public:
                HTTPController() {
                }
                
                void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
                    Poco::Util::Application& app = Poco::Util::Application::instance();
                    app.logger().information("Request from " + request.clientAddress().toString());

                    Poco::Timestamp now;
                    std::string timeFormat = Poco::DateTimeFormat::SORTABLE_FORMAT;
                    std::string dt(Poco::DateTimeFormatter::format(now, timeFormat));

                    response.setChunkedTransferEncoding(true);
                    response.setContentType("text/html");

                    std::ostream& ostr = response.send();
                    ostr << "<html><head><title>HTTPTimeServer powered by POCO C++ Libraries</title>";
                    ostr << "<meta http-equiv=\"refresh\" content=\"1\"></head>";
                    ostr << "<body><p style=\"text-align: center; font-size: 48px;\">";
                    ostr << dt;
                    ostr << "</p></body></html>";
                }
        };
    }
}
#endif
