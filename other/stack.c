#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STACK_TEST_TAG 0xAB
#define STACK_TEST_SIZE 1024 * 64

static const char* stack_usage_fill(int tag, size_t stack_size) {
    char* top = (char*)alloca(stack_size);
    if (top) {
        memset(top, tag, stack_size);
    }
    return top;
}

static size_t stack_usage_test(int tag, size_t stack_size, const char* top) {
    size_t unused = 0;
    char ctag = (char)tag;
    for (unused = 0; unused < stack_size && top[unused] == ctag; unused++) {
    }
    return stack_size - unused;
}

static int foo() {
    char buf[2048];
    memset(buf, 0x11, sizeof(buf));
    int sum = 0;
    for (int i = 0; i < sizeof(buf); i++) {
        sum += buf[i];
    }
    return sum;
}


//https://zhuanlan.zhihu.com/p/435978191
int main() {
    const char* top = stack_usage_fill(STACK_TEST_TAG, STACK_TEST_SIZE);
    size_t stack_max_used1 = stack_usage_test(STACK_TEST_TAG, STACK_TEST_SIZE, top);
    int sum = foo();
    size_t stack_max_used = stack_usage_test(STACK_TEST_TAG, STACK_TEST_SIZE, top);
    printf("stack_max_used %ld\n", stack_max_used1);
    printf("stack_max_used %ld, sum=%d\n", stack_max_used, sum);
    return 0;
}

