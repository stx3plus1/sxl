#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/utsname.h>
#include <sys/stat.h>

char layout[4][64] = {
    "\x1b[0m\x1b[1;30m  .-.  \x1b[1;30m dis | ",
    "\x1b[0m  oo\x1b[1;30m|  \x1b[1;30m ker | ",
    "\x1b[0m\x1b[1;37m /`'\\  \x1b[1;30m upt | ",
    "\x1b[0m\x1b[1;33m(\\\x1b[1;30m_;\x1b[1;33m/) \x1b[1;30m mem | "
};

int main(int argc, char **argv) {
    struct utsname kernel;
    uname(&kernel);
    // dist
    char* distro;
    FILE* osrelease = fopen("/etc/os-release", "r");
    if (!osrelease) {
        printf("\n");
    } else {
        char osline[256];
        while (fgets(osline, 128, osrelease)) {
            if (strstr(osline, "ID")) {
                distro = strtok(osline, "=");
                distro = strtok(NULL, "=");
                printf("\n%s%s", layout[0], distro);
            	goto cont;
	    }
        }
    }
    cont:
    // kern
    printf("%s%s\n", layout[1], kernel.release);
    // up
    printf("%s", layout[2]);
    long uptime_seconds = 0;
    FILE *file = fopen("/proc/uptime", "r");
    double uptime;
    fscanf(file, "%lf", &uptime) 
    fclose(file);
    *uptime_seconds = (long)uptime;
    int days = uptime_seconds / (60 * 60 * 
    int hours = (uptime_seconds % (60 * 60 * 24)) / (60 * 60);
    int minutes = (uptime_seconds % (60 * 60)) / 60;
    if (days > 0) {
        printf("%dd ", days);
    }
    if (hours > 0) {
        printf("%dh ", hours);
    }
    printf("%dm\n", minutes);
    // mem
    FILE *file = fopen("/proc/meminfo", "r");
    if (!file) {
        return 1;
    }
    char line[256];
    int total_memory = 0;
    int free_memory = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "MemTotal:", 9) == 0) {
            sscanf(line + 9, "%d", &total_memory);
        } else if (strncmp(line, "MemAvailable:", 8) == 0) {
            sscanf(line + 13, "%d", &free_memory);
        } 
    }
    fclose(file);
    printf("%s%.2fGiB / %.2fGiB\n\n\x1b[0m", layout[3], (double)(total_memory - free_memory) / 1048576, (double)total_memory / 1048576);
    return 0;
}
