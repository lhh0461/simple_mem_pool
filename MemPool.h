#ifndef __MEM_POOL_
#define __MEM_POOL_

#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include <list>

namespace XEngine
{

template <typename T>
class MemPool
{
public:
    const int DEFAULT_MALLOC_SIZE = 32;
public:
    MemPool(): MemPool(DEFAULT_MALLOC_SIZE) {}
    MemPool(size_t size) { NewBlock(size); }
    MemPool(const MemPool &) = delete; //阻止拷贝
    MemPool &operator=(const MemPool &)  = delete; //阻止赋值
    ~MemPool()
    {
        m_FreeList.clear();
        auto it = m_BlockList.begin();
        for (; it != m_BlockList.end(); it++) {
            delete [] *it;
        }
        m_BlockList.clear();
        m_TotalSize = 0;
        m_TotalBytes = 0;
        printf("~MemPool\n");   
    }
private:
    int NewBlock(size_t block_size)
    {
        if (block_size <= 0) {
            block_size = DEFAULT_MALLOC_SIZE;
        }
        T* p = new T[block_size]();
        if (p == NULL) {
            return 0;
        }

        for (int i = 0; i < block_size; i++) {
            m_FreeList.push_back(&p[i]);
        }

        m_BlockList.push_back(p);

        m_TotalSize += block_size;
        m_TotalBytes += block_size*sizeof(T);
        printf("NewBlock block_size=%ld,total_size=%ld,total_bytes=%ld\n", block_size, m_TotalSize, m_TotalBytes);   
        return 1;
    }
public:
    T* Malloc()
    {
        T* ret;

        if (m_FreeList.empty()) {
            if (NewBlock(m_TotalSize) == false) {
                return NULL;
            }
        }

        assert(!m_FreeList.empty());
        if (!m_FreeList.empty()) {
            ret = m_FreeList.front();
            m_FreeList.pop_front();
        }
        printf("Malloc\n");   

        return ret;
    }
    void Free(T *ptr)
    {
        m_FreeList.push_back(ptr);
    }
    size_t GetTotalSize()
    {
        return m_TotalSize;
    }
    size_t GetTotalBytes()
    {
        return m_TotalBytes;
    }
private:
    std::list<T*> m_FreeList; 
    std::list<T*> m_BlockList; 
    size_t m_TotalSize;
    size_t m_TotalBytes;
};

};

#endif //__MEM_POOL_
