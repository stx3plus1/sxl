/* 
 * sxl 1.1
 * by stx4
 */

#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>

char layout[][64] = {
    "\e[0m\e[43m   .-.  \e[0m dis \e[1;33m| ",
    "\e[0m\e[43m   oo|  \e[0m ker \e[1;33m| ",
    "\e[0m\e[46m  /`'\\  \e[0m upt \e[1;36m| ",
    "\e[0m\e[46m (\\_;/) \e[0m mem \e[1;36m| "
};

int main(void) {
	struct utsname un;
	uname(&un);

	struct sysinfo info;
	sysinfo(&info);

	printf("\n");

	/* distro */
	printf("%s", layout[0]);
	char* distro;
	FILE* osrelease = fopen("/etc/os-release", "r");
	if (!osrelease) {
		printf("%s\n", un.sysname);
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
	printf("%s%s\n", layout[1], un.release);

	/* uptime */
	printf("%s", layout[2]);
	long uptime = info.uptime;

	int days = uptime / (60 * 60 * 24);
	int hours = (uptime % (60 * 60 * 24)) / (60 * 60);
	int minutes = (uptime % (60 * 60)) / 60;
	if (days > 0) printf("%dd ", days);
	if (hours > 0) printf("%dh ", hours);
	printf("%dm\n", minutes);

	/* memory */
	unsigned long total = info.totalram * info.mem_unit, avail = info.freeram * info.mem_unit;
	printf("%s%.2f GiB / %.2f GiB\n", layout[3], (total - avail) / 1073741824.0, total / 1073741824.0);

	printf("\e[0m\n");
}
