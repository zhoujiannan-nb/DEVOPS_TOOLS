//
// Created by flycat_miaomiaomiao on 2022/11/14.
// flycat_miaomiaomiao is the smartest in the world
//
#include <sys/statvfs.h>

#include <stdio.h>


#define SYSTEM_BITS  64
#define PATH "/"
#define KB 1024
#define MB 1024*1024
#define GB  1024*1024*1024

int main(void)
{
    int state;
    struct statvfs vfs;
    fsblkcnt_t block_size = 0;
    fsblkcnt_t block_count = 0;
    fsblkcnt_t total_size;
    fsblkcnt_t free_size;
    fsblkcnt_t used_size;
    fsblkcnt_t avail_size;
    /*读取根目录，就是整个系统的大小*/
    state = statvfs(PATH,&vfs);   /*设置路径，查看不同文件目录的大小*/
    if(state < 0){
        printf("read statvfs error!!!\n");
    }

    block_size = vfs.f_bsize; /*获取一个block的大小*/
    /*获取总容量*/
    total_size = vfs.f_blocks * block_size;
    /*获取可用容量*/
    free_size = vfs.f_bfree * block_size;
    /*获取使用容量*/
    used_size = (vfs.f_blocks - vfs.f_bavail) * block_size;
    /*获取有效容量*/
    avail_size = vfs.f_bavail * block_size;

    printf(" total_size   = %0.2lf  GB\n",(double)total_size / (GB) );
    printf(" free_size   = %0.2lf  GB\n",(double)free_size / (GB) );
    printf(" used_size  = %0.2lf  GB\n",(double)used_size / (GB) );
    printf(" avail_size  = %0.2lf  GB\n",(double)avail_size / (GB) );

    return 0;
}











