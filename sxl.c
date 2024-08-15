#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>

char layout[][64] = {
    "\x1b[0m\x1b[43m   .-.  \x1b[0m dis \x1b[1;33m| ",
    "\x1b[0m\x1b[43m   oo|  \x1b[0m ker \x1b[1;33m| ",
    "\x1b[0m\x1b[46m  /`'\\  \x1b[0m upt \x1b[1;36m| ",
    "\x1b[0m\x1b[46m (\\_;/) \x1b[0m mem \x1b[1;36m| "
};

int main(int argc, char **argv) {
    struct utsname kernel;
    uname(&kernel);
    // dist
    char* distro;
    FILE* osrelease = fopen("/etc/os-release", "r");
    if (!osrelease) {
        return 1;
    } else {
        char osline[64];
        while (fgets(osline, 63, osrelease)) {
            if (strstr(osline, "ID")) {
                distro = strtok(osline, "=");
                distro = strtok(NULL, "=");
                printf("\n%s%s", layout[0], distro);
	        break;
	    }
        }
    }
    // kern
    printf("%s%s\n", layout[1], kernel.release);
    // up
    printf("%s", layout[2]);
    long uptime_seconds = 0;
    FILE *puptime = fopen("/proc/uptime", "r");
    double uptime;
    fscanf(puptime, "%lf", &uptime);
    fclose(puptime);
    uptime_seconds = (long)uptime;
    int days = uptime_seconds / (60 * 60 * 24); 
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
    FILE *meminfo = fopen("/proc/meminfo", "r");
    char line[256];
    int total_memory = 0;
    int free_memory = 0;
    while (fgets(line, sizeof(line), meminfo)) {
        if (strncmp(line, "MemTotal:", 9) == 0) {
            sscanf(line + 9, "%d", &total_memory);
        } else if (strncmp(line, "MemAvailable:", 8) == 0) {
            sscanf(line + 13, "%d", &free_memory);
        } 
    }
    fclose(meminfo);
    printf("%s%.2fGiB / %.2fGiB\n\n\x1b[0m", layout[3], (double)(total_memory - free_memory) / 1048576, (double)total_memory / 1048576);
}
