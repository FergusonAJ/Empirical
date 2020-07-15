/**
 *  @note This file is part of Empirical, https://github.com/devosoft/Empirical
 *  @copyright Copyright (C) Michigan State University, MIT Software license; see doc/LICENSE.md
 *  @date 2019-2020
 *
 *  @file  hash_utils.h
 *  @brief This file provides tools for hashing values and containers.
 *  @note Status: BETA
 */

#ifndef EMP_HASH_UTILS_H
#define EMP_HASH_UTILS_H

#include <type_traits>
#include <cstddef>
#include <cstring>
#include <stdint.h>

#include "../../third-party/span-lite/include/nonstd/span.hpp"

// alias span-lite's nonstd::span to std::span
// this is done to ease transition to C++20 spans at a later point
namespace std {
  template <typename ...Args>
  using span = nonstd::span<Args...>;
}

namespace emp {

  /// Generate a unique long from a pair of ints.
  /// @param a First 32-bit unsigned int.
  /// @param b Second 32-bit unsigned int.
  /// @return 64-bit unsigned int representing the szudzik hash of both inputs.
  uint64_t szudzik_hash(uint32_t a_, uint32_t b_)
  {
    uint64_t a = a_, b = b_;
    return a >= b ? a * a + a + b : a + b * b;
  }
  /// Boost's implementation of a simple hash-combining function.
  /// Taken from https://www.boost.org/doc/libs/1_37_0/doc/html/hash/reference.html#boost.hash_combine
  /// @param hash1 First hash to combine.
  /// @param hash2 Second hash to combine.
  /// @return Combined hash.
  constexpr inline std::size_t hash_combine(std::size_t hash1, std::size_t hash2)
  {
    return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
  }

  // helper functions for murmur hash
  namespace internal {
    constexpr uint64_t rotate(const size_t x, const size_t r) {
      return (x << r) | (x >> (64 - r));
    }
    constexpr inline void fmix64(uint64_t& k) {
      k ^= k >> 33;
      k *= 0xff51afd7ed558ccd;
      k ^= k >> 33;
      k *= 0xc4ceb9fe1a85ec53;
      k ^= k >> 33;
    }
  }

  /// Implementation of the murmur3 hash, a fast hash with low collisions.
  /// This hash makes it suitable for hash-based lookups.
  /// For more info, see: https://en.wikipedia.org/wiki/MurmurHash
  /// This implementation was directly based on:
  /// https://github.com/aappleby/smhasher/blob/92cf3702fcfaadc84eb7bef59825a23e0cd84f56/src/MurmurHash3.cpp
  /// @param key Span of bytes to hash.
  /// @param seed Optional seed.
  /// @return Hash of key.
  constexpr size_t murmur_hash(
    const std::span<const std::byte> key,
    const size_t seed = 0
  ) {
    // define constants
    const size_t numbytes = key.size();
    const size_t nblocks = numbytes / 16;
    const uint64_t c1 = 0x87c37b91114253d5LLU;
    const uint64_t c2 = 0x4cf5ad432745937fLLU;

    // initialize seeds
    uint64_t h1 = seed;
    uint64_t h2 = seed;

    // helper lambda for reading std::bytes into uint64_ts
    const auto get_ints = [&key](const size_t idx) {
      uint64_t ret{0};
      std::memcpy(&ret, key.data() + idx * sizeof(uint64_t), sizeof(uint64_t));
      return ret;
    };

    // main algorithm loop
    // does not run if length < 16
    for (size_t i = 0; i < nblocks; i++) {
      uint64_t k1 = get_ints(2 * i);
      uint64_t k2 = get_ints(2 * i + 1);

      k1 *= c1;
      k1 = internal::rotate(k1, 31);
      k1 *= c2;
      h1 ^= k1;

      h1 = internal::rotate(h1, 27);
      h1 += h2;
      h1 = 5 * h1 + 0x52dce729;

      k2 *= c2;
      k2 = internal::rotate(k2, 33);
      k2 *= c1;
      h2 ^= k2;

      h2 = internal::rotate(h2, 31);
      h2 += h1;
      h2 = 5 * h2 + 0x38495ab5;
    }
    // tail of algorithm
    uint64_t k1 = 0;
    uint64_t k2 = 0;

    // helper lambda for reading std::bytes as uint64_ts and then shifting them left
    const auto do_magic = [&key, nblocks](const size_t a, const size_t b) {
      uint64_t tail_{0};
      std::memcpy(&tail_, key.data() + nblocks*16 + a, sizeof(uint8_t));
      return tail_ << b;
    };

    switch(numbytes & 15) {
      case 15: k2 ^= do_magic(14, 48);
      case 14: k2 ^= do_magic(13, 40);
      case 13: k2 ^= do_magic(12, 32);
      case 12: k2 ^= do_magic(11, 24);
      case 11: k2 ^= do_magic(10, 16);
      case 10: k2 ^= do_magic(9, 8);
      case  9: k2 ^= do_magic(8, 0);
               k2 *= c2;
               k2  = internal::rotate(k2,33);
               k2 *= c1;
               h2 ^= k2;
      case  8: k1 ^= do_magic(7, 56);
      case  7: k1 ^= do_magic(6, 48);
      case  6: k1 ^= do_magic(5, 40);
      case  5: k1 ^= do_magic(4, 32);
      case  4: k1 ^= do_magic(3, 24);
      case  3: k1 ^= do_magic(2, 16);
      case  2: k1 ^= do_magic(1, 8);
      case  1: k1 ^= do_magic(0, 0);
               k1 *= c1;
               k1  = internal::rotate(k1,31);
               k1 *= c2;
               h1 ^= k1;
    };

    // finalization
    h1 ^= numbytes;
    h2 ^= numbytes;

    h1 += h2;
    h2 += h1;

    internal::fmix64(h1);
    internal::fmix64(h2);

    h1 += h2;

    return h1;
  }

  /// This structure serves as a hash for containers that are iterable.
  /// Use as a drop-in replacement for std::hash.
  template <typename Container, size_t Seed = 0>
  struct ContainerHash
  {
    size_t operator()(const Container& v) const {
        size_t data = Seed;
        for (const auto& item : v) {
            using item_type = typename std::decay<decltype(item)>::type;
            const std::hash<item_type> hasher;
            data = hash_combine(data, hasher(item));
        }
        return data;
    }
  };
}

#endif
