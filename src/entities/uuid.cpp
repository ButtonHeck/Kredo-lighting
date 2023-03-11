#include "uuid.h"

#include <random>

namespace Kredo
{

static std::random_device randomDevice;
static std::mt19937_64 engine(randomDevice());
static std::uniform_int_distribution<uint64_t> uniformDistribution;

UUID::UUID()
    : _uuid(uniformDistribution(engine))
{
}

UUID::UUID(uint64_t uuid)
    : _uuid(uuid)
{
}

UUID::operator uint64_t() const
{
    return _uuid;
}

}
