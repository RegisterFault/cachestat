#include <stdio.h>
#include <stdlib.h>
#include <linux/mman.h>
#include <unistd.h>
#include <asm/unistd_64.h>
#include <errno.h>

void runCachestat(int fd, struct cachestat *cs) {
	//tell cachestat to look at the whole file's cache
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
			(double)(cs->nr_dirty) / (double)(cs->nr_cache) * 100
	);

}

int main( int argc, char **argv){
	if (argc != 2) {
		fprintf(stderr, "Failed to find argument for file name\n");
		exit(EXIT_FAILURE);
	}	

	FILE *f = fopen(argv[1], "w");
	if (f == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	struct cachestat cs;
	runCachestat(fileno(f), &cs);
}
