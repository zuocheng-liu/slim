#ifndef SLIM_NET_HTTPREQUEST_HPP
#define SLIM_NET_HTTPREQUEST_HPP

#include "Poco/Net/HTTPServerRequest.h"
#include <map>
namespace Slim {
    namespace Net {
        class HTTPRequest: public Poco::Net::HTTPServerRequest {
            public :
                HTTPRequest(const Poco::Net::HTTPServerRequest& request) {
                }

                inline std::string getQuery() {
                    return query;
                }
                
                inline std::string getQuery(string key) {
                    return query[key];
                }

                inline std::string getRequest() {
                    return request;
                }

                inline std::string getRequest(string key) {
                    return request[key];
                }

            private :
                std::map<string, string> query;
                std::map<string, string> request;  
        };
    }
}
#endif
