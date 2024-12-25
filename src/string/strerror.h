E(EPERM          , "Operation not permitted")
E(ENOENT         , "No such file or directory")
E(ESRCH          , "No such process")
E(EINTR          , "Interrupted system call")
E(EIO            , "I/O error")
E(ENXIO          , "No such device or address")
E(E2BIG          , "Argument list too long")
E(ENOEXEC        , "Exec format error")
E(EBADF          , "Bad file number")
E(ECHILD         , "No child processes")
E(EAGAIN         , "Try again")
E(ENOMEM         , "Out of memory")
E(EACCES         , "Permission denied")
E(EFAULT         , "Bad address")
E(ENOTBLK        , "Block device required")
E(EBUSY          , "Device or resource busy")
E(EEXIST         , "File exists")
E(EXDEV          , "Cross-device link")
E(ENODEV         , "No such device")
E(ENOTDIR        , "Not a directory")
E(EISDIR         , "Is a directory")
E(EINVAL         , "Invalid argument")
E(ENFILE         , "File table overflow")
E(EMFILE         , "Too many open files")
E(ENOTTY         , "Not a typewriter")
E(ETXTBSY        , "Text file busy")
E(EFBIG          , "File too large")
E(ENOSPC         , "No space left on device")
E(ESPIPE         , "Illegal seek")
E(EROFS          , "Read-only file system")
E(EMLINK         , "Too many links")
E(EPIPE          , "Broken pipe")
E(EDOM           , "Math argument out of domain of func")
E(ERANGE         , "Math result not representable")
E(EDEADLK        , "Resource deadlock would occur")
E(ENAMETOOLONG   , "File name too long")
E(ENOLCK         , "No record locks available")
E(ENOSYS         , "Invalid system call number")
E(ENOTEMPTY      , "Directory not empty")
E(ELOOP          , "Too many symbolic links encountered")
E(ENOMSG         , "No message of desired type")
E(EIDRM          , "Identifier removed")
E(ECHRNG         , "Channel number out of range")
E(EL2NSYNC       , "Level 2 not synchronized")
E(EL3HLT         , "Level 3 halted")
E(EL3RST         , "Level 3 reset")
E(ELNRNG         , "Link number out of range")
E(EUNATCH        , "Protocol driver not attached")
E(ENOCSI         , "No CSI structure available")
E(EL2HLT         , "Level 2 halted")
E(EBADE          , "Invalid exchange")
E(EBADR          , "Invalid request descriptor")
E(EXFULL         , "Exchange full")
E(ENOANO         , "No anode")
E(EBADRQC        , "Invalid request code")
E(EBADSLT        , "Invalid slot")
E(EBFONT         , "Bad font file format")
E(ENOSTR         , "Device not a stream")
E(ENODATA        , "No data available")
E(ETIME          , "Timer expired")
E(ENOSR          , "Out of streams resources")
E(ENONET         , "Machine is not on the network")
E(ENOPKG         , "Package not installed")
E(EREMOTE        , "Object is remote")
E(ENOLINK        , "Link has been severed")
E(EADV           , "Advertise error")
E(ESRMNT         , "Srmount error")
E(ECOMM          , "Communication error on send")
E(EPROTO         , "Protocol error")
E(EMULTIHOP      , "Multihop attempted")
E(EDOTDOT        , "RFS specific error")
E(EBADMSG        , "Not a data message")
E(EOVERFLOW      , "Value too large for defined data type")
E(ENOTUNIQ       , "Name not unique on network")
E(EBADFD         , "File descriptor in bad state")
E(EREMCHG        , "Remote address changed")
E(ELIBACC        , "Can not access a needed shared library")
E(ELIBBAD        , "Accessing a corrupted shared library")
E(ELIBSCN        , ".lib section in a.out corrupted")
E(ELIBMAX        , "Attempting to link in too many shared libraries")
E(ELIBEXEC       , "Cannot exec a shared library directly")
E(EILSEQ         , "Illegal byte sequence")
E(ERESTART       , "Interrupted system call should be restarted")
E(ESTRPIPE       , "Streams pipe error")
E(EUSERS         , "Too many users")
E(ENOTSOCK       , "Socket operation on non-socket")
E(EDESTADDRREQ   , "Destination address required")
E(EMSGSIZE       , "Message too long")
E(EPROTOTYPE     , "Protocol wrong type for socket")
E(ENOPROTOOPT    , "Protocol not available")
E(EPROTONOSUPPORT, "Protocol not supported")
E(ESOCKTNOSUPPORT, "Socket type not supported")
E(EOPNOTSUPP     , "Operation not supported")
E(EPFNOSUPPORT   , "Protocol family not supported")
E(EAFNOSUPPORT   , "Address family not supported by protocol")
E(EADDRINUSE     , "Address already in use")
E(EADDRNOTAVAIL  , "Cannot assign requested address")
E(ENETDOWN       , "Network is down")
E(ENETUNREACH    , "Network is unreachable")
E(ENETRESET      , "Network dropped connection because of reset")
E(ECONNABORTED   , "Software caused connection abort")
E(ECONNRESET     , "Connection reset by peer")
E(ENOBUFS        , "No buffer space available")
E(EISCONN        , "Transport endpoint is already connected")
E(ENOTCONN       , "Transport endpoint is not connected")
E(ESHUTDOWN      , "Cannot send after transport endpoint shutdown")
E(ETOOMANYREFS   , "Too many references: cannot splice")
E(ETIMEDOUT      , "Connection timed out")
E(ECONNREFUSED   , "Connection refused")
E(EHOSTDOWN      , "Host is down")
E(EHOSTUNREACH   , "No route to host")
E(EALREADY       , "Operation already in progress")
E(EINPROGRESS    , "Operation now in progress")
E(ESTALE         , "Stale file handle")
E(EUCLEAN        , "Structure needs cleaning")
E(ENOTNAM        , "Not a XENIX named type file")
E(ENAVAIL        , "No XENIX semaphores available")
E(EISNAM         , "Is a named type file")
E(EREMOTEIO      , "Remote I/O error")
E(EDQUOT         , "Quota exceeded")
E(ENOMEDIUM      , "No medium found")
E(EMEDIUMTYPE    , "Wrong medium type")
E(ECANCELED      , "Operation Canceled")
E(ENOKEY         , "Required key not available")
E(EKEYEXPIRED    , "Key has expired")
E(EKEYREVOKED    , "Key has been revoked")
E(EKEYREJECTED   , "Key was rejected by service")
E(EOWNERDEAD     , "Owner died")
E(ENOTRECOVERABLE, "State not recoverable")
E(ERFKILL        , "Operation not possible due to RF-kill")
E(EHWPOISON      , "Memory page has hardware error")
#if EWOULDBLOCK != EAGAIN
E(EWOULDBLOCK, "Operation would block")
#endif
#if EDEADLOCK != EDEADLK
E(EDEADLOCK, "File deadlock error")
#endif
