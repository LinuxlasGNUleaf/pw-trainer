#ifndef SHA256_H
#define SHA256_H

#include "iostream"
#include "iomanip"
#include "sstream"
#include "string"
#include "openssl/sha.h"

#include "QString"

QString sha256(const std::string);
#endif // SHA256_H
