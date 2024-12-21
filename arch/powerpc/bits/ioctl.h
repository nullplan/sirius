#define _IOC_NONE       1U
#define _IOC_READ       2U
#define _IOC_WRITE      4U
#define _IOC(dir,type,nr,size)  ((dir)<<29|(size)<<16|(type)<<8|(nr))
#define _IO(type,nr)            _IOC(_IOC_NONE,type,nr,0)
#define _IOR(type,nr,size)      _IOC(_IOC_READ,type,nr,sizeof (size))
#define _IOW(type,nr,size)      _IOC(_IOC_WRITE,type,nr,sizeof (size))
#define _IOWR(type,nr,size)     _IOC(_IOC_WRITE|_IOC_READ,type,nr,sizeof (size))
