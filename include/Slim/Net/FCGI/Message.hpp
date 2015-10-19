#ifndef SLIM_NET_FCGI_MESSAGE_H
#define SLIM_NET_FCGI_MESSAGE_H


#include "Definition.hpp"
#include "Exception.hpp"
#include <string>
#include <hash_map>
#include <cstring>
#include <fstream>
using std::hash_map;
using std::pair;
using std::string;
using std::EOF;

namespace Slim {
namespace Net {
namespace FCGI {
    typedef unsigned char BYTE;
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
            Message () :
                version(FCGI_VERSION_1),
                appStatus(0),
                protocolStatus(FCGI_CANT_MPX_CONN) {
                }
            /*
             * Interpret the FCGI Message Header. Processes FCGI
             * BeginRequest and Management messages. Param hdr is the header.
             * The calling routine has to keep track of the stream reading
             * management or use FCGIInputStream.fill() which does just that.
             */
            public int processHeader(BYTE* hdr) {
                processHeaderBytes(hdr);

                /*
                   if (requestID != in.request.requestID) {
                   return(FCGIGlobalDefs.def_FCGISkip);
                   }
                   if (type != in.type) {
                   throw Exception(ProtocolError);
                   }
                   */
                return(FCGIGlobalDefs.def_FCGIStreamRecord);
            }

            /*
             * parse FCGI Begin Request Record.
             */
            void processBeginRecord(CGIBeginRequestBody& body) {
                if (requestID == 0 ||
                        contentLen != FCGI_BEGIN_REQ_BODY_LEN) {
                    throw Exception(FCGI_PROTOCOL_ERROR);
                }

                flags = body->flags & 0xFF;
                keepConnection = (flags & FCGI_KEEP_CONN) != 0;
                role = (body->roleB1 & 0xFF) << 8;
                role |= body->roleB0 & 0xFF;
            }

            void processHeaderBytes(const BYTE* const hdrBuf){
                CGIHeader* cgiHeader = (CGIHeader*)hdrBuff;
                type = cgiHeader->type;
                version = cgiHeader->version;

                if (version != FCGI_VERSION_1) {
                    throw Exception(UnsupportedVersion);
                }

                requestID = cgiHeader->requestIdB1;
                requestID << 8;
                requestID |= cgiHeader->requestIdB0;
                contentLength = cgiHeader->contentLengthB1;
                contentLength << 8;
                contentLength |= cgiHeader->contentLengthB0;
                paddingLength = cgiHeader->paddingLength;
            }

            void setRoleParam() {
                switch(role) {
                    case FCGI_RESPONDER:
                        params.insert(pair<"ROLE","RESPONDER">);
                        break;
                    case FCGI_AUTHORIZER:
                        params.insert(pair<"ROLE", "AUTHORIZER">);
                        break;
                    case FCGI_FILTER:
                        params.insert(pair<"ROLE", "FILTER">);
                        break;
                    default :
                        throw Exception(FCGI_ROLE_ERROR);
                }
            }

            /*
             * Read FCGI name-value pairs from a stream until EOF. Put them
             * into a map object, storing both as strings.
             */
            int processParams(BYTE* content) {
                int nameLen,valueLen;
                int pos = 0;
                BYTE* name = NULL;
                BYTE* value = NULL;
                while (content[pos] != EOF) {
                    /*
                     * receive name length
                     */
                    nameLen = content[pos++];
                    if ((nameLen & 0x80) != 0) {
                        nameLen = ((nameLen & 0x7F)    << 24)
                            | ((content[pos++] & 0xFF) << 16)
                            | ((content[pos++] & 0xFF) << 8)
                            | ( content[pos++] & 0xFF);
                    }

                    /*
                     * receive value length
                     */
                    valueLen = content[pos++];
                    if ((valueLen & 0x80) != 0) {
                        valueLen = ((valueLen & 0x7f)  << 24)
                            | ((content[pos++] & 0xFF) << 16)
                            | ((content[pos++] & 0xFF) << 8)
                            | ( content[pos++] & 0xFF);
                    }

                    /*
                     * nameLen and valueLen are now valid; read the name
                     * and the value from the stream and construct a standard
                     * environmental entity
                     */
                    name = content + pos;
                    pos += nameLen;
                    value = content + pos;
                    pos += valueLen;
                    params.insert(
                            pair<string(name, nameLen), string(value, valueLen)>);
                } // end while
            }

            /*
             * Build an FCGI Message Header
             */
            void makeHeader(CGIHeader &header) {
                header.version = FCGI_VERSION_1;
                header.type = (BYTE)type;
                header.requestIdB1 = (BYTE)((requestID >> 8) & 0xff);
                header.requestIdB0 = (BYTE)(requestID & 0xff);
                header.contentLengthB1 = (BYTE)((contentLength >> 8) & 0xff);
                header.contentLengthB0 = (BYTE)(contentLength & 0xff);
                header.paddingLength = (BYTE)paddingLength;
                header.reserved =  0;  //reserved byte
            }

            /*
             * Build an FCGI Message UnknownTypeBodyBody
             */
            void makeEndRequestBody(CGIEndRequestBody& body) {
                body.appStatusB3 = (BYTE)((appStatus >> 24) & 0xff);
                body.appStatusB2 = (BYTE)((appStatus >> 16) & 0xff);
                body.appStatusB1 = (BYTE)((appStatus >>  8) & 0xff);
                body.appStatusB0 = (BYTE)((appStatus      ) & 0xff);
                body.protocolStatus = (BYTE)((protocolStatus) & 0xff);
            }

            /*
             * Build a name-value body
             */
            void makeNameValue(
                    string& name,
                    string& value,
                    BYTE* dest,
                    int& pos) {
                int nameLen = name.length();
                if (nameLen < 0x80) {
                    dest[pos++] = (BYTE)nameLen;
                }else {
                    dest[pos++] = (BYTE)(((nameLen >> 24) | 0x80) & 0xff);
                    dest[pos++] = (BYTE)((nameLen >> 16) & 0xff);
                    dest[pos++] = (BYTE)((nameLen >> 8) & 0xff);
                    dest[pos++] = (BYTE)nameLen;
                }
                int valLen = value.length();
                if (valLen < 0x80) {
                    dest[pos++] =  (BYTE)valLen;
                }else {
                    dest[pos++] = (BYTE)(((valLen >> 24) | 0x80) & 0xff);
                    dest[pos++] = (BYTE)((valLen >> 16) & 0xff);
                    dest[pos++] = (BYTE)((valLen >> 8) & 0xff);
                    dest[pos++] = (BYTE)valLen;
                }

                name.copy(dest + pos, nameLen);
                pos += nameLen;
                value.copy(dest + pos, valLen);
                pos += valLen;
            }

            /*
             * Build an FCGI Message UnknownTypeBody
             */
            void makeUnknownTypeBody(CGIUnknownTypeBody& body, int unknownType){
                body.type = (BYTE)unknownType;
            }

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

            /*
             * environment parameters
             */
            hash_map<string, string> params;
    };
}
}
}

#endif
