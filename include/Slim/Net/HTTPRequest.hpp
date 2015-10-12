#ifndef SLIM_NET_HTTPREQUEST_HPP
#define SLIM_NET_HTTPREQUEST_HPP

#include <map>
#include <Poco/Net/HTTPServerRequest.h>
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
                
                inline std::string getCookie() {
                    return cookie;
                }

                inline std::string getCookie(string key) {
                    return cookie[key];
                }
                
                inline std::string getSession() {
                    return session;
                }

                inline std::string getSession(string key) {
                    return session[key];
                }

            private :
                std::map<string, string> query;
                std::map<string, string> request;  
                std::map<string, string> cookie;  
                std::map<string, string> session;  
        };
    }
}
#endif
