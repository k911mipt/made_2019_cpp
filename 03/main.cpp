#ifdef WINDOWS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define MEMORY_CHECK_ON _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); \
                        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE); \
                        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
#else
#define MEMORY_CHECK_ON ((void)0);
#endif

#include <iostream>
#include <vector>
#include "LinearAllocator.hpp"

using std::vector;

int main(int argc, char** argv)
{
    MEMORY_CHECK_ON
    made::stl::LinearAllocator<int> allocator(0);
    int* arr = allocator.Alloc(5);
    //made::stl::LinearAllocator<double> *pallocator = new made::stl::LinearAllocator<double>(10);
    //int *a = new int[5];
    //auto allocator = *pallocator;
    //int* p = allocator.alloc(3);
    //int* p2 = allocator.alloc(3);
    //int* p3 = allocator.alloc(3);
    //allocator.reset();
    //int* p4 = allocator.alloc(3);
    //p4 = allocator.alloc(3);
    //p4 = allocator.alloc(3);
    //p4 = allocator.alloc(1);
    //vector<int> v({ 1, 2, 3 }, alloc);
    return EXIT_SUCCESS;
}
