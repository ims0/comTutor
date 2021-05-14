/*********************************************************
 *
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

struct net net_obj={0};
typedef struct{
    int a;
    int b;
}st;

unsigned int my_nf_hookfn(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
    return 0;
}

static struct nf_hook_ops hook_ops ={
    .hook = my_nf_hookfn,
    .pf = NFPROTO_IPV4,
    .hooknum = NF_BR_PRE_ROUTING,
    .priority = NF_IP_PRI_FIRST
};

int hello_init(void)
{
    int ret = 0;
    printk(KERN_INFO "Hello World \n");
    ret = nf_register_net_hook(&net_obj, &hook_ops);
    if (0 != ret)
    {
        printk(KERN_WARNING "nf_register_net_hook failed,ret%d\n", ret);
        return ret;
    }

    return 0;
}

void hello_exit(void)
{
    printk(KERN_INFO "Goodbye World\n");
    nf_unregister_net_hook(&net_obj, &hook_ops);
}

module_init(hello_init);
module_exit(hello_exit);
