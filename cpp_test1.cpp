#include <openssl/sha.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
std::string sha256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);
    std::ostringstream oss;
    for (unsigned char c : hash) {
        oss << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(c);
    }
    return oss.str();
}
int main() {
    std::string text = "2026-03-14-Jared-02";
    std::cout << sha256(text) << '\n';
    return 0;
}
