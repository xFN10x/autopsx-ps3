#include <stdio.h>
#include <string.h>

#define ASSETS_PATH "/dev_hdd0/game/FNHB00001/assets/"

char* getAssetPath(char* localPath) {
    char base[80];
    strcpy(base, ASSETS_PATH);
    char* resolved = strcat(base, localPath);
    printf("Resolved asset of path: %s to: %s\n", localPath, resolved);
    return resolved;
}