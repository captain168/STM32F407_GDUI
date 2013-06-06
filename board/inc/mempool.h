#ifndef _MEMPOOL_H_
#define _MEMPOOL_H_


#define MEM_ALIGNMENT	1
#define MIN_SIZE 		12
#define MEM_SIZE		1*1024


#define SIZEOF_STRUCT_MEM (sizeof(struct mem)+\
	(((sizeof(struct mem)%MEM_ALIGNMENT)==0)?0:\
    (4-(sizeof(struct mem)%MEM_ALIGNMENT))))
#ifndef MEM_ALIGN_SIZE
    #define MEM_ALIGN_SIZE(size) (((size)+MEM_ALIGNMENT-1)&~(MEM_ALIGNMENT-1))
#endif
#ifndef MEM_ALIGN
   	#define MEM_ALIGN(addr) ((void*)(((mem_ptr_t)(addr)+MEM_ALIGNMENT-1)&\~(mem_ptr_t)(MEM_ALIGNMENT-1)))
#endif
#ifndef NULL
	#define NULL	0
#endif
//在此可添加对调试信息的打印
#define ASSERT(x, y) 
#define DEBUGF(x, y)


typedef	unsigned int 	mem_size_t;
typedef	unsigned char	u8_t;
typedef unsigned short	u16_t;
typedef unsigned int 	u32_t;
struct mem
{
    mem_size_t next,prev;
    u8_t used;
};


void mem_init(void);
void *mem_malloc(mem_size_t size);
void mem_free(void *mem);
void *mem_realloc(void *mem, mem_size_t size);
void *mem_reallocm(void *mem, mem_size_t size);


#endif 



