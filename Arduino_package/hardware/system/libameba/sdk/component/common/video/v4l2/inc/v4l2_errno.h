#ifndef _V4L2_ERRNO_H
#define _V4L2_ERRNO_H

#define	V4L2_ESUCCESS	0
#define	V4L2_EPERM		1	/* Operation not permitted */
#define	V4L2_ENOENT		2	/* No such file or directory */
#define	V4L2_ESRCH		3	/* No such process */
#define	V4L2_EINTR		4	/* Interrupted system call */
#define	V4L2_EIO			5	/* I/O error */
#define	V4L2_ENXIO		6	/* No such device or address */
#define	V4L2_E2BIG		7	/* Argument list too long */
#define	V4L2_ENOEXEC		8	/* Exec format error */
#define	V4L2_EBADF		9	/* Bad file number */
#define	V4L2_ECHILD		10	/* No child processes */
#define	V4L2_EAGAIN		11	/* Try again */
#define	V4L2_ENOMEM		12	/* Out of memory */
#define	V4L2_EACCES		13	/* Permission denied */
#define	V4L2_EFAULT		14	/* Bad address */
#define	V4L2_ENOTBLK		15	/* Block device required */
#define	V4L2_EBUSY		16	/* Device or resource busy */
#define	V4L2_EEXIST		17	/* File exists */
#define	V4L2_EXDEV		18	/* Cross-device link */
#define	V4L2_ENODEV		19	/* No such device */
#define	V4L2_ENOTDIR		20	/* Not a directory */
#define	V4L2_EISDIR		21	/* Is a directory */
#define	V4L2_EINVAL		22	/* Invalid argument */
#define	V4L2_ENFILE		23	/* File table overflow */
#define	V4L2_EMFILE		24	/* Too many open files */
#define	V4L2_ENOTTY		25	/* Not a typewriter */
#define	V4L2_ETXTBSY		26	/* Text file busy */
#define	V4L2_EFBIG		27	/* File too large */
#define	V4L2_ENOSPC		28	/* No space left on device */
#define	V4L2_ESPIPE		29	/* Illegal seek */
#define	V4L2_EROFS		30	/* Read-only file system */
#define	V4L2_EMLINK		31	/* Too many links */
#define	V4L2_EPIPE		32	/* Broken pipe */
#define	V4L2_EDOM		33	/* Math argument out of domain of func */
#define	V4L2_ERANGE		34	/* Math result not representable */

#define	V4L2_EDEADLK		35	/* Resource deadlock would occur */
#define	V4L2_ENAMETOOLONG	36	/* File name too long */
#define	V4L2_ENOLCK		37	/* No record locks available */
#define	V4L2_ENOSYS		38	/* Function not implemented */
#define	V4L2_ENOTEMPTY	39	/* Directory not empty */
#define	V4L2_ELOOP		40	/* Too many symbolic links encountered */
#define	V4L2_EWOULDBLOCK	V4L2_EAGAIN	/* Operation would block */
#define	V4L2_ENOMSG		42	/* No message of desired type */
#define	V4L2_EIDRM		43	/* Identifier removed */
#define	V4L2_ECHRNG		44	/* Channel number out of range */
#define	V4L2_EL2NSYNC	45	/* Level 2 not synchronized */
#define	V4L2_EL3HLT		46	/* Level 3 halted */
#define	V4L2_EL3RST		47	/* Level 3 reset */
#define	V4L2_ELNRNG		48	/* Link number out of range */
#define	V4L2_EUNATCH		49	/* Protocol driver not attached */
#define	V4L2_ENOCSI		50	/* No CSI structure available */
#define	V4L2_EL2HLT		51	/* Level 2 halted */
#define	V4L2_EBADE		52	/* Invalid exchange */
#define	V4L2_EBADR		53	/* Invalid request descriptor */
#define	V4L2_EXFULL		54	/* Exchange full */
#define	V4L2_ENOANO		55	/* No anode */
#define	V4L2_EBADRQC		56	/* Invalid request code */
#define	V4L2_EBADSLT		57	/* Invalid slot */

#define	V4L2_EDEADLOCK	V4L2_EDEADLK

#define	V4L2_EBFONT		59	/* Bad font file format */
#define	V4L2_ENOSTR		60	/* Device not a stream */
#define	V4L2_ENODATA		61	/* No data available */
#define	V4L2_ETIME		62	/* Timer expired */
#define	V4L2_ENOSR		63	/* Out of streams resources */
#define	V4L2_ENONET		64	/* Machine is not on the network */
#define	V4L2_ENOPKG		65	/* Package not installed */
#define	V4L2_EREMOTE		66	/* Object is remote */
#define	V4L2_ENOLINK		67	/* Link has been severed */
#define	V4L2_EADV		68	/* Advertise error */
#define	V4L2_ESRMNT		69	/* Srmount error */
#define	V4L2_ECOMM		70	/* Communication error on send */
#define	V4L2_EPROTO		71	/* Protocol error */
#define	V4L2_EMULTIHOP	72	/* Multihop attempted */
#define	V4L2_EDOTDOT		73	/* RFS specific error */
#define	V4L2_EBADMSG		74	/* Not a data message */
#define	V4L2_EOVERFLOW	75	/* Value too large for defined data type */
#define	V4L2_ENOTUNIQ	76	/* Name not unique on network */
#define	V4L2_EBADFD		77	/* File descriptor in bad state */
#define	V4L2_EREMCHG		78	/* Remote address changed */
#define	V4L2_ELIBACC		79	/* Can not access a needed shared library */
#define	V4L2_ELIBBAD		80	/* Accessing a corrupted shared library */
#define	V4L2_ELIBSCN		81	/* .lib section in a.out corrupted */
#define	V4L2_ELIBMAX		82	/* Attempting to link in too many shared libraries */
#define	V4L2_ELIBEXEC	83	/* Cannot exec a shared library directly */
#define	V4L2_EILSEQ		84	/* Illegal byte sequence */
#define	V4L2_ERESTART	85	/* Interrupted system call should be restarted */
#define	V4L2_ESTRPIPE	86	/* Streams pipe error */
#define	V4L2_EUSERS		87	/* Too many users */
#define	V4L2_ENOTSOCK	88	/* Socket operation on non-socket */
#define	V4L2_EDESTADDRREQ	89	/* Destination address required */
#define	V4L2_EMSGSIZE	90	/* Message too long */
#define	V4L2_EPROTOTYPE	91	/* Protocol wrong type for socket */
#define	V4L2_ENOPROTOOPT	92	/* Protocol not available */
#define	V4L2_EPROTONOSUPPORT	93	/* Protocol not supported */
#define	V4L2_ESOCKTNOSUPPORT	94	/* Socket type not supported */
#define	V4L2_EOPNOTSUPP	95	/* Operation not supported on transport endpoint */
#define	V4L2_EPFNOSUPPORT	96	/* Protocol family not supported */
#define	V4L2_EAFNOSUPPORT	97	/* Address family not supported by protocol */
#define	V4L2_EADDRINUSE	98	/* Address already in use */
#define	V4L2_EADDRNOTAVAIL	99	/* Cannot assign requested address */
#define	V4L2_ENETDOWN	100	/* Network is down */
#define	V4L2_ENETUNREACH	101	/* Network is unreachable */
#define	V4L2_ENETRESET	102	/* Network dropped connection because of reset */
#define	V4L2_ECONNABORTED	103	/* Software caused connection abort */
#define	V4L2_ECONNRESET	104	/* Connection reset by peer */
#define	V4L2_ENOBUFS		105	/* No buffer space available */
#define	V4L2_EISCONN		106	/* Transport endpoint is already connected */
#define	V4L2_ENOTCONN	107	/* Transport endpoint is not connected */
#define	V4L2_ESHUTDOWN	108	/* Cannot send after transport endpoint shutdown */
#define	V4L2_ETOOMANYREFS	109	/* Too many references: cannot splice */
#define	V4L2_ETIMEDOUT	110	/* Connection timed out */
#define	V4L2_ECONNREFUSED	111	/* Connection refused */
#define	V4L2_EHOSTDOWN	112	/* Host is down */
#define	V4L2_EHOSTUNREACH	113	/* No route to host */
#define	V4L2_EALREADY	114	/* Operation already in progress */
#define	V4L2_EINPROGRESS	115	/* Operation now in progress */
#define	V4L2_ESTALE		116	/* Stale NFS file handle */
#define	V4L2_EUCLEAN		117	/* Structure needs cleaning */
#define	V4L2_ENOTNAM		118	/* Not a XENIX named type file */
#define	V4L2_ENAVAIL		119	/* No XENIX semaphores available */
#define	V4L2_EISNAM		120	/* Is a named type file */
#define	V4L2_EREMOTEIO	121	/* Remote I/O error */
#define	V4L2_EDQUOT		122	/* Quota exceeded */

#define	V4L2_ENOMEDIUM	123	/* No medium found */
#define	V4L2_EMEDIUMTYPE	124	/* Wrong medium type */
#define	V4L2_ECANCELED	125	/* Operation Canceled */
#define	V4L2_ENOKEY		126	/* Required key not available */
#define	V4L2_EKEYEXPIRED	127	/* Key has expired */
#define	V4L2_EKEYREVOKED	128	/* Key has been revoked */
#define	V4L2_EKEYREJECTED	129	/* Key was rejected by service */

/* for robust mutexes */
#define	V4L2_EOWNERDEAD	130	/* Owner died */
#define	V4L2_ENOTRECOVERABLE	131	/* State not recoverable */

#define V4L2_ERFKILL		132	/* Operation not possible due to RF-kill */

#define V4L2_EHWPOISON	133	/* Memory page has hardware error */

#endif
