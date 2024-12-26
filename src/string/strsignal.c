#include <string.h>
#include <signal.h>

static const char *const sigs[] = {
    [0] = "No signal",
    [SIGHUP] = "Hang-up",
    [SIGINT] = "Interrupt",
    [SIGQUIT] = "Quit",
    [SIGILL] = "Illegal instruction",
    [SIGTRAP] = "Debug trap",
    [SIGABRT] = "Abort",
    [SIGBUS] = "Bus error",
    [SIGFPE] = "Floating-point exception",
    [SIGKILL] = "Killed",
    [SIGUSR1] = "User-defined signal 1",
    [SIGSEGV] = "Segmentation violation",
    [SIGUSR2] = "User-defined signal 2",
    [SIGPIPE] = "Write to closed pipe",
    [SIGALRM] = "Alarm",
    [SIGTERM] = "Terminated",
    [SIGSTKFLT] = "Stack fault",
    [SIGCHLD] = "Child process",
    [SIGCONT] = "Continued",
    [SIGSTOP] = "Stopped",
    [SIGTSTP] = "Stopped (tty)",
    [SIGTTIN] = "Stopped (tty input)",
    [SIGTTOU] = "Stopped (tty output)",
    [SIGURG] = "Urgent data",
    [SIGXCPU] = "CPU time exceeded",
    [SIGXFSZ] = "File size exceeded",
    [SIGVTALRM] = "Virtual alarm",
    [SIGPROF] = "Profiling signal",
    [SIGWINCH] = "Window size changed",
    [SIGPOLL] = "Polled I/O",
    [SIGPWR] = "Power failure",
    [SIGSYS] = "Bad system call",
    [32] = "RT32", [33] = "RT33", [34] = "RT34", [35] = "RT35", [36] = "RT36",
    [37] = "RT37", [38] = "RT38", [39] = "RT39", [40] = "RT40", [41] = "RT41",
    [42] = "RT42", [43] = "RT43", [44] = "RT44", [45] = "RT45", [46] = "RT46",
    [47] = "RT47", [48] = "RT48", [49] = "RT49", [50] = "RT50", [51] = "RT51",
    [52] = "RT52", [53] = "RT53", [54] = "RT54", [55] = "RT55", [56] = "RT56",
    [57] = "RT57", [58] = "RT58", [59] = "RT59", [60] = "RT60", [61] = "RT61",
    [62] = "RT62", [63] = "RT63", [64] = "RT64",
#if _NSIG > 65
    [65] = "RT65", [66] = "RT66", [67] = "RT67", [68] = "RT68", [69] = "RT69",
    [70] = "RT70", [71] = "RT71", [72] = "RT72", [73] = "RT73", [74] = "RT74",
    [75] = "RT75", [76] = "RT76", [77] = "RT77", [78] = "RT78", [79] = "RT79",
    [80] = "RT80", [81] = "RT81", [82] = "RT82", [83] = "RT83", [84] = "RT84",
    [85] = "RT85", [86] = "RT86", [87] = "RT87", [88] = "RT88", [89] = "RT89",
    [90] = "RT90", [91] = "RT91", [92] = "RT92", [93] = "RT93", [94] = "RT94",
    [95] = "RT95", [96] = "RT96", [97] = "RT97", [98] = "RT98", [99] = "RT99",
    [100] = "RT100", [101] = "RT101", [102] = "RT102", [103] = "RT103",
    [104] = "RT104", [105] = "RT105", [106] = "RT106", [107] = "RT107",
    [108] = "RT108", [109] = "RT109", [110] = "RT110", [111] = "RT111",
    [112] = "RT112", [113] = "RT113", [114] = "RT114", [115] = "RT115",
    [116] = "RT116", [117] = "RT117", [118] = "RT118", [119] = "RT119",
    [120] = "RT120", [121] = "RT121", [122] = "RT122", [123] = "RT123",
    [124] = "RT124", [125] = "RT125", [126] = "RT126", [127] = "RT127",
#endif
};

char *strsignal(int s)
{
    if (s >= sizeof sigs / sizeof *sigs || !sigs[s]) return "Unknown signal";
    return (char *)sigs[s];
}
