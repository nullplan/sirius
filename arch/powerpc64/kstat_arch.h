struct kstat {
    unsigned long kst_dev;
    unsigned long kst_ino;
    unsigned long kst_nlink;
    unsigned kst_mode;
    unsigned kst_uid;
    unsigned kst_gid;
    unsigned long kst_rdev;
    long kst_size;
    unsigned long kst_blksize;
    unsigned long kst_blocks;
    unsigned long kst_atime;
    unsigned long kst_atime_nsec;
    unsigned long kst_mtime;
    unsigned long kst_mtime_nsec;
    unsigned long kst_ctime;
    unsigned long kst_ctime_nsec;
    unsigned long __unused[3];
};
