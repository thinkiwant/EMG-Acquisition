#pragma once
#include<cstdio>

#define FSAMP 2 // 0:500Hz 1:1k 2:2k 3:4k
#define NCH 3
#define MODE 0
#define HRES 1
#define HPF 1
#define EXTEN 0
#define TRIG 0
#define GO 1
#define ConvFact 286e-6
#define PRINT_TYPE 1

#if HRES == 1
#define DATA_BYTES  3
#else
define DATA_BYTES  2
#endif // HRES == 1


#if NCH == 0
#if MODE == 1
#define NUMCHAN 8

#else

#define NUMCHAN 12
#endif
#elif NCH == 1
#if MODE == 1

#define NUMCHAN 12
#else

#define NUMCHAN 20
#endif
#elif NCH == 2
#if MODE == 1


#define NUMCHAN 20
#else

#define NUMCHAN 36
#endif
#elif NCH == 3
#if MODE == 1

#define NUMCHAN 36
#else

#define NUMCHAN 68
#endif
#endif // 0

#if FSAMP == 0
#if MODE == 3
#define SAMPFREQ 2000
#else
#define SAMPFREQ 500
#endif
#elif FSAMP == 1
#if MODE == 3
#define SAMPFREQ 4000
#else
#define SAMPFREQ 1000
#endif
#elif FSAMP == 2
#if MODE == 3
#define SAMPFREQ 8000
#else
#define SAMPFREQ 2000
#endif
#elif FSAMP == 3
#if MODE == 3
#define SAMPFREQ 16000
#else
#define SAMPFREQ 4000
#endif
#endif



