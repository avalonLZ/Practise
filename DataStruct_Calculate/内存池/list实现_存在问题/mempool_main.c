/*************************************************************************
	> File Name: mempool_u.c
	> Author:lizhong
	> Mail:423810942@qq.com
	> Instruction:
 ************************************************************************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include"mempool_list.h"


int main(int argc, char *argv[])
{
    void *addr = NULL;
    mem_pool_t *mp = NULL;
    mp = mem_pool_init(2, 3, 16, 5, sizeof(char));


    addr = malloc_from_mp(mp, 16);
    printf("%s addr:%ld\n", __FUNCTION__, addr);
    memset(addr, 5, 16);
    free_to_mp(mp, addr);
    printf("%s addr:%ld\n", __FUNCTION__, addr);
    addr = malloc_from_mp(mp, 16);
    printf("%s addr:%ld\n", __FUNCTION__, addr);
    addr = malloc_from_mp(mp, 16);
    printf("%s addr:%ld\n", __FUNCTION__, addr);
    addr = malloc_from_mp(mp, 16);
    printf("%s addr:%ld\n", __FUNCTION__, addr);
    addr = malloc_from_mp(mp, 16);
    printf("%s addr:%ld\n", __FUNCTION__, addr);
    return 0;
}
