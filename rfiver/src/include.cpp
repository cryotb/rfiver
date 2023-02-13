#include <include.h>

g::self_t g::self;
g::rce_t g::rce;

c_game_thread_callbacks* g_gtc;
std::atomic<u8> g_ignore_mdl_checks;
