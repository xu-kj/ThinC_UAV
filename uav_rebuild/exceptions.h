#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

/* Generic error class */
class Error {
public:
    Error(const char * const msg_) : msg(msg_) {}
    const char * const msg;
};

#endif /* EXCEPTIONS_H */