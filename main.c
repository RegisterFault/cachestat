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

void runCachestat(int fd, struct cachestat *cs) {
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
		return EXIT_FAILURE;
	}	

	FILE *f = fopen(argv[1], "w");
	if (f == NULL) {
		perror("fopen");
		exit(1);
	}
	struct cachestat cs;
	runCachestat(fileno(f), &cs);
}
