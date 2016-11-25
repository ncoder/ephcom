// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the EPHCOMWIN32_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EPHCOMWIN32_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef EPHCOMWIN32_EXPORTS
#define EPHCOMWIN32_API __declspec(dllexport)
#else
#define EPHCOMWIN32_API __declspec(dllimport)
#endif

extern "C" EPHCOMWIN32_API int nephcomwin32;

extern "C" EPHCOMWIN32_API int fnephcomwin32(void);
 