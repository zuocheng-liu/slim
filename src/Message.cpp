#include "Slim/Net/CGI/Message.h"

using std::pair;
using std::string;

namespace Slim {
namespace Net {
namespace CGI {

Message::Message() : 
    version(FCGI_VERSION_1), 
    appStatus(0), 
    protocolStatus(FCGI_CANT_MPX_CONN) {
}

/*
 * Interpret the FCGI Message Header.
 */
void Message::processHeader(const CGIHeader& cgiHeader) {
    type = cgiHeader.type;
    version = cgiHeader.version;

    if (version != FCGI_VERSION_1) {
        throw Exception(UNSUPPORTED_VERSION);
    }

    requestID = cgiHeader.requestIdB1;
    requestID << 8;
    requestID |= cgiHeader.requestIdB0;
    contentLength = cgiHeader.contentLengthB1;
    contentLength << 8;
    contentLength |= cgiHeader.contentLengthB0;
    paddingLength = cgiHeader.paddingLength;
}

void Message::processBeginRecord(const CGIBeginRequestBody& body) {
    flags = body.flags & 0xFF;
    keepConnection = (flags & FCGI_KEEP_CONN) != 0;
    role = (body.roleB1 & 0xFF) << 8;
    role |= body.roleB0 & 0xFF;
}
/*
void Message::setRoleParam() {
    switch(role) {
        case FCGI_RESPONDER:
            params.insert(pair<string, string>("ROLE","RESPONDER"));
            break;
        case FCGI_AUTHORIZER:
            params.insert(pair<string, string>("ROLE", "AUTHORIZER"));
            break;
        case FCGI_FILTER:
            params.insert(pair<string, string>("ROLE", "FILTER"));
            break;
        default :
            throw Exception(FCGI_ROLE_ERROR);
    }
}
*/
void Message::processParams(ByteArray& content, map<string, string>& params) {
    int nameLen,valueLen;
    int pos = 0;
    char* name = NULL;
    char* value = NULL;
    BYTE* contentPtr = const_cast<BYTE*>(content.c_str());
    int contentLength = content.length();
    while (pos < contentLength) {
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
        name = (char*)(contentPtr + pos);
        pos += nameLen;
        value = (char*)(contentPtr + pos);
        pos += valueLen;
        params.insert(pair<string, string>(string(name, nameLen), string(value, valueLen)));
    } // end while
}


void Message::processStdIn(ByteArray& content, string& in) {
    int len = content.length();
    in.resize(len);
    char* p = (char*)const_cast<BYTE*>(content.c_str());
    in.assign(p, len);
}

void Message::makeHeader(CGIHeader &header) {
    header.version = FCGI_VERSION_1;
    header.type = (BYTE)type;
    header.requestIdB1 = (BYTE)((requestID >> 8) & 0xff);
    header.requestIdB0 = (BYTE)(requestID & 0xff);
    header.contentLengthB1 = (BYTE)((contentLength >> 8) & 0xff);
    header.contentLengthB0 = (BYTE)(contentLength & 0xff);
    header.paddingLength = (BYTE)paddingLength;
    header.reserved =  0;  //reserved byte
}

void Message::makeEndRequestBody(CGIEndRequestBody& body) {
    body.appStatusB3 = (BYTE)((appStatus >> 24) & 0xff);
    body.appStatusB2 = (BYTE)((appStatus >> 16) & 0xff);
    body.appStatusB1 = (BYTE)((appStatus >>  8) & 0xff);
    body.appStatusB0 = (BYTE)((appStatus      ) & 0xff);
    body.protocolStatus = (BYTE)((protocolStatus) & 0xff);
}

void Message::makeNameValue(const char* name, const char* value, BYTE* dest, int& pos) {
    string nameString = string(name);
    string valueString = string(value);
    makeNameValue(nameString, valueString, dest, pos);
}

void Message::makeNameValue(string& name, string& value, BYTE* dest, int& pos) {
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

    name.copy((char*)(dest + pos), nameLen);
    pos += nameLen;
    value.copy((char*)(dest + pos), valLen);
    pos += valLen;
}

void Message::makeUnknownTypeBody(CGIUnknownTypeBody& body, int unknownType) {
    body.type = (BYTE)unknownType;
}

void Message::makeStdOutBody(ByteArray& ba) {
    BYTE padding[8];
    CGIHeader header;
    makeHeader(header);
    ba.assign((BYTE*)&header, FCGI_HEADER_LEN);
    ba.append(data);
    ba.append(padding, paddingLength);
}

void Message::makeStdErrBody(ByteArray& ba) {
    BYTE padding[8];
    CGIHeader header;
    makeHeader(header);
    ba.assign((BYTE*)&header, FCGI_HEADER_LEN);
    ba.append(data);
    ba.append(padding, paddingLength);
}

} // end namespace CGI
} // end namespace Net
} // end namespace Slim

