#pragma once

namespace suho {
    namespace idioms
    {
        class NonMovable
        {
        protected:
            NonMovable() {}
            virtual ~NonMovable() {}
        private:
            NonMovable(NonMovable&&) = delete;
            NonMovable& operator=(NonMovable&&) = delete;
        };
    };
}   // end namespace idioms
}   // end namespace suho