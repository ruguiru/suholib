#pragma once

namespace suho {
namespace idioms
{
    class NonCopyable
    {
    protected:
        NonCopyable() {}
        virtual ~NonCopyable() {}
    private:
        NonCopyable(const NonCopyable&) = delete;
        const NonCopyable& operator=(const NonCopyable&) = delete;
    };
}   // end namespace idioms
}   // end namespace suho