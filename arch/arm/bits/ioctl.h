#define _IOC_NONE       0U
#define _IOC_WRITE      1U
#define _IOC_READ       2U
#define _IOC(dir,type,nr,size)  ((dir)<<30|(size)<<16|(type)<<8|(nr))
#define _IO(type,nr)            _IOC(_IOC_NONE,type,nr,0)
#define _IOR(type,nr,size)      _IOC(_IOC_READ,type,nr,sizeof (size))
#define _IOW(type,nr,size)      _IOC(_IOC_WRITE,type,nr,sizeof (size))
#define _IOWR(type,nr,size)     _IOC(_IOC_WRITE|_IOC_READ,type,nr,sizeof (size))
