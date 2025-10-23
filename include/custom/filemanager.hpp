#pragma once
#include "files/main_files.h"
namespace FileManagementLib {
    bool fileExists(std::string path) {
        return FILE_MAPPING.contains(path);
    }
    EMBEDDED_FILE_STRUCT getFile(std::string path) {
        return FILE_MAPPING[path];
    }
    std::string readFileText(std::string path) {
        EMBEDDED_FILE_STRUCT file_data = getFile(path);
        std::string my_string(reinterpret_cast<const char*>(file_data.data), file_data.size);
        return my_string;
    }
    uint8_t readFileContent(std::string path) {
        EMBEDDED_FILE_STRUCT file_data = getFile(path);
        return *file_data.data;
    }

}