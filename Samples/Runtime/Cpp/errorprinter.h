// Copyright (C) 2021 Thales Group. All rights reserved.

#ifndef ERRORPRINTER_H
#define ERRORPRINTER_H

#include <map>

class ErrorPrinter
{
public:
    ErrorPrinter();
    void printError(haspStatus status);
    const char* getError(haspStatus status);
    typedef std::map<haspStatus, const char*> StatusMap;

protected:
    StatusMap errorMap;
};

#endif // ERRORPRINTER_H
