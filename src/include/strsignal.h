S(0, "EXIT", "No signal")
S(SIGHUP, "HUP", "Hang-up")
S(SIGINT, "INT", "Interrupt")
S(SIGQUIT, "QUIT", "Quit")
S(SIGILL, "ILL",  "Illegal instruction")
S(SIGTRAP, "TRAP",  "Debug trap")
S(SIGABRT, "ABRT",  "Abort")
S(SIGBUS, "BUS",  "Bus error")
S(SIGFPE, "FPE",  "Floating-point exception")
S(SIGKILL, "KILL",  "Killed")
S(SIGUSR1, "USR1",  "User-defined signal 1")
S(SIGSEGV, "SEGV",  "Segmentation violation")
S(SIGUSR2, "USR2",  "User-defined signal 2")
S(SIGPIPE, "PIPE",  "Write to closed pipe")
S(SIGALRM, "ALRM",  "Alarm")
S(SIGTERM, "TERM",  "Terminated")
S(SIGSTKFLT, "STKFLT",  "Stack fault")
S(SIGCHLD, "CHLD",  "Child process")
S(SIGCONT, "CONT",  "Continued")
S(SIGSTOP, "STOP",  "Stopped")
S(SIGTSTP, "TSTP",  "Stopped (tty)")
S(SIGTTIN, "TTIN",  "Stopped (tty input)")
S(SIGTTOU, "TTOU",  "Stopped (tty output)")
S(SIGURG, "URG",  "Urgent data")
S(SIGXCPU, "XCPU",  "CPU time exceeded")
S(SIGXFSZ, "XFSZ",  "File size exceeded")
S(SIGVTALRM, "VTALRM",  "Virtual alarm")
S(SIGPROF, "PROF",  "Profiling signal")
S(SIGWINCH, "WINCH",  "Window size changed")
S(SIGPOLL, "POLL",  "Polled I/O")
S(SIGPWR, "PWR",  "Power failure")
S(SIGSYS, "SYS",  "Bad system call")
S(32, "RESV1", "RT32")
S(33, "RESV2", "RT33")
S(34, "RESV3", "RT34")
#if _NSIG == 65
S(35, "RTMIN", "RT35")
S(36, "RTMIN+1", "RT36")
S(37, "RTMIN+2", "RT37")
S(38, "RTMIN+3", "RT38")
S(39, "RTMIN+4", "RT39")
S(40, "RTMIN+5", "RT40")
S(41, "RTMIN+6", "RT41")
S(42, "RTMIN+7", "RT42")
S(43, "RTMIN+8", "RT43")
S(44, "RTMIN+9", "RT44")
S(45, "RTMIN+10", "RT45")
S(46, "RTMIN+11", "RT46")
S(47, "RTMIN+12", "RT47")
S(48, "RTMIN+13", "RT48")
S(49, "RTMIN+14", "RT49")
S(50, "RTMAX-14", "RT50")
S(51, "RTMAX-13", "RT51")
S(52, "RTMAX-12", "RT52")
S(53, "RTMAX-11", "RT53")
S(54, "RTMAX-10", "RT54")
S(55, "RTMAX-9", "RT55")
S(56, "RTMAX-8", "RT56")
S(57, "RTMAX-7", "RT57")
S(58, "RTMAX-6", "RT58")
S(59, "RTMAX-5", "RT59")
S(60, "RTMAX-4", "RT60")
S(61, "RTMAX-3", "RT61")
S(62, "RTMAX-2", "RT62")
S(63, "RTMAX-1", "RT63")
S(64, "RTMAX", "RT64")
#else
S(35, "RTMIN", "RT35")
S(36, "RTMIN+1", "RT36")
S(37, "RTMIN+2", "RT37")
S(38, "RTMIN+3", "RT38")
S(39, "RTMIN+4", "RT39")
S(40, "RTMIN+5", "RT40")
S(41, "RTMIN+6", "RT41")
S(42, "RTMIN+7", "RT42")
S(43, "RTMIN+8", "RT43")
S(44, "RTMIN+9", "RT44")
S(45, "RTMIN+10", "RT45")
S(46, "RTMIN+11", "RT46")
S(47, "RTMIN+12", "RT47")
S(48, "RTMIN+13", "RT48")
S(49, "RTMIN+14", "RT49")
S(50, "RTMIN+15", "RT50")
S(51, "RTMIN+16", "RT51")
S(52, "RTMIN+17", "RT52")
S(53, "RTMIN+18", "RT53")
S(54, "RTMIN+19", "RT54")
S(55, "RTMIN+20", "RT55")
S(56, "RTMIN+21", "RT56")
S(57, "RTMIN+22", "RT57")
S(58, "RTMIN+23", "RT58")
S(59, "RTMIN+24", "RT59")
S(60, "RTMIN+25", "RT60")
S(61, "RTMIN+26", "RT61")
S(62, "RTMIN+27", "RT62")
S(63, "RTMIN+28", "RT63")
S(64, "RTMIN+29", "RT64")
S(65, "RTMIN+30", "RT65")
S(66, "RTMIN+31", "RT66")
S(67, "RTMIN+32", "RT67")
S(68, "RTMIN+33", "RT68")
S(69, "RTMIN+34", "RT69")
S(70, "RTMIN+35", "RT70")
S(71, "RTMIN+36", "RT71")
S(72, "RTMIN+37", "RT72")
S(73, "RTMIN+38", "RT73")
S(74, "RTMIN+39", "RT74")
S(75, "RTMIN+40", "RT75")
S(76, "RTMIN+41", "RT76")
S(77, "RTMIN+42", "RT77")
S(78, "RTMIN+43", "RT78")
S(79, "RTMIN+44", "RT79")
S(80, "RTMIN+45", "RT80")
S(81, "RTMIN+46", "RT81")
S(82, "RTMAX-45", "RT82")
S(83, "RTMAX-44", "RT83")
S(84, "RTMAX-43", "RT84")
S(85, "RTMAX-42", "RT85")
S(86, "RTMAX-41", "RT86")
S(87, "RTMAX-40", "RT87")
S(88, "RTMAX-39", "RT88")
S(89, "RTMAX-38", "RT89")
S(90, "RTMAX-37", "RT90")
S(91, "RTMAX-36", "RT91")
S(92, "RTMAX-35", "RT92")
S(93, "RTMAX-34", "RT93")
S(94, "RTMAX-33", "RT94")
S(95, "RTMAX-32", "RT95")
S(96, "RTMAX-31", "RT96")
S(97, "RTMAX-30", "RT97")
S(98, "RTMAX-29", "RT98")
S(99, "RTMAX-28", "RT99")
S(100, "RTMAX-27", "RT100")
S(101, "RTMAX-26", "RT101")
S(102, "RTMAX-25", "RT102")
S(103, "RTMAX-24", "RT103")
S(104, "RTMAX-23", "RT104")
S(105, "RTMAX-22", "RT105")
S(106, "RTMAX-21", "RT106")
S(107, "RTMAX-20", "RT107")
S(108, "RTMAX-19", "RT108")
S(109, "RTMAX-18", "RT109")
S(110, "RTMAX-17", "RT110")
S(111, "RTMAX-16", "RT111")
S(112, "RTMAX-15", "RT112")
S(113, "RTMAX-14", "RT113")
S(114, "RTMAX-13", "RT114")
S(115, "RTMAX-12", "RT115")
S(116, "RTMAX-11", "RT116")
S(117, "RTMAX-10", "RT117")
S(118, "RTMAX-9", "RT118")
S(119, "RTMAX-8", "RT119")
S(120, "RTMAX-7", "RT120")
S(121, "RTMAX-6", "RT121")
S(122, "RTMAX-5", "RT122")
S(123, "RTMAX-4", "RT123")
S(124, "RTMAX-3", "RT124")
S(125, "RTMAX-2", "RT125")
S(126, "RTMAX-1", "RT126")
S(127, "RTMAX", "RT127")
#endif
