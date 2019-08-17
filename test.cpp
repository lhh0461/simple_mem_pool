#include <stdio.h>
#include "MemPool.h"


struct test {
    int a;
    int b;
    int c;
    ~test()
    {
        printf("on ~test, a=%d,b=%d,c=%d\n", a, b, c);   
    }
};

int test()
{
    XEngine::MemPool<struct test> *pool = new XEngine::MemPool<struct test>(10);
    std::list<struct test*> l;
    for (int i = 0; i < 40; i++) {
        struct test *t = pool->Malloc();
        t->a = 10 + i;
        t->b = 10 + i;
        t->c = 10 + i;
        l.push_back(t);
        printf("t->a=%d,t->b=%d,t->c=%d\n", t->a, t->b, t->c);   
    }

    while (!l.empty())
    {
        struct test *p = l.front();
        l.pop_front();
        pool->Free(p);
    }

    printf("totalBytes=%ld,totalSize=%ld\n", pool->GetTotalBytes(), pool->GetTotalSize());   

    assert(40 * sizeof(struct test) == pool->GetTotalBytes());
    assert(40 == pool->GetTotalSize());

    delete pool;
}

int main()
{
    test();
}
