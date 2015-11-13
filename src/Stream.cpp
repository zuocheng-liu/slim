#include "Slim/Net/CGI/Stream.h"
namespace Slim {
namespace Net {
namespace CGI {

InputStreamBuffer::InputStreamBuffer(StreamSocket& socket) : _socket(socket) {
    setg(_buffer, _buffer, _buffer);
}

InputStreamBuffer::~InputStreamBuffer() {
}

int_type InputStreamBuffer::underflow() {
    int len = _socket.receiveBytes(_buffer, BUFFER_SIZE);
    if (len <= 0) {
        return -1;
    }
    setg(_buffer, _buffer, _buffer + len);
    return *gptr();
}

// InputStreamBuffer ends

OutputStreamBuffer::OutputStreamBuffer(StreamSocket& socket) : _socket(socket) {
    setp(_buffer, _buffer + BUFFER_SIZE);
}

OutputStreamBuffer::~OutputStreamBuffer() {
    sync();
}

int_type OutputStreamBuffer::overflow(int_type c) {
    if (flushBuffer() < 0) {
        return -1;
    }
    return c;
}

int OutputStreamBuffer::sync() {
    if (flushBuffer() < 0) {
        return -1;
    }
    return 0;
}

int OutputStreamBuffer::flushBuffer() {
    int len = pptr() - pbase();
    int count = _socket.sendBytes(_buffer, len);
    if (count <= 0) {
        return -1;
    }
    pbump(-len);
    return len;
}

// OutputStreamBuffer ends

InputStream::InputStream(InputStreamBuffer& buf) : byte_istream(&buf) {
}

InputStream::InputStream(InputStreamBuffer* buf) : byte_istream(buf) {
}


InputStream::~InputStream() {
}

InputStream& InputStream::getline(ByteArray& s, std::streamsize n) {
    char_type* p = const_cast<char_type *>(s.c_str());
    return getline(p, n);
}

InputStream& InputStream::getline(char_type* s, std::streamsize n) {
    byte_istream::getline(s, n);
    return *this;
}



// InputStream ends

OutputStream::OutputStream(OutputStreamBuffer& buf) : byte_ostream(&buf) {
}

OutputStream::OutputStream(OutputStreamBuffer* buf) : byte_ostream(buf) {
}

OutputStream::~OutputStream() {
}

OutputStream& OutputStream::write(const ByteArray& s, std::streamsize n) {
    char_type* p = const_cast<char_type *>(s.c_str());
    int size = s.size();
    std::streamsize len = size < n ? size : n;
    return write(p, len);
}

OutputStream& OutputStream::write(const char_type* s, std::streamsize n) {
    byte_ostream::write(s, n);
    return *this;
}


// OutputStream ends

} // end namespace CGI
} // end namespace Net
} // end namespace Slim
