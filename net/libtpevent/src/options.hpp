#include <stdint.h>

enum class engine_t: uint8_t {UNKNOWN, SELECT, POLL};

/*
    Programm option class.
*/
class Options
{
    public:
        Options(int count, const char *const *args);
        uint16_t port() const noexcept { return m_Port; }
        engine_t engine() const noexcept { return m_Engine; }
        bool async() const noexcept    { return m_Async; }

    private:
        uint16_t m_Port;
        engine_t m_Engine;
        bool m_Async;
};
