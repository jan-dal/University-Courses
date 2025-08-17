#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <inttypes.h>
#include "sha256_mb.h"

#define HTML_SIZE 128
#define NUM_JOBS 256
#define HEAP_SIZE ((long)1 << 32)

static uint64_t comb = 0;
static char cmb[] = "00000000";
//static char abc[] = "0123456789abcdefABCDEF";
static uint64_t hash_counter = 0;
static uint64_t col_map_count = 0;
static uint64_t searched = 0;

void *print_stats()
{
	while (1) {		
		printf("Hashes calculated %ld; Collisions written %ld; Searched %ld\n", hash_counter, col_map_count, searched);
		printf("Ratio: %.02f\n", (double)col_map_count / hash_counter);
		sleep(5);
	}
}

void start_stats_thread()
{
	pthread_t thread;
	long code = pthread_create(&thread, NULL, print_stats, NULL);

	if (code > 0) {
		printf("Error creating thread.\n");
	}
}

void write_result(char* html) 
{
	FILE *fd = fopen("result1.html", "w");
	fwrite(html, sizeof(char), HTML_SIZE, fd);
	fclose(fd);
}

void next() {
	if (comb >= HEAP_SIZE) {
		int found = 0;
		while (!found) {
			sprintf(cmb, "%08X", (uint32_t)comb++);
			for (int i = 0; i < 8; i++) {
				if ((cmb[i] >= 'A') && (cmb[i] <= 'F')) {
					found = 1;
					break;
				}	
			}
		}	
	} else {
		sprintf(cmb, "%08x", (uint32_t)++comb);
	}
}

void create_batch(char ptr[][HTML_SIZE])
{
	for (uint32_t i = 0; i < NUM_JOBS; i++) {
		memcpy(&ptr[i][53], cmb, 8);
		next();
	}
}

int compare(const void* pkey, const void* pelem) {
	if (*(uint64_t*)pkey == *(uint64_t*)pelem) return 0;
	if (*(uint64_t*)pkey < *(uint64_t*)pelem) return -1;
	return 1;
}

void update_collision(SHA256_HASH_CTX *ctx, SHA256_HASH_CTX *ctxpool, uint64_t *collision, char content[][HTML_SIZE]) {
	uint32_t t = (unsigned long)(ctx->user_data);
	uint64_t prefix1 = ctxpool[t].job.result_digest[0];
	uint64_t prefix2 = ctxpool[t].job.result_digest[1];

	uint64_t current_hash = (prefix1 << 32) | prefix2;
	
	if (col_map_count < HEAP_SIZE) {
		collision[col_map_count++] = current_hash;
		if (col_map_count == HEAP_SIZE) {
			printf("Sorting collision array\n");
			qsort((void*)collision, HEAP_SIZE, sizeof(uint64_t), compare);
			printf("Sorted.\n");
		}
	} else {
		uint64_t* result = (uint64_t *)bsearch((void*)&current_hash, (void*)collision, HEAP_SIZE, sizeof(uint64_t), compare);
		searched++;
		if (result != NULL) {
			printf("Found collision!\n");
			printf("%.*s\n", HTML_SIZE, content[t]);
			write_result(content[t]);
		}
	}
}

void sha256_start()
{
	char content[NUM_JOBS][HTML_SIZE];
	uint64_t * collision = malloc(sizeof(uint64_t) * HEAP_SIZE);

	memset(collision, 0, HEAP_SIZE);
	
	FILE *fp = fopen("temp.html", "r");
	fread(&content[0], sizeof(char), HTML_SIZE, fp);
	fclose(fp);

	for (uint32_t i = 1; i < NUM_JOBS; i++) {
		memcpy(&content[i], &content[0], HTML_SIZE);
	}

	SHA256_HASH_CTX_MGR *mgr = NULL;
	SHA256_HASH_CTX ctxpool[NUM_JOBS], *ctx = NULL;
	
	posix_memalign((void *)&mgr, 16, sizeof(SHA256_HASH_CTX_MGR));

	sha256_ctx_mgr_init(mgr);	
	
	for (uint32_t i = 0; i < NUM_JOBS; i++) {
		hash_ctx_init(&ctxpool[i]);
		ctxpool[i].user_data = (void *)((uint64_t) i);
	}

	for (uint32_t i = 0; i < 0x01ffffff; i++) {
		create_batch(&content[0]);
		for (uint32_t j = 0; j < NUM_JOBS; j++) {
			ctx = sha256_ctx_mgr_submit(mgr, &ctxpool[j], &content[j], HTML_SIZE, HASH_ENTIRE);
			if (ctx) {
				hash_counter++;
				update_collision(ctx, &ctxpool[0], &collision[0], &content[0]);
			}
		}
		while (1) {
			ctx = sha256_ctx_mgr_flush(mgr);
			if (ctx) {
				hash_counter++;
				update_collision(ctx, &ctxpool[0], &collision[0], &content[0]);
			} else {
				break;
			}
		}
	}
	free(collision);
}

int main() {
	printf("Heap size %lu\n", HEAP_SIZE);	
    start_stats_thread();

    clock_t t; 
    t = clock(); 

	sha256_start();

    t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
 
    printf("sha256() took %f seconds to execute \n", time_taken); 
	return 0;
}
