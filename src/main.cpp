#include "../include/Singlton.h"


int main() {
    std::string path, password;
    int choice;
    
    std::cout << "1. Encrypt folder\n2. Decrypt folder\nChoose action: ";
    std::cin >> choice;
    std::cin.ignore();
    
    std::cout << "Enter folder path: ";
    std::getline(std::cin, path);
    
    std::cout << "Enter password: ";
    std::getline(std::cin, password);
    
    switch (choice) {
        case 1:
            Singlton::Instance().EncryptFolder(path, password);
            std::cout<<"Encryption is done!";
            break;
        case 2:
            Singlton::Instance().DecryptFolder(path, password);
            std::cout<<"Decryption is done!";
            break;
        default:
            std::cerr << "Invalid case!" << std::endl;
            return 1;
    }
    
    return 0;
}
