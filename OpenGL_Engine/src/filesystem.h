#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>

class FileSystem
{
public:
    // Set base path (e.g. project root or resources folder)
    static void setBasePath(const std::string& basePath)
    {
        getBasePath() = basePath;
    }

    // Get full path by prepending base path if set
    static std::string getPath(const std::string& relativePath)
    {
        if (getBasePath().empty())
            return relativePath;
        else
            return getBasePath() + "/" + relativePath;
    }

private:
    // Store base path internally
    static std::string& getBasePath()
    {
        static std::string basePath;
        return basePath;
    }
};

#endif // FILESYSTEM_H
