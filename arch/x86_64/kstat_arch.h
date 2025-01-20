struct kstat {
    unsigned long long      kst_dev;
    unsigned long long      kst_ino;
    unsigned long long      kst_nlink;

    unsigned int            kst_mode;
    unsigned int            kst_uid;
    unsigned int            kst_gid;
    unsigned int            __pad0;
    unsigned long long      kst_rdev;
    long long               kst_size;
    long long               kst_blksize;
    long long               kst_blocks;

    unsigned long long      kst_atime;
    unsigned long long      kst_atime_nsec;
    unsigned long long      kst_mtime;
    unsigned long long      kst_mtime_nsec;
    unsigned long long      kst_ctime;
    unsigned long long      kst_ctime_nsec;
    long long               __unused[3];
};
