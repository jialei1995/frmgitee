#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "mycmd.h"
#include <stdio.h>
#include <string.h>
int main()
{
	int fd;
	int adcval;
	char buf[10] = {0};
	fd = open("/dev/hello0",O_RDWR);
	perror("open:");
	
		getchar();
		adcval = ioctl(fd,READ_ADC);
		
	
	close(fd);
	return 0;
	
}
