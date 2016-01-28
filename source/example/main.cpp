#include <engine/Core.hpp>

int main(int /*argc*/, char* /*argv*/[])
{
    Core::create();

    Core::destroy();
    return 0;
}
