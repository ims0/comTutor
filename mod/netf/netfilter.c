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
MODULE_AUTHOR("ims");

/* IP地址转字符串 */
static void ip_int_2_str(char *ipaddr, int size, uint32_t ip)
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

const char *const_src_ip="172.29.192.1";
static char ip_str_array[16] = {0};
unsigned int my_nf_hookfn(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
    struct iphdr *ip_header = NULL;
    struct ethhdr *ether_header;
    if (unlikely(NULL == skb)) {
        return NF_ACCEPT;
    }
    ether_header = (struct ethhdr *)(skb_mac_header(skb)); //unuse
    ip_header = ip_hdr(skb);
    if (unlikely(NULL == ip_header)) {
        return NF_ACCEPT;
    }
    if (ip_header->protocol != IPPROTO_ICMP) {
        return NF_ACCEPT;
    }
    ip_int_2_str(ip_str_array, sizeof(ip_str_array), ntohl(ip_header->saddr));
    if (0 == strcmp(ip_str_array, const_src_ip))
    {
        printk(KERN_INFO "[%s]:Recv icmp packet from %s\n", __func__, const_src_ip);
    }

    if('0' != ip_header->saddr){
        printk("[%s]:src IP:'"NIPQUAD_FMT"', dst IP:'"NIPQUAD_FMT"' \n", __func__,
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
    printk(KERN_INFO "[%s]:customer module netf entry\n", __func__);

    ret = nf_register_net_hook(&init_net, &hook_ops);
    if (0 != ret) {
        printk(KERN_WARNING "[%s]:call the nf_register_net_hook failed, ret:%d\n", __func__, ret);
        return ret;
    } else {
        printk(KERN_WARNING "[%s]:call the nf_register_net_hook succ.\n", __func__);
    }
    return 0;
}

void netf_exit(void)
{
    nf_unregister_net_hook(&init_net, &hook_ops);
    printk(KERN_INFO "[%s]call the nf_unregister_net_hook, module exit.\n", __func__);
}

module_init(netf_init);
module_exit(netf_exit);
