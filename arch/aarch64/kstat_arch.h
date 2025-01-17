struct kstat {
        unsigned long   kst_dev;         /* Device.  */
        unsigned long   kst_ino;         /* File serial number.  */
        unsigned int    kst_mode;        /* File mode.  */
        unsigned int    kst_nlink;       /* Link count.  */
        unsigned int    kst_uid;         /* User ID of the file's owner.  */
        unsigned int    kst_gid;         /* Group ID of the file's group. */
        unsigned long   kst_rdev;        /* Device number, if device.  */
        unsigned long   __pad1;
        long            kst_size;        /* Size of file, in bytes.  */
        int             kst_blksize;     /* Optimal block size for I/O.  */
        int             __pad2;
        long            kst_blocks;      /* Number 512-byte blocks allocated. */
        long            kst_atime;       /* Time of last access.  */
        unsigned long   kst_atime_nsec;
        long            kst_mtime;       /* Time of last modification.  */
        unsigned long   kst_mtime_nsec;
        long            kst_ctime;       /* Time of last status change.  */
        unsigned long   kst_ctime_nsec;
        unsigned int    __unused4;
        unsigned int    __unused5;
};
