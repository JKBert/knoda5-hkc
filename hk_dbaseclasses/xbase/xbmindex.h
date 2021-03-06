#ifndef xbMultiIndex_h
#define xbMultiIndex_h

#include "xbase64.h"

class xbMultiIndex: public xbIndex
{
  public:
    xbMultiIndex(xbDbf* db): xbIndex(db) {}
    virtual ~xbMultiIndex() {}
    virtual xbShort CreateIndex(const char * filename, const char* tag, 
         const char* expr, xbShort unique, xbShort overwrite)=0;
};
#endif
