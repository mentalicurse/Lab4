#ifndef SINGLTON_H
#define SINGLTON_H

#include <string>
#include <iostream>

class Singlton {
public:
    static Singlton& Instance();
    
    void EncryptFolder(const std::string& path, const std::string& password);
    void DecryptFolder(const std::string& path, const std::string& password);

private:
    Singlton() = default;
    ~Singlton() = default;
    Singlton(const Singlton&) = delete;
    Singlton& operator=(const Singlton&) = delete;
    void ProcessFile(const std::string& filePath, bool encrypt, const std::string& password);
};

#endif // SINGLTON_H
