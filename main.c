#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <asm/unistd_64.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <linux/mman.h>

#define NUMPAGES 100

void runMincore(void *addr, size_t len){
	long pageSize = sysconf(_SC_PAGE_SIZE);
	unsigned char vec[len];
	int res = mincore(addr,len*pageSize,vec);
	assert(res == 0);
	size_t paged = 0;
	size_t unpaged = 0;
	for (int i=0; i<len; i++){
		vec[i] ? paged++ : unpaged++;
	}
	fprintf(stdout, "paged: %lld unpaged: %lld\n",paged, unpaged);
}

void runCachestat(int fd, struct cachestat *cs) {
	long pageSize = sysconf(_SC_PAGE_SIZE);
	struct cachestat_range cr = {
		.off = 0,
		.len = 0,
	};
	//cachestat still not implemented in glibc
	if (syscall(__NR_cachestat,fd, &cr, cs, 0) == -1) {
		perror("cachestat");
		exit(1);
	}

	fprintf(stderr,"cache: %lld dirty: %lld precent_dirty: %.f\n",
			cs->nr_cache,
			cs->nr_dirty,
			(double)(cs->nr_dirty) / (double)(cs->nr_cache)
	);

}

int main( int argc, char **argv){
	/*
	long pageSize = sysconf(_SC_PAGE_SIZE);
	void *addr  = mmap(NULL, NUMPAGES*pageSize,  PROT_READ | PROT_WRITE, MAP_NORESERVE | MAP_PRIVATE | MAP_ANONYMOUS, 0 ,0);
	
	if (addr == MAP_FAILED){
		perror("mmap");
		exit(1);
	}
	*/
	if (argc != 2) {
		fprintf(stderr, "Failed to find argument for file name\n");
		return EXIT_FAILURE;
	}	
	/*
	unsigned char *buffer = addr;
	FILE *writeHere = fopen("foo","w+");
	FILE *readHere = fopen(argv[1],"r");
	struct cachestat cs;

	for (int i =0; i<NUMPAGES; i++){
		fread(&buffer[pageSize+i], pageSize, 1, readHere);
		fwrite(&buffer[pageSize*i],pageSize,1,writeHere);
		runCachestat(fileno(writeHere),	&cs);
	}
	fprintf(stdout,"syning...\n");
	sync();
	runCachestat(fileno(writeHere),	&cs);
	*/

	FILE *f = fopen(argv[1], "w");
	struct cachestat cs;
	while (1){
		runCachestat(fileno(f), &cs);
		sleep(5);
	}
}
