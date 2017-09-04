#if HAVE_LINUX_IF_H
// #include <linux/if.h>
#elif HAVE_NET_IF_H
#include <net/if.h>
#endif

#if HAVE_LINUX_NETDEVICE_H
// #include <linux/netdevice.h>
#endif
#if HAVE_IFADDRS_H
#include <ifaddrs.h>
#endif