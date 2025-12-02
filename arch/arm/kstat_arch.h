struct kstat {
    unsigned long long  kst_dev;
    char __pad0[8];
    unsigned            kst_mode;
    unsigned            kst_nlink;
    unsigned long       kst_uid;
    unsigned long       kst_gid;
    unsigned long long  kst_rdev;
    unsigned char __pad1[4];
    long long           kst_size;
    unsigned long       kst_blksize;
    unsigned long long  kst_blocks;

    /* are they hoping to extend the interface's life to 2100? */
    unsigned long       kst_atime;
    unsigned long       kst_atime_nsec;
    unsigned long       kst_mtime;
    unsigned long       kst_mtime_nsec;
    unsigned long       kst_ctime;
    unsigned long       kst_ctime_nsec;
    unsigned long long  kst_ino;
};
