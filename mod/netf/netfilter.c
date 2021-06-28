/**********************************************************
 > File Name: hello.c
 > Author: ims
 > Created Time: Fri Apr  9 19:15:35 2021
 *********************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <uapi/linux/netfilter_bridge.h>
#include <uapi/linux/netfilter_ipv4.h>
#include <uapi/linux/netfilter.h>
#include <net/net_namespace.h>
#include <linux/ip.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hcamal");



//struct nf_hook_ops {
//    struct list_head    list;
//
//    /* User fills in from here down. */
//    nf_hookfn        *hook; /* 钩子函数 */
//    struct net_device    *dev; /* 设备 */
//    void            *priv; /* 私有数据 */
//    u_int8_t        pf; /* 协议族 */
//    unsigned int        hooknum; /* 钩子点 */
//    /* Hooks are ordered in ascending priority. */
//    int            priority; /* 优先级 */
//};
//
//182-/* Function to register/unregister hook points. */
//183:int nf_register_net_hook(struct net *net, const struct nf_hook_ops *ops);
//184:void nf_unregister_net_hook(struct net *net, const struct nf_hook_ops *ops);

typedef struct{
    int a;
    int b;
}st;
/* IP地址转字符串 */
static void IP2Str(char *ipaddr, int size, uint32_t ip)  
{  
    snprintf(ipaddr, size, "%d.%d.%d.%d", ( ip >> 24 ) & 0xff  
                                        , ( ip >> 16 ) & 0xff  
                                        , ( ip >> 8 ) & 0xff  
                                        , ip & 0xff);
    return;
} 
#define NIPQUAD(addr) \
        ((unsigned char *)&addr)[0], \
        ((unsigned char *)&addr)[1], \
        ((unsigned char *)&addr)[2], \
        ((unsigned char *)&addr)[3]
#define NIPQUAD_FMT "%u.%u.%u.%u"

const char *src_ip="172.29.192.1";
unsigned int my_nf_hookfn(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
    struct iphdr *pstIpHdr = NULL;
    char szIpstr[20] = {0};
    struct ethhdr *eth_header;
    struct iphdr *ip_header;
    
    eth_header = (struct ethhdr *)(skb_mac_header(skb));
    ip_header = (struct iphdr *)(skb_network_header(skb));
    if (unlikely(NULL == skb))
    {
        return NF_ACCEPT;
    }
    pstIpHdr = ip_hdr(skb);
    if (unlikely(NULL == pstIpHdr))
    {
        return NF_ACCEPT;
    }
    if (pstIpHdr->protocol != IPPROTO_ICMP)
    {
        return NF_ACCEPT;
    }
    /* 网络序转主机序，然后转成字符串 */
    IP2Str(szIpstr, sizeof(szIpstr), ntohl(pstIpHdr->saddr));
    if (0 == strcmp(szIpstr, src_ip))   /* 字符串比较 */
    {
        printk(KERN_INFO "Recv icmp packet from %s\n", src_ip);   /* printk打印 */
    }

    if('0' != ip_header->saddr){
        printk("src IP:'"NIPQUAD_FMT"', dst IP:'"NIPQUAD_FMT"' \n",
        NIPQUAD(ip_header->saddr), NIPQUAD(ip_header->daddr));
    }
    return NF_ACCEPT;
}

static struct nf_hook_ops hook_ops ={
    .hook = my_nf_hookfn,
    .pf = NFPROTO_INET,
    .hooknum = NF_BR_PRE_ROUTING,
    .priority = NF_IP_PRI_FIRST
};

int netf_init(void)
{
    int ret = 0;
    printk(KERN_INFO "[%s] entry\n", __func__);

    ret = nf_register_net_hook(&init_net, &hook_ops);
    if (0 != ret)
    {
        printk(KERN_WARNING "nf_register_net_hook failed,ret%d\n", ret);
        return ret;
    }
    else
    {
        printk(KERN_WARNING "nf_register_net_hook succ\n");
    }

    return 0;
}

void netf_exit(void)
{
    nf_unregister_net_hook(&init_net, &hook_ops);
    printk(KERN_INFO "[%s]nf_unregister_net_hook \n", __func__);
}

module_init(netf_init);
module_exit(netf_exit);
