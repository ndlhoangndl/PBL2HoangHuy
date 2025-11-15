//
// Created by Admin on 11/15/2025.
//

#ifndef PBL2_HASH_H
#define PBL2_HASH_H

#pragma once

#include <sha.h>
#include <filters.h>
#include <hex.h>

inline std::string sha256(const std::string& input) {
    using namespace CryptoPP;

    std::string hash, hex;

    SHA256 sha256;
    StringSource(input, true,
        new HashFilter(sha256,
            new StringSink(hash)
        )
    );

    StringSource(hash, true,
        new HexEncoder(
            new StringSink(hex),
            false
        )
    );

    return hex;
}

#endif //PBL2_HASH_H