#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#define ONE_GIG UINT64_C(1073741824)
#define TOTAL_SIZE (UINT64_C(8)*ONE_GIG)

#define HIGH32(x) ( (uint32_t)(((uint64_t)x) >> 32) )
#define LOW32(x) ( (uint32_t)(((uint64_t)x) & ((uint64_t)UINT32_MAX)) )

void fill_mapping(HANDLE mapping, uint64_t offset, uint32_t size) {
    printf("Mapping memory at offset %" PRIX64 " and size %" PRIX32 "\n", offset, size);

    LPVOID addr = MapViewOfFile(
            mapping,
            FILE_MAP_ALL_ACCESS,
            HIGH32(offset),
            LOW32(offset),
            size
    );
    if (addr == NULL) {
        fprintf(stderr, "Failed to MapViewOfFile");
        exit(EXIT_FAILURE);
    }

    printf("Setting memory\n");
    memset(addr, 0xCC, size);

    printf("Unmapping memory\n");
    if (UnmapViewOfFile(addr) == 0) {
        fprintf(stderr, "Failed to UnmapViewOfFile");
        exit(EXIT_FAILURE);
    }
}

void check_mapping(HANDLE mapping, uint64_t offset, uint32_t size) {
    printf("Mapping memory at offset %" PRIX64 " and size %" PRIX32 "\n", offset, size);

    LPVOID addr = MapViewOfFile(
            mapping,
            FILE_MAP_ALL_ACCESS,
            HIGH32(offset),
            LOW32(offset),
            size
    );
    if (addr == NULL) {
        fprintf(stderr, "Failed to MapViewOfFile");
        exit(EXIT_FAILURE);
    }

    printf("Checking memory\n");
    unsigned char* ptr = (unsigned char*) addr;
    for (uint32_t i = 0; i < size; i++) {
        if (ptr[i] != 0xCC) {
            fprintf(stderr, "Memory corrupted at offset %"PRIX32"\n", i);
            exit(EXIT_FAILURE);
        }
    }

    printf("Unmapping memory\n");
    if (UnmapViewOfFile(addr) == 0) {
        fprintf(stderr, "Failed to UnmapViewOfFile");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char** argv) {
    printf("Creating mapping with a size of %"PRIX64"\n", TOTAL_SIZE);
    HANDLE mapping = CreateFileMappingA(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        HIGH32(TOTAL_SIZE),
        LOW32(TOTAL_SIZE),
        NULL
    );

    if (mapping == NULL) {
        fprintf(stderr, "Failed to CreateFileMappingA");
        return EXIT_FAILURE;
    }

    for (uint64_t i = 0; i < (TOTAL_SIZE / ONE_GIG); i++) {
        fill_mapping(mapping, i * ONE_GIG, ONE_GIG);
    }

    printf("\n\nDone. Check the task manager. Press enter to exit.\n\n");

    char dummy;
    scanf("%c", &dummy);

    return EXIT_SUCCESS;
}
