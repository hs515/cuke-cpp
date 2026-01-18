#include "CukeUtilities.hpp"

#include <chrono>

namespace cuke::internal
{
    uint64_t now()
    {
        using namespace std::chrono;
        return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }
} // namespace cuke::internal
