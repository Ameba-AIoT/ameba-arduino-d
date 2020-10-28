#ifndef _USB_ERRNO_H
#define _USB_ERRNO_H

#define	USB_ESUCCESS	0
#define	USB_EPERM		1	/* Operation not permitted */
#define	USB_ENOENT		2	/* No such file or directory */
#define	USB_ESRCH		3	/* No such process */
#define	USB_EINTR		4	/* Interrupted system call */
#define	USB_EIO			5	/* I/O error */
#define	USB_ENXIO		6	/* No such device or address */
#define	USB_E2BIG		7	/* Argument list too long */
#define	USB_ENOEXEC		8	/* Exec format error */
#define	USB_EBADF		9	/* Bad file number */
#define	USB_ECHILD		10	/* No child processes */
#define	USB_EAGAIN		11	/* Try again */
#define	USB_ENOMEM		12	/* Out of memory */
#define	USB_EACCES		13	/* Permission denied */
#define	USB_EFAULT		14	/* Bad address */
#define	USB_ENOTBLK		15	/* Block device required */
#define	USB_EBUSY		16	/* Device or resource busy */
#define	USB_EEXIST		17	/* File exists */
#define	USB_EXDEV		18	/* Cross-device link */
#define	USB_ENODEV		19	/* No such device */
#define	USB_ENOTDIR		20	/* Not a directory */
#define	USB_EISDIR		21	/* Is a directory */
#define	USB_EINVAL		22	/* Invalid argument */
#define	USB_ENFILE		23	/* File table overflow */
#define	USB_EMFILE		24	/* Too many open files */
#define	USB_ENOTTY		25	/* Not a typewriter */
#define	USB_ETXTBSY		26	/* Text file busy */
#define	USB_EFBIG		27	/* File too large */
#define	USB_ENOSPC		28	/* No space left on device */
#define	USB_ESPIPE		29	/* Illegal seek */
#define	USB_EROFS		30	/* Read-only file system */
#define	USB_EMLINK		31	/* Too many links */
#define	USB_EPIPE		32	/* Broken pipe */
#define	USB_EDOM		33	/* Math argument out of domain of func */
#define	USB_ERANGE		34	/* Math result not representable */

#define	USB_EDEADLK		35	/* Resource deadlock would occur */
#define	USB_ENAMETOOLONG	36	/* File name too long */
#define	USB_ENOLCK		37	/* No record locks available */
#define	USB_ENOSYS		38	/* Function not implemented */
#define	USB_ENOTEMPTY	39	/* Directory not empty */
#define	USB_ELOOP		40	/* Too many symbolic links encountered */
#define	USB_EWOULDBLOCK	USB_EAGAIN	/* Operation would block */
#define	USB_ENOMSG		42	/* No message of desired type */
#define	USB_EIDRM		43	/* Identifier removed */
#define	USB_ECHRNG		44	/* Channel number out of range */
#define	USB_EL2NSYNC	45	/* Level 2 not synchronized */
#define	USB_EL3HLT		46	/* Level 3 halted */
#define	USB_EL3RST		47	/* Level 3 reset */
#define	USB_ELNRNG		48	/* Link number out of range */
#define	USB_EUNATCH		49	/* Protocol driver not attached */
#define	USB_ENOCSI		50	/* No CSI structure available */
#define	USB_EL2HLT		51	/* Level 2 halted */
#define	USB_EBADE		52	/* Invalid exchange */
#define	USB_EBADR		53	/* Invalid request descriptor */
#define	USB_EXFULL		54	/* Exchange full */
#define	USB_ENOANO		55	/* No anode */
#define	USB_EBADRQC		56	/* Invalid request code */
#define	USB_EBADSLT		57	/* Invalid slot */

#define	USB_EDEADLOCK	USB_EDEADLK

#define	USB_EBFONT		59	/* Bad font file format */
#define	USB_ENOSTR		60	/* Device not a stream */
#define	USB_ENODATA		61	/* No data available */
#define	USB_ETIME		62	/* Timer expired */
#define	USB_ENOSR		63	/* Out of streams resources */
#define	USB_ENONET		64	/* Machine is not on the network */
#define	USB_ENOPKG		65	/* Package not installed */
#define	USB_EREMOTE		66	/* Object is remote */
#define	USB_ENOLINK		67	/* Link has been severed */
#define	USB_EADV		68	/* Advertise error */
#define	USB_ESRMNT		69	/* Srmount error */
#define	USB_ECOMM		70	/* Communication error on send */
#define	USB_EPROTO		71	/* Protocol error */
#define	USB_EMULTIHOP	72	/* Multihop attempted */
#define	USB_EDOTDOT		73	/* RFS specific error */
#define	USB_EBADMSG		74	/* Not a data message */
#define	USB_EOVERFLOW	75	/* Value too large for defined data type */
#define	USB_ENOTUNIQ	76	/* Name not unique on network */
#define	USB_EBADFD		77	/* File descriptor in bad state */
#define	USB_EREMCHG		78	/* Remote address changed */
#define	USB_ELIBACC		79	/* Can not access a needed shared library */
#define	USB_ELIBBAD		80	/* Accessing a corrupted shared library */
#define	USB_ELIBSCN		81	/* .lib section in a.out corrupted */
#define	USB_ELIBMAX		82	/* Attempting to link in too many shared libraries */
#define	USB_ELIBEXEC	83	/* Cannot exec a shared library directly */
#define	USB_EILSEQ		84	/* Illegal byte sequence */
#define	USB_ERESTART	85	/* Interrupted system call should be restarted */
#define	USB_ESTRPIPE	86	/* Streams pipe error */
#define	USB_EUSERS		87	/* Too many users */
#define	USB_ENOTSOCK	88	/* Socket operation on non-socket */
#define	USB_EDESTADDRREQ	89	/* Destination address required */
#define	USB_EMSGSIZE	90	/* Message too long */
#define	USB_EPROTOTYPE	91	/* Protocol wrong type for socket */
#define	USB_ENOPROTOOPT	92	/* Protocol not available */
#define	USB_EPROTONOSUPPORT	93	/* Protocol not supported */
#define	USB_ESOCKTNOSUPPORT	94	/* Socket type not supported */
#define	USB_EOPNOTSUPP	95	/* Operation not supported on transport endpoint */
#define	USB_EPFNOSUPPORT	96	/* Protocol family not supported */
#define	USB_EAFNOSUPPORT	97	/* Address family not supported by protocol */
#define	USB_EADDRINUSE	98	/* Address already in use */
#define	USB_EADDRNOTAVAIL	99	/* Cannot assign requested address */
#define	USB_ENETDOWN	100	/* Network is down */
#define	USB_ENETUNREACH	101	/* Network is unreachable */
#define	USB_ENETRESET	102	/* Network dropped connection because of reset */
#define	USB_ECONNABORTED	103	/* Software caused connection abort */
#define	USB_ECONNRESET	104	/* Connection reset by peer */
#define	USB_ENOBUFS		105	/* No buffer space available */
#define	USB_EISCONN		106	/* Transport endpoint is already connected */
#define	USB_ENOTCONN	107	/* Transport endpoint is not connected */
#define	USB_ESHUTDOWN	108	/* Cannot send after transport endpoint shutdown */
#define	USB_ETOOMANYREFS	109	/* Too many references: cannot splice */
#define	USB_ETIMEDOUT	110	/* Connection timed out */
#define	USB_ECONNREFUSED	111	/* Connection refused */
#define	USB_EHOSTDOWN	112	/* Host is down */
#define	USB_EHOSTUNREACH	113	/* No route to host */
#define	USB_EALREADY	114	/* Operation already in progress */
#define	USB_EINPROGRESS	115	/* Operation now in progress */
#define	USB_ESTALE		116	/* Stale NFS file handle */
#define	USB_EUCLEAN		117	/* Structure needs cleaning */
#define	USB_ENOTNAM		118	/* Not a XENIX named type file */
#define	USB_ENAVAIL		119	/* No XENIX semaphores available */
#define	USB_EISNAM		120	/* Is a named type file */
#define	USB_EREMOTEIO	121	/* Remote I/O error */
#define	USB_EDQUOT		122	/* Quota exceeded */

#define	USB_ENOMEDIUM	123	/* No medium found */
#define	USB_EMEDIUMTYPE	124	/* Wrong medium type */
#define	USB_ECANCELED	125	/* Operation Canceled */
#define	USB_ENOKEY		126	/* Required key not available */
#define	USB_EKEYEXPIRED	127	/* Key has expired */
#define	USB_EKEYREVOKED	128	/* Key has been revoked */
#define	USB_EKEYREJECTED	129	/* Key was rejected by service */

/* for robust mutexes */
#define	USB_EOWNERDEAD	130	/* Owner died */
#define	USB_ENOTRECOVERABLE	131	/* State not recoverable */

#define USB_ERFKILL		132	/* Operation not possible due to RF-kill */

#define USB_EHWPOISON	133	/* Memory page has hardware error */

#endif
