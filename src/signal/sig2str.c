#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static const char *const signames[] = {
    [0] = "EXIT", [SIGHUP] = "HUP", [SIGINT] = "INT", [SIGQUIT] = "QUIT",
    [SIGILL] = "ILL", [SIGTRAP] = "TRAP", [SIGABRT] = "ABRT", [SIGBUS] = "BUS",
    [SIGFPE] = "FPE", [SIGKILL] = "KILL", [SIGUSR1] = "USR1", [SIGSEGV] = "SEGV",
    [SIGUSR2] = "USR2", [SIGPIPE] = "PIPE", [SIGALRM] = "ALRM",
    [SIGTERM] = "TERM", [SIGSTKFLT] = "STKFLT", [SIGCHLD] = "CHLD",
    [SIGCONT] = "CONT", [SIGSTOP] = "STOP", [SIGTSTP] = "TSTP",
    [SIGTTIN] = "TTIN", [SIGTTOU] = "TTOU", [SIGURG] = "URG", [SIGXCPU] = "XCPU",
    [SIGXFSZ] = "XFSZ", [SIGVTALRM] = "VTALRM", [SIGPROF] = "PROF",
    [SIGWINCH] = "WINCH", [SIGPOLL] = "POLL", [SIGPWR] = "PWR", [SIGSYS] = "SYS",
    [32] = "RESV1", [33] = "RESV2", [34] = "RESV3",
#if _NSIG == 65
    [35] = "RTMIN", [36] = "RTMIN+1", [37] = "RTMIN+2", [38] = "RTMIN+3",
    [39] = "RTMIN+4", [40] = "RTMIN+5", [41] = "RTMIN+6", [42] = "RTMIN+7",
    [43] = "RTMIN+8", [44] = "RTMIN+9", [45] = "RTMIN+10", [46] = "RTMIN+11",
    [47] = "RTMIN+12", [48] = "RTMIN+13", [49] = "RTMIN+14", [50] = "RTMAX-14",
    [51] = "RTMAX-13", [52] = "RTMAX-12", [53] = "RTMAX-11", [54] = "RTMAX-10",
    [55] = "RTMAX-9", [56] = "RTMAX-8", [57] = "RTMAX-7", [58] = "RTMAX-6",
    [59] = "RTMAX-5", [60] = "RTMAX-4", [61] = "RTMAX-3", [62] = "RTMAX-2",
    [63] = "RTMAX-1", [64] = "RTMAX",
#else
    [35] = "RTMIN", [36] = "RTMIN+1", [37] = "RTMIN+2", [38] = "RTMIN+3",
    [39] = "RTMIN+4", [40] = "RTMIN+5", [41] = "RTMIN+6", [42] = "RTMIN+7",
    [43] = "RTMIN+8", [44] = "RTMIN+9", [45] = "RTMIN+10", [46] = "RTMIN+11",
    [47] = "RTMIN+12", [48] = "RTMIN+13", [49] = "RTMIN+14", [50] = "RTMIN+15",
    [51] = "RTMIN+16", [52] = "RTMIN+17", [53] = "RTMIN+18", [54] = "RTMIN+19",
    [55] = "RTMIN+20", [56] = "RTMIN+21", [57] = "RTMIN+22", [58] = "RTMIN+23",
    [59] = "RTMIN+24", [60] = "RTMIN+25", [61] = "RTMIN+26", [62] = "RTMIN+27",
    [63] = "RTMIN+28", [64] = "RTMIN+29", [65] = "RTMIN+30", [66] = "RTMIN+31",
    [67] = "RTMIN+32", [68] = "RTMIN+33", [69] = "RTMIN+34", [70] = "RTMIN+35",
    [71] = "RTMIN+36", [72] = "RTMIN+37", [73] = "RTMIN+38", [74] = "RTMIN+39",
    [75] = "RTMIN+40", [76] = "RTMIN+41", [77] = "RTMIN+42", [78] = "RTMIN+43",
    [79] = "RTMIN+44", [80] = "RTMIN+45", [81] = "RTMIN+46", [82] = "RTMAX-45",
    [83] = "RTMAX-44", [84] = "RTMAX-43", [85] = "RTMAX-42", [86] = "RTMAX-41",
    [87] = "RTMAX-40", [88] = "RTMAX-39", [89] = "RTMAX-38", [90] = "RTMAX-37",
    [91] = "RTMAX-36", [92] = "RTMAX-35", [93] = "RTMAX-34", [94] = "RTMAX-33",
    [95] = "RTMAX-32", [96] = "RTMAX-31", [97] = "RTMAX-30", [98] = "RTMAX-29",
    [99] = "RTMAX-28", [100] = "RTMAX-27", [101] = "RTMAX-26", [102] = "RTMAX-25",
    [103] = "RTMAX-24", [104] = "RTMAX-23", [105] = "RTMAX-22", [106] = "RTMAX-21",
    [107] = "RTMAX-20", [108] = "RTMAX-19", [109] = "RTMAX-18", [110] = "RTMAX-17",
    [111] = "RTMAX-16", [112] = "RTMAX-15", [113] = "RTMAX-14", [114] = "RTMAX-13",
    [115] = "RTMAX-12", [116] = "RTMAX-11", [117] = "RTMAX-10", [118] = "RTMAX-9",
    [119] = "RTMAX-8", [120] = "RTMAX-7", [121] = "RTMAX-6", [122] = "RTMAX-5",
    [123] = "RTMAX-4", [124] = "RTMAX-3", [125] = "RTMAX-2", [126] = "RTMAX-1",
    [127] = "RTMAX",
#endif
};

int sig2str(int sig, char *str)
{
    if (sig + 0u >= _NSIG) return -1;
    strcpy(str, signames[sig]);
    return 0;
}

int str2sig(const char *restrict str, int *restrict sig)
{
    for (int i = 0; i < _NSIG; i++)
        if (!strcmp(signames[i], str)) {
            *sig = i;
            return 0;
        }
    if ((!strncmp(str, "RTMIN+", 6) || !strncmp(str, "RTMAX-", 6)) && isdigit(str[6])) {
        unsigned long n = strtoul(str + 6, 0, 10);
        if (n - 1ul < _NSIG - 37ul) {
            *sig = str[5] == '+'? 35 + n : _NSIG - 1 - n;
            return 0;
        }
    } else if (isdigit(str[0])) {
        unsigned long n = strtoul(str, 0, 10);
        if (n <= _NSIG - 1ul) {
            *sig = n;
            return 0;
        }
    }
    return -1;
}
