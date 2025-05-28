#include "../include/Singlton.h"
#include "../include/FileSearcher.h"
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>      // для AutoSeededRandomPool
#include <cryptopp/pwdbased.h>   // для PKCS5_PBKDF2_HMAC
#include <cryptopp/sha.h>       // для SHA256
#include <fstream>
#include <filesystem> // Для работы с путями файлов

using namespace CryptoPP;

namespace fs = std::filesystem;

void Singlton::ProcessFile(const std::string& filePath, bool encrypt, const std::string& password) {
    try {
        // Читаем исходный файл
        std::ifstream in(filePath, std::ios::binary);
        if (!in) throw std::runtime_error("Cannot open input file");
        std::string content((std::istreambuf_iterator<char>(in)), 
                          std::istreambuf_iterator<char>());
        in.close();

        // Генерация ключа
        byte key[AES::DEFAULT_KEYLENGTH], iv[AES::BLOCKSIZE];
        PKCS5_PBKDF2_HMAC<SHA256> pbkdf;
        pbkdf.DeriveKey(key, sizeof(key), 0, 
                       (byte*)password.data(), password.size(), 
                       (byte*)"", 0, 10000);

        std::string processedData;
        
        if (encrypt) {
            // Шифрование
            AutoSeededRandomPool prng;
            prng.GenerateBlock(iv, sizeof(iv));

            CBC_Mode<AES>::Encryption encryptor(key, sizeof(key), iv);
            StringSource(content, true,
                new StreamTransformationFilter(encryptor,
                    new StringSink(processedData)
                )
            );
            
            // Добавляем IV в начало
            processedData.insert(0, (char*)iv, AES::BLOCKSIZE);
        } else {
            // Дешифрование
            if (content.size() < AES::BLOCKSIZE) 
                throw std::runtime_error("Invalid file format, because missing IV");

            byte fileIv[AES::BLOCKSIZE];
            memcpy(fileIv, content.data(), AES::BLOCKSIZE);
            content.erase(0, AES::BLOCKSIZE);

            CBC_Mode<AES>::Decryption decryptor(key, sizeof(key), fileIv);
            StringSource(content, true,
                new StreamTransformationFilter(decryptor,
                    new StringSink(processedData)
                )
            );
        }

        // Создаем временный файл
        std::string tempPath = filePath + ".tmp";
        {
            std::ofstream out(tempPath, std::ios::binary);
            if (!out) throw std::runtime_error("Cannot create temp file");
            out << processedData;
        }

        // Заменяем оригинальный файл временным
        fs::rename(tempPath, filePath);

    } catch (const std::exception& e) {
        // Удаляем временный файл при ошибке
        if (fs::exists(filePath + ".tmp")) {
            fs::remove(filePath + ".tmp");
        }
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
}

void Singlton::EncryptFolder(const std::string& path, const std::string& password) {
    auto files = FileSearcher::GetAllFiles(path); 
    for (const auto& file : files) {
        ProcessFile(file, true, password);
    }
}

void Singlton::DecryptFolder(const std::string& path, const std::string& password) {
    auto files = FileSearcher::GetAllFiles(path);
    for (const auto& file : files) {
        if (file.size() > 4) {
            ProcessFile(file, false, password);
        }
    }
}

Singlton& Singlton::Instance() {
    static Singlton instance;
    return instance;
}
