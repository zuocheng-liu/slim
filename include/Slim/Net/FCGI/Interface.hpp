#ifndef SLIM_NET_FCGI_MESSAGE_H
#define SLIM_NET_FCGI_MESSAGE_H

#include <ostream> 
#include <istream>
#include "Request.hpp"
#include "Response.hpp"
namespace Slim {
    namespace Net {
        namespace FCGI {
            class Interface {
                public :
                    Interface() {
                    }

                    int accept() {
                        request.clear();
                        response.clear();

                        request.recv();

                    }
                private :

                    Request request;
                    Response response;
            }
        }
    }
}
#endif
