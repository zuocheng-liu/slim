#ifndef SLIM_NET_FCGI_MESSAGE_H
#define SLIM_NET_FCGI_MESSAGE_H

#include <string>
#include <hash_map>

#include "Definition.hpp"
#include "Exception.hpp"
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
                    /*
                     * Interpret the FCGI Message Header. Processes FCGI
                     * BeginRequest and Management messages. Param hdr is the header.
                     * The calling routine has to keep track of the stream reading
                     * management or use FCGIInputStream.fill() which does just that.
                     */
                    public int processHeader(BYTE* hdr) {
                        processHeaderBytes(hdr);
                        if (version != FCGI_VERSION_1) {
                            throw Exception(UnsupportedVersion);
                        }
                        //in.contentLen = h_contentLength;
                        //in.paddingLen = h_paddingLength;
                        if (FCGI_BEGIN_REQUEST == type) {
                            return processBeginRecord();
                        }
                        if (FCGI_NULL_REQUEST_ID == requestID) {
                            return processManagementRecord();
                        }
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
                     * Reads FCGI Begin Request Record.
                     */
                    public int processBeginRecord() {
                        byte beginReqBody[];
                        byte endReqMsg[];
                        if (requestID == 0 || in.contentLen
                                != FCGIGlobalDefs.def_FCGIEndReqBodyLen) {
                            return FCGIGlobalDefs.def_FCGIProtocolError;
                        }
                        /*
                         * If the webserver is multiplexing the connection,
                         * this library can't deal with it, so repond with
                         * FCGIEndReq message with protocolStatus FCGICantMpxConn
                         */
                        if (isBeginProcessed) {
                            endReqMsg = new byte[FCGIGlobalDefs.def_FCGIHeaderLen
                                + FCGIGlobalDefs.def_FCGIEndReqBodyLen];
                            System.arraycopy(makeHeader(
                                        FCGIGlobalDefs.def_FCGIEndRequest,
                                        requestID,
                                        FCGIGlobalDefs.def_FCGIEndReqBodyLen,
                                        0), 0,  endReqMsg, 0,
                                    FCGIGlobalDefs.def_FCGIHeaderLen);
                            System.arraycopy(makeEndrequestBody(0,
                                        FCGIGlobalDefs.def_FCGICantMpxConn), 0,
                                    endReqMsg,
                                    FCGIGlobalDefs.def_FCGIHeaderLen,
                                    FCGIGlobalDefs.def_FCGIEndReqBodyLen);
                            /*
                             * since isBeginProcessed is first set below,this
                             * can't be out first call, so request.out is properly set
                             */
                            try {
                                in.request.outStream.write(endReqMsg, 0,
                                        FCGIGlobalDefs.def_FCGIHeaderLen
                                        + FCGIGlobalDefs.def_FCGIEndReqBodyLen);
                            } catch (IOException e){
                                in.request.outStream.setException(e);
                                return -1;
                            }
                        }
                        /*
                         * Accept this  new request. Read the record body
                         */
                        in.request.requestID = requestID;
                        beginReqBody =
                            new byte[FCGIGlobalDefs.def_FCGIBeginReqBodyLen];
                        if (in.read(beginReqBody, 0,
                                    FCGIGlobalDefs.def_FCGIBeginReqBodyLen) !=
                                FCGIGlobalDefs.def_FCGIBeginReqBodyLen) {
                            return FCGIGlobalDefs.def_FCGIProtocolError;
                        }
                        br_flags = beginReqBody[2] & 0xFF;
                        in.request.keepConnection
                            = (br_flags & FCGIGlobalDefs.def_FCGIKeepConn) != 0;
                        br_role = ((beginReqBody[0] & 0xFF) << 8) | (beginReqBody[1] & 0xFF);
                        in.request.role = br_role;
                        in.request.isBeginProcessed = true;
                        return FCGIGlobalDefs.def_FCGIBeginRecord;
                    }
                    
                    void readParams() {
                    }

                    void clear() {
                        isBeginProcessed = false;
                    }
                private :
                    void processHeaderBytes(const BYTE* const hdrBuf){
                        CGIHeader* cgiHeader;
                        cgiHeader = (CGIHeader*)hdrBuff;
                        type = cgiHeader->type;
                        version = cgiHeader->version;
                        requestID = cgiHeader->requestIdB1;
                        requestID << 8;
                        requestID |= cgiHeader->requestIdB0;
                        contentLength = cgiHeader->contentLengthB1;
                        contentLength << 8;
                        contentLength |= cgiHeader->contentLengthB0;
                        paddingLength = cgiHeader->paddingLength;
                    }

                    /*
                     * FCGI Header
                     */
                    int version;
                    int type;
                    int requestID; // 2 bytes
                    int contentLength; // 2 bytes
                    int paddingLength;
                    bool isBeginProcessed;

                    /*
                     * FCGI BeginRequest body.
                     */
                    int  role; // 2 bytes
                    int  flags;

                    /*
                     * Web server connection
                     */
                    bool  keepConnection;
                    std::hash_map<std::string, std::string> params;
            };
        }
    }
}

#endif
