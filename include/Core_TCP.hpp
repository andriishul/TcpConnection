#pragma once

#ifdef IS_WINDOWS
#define TCP_API __declspec(dllexport)
#else
#define TCP_API __declspec(dllimport)
#endif

#ifdef DEFAULT_SIZE
#define DEFAULT_SIZE
#else
#define DEFAULT_SIZE 1000
#endif 


