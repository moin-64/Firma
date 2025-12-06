#ifndef FILE_MANAGEMENT_SERVICE_H
#define FILE_MANAGEMENT_SERVICE_H

#include <string>

class FileManagementService {
public:
    FileManagementService();
    ~FileManagementService();

    bool uploadFile(const std::string& userId, const std::string& filePath);
    bool downloadFile(const std::string& userId, const std::string& fileId);
    bool deleteFile(const std::string& userId, const std::string& fileId);

private:
    // Private members
};

#endif // FILE_MANAGEMENT_SERVICE_H
