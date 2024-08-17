#ifndef __TESTUTIL_H
#define __TESTUTIL_H

int passed = 0;
int failed = 0;
int test_num = 0;

#define test(desc, cond) do {\
		++test_num; \
		printf("%d - %s: ",test_num, desc); \
		if(cond) {printf("PASSED\n"); passed++;} else {printf("FAILED\n"); failed++;} \
} while(0);
#endif // __TESTUTIL_H
