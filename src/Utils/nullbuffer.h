#ifndef NULLBUFFER_H
#define NULLBUFFER_H

#include <streambuf>

/* A buffer which doesn't print anything. */
class NullBuffer : public std::streambuf
{
  public:
    int overflow(int c) { return c; }
};

#endif // NULLBUFFER_H
