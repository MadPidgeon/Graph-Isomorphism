#pragma once

// toggle for multi-threading
#define THREADED

#ifdef  THREADED
#define THREADS		12
#include <future>
#else
#define THREADS		1
#endif