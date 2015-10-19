#ifndef SLIM_NET_FCGI_DEFINITION_H 
#define SLIM_NET_FCGI_DEFINITION_H 

#define FCGI_MAX_CONNS  "FCGI_MAX_CONNS"
#define FCGI_MAX_REQS   "FCGI_MAX_REQS"
#define FCGI_MPXS_CONNS "FCGI_MPXS_CONNS"
namespace Slim {
namespace Net {
namespace FCGI {
    enum BufferSize {
        BUFFER_SIZE = 8192
    };
    /*
     * Define Length of FCGI message bodies in bytes
     */
    enum MessageLength {
        FCGI_HEADER_LEN = 8,
        FCGI_END_REQ_BODY_LEN = 8,
        FCGI_BEGIN_REQ_BODY_LEN = 8,
        FCGI_UNKOWN_BODY_TYPE_BODY_LEN = 8,
        FCGI_MAX_LEN = 0xffff
    };

    enum Version {
        FCGI_VERSION_1 = 1
    };

    /**
     * Values for type component of FCGI_Header
     */
    enum HeaderType {
        FCGI_BEGIN_REQUEST = 1,
        FCGI_ABORT_REQUEST = 2,
        FCGI_END_REQUEST = 3,

        FCGI_PARAMS = 4,
        FCGI_STDIN = 5,
        FCGI_STDOUT = 6,
        FCGI_STDERR = 7,
        FCGI_DATA = 8,

        /* Manage Record */
        FCGI_GET_VALUES  = 9,
        FCGI_GET_VALUES_RESULT = 10,

        FCGI_UNKNOWN_TYPE = 11,
        FCGI_MAXTYPE = 12

    };

    /*
     *  Value for requestId component of FCGI_Header
     */
    enum HeaderRequestId {
        FCGI_NULL_REQUEST_ID = 0
    };
    /*
     * Values for role component of FCGI_BeginRequestBody
     */
    enum BeginRequestBodyRole {
        FCGI_RESPONDER = 1,
        FCGI_AUTHORIZER = 2,
        FCGI_FILTER = 3
    };

    /*
     * Mask for flags component of FCGI_BeginRequestBody 
     */
    enum BeginRequestBodyFlag {
        FCGI_KEEP_CONN = 1
    };

    /*
     * Values for protocolStatus component of FCGI_EndRequestBody
     */
    enum ProtocolStatus {
        FCGI_REQUEST_COMPLETE = 0,
        FCGI_CANT_MPX_CONN = 1,
        FCGI_OVERLOADED = 2,
        FCGI_UNKNOWN_ROLE = 3
    };

}
}
}


