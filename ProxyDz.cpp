#include <iostream>
#include <string>
#include <map>

class File {
public:
    virtual std::string Read() = 0;
    virtual void Write(const std::string& data) = 0;
};

class RealFile : public File {
private:
    std::string filename;
    std::string content;

public:
    RealFile(const std::string& name) : filename(name) {}

    std::string Read() override {
        return content;
    }

    void Write(const std::string& data) override {
        content = data;
    }
};

class IProxy {
public:
    virtual std::string Read() = 0;
    virtual void SetPassword(const std::string& password) = 0;
    virtual std::string GetPassword() = 0;
};

class ProxyFile : public IProxy {
private:
    RealFile* realFile;
    bool hasAccess;
    std::string password;

    void CheckAccess() {
        if (!hasAccess) {
            std::cout << "Access denied." << std::endl;
            exit(1);
        }
    }

public:
    ProxyFile(const std::string& name, bool access) : hasAccess(access) {
        realFile = new RealFile(name);
    }

    std::string Read() override {
        CheckAccess();
        return realFile->Read();
    }

    void SetPassword(const std::string& newPassword) override {
        password = newPassword;
        std::cout << "Password set." << std::endl;
    }

    std::string GetPassword() override {
        return password;
    }
};

int main() {
    std::map<std::string, IProxy*> fileProxies;

    while (true) {
        std::cout << "1. Set password for file\n2. Open file\n3. Exit\n";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            std::string filename, newPassword;
            std::cout << "Enter the file name: ";
            std::cin >> filename;

            IProxy* proxy = new ProxyFile(filename, true);
            fileProxies[filename] = proxy;

            std::cout << "Enter the new password: ";
            std::cin >> newPassword;

            proxy->SetPassword(newPassword);
        }
        else if (choice == 2) {
            std::string filename, password;
            std::cout << "Enter the file name: ";
            std::cin >> filename;

            if (fileProxies.find(filename) != fileProxies.end()) {
                std::cout << "Enter the password: ";
                std::cin >> password;

                if (password == fileProxies[filename]->GetPassword()) {
                    std::cout << "File content: " << fileProxies[filename]->Read() << std::endl;
                }
                else {
                    std::cout << "Incorrect password." << std::endl;
                }
            }
            else {
                std::cout << "File not found." << std::endl;
            }
        }
        else if (choice == 3) {
            for (auto& pair : fileProxies) {
                delete pair.second;
            }
            break;
        }
    }

    return 0;
}