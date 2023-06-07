#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int launchCMakeBuild(const char* cmakePath, const char* buildPath) {
    char command[256];
    sprintf(command, "%s %s", cmakePath, buildPath);
    return system(command);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <CMakePath> <BuildPath>\n", argv[0]);
        return 1;
    }

    const char* cmakePath = argv[1];
    const char* buildPath = argv[2];

    int result = launchCMakeBuild(cmakePath, buildPath);
    if (result == 0) {
        printf("CMake build completed successfully.\n");
    } else {
        printf("CMake build failed.\n");
    }

    return result;
}
