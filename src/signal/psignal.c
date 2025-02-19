#include <signal.h>
#include <stdio.h>

static const char *const signames[] = {
    [0] = "No signal",
    [SIGHUP] = "Hang-up",
    [SIGINT] = "Interrupt",
    [SIGQUIT] = "Quit",
    [SIGILL] = "Illegal instruction",
    [SIGTRAP] = "Breakpoint trap",
    [SIGABRT] = "Abort",
    [SIGBUS] = "Bus error",
    [SIGFPE] = "Math error",
    [SIGKILL] = "Killed",
    [SIGUSR1] = "User-defined signal 1",
    [SIGSEGV] = "Segmentation violation",
    [SIGUSR2] = "User-defined signal 2",
    [SIGPIPE] = "Pipe error",
    [SIGALRM] = "Alarm",
    [SIGTERM] = "Terminated",
    [SIGSTKFLT] = "Stack fault",
    [SIGCHLD] = "Child error",
    [SIGCONT] = "Continued",
    [SIGSTOP] = "Stopped",
    [SIGTSTP] = "Stopped (tty)",
    [SIGTTIN] = "Stopped (tty input)",
    [SIGTTOU] = "Stopped (tty output)",
    [SIGURG] = "Urgent data available",
    [SIGXCPU] = "CPU-time exceeded",
    [SIGXFSZ] = "File size exceeded",
    [SIGVTALRM] = "Virtual alarm",
    [SIGPROF] = "Profiling signal",
    [SIGWINCH] = "Window-size changed",
    [SIGPOLL] = "I/O available",
    [SIGPWR] = "Power loss",
    [SIGSYS] = "Bad system call",
    [32] = "RT32", "RT33", "RT34", "RT35", "RT36", "RT37", "RT38", "RT39",
    "RT40", "RT41", "RT42", "RT43", "RT44", "RT45", "RT46", "RT47", "RT48",
    "RT49", "RT50", "RT51", "RT52", "RT53", "RT54", "RT55", "RT56", "RT57",
    "RT58", "RT59", "RT60", "RT61", "RT62", "RT63", "RT64",
#if _NSIG > 65
    "RT65", "RT66", "RT67", "RT68", "RT69", "RT70", "RT71", "RT72", "RT73",
    "RT74", "RT75", "RT76", "RT77", "RT78", "RT79", "RT80", "RT81", "RT82",
    "RT83", "RT84", "RT85", "RT86", "RT87", "RT88", "RT89", "RT90", "RT91",
    "RT92", "RT93", "RT94", "RT95", "RT96", "RT97", "RT98", "RT99", "RT100",
    "RT101", "RT102", "RT103", "RT104", "RT105", "RT106", "RT107", "RT108",
    "RT109", "RT110", "RT111", "RT112", "RT113", "RT114", "RT115", "RT116",
    "RT117", "RT118", "RT119", "RT120", "RT121", "RT122", "RT123", "RT124",
    "RT125", "RT126", "RT127",
#endif

};

void psignal(int sig, const char *msg)
{
    const char *sigstr = sig + 0u < _NSIG? signames[sig] : "Unknown signal";
    fprintf(stderr, "%s%s%s\n", msg? msg : "", msg && *msg? ": " : "", sigstr);
}
