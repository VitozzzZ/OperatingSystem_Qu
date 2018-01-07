#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mount.h>
#include <unistd.h>

int main(){
    //系统自动挂载
    umount("/zsq/media/RHAPSODY");
    if (mount("/dev/sdb", "/mnt/USB/", "vfat", MS_NOATIME, NULL)) {
        if (errno == EBUSY) {
            printf("Mountpoint busy");
        }
        else {
            printf("Mount error: %s", strerror(errno));
        }
        return -1;
    }
    else {
        printf("Mount successful");
        printf("Press <return> to unmount the usb disk:");
        getchar();
        
        umount("/mnt/USB/");
        printf("Unmount successful");
    }
    return 0;
   
}
