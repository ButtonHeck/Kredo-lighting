#pragma once

#include <cstdint>

namespace Kredo
{

class UUID
{
public:
    UUID();
    UUID(uint64_t uuid);
    UUID(const UUID&) = default;

    operator uint64_t() const;

private:
    uint64_t _uuid;
};

}

namespace std
{

template <typename T> struct hash;

template<>
struct hash<Kredo::UUID>
{
    std::size_t operator()(const Kredo::UUID& uuid) const
    {
        return uint64_t(uuid);
    }
};

}
