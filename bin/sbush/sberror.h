#ifndef ERRNO_H
#define ERRNO_H

#define EPERM            1      /* Operation not permitted */
#define ENOENT           2      /* No such file or directory */
#define ESRCH            3      /* No such process */
#define EINTR            4      /* Interrupted system call */
#define EIO              5      /* I/O error */
#define ENXIO            6      /* No such device or address */
#define E2BIG            7      /* Argument list too long */
#define ENOEXEC          8      /* Exec format error */
#define EBADF            9      /* Bad file number */
#define ECHILD          10      /* No child processes */
#define EAGAIN          11      /* Try again */
#define ENOMEM          12      /* Out of memory */
#define EACCES          13      /* Permission denied */
#define EFAULT          14      /* Bad address */
#define ENOTBLK         15      /* Block device required */
#define EBUSY           16      /* Device or resource busy */
#define EEXIST          17      /* File exists */
#define EXDEV           18      /* Cross-device link */
#define ENODEV          19      /* No such device */
#define ENOTDIR         20      /* Not a directory */
#define EISDIR          21      /* Is a directory */
#define EINVAL          22      /* Invalid argument */
#define ENFILE          23      /* File table overflow */
#define EMFILE          24      /* Too many open files */
#define ENOTTY          25      /* Not a typewriter */
#define ETXTBSY         26      /* Text file busy */
#define EFBIG           27      /* File too large */
#define ENOSPC          28      /* No space left on device */
#define ESPIPE          29      /* Illegal seek */
#define EROFS           30      /* Read-only file system */
#define EMLINK          31      /* Too many links */
#define EPIPE           32      /* Broken pipe */
#define EDOM            33      /* Math argument out of domain of func */
#define ERANGE          34      /* Math result not representable */

const char *sys_errlist[] =
{
  "Error 0",
  "Operation not permitted",                    /* EPERM */
  "No such file or directory",                  /* ENOENT */
  "No such process",                            /* ESRCH */
  "Interrupted system call",                    /* EINTR */
  "Input/output error",                         /* EIO */
  "No such device or address",                  /* ENXIO */
  "Argument list too long",                     /* E2BIG */
  "Exec format error",                          /* ENOEXEC */
  "Bad file descriptor",                        /* EBADF */

  "No child processes",                         /* ECHILD */
  "Resource temporarily unavailable",           /* EAGAIN */
  "Cannot allocate memory",                     /* ENOMEM */
  "Permission denied",                          /* EACCES */
  "Bad address",                                /* EFAULT */
  "Block device required",                      /* ENOTBLK */
  "Device busy",                                /* EBUSY */
  "File exists",                                /* EEXIST */
  "Invalid cross-device link",                  /* EXDEX */
  "Operation not supported by device",          /* ENODEV */

  "Not a directory",                            /* ENOTDIR */
  "Is a directory",                             /* EISDIR */
  "Invalid argument",                           /* EINVAL */
  "Too many open files in system",              /* ENFILE */
  "Too many open files",                        /* EMFILE */
  "Inappropriate ioctl for device",             /* ENOTTY */
  "Text file busy",                             /* ETXTBSY */
  "File too large",                             /* EFBIG */
  "No space left on device",                    /* ENOSPC */
  "Illegal seek",                               /* ESPIPE */

  "Read-only file system",                      /* EROFS */
  "Too many links",                             /* EMLINK */
  "Broken pipe",                                /* EPIPE */
  "Argument out of domain",                     /* EDOM */
  "Range error",                                /* ERANGE */
  "Operation would block",                      /* EWOULDBLOCK */
  "Operation now in progress",                  /* EINPROGRESS */
  "Operation already in progress",              /* EALREADY */
  "Socket operation on non-socket",             /* ENOTSOCK */
  "Destination address required",               /* EDESTADDRREQ */

  "Message too long",                           /* EMSGSIZE */
  "Protocol wrong type for socket",             /* EPROTOTYPE */
  "Option not supported by protocol",           /* ENOPROTOOPT */
  "Protocol not supported",                     /* EPROTONOSUPPORT */
  "Socket type not supported",                  /* ESOCKTNOSUPPORT */
  "Operation not supported",                    /* EOPNOTSUPP */
  "Protocol family not supported",              /* EPFNOSUPPORT */
  "Address family not supported by protocol family",    /* EAFNOSUPPORT */
  "Address already in use",                     /* EADDRINUSE */
  "Can't assign requested address",             /* EADDRNOTAVAIL */

  "Network is down",                            /* ENETDOWN */
  "Network unreachable",                        /* ENETUNREACH */
  "Network dropped connection on reset",        /* ENETRESET */
  "Software caused connection abort",           /* ECONNABORTED */
  "Connection reset by peer",                   /* ECONNRESET */
  "No buffer space available",                  /* ENOBUFS */
  "Socket is already connected",                /* EISCONN */
  "Socket is not connected",                    /* ENOTCONN */
  "Can't send after socket shutdown",           /* ESHUTDOWN */
  "Too many references: can't splice",          /* ETOOMANYREFS */

  "Connection timed out",                       /* ETIMEDOUT */
  "Connection refused",                         /* ECONNREFUSED */
  "Too many levels of symbolic links",          /* ELOOP */
  "File name too long",                         /* ENAMETOOLONG */
  "Host is down",                               /* EHOSTDOWN */
  "Host unreachable",                           /* EHOSTUNREACH */
  "Directory not empty",                        /* ENOTEMPTY */
  "Too many processes",                         /* EPROCLIM */
  "Too many users",                             /* EUSERS */
  "Disc quota exceeded",                        /* EDQUOT */

  "Stale NFS file handle",                      /* ESTALE */
  "Too many levels of remote in path",          /* EREMOTE */
  "Not a stream device",                        /* ENOSTR */
  "Timer expired",                              /* ETIME */
  "Out of stream resources",                    /* ENOSR */
  "No message of desired type",                 /* ENOMSG */
  "Not a data message",                         /* EBADMSG */
  "Identifier removed",                         /* EIDRM */
  "Deadlock situation detected/avoided",        /* EDEADLK */
  "No record locks available",                  /* ENOLCK */

  "No suitable message on queue",               /* 80 */
  "Identified removed from system",             /* 81 */
  "Wrong version of shared library",            /* ELIBVER */
  "Permission denied (shared library)",         /* ELIBACC */
  "Shared libraries nested too deeply",         /* ELIBLIM */
  "Shared library file not found",              /* ELIBNOENT */
  "Shared library exec format error",           /* ELIBNOEXEC */
  "Function not implemented",                   /* ENOSYS */
  "RISC OS error",                              /* EOPSYS */
  "Signal Received"                             /* as flagged by SIG_ERR */
};

const char * strerror(int errno)
{
    return  sys_errlist[errno];
}
#endif
