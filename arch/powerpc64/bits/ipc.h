struct ipc_perm {
    key_t   __NSG(key);
    uid_t   uid;
    gid_t   gid;
    uid_t   cuid;
    gid_t   cgid;
    mode_t  mode;
    unsigned __NSG(seq);
    unsigned __pad;
    unsigned long long __unused[2];
};
