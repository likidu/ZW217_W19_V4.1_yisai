/*******************************************************************************
IIMEMemory.h

    Description:  内存操作定义文件
    
    Author:        Liutingchao、Huangxiaoru
    Date time:     2004-06-21

    Copyright (C) 2000-2004 Beijing Chinese Star Cyber Technology Ltd.
    All rights reserved.

Changed by kewen. 2004-12-20

*******************************************************************************/
#ifndef __IIMEMEMORY_H__
#define __IIMEMEMORY_H__


#if defined SYMBIAN_PLATFORM
	#include <string.h>
	#include <stdlib.h>
	#define IIME_MALLOC malloc
	#define IIME_FREE free

#elif defined(SMARTPHONE_PLATFORM) || defined(POCKETPC_PLATFORM) 
	#include <string.h>
	#include <stdlib.h>
	#define IIME_MALLOC malloc
	#define IIME_FREE free

#elif defined (WIN32_PLATFORM) || defined(WIN32LIB_PLATFORM)
//    #ifdef MTK_TOUCH_PANEL
        #define NOMALLOC_PRODUCT
//    #else
//	    #include <windows.h>
//	    #define IIME_MALLOC malloc
//	    #define IIME_FREE free
//    #endif

#elif defined (LINUX_PLATFORM)
    #define IIME_MALLOC malloc
    #define IIME_FREE free

#elif defined ARM_PLATFORM
    #if defined LONGCHEER_MALLOC_PRODUCT
        void* longcheer_malloc(unsigned int size);
        void longcheer_free(void* block);

        #define IIME_MALLOC longcheer_malloc
        #define IIME_FREE longcheer_free
    //#if defined CECW_PRODUCT
    //    #define IIME_MALLOC ImmAllocMemory
    //    #define IIME_FREE ImmFreeMemory

    //#elif defined MOBILSOFT_PRODUCT
    //    #define IIME_MALLOC malloc
    //    #define IIME_FREE free
    //#elif defined DATANG_PRODUCT
    //    #define IIME_MALLOC malloc
    //    #define IIME_FREE free
    //#elif defined CECW_LINUX_PRODUCT
    //    #define IIME_MALLOC malloc
    //    #define IIME_FREE free
    //#elif defined SPREADTRUM_PRODUCT
    //    void *SCI_Malloc(unsigned int s, const char * file, unsigned int line);
    //    void SCI_Free(void * p);
    //    #define IIME_MALLOC(x) SCI_Malloc(x, __FILE__, __LINE__)
    //    #define IIME_FREE SCI_Free

    //#elif defined E28_PRODUCT
    //    #define IIME_MALLOC malloc
    //    #define IIME_FREE free
    //#elif defined BMR_PRODUCT
    //    #define IIME_MALLOC malloc
    //    #define IIME_FREE free
    //#elif defined LENOVO_PRODUCT
    //    #define IIME_MALLOC IIMEk2_get_memory
    //    #define IIME_FREE IIMEk2_free_memory
    #else
        #define NOMALLOC_PRODUCT
    #endif
#else
    #error Can not find PLATFORM define!
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	#ifdef NOMALLOC_PRODUCT
		#undef NOMALLOC_PRODUCT
		#define SELFALLOC_PRODUCT

		void IIMEMemory_SelfReset(void *pFrom);
		void *IIMEMemory_SelfAlloc(unsigned int size);
		void IIMEMemory_SelfFree(void* pBlock);

        #define IIME_MRESET IIMEMemory_SelfReset 
		#define IIME_MALLOC IIMEMemory_SelfAlloc
		#define IIME_FREE IIMEMemory_SelfFree
	#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__IIMEMEMORY_H__*/
