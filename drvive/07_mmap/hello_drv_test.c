/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-05-19 12:43:52
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-05-31 15:04:15
 * @FilePath: \07_mmap\hello_drv_test.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

/*
 * ./hello_drv_test
 */
int main(int argc, char **argv)
{
	int fd;
	char *buf;
	int len;
	char str[1024];
	
	
	/* 1. 打开文件 */
	fd = open("/dev/hello", O_RDWR);
	if (fd == -1)
	{
		printf("can not open file /dev/hello\n");
		return -1;
	}

	/* 2. mmap 
	 * MAP_SHARED  : 多个APP都调用mmap映射同一块内存时, 对内存的修改大家都可以看到。
	 *               就是说多个APP、驱动程序实际上访问的都是同一块内存
	 * MAP_PRIVATE : 创建一个copy on write的私有映射。
	 *               当APP对该内存进行修改时，其他程序是看不到这些修改的。
	 *               就是当APP写内存时, 内核会先创建一个拷贝给这个APP, 
	 *               这个拷贝是这个APP私有的, 其他APP、驱动无法访问。
	 */
	//映射后用户空间的buf指针就指向内核空间的kernelbuf区域，可以直接读写往内核空间的此区域写数据
	buf =  mmap(NULL, 1024*8, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (buf == MAP_FAILED)
	{
		printf("can not mmap file /dev/hello\n");
		return -1;
	}

	printf("mmap address = 0x%x\n", buf);
	printf("buf origin data = %s\n", buf); /* old */

	/* 3. write，往内核中的buf写入new数据 */
	strcpy(buf, "new");

	/* 4. read & compare */
	/* 对于MAP_SHARED映射:  str = "new" 
	 * 对于MAP_PRIVATE映射: str = "old" 
	 */
	read(fd, str, 1024);  //再次读内核buf中的数据，发现数据从old变成了new，说明我们直接在用户空间访问了内核空间的数据
	if (strcmp(buf, str) == 0)
	{
		/* 对于MAP_SHARED映射，APP写的数据驱动可见
		 * APP和驱动访问的是同一个内存块
		 */
		printf("compare ok!\n");
	}
	else
	{
		/* 对于MAP_PRIVATE映射，APP写数据时, 是写入原来内存块的"拷贝"
		 */
		printf("compare err!\n");
		printf("str = %s!\n", str);  /* old */
		printf("buf = %s!\n", buf);  /* new */
	}

	while (1)
	{
		sleep(10);  /* cat /proc/pid/maps */
	}
	
	munmap(buf, 1024*8);
	close(fd);
	
	return 0;
}


