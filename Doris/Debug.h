#ifndef DORIS_DEBUG_H__
#define DORIS_DEBUG_H__

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

#include <xmmintrin.h>
#include <ctime>
#define DORIS_TOTAL_DEBUG
#define DORIS_DEBUG
#define DORIS_COUNT






#endif 
