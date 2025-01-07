struct ksigaction {
    void (*handler)(int);
    unsigned long flags;
    void *restorer;
    unsigned long mask;
};
