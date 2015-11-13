#ifndef SLIM_NET_CGI_MESSAGE_H
#define SLIM_NET_CGI_MESSAGE_H

#include "Slim/Net/CGI/Definition.h"
#include "Slim/Net/CGI/Exception.h"
#include <map>
#include <string>

using std::map;
using std::string;

namespace Slim {
namespace Net {
namespace CGI {

typedef struct {
    BYTE version;
    BYTE type;
    BYTE requestIdB1;
    BYTE requestIdB0;
    BYTE contentLengthB1;
    BYTE contentLengthB0;
    BYTE paddingLength;
    BYTE reserved;
} CGIHeader;

typedef struct {
    BYTE roleB1;
    BYTE roleB0;
    BYTE flags;
    BYTE reserved[5];
} CGIBeginRequestBody;

typedef struct {
    CGIHeader header;
    CGIBeginRequestBody body;
} CGIBeginRequestRecord;

typedef struct {
    BYTE appStatusB3;
    BYTE appStatusB2;
    BYTE appStatusB1;
    BYTE appStatusB0;
    BYTE protocolStatus;
    BYTE reserved[3];
} CGIEndRequestBody;

typedef struct {
    CGIHeader header;
    CGIEndRequestBody body;
} CGIEndRequestRecord;

typedef struct {
    BYTE type;
    BYTE reserved[7];
} CGIUnknownTypeBody;

typedef struct {
    CGIHeader header;
    CGIUnknownTypeBody body;
} CGIUnknownTypeRecord;


/* This class handles reading and building the fastcgi messages.
 * For reading incoming mesages, we pass the input
 * stream as a param to the constructor rather than to each method.
 * Methods that build messages use and return internal buffers, so they
 * dont need a stream.
 */
class Message {

    public :
        Message ();
        void clear();

        /*
         *
         */
        void process(const BYTE* const buffer);
        /*
         * parse FCGI Begin Request Record.
         */
        void processBeginRecord(const CGIBeginRequestBody& body);

        /*
         * Interpret the FCGI Message Header.
         */
        void processHeader(const CGIHeader& cgiHeader);
        
        /*
         * Read FCGI name-value pairs from a stream until EOF. Put them
         * into a map object, storing both as strings.
         */
        void processParams(ByteArray& content, map<string, string>& params);
        
        void processStdIn(ByteArray& content, string& params);

        /*
         * Build an FCGI Message Header
         */
        void makeHeader(CGIHeader &header);

        /*
         * Build an FCGI Message UnknownTypeBodyBody
         */
        void makeEndRequestBody(CGIEndRequestBody& body);

        /*
         * Build a name-value body
         */
        void makeNameValue(const char* name, const char* value, BYTE* dest, int& pos);
        void makeNameValue(string& name, string& value, BYTE* dest, int& pos);

        /*
         * Build a FCGI Message UnknownTypeBody
         */
        void makeUnknownTypeBody(CGIUnknownTypeBody& body, int unknownType);
        
        /*
         * Build a FCGI STDIN Body
         */
        void makeStdOutBody(ByteArray& ba);

        /*
         * Build an FCGI STDIN Body 
         */
        void makeStdErrBody(ByteArray& ba);
        /*
         * FCGI Header
         */
        int version;
        int type;
        int requestID; // 2 bytes
        int contentLength; // 2 bytes
        int paddingLength;

        /*
         * FCGI begin request body.
         */
        int  role; // 2 bytes
        int  flags;

        /*
         * FCGI end request body.
         */
        int appStatus;
        int protocolStatus;
        
        /*
         * Web server connection
         */
        bool  keepConnection;

        ByteArray data;

}; // end class Message

} // end namespace CGI
} // end namespace Net
} // end namespace Slim
#endif
