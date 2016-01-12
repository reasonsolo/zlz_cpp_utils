//
// Created by zlz on 2016/1/11.
//

#ifndef ZUTILS_FILE_H
#define ZUTILS_FILE_H

#include "common.h"
#include <cstdio>

ZUTIL_NAMESPACE_BEGIN

    class File: private NoCopy {
    public:

        File(const string& filename);
        virtual ~File();
        void Touch();

        string filename() const;
    private:
        /*
         * a file must has a name
         */
        File();
        virtual void Open();
        virtual void Close();

        string filename_;
    };


    class ReadableFile: public File {
    public:
        /*
         * read from file for a max length (all if len = 0)
         * return size of bytes read
         */
        size_t Read(string& buf, size_t len = 0);
    };

    class WritableFile: public ReadableFile {
    public:
        WritableFile(const string& filename, bool append = false);

        size_t Write(string& buf);

    };

ZUTIL_NAMESPACE_END

#endif //ZUTILS_FILE_H
