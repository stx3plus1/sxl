/* 
 * sxl 1.1
 * by stx4
 */

#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>

char layout[][64] = {
    "\e[0m\e[43m   .-.  \e[0m dis \e[1;33m| ",
    "\e[0m\e[43m   oo|  \e[0m ker \e[1;33m| ",
    "\e[0m\e[46m  /`'\\  \e[0m upt \e[1;36m| ",
    "\e[0m\e[46m (\\_;/) \e[0m mem \e[1;36m| "
};

int main(void) {
	struct utsname sys;
	uname(&sys);

	/* first blank */
	printf("\n");

	/* distro */
	printf("%s", layout[0]);
	char* distro;
	FILE* osrelease = fopen("/etc/os-release", "r");
	if (!osrelease) {
		printf("%s\n", sys.sysname);
	} else {
		char osline[256];
		while (fgets(osline, 128, osrelease)) {
			if (strstr(osline, "ID")) {
				distro = strtok(osline, "=");
				distro = strtok(NULL, "=");
				printf("%s", distro);
				fclose(osrelease);
				break;
			}
		}
	}

	/* kernel */
	printf("%s%s\n", layout[1], sys.release);

	/* uptime */
	printf("%s", layout[2]);
	FILE* uptime_file = fopen("/proc/uptime", "r");
	if (uptime_file == NULL) {
		printf("Error opening /proc/uptime.\e[0m\n");
		return 1;
	}
	double uptime;
	if (fscanf(uptime_file, "%lf", &uptime) != 1) {
		fclose(uptime_file);
		printf("Error parsing /proc/uptime.\e[0m\n");
		return 1;
	}
	fclose(uptime_file);
	long uptime_seconds = (long)uptime;

	int days = uptime_seconds / (60 * 60 * 24);
	int hours = (uptime_seconds % (60 * 60 * 24)) / (60 * 60);
	int minutes = (uptime_seconds % (60 * 60)) / 60;
	if (days > 0) printf("%dd ", days);
	if (hours > 0) printf("%dh ", hours);
	printf("%dm\n", minutes);

	/* memory */
	printf("%s", layout[3]);
	FILE* meminfo = fopen("/proc/meminfo", "r");
	if (!meminfo) {
		printf("Error opening /proc/meminfo.\e[0m\n");
		return 1;
	}
	char line[64];
	char* totstr = "MemTotal:";
	char* availstr = "MemAvailable:";
	long total = -1, avail = -1;
	while (fgets(line, sizeof(line), meminfo)) {
		if (!strncmp(line, totstr, strlen(totstr))) {
			sscanf(line, "MemTotal: %ld kB", &total);
		}
		if (!strncmp(line, availstr, strlen(availstr))) {
			sscanf(line, "MemAvailable: %ld kB", &avail);
		}
	}

	fclose(meminfo);

	if (total == -1 || avail == -1) {
		printf("Error parsing /proc/meminfo.\e[0m\n");
		return 1;
	}
	printf("%.2f GiB / %.2f GiB\n", (total - avail) / 1048576.0, total / 1048576.0);

	/* cleanup */
	printf("\e[0m\n");
}
