struct ksigaction {
    void (*handler)(int);
    unsigned long flags;
    void *restorer;
    struct { unsigned long x[2]; } mask;
};
