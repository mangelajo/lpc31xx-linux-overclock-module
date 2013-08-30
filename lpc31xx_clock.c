
//
// we must evolve this module to export a procfs
// also to avoid the USB disconnect/connect or hangup problems.
//


#include <linux/module.h>   /* Needed by all modules */
#include <linux/init.h>		/* Needed for the macros */
#include <mach/hardware.h>
#include <mach/cgu.h>
#include <linux/spinlock.h>
#include <linux/delay.h>


#define PLL_180M  {CGU_FIN_SELECT_FFAST, 770,  8191, 98, 0, 16,  8, 0, 180000000}
#define PLL_270M  {CGU_FIN_SELECT_FFAST, 514, 19660, 98, 0, 48, 23, 0, 270000000}

CGU_HPLL_SETUP_T const g_pll_270 = PLL_270M;
CGU_HPLL_SETUP_T const g_pll_180 = PLL_180M;

extern u32 cgu_get_pll_freq(CGU_HPLL_ID_T pll_id, u32 infreq);
extern void cgu_hpll_config(CGU_HPLL_ID_T id, CGU_HPLL_SETUP_T* pllsetup);
extern unsigned long loops_per_jiffy;

spinlock_t lock_for_pll;

static int mhz = 0;
static int mpcdiv = 2;
module_param(mhz, int, S_IRUGO);
module_param(mpcdiv, int, S_IRUGO);

int init_module(void)
{

    unsigned int speed1,speed2;

    unsigned long flags;

    speed1 = cgu_get_pll_freq(CGU_HPLL1_ID, FFAST_CLOCK);
    printk("PLL Speed: %d\n",speed1);
    
    if (mhz==180)
    {
	spin_lock_irqsave(&lock_for_pll,flags); 
    	cgu_hpll_config (CGU_HPLL1_ID, &g_pll_180);
    	loops_per_jiffy = 178688;
	mdelay(150);
	spin_unlock_irqrestore(&lock_for_pll,flags);

    } else if (mhz==270)
    {
	spin_lock_irqsave(&lock_for_pll,flags); 
    	cgu_hpll_config (CGU_HPLL1_ID, &g_pll_270);
    	loops_per_jiffy = 268032;
	mdelay(150);
	spin_unlock_irqrestore(&lock_for_pll,flags);
    
    }
    else 
    {
       printk("Invalid mhz value, must be 180 or 270\n");
       return -EINTR;
    }
    printk("PLL setup done\n");
 
    speed2 = cgu_get_pll_freq(CGU_HPLL1_ID, FFAST_CLOCK);
    printk("PLL Speed: %d\n",speed2);


  
  printk("MPMC CLK1: %d Hz\n",cgu_get_clk_freq(CGU_SB_MPMC_CFG_CLK_ID));
  printk("MPMC CLK2: %d Hz\n",cgu_get_clk_freq(CGU_SB_MPMC_CFG_CLK2_ID));
  printk("MPMC CLK3: %d Hz\n",cgu_get_clk_freq(CGU_SB_MPMC_CFG_CLK3_ID));


    printk(KERN_CONT "%lu.%02lu BogoMIPS (lpj=%lu)\n",
                        loops_per_jiffy/(500000/HZ),
                        (loops_per_jiffy/(5000/HZ)) % 100, loops_per_jiffy);

	return -EINTR;
}

void cleanup_module(void)
{

}

MODULE_LICENSE("GPL");       
MODULE_AUTHOR("Miguel Angel Ajo Pelayo");                       /* Who wrote this */
MODULE_DESCRIPTION("Module to update the processor frequency");  /* Helpful info   */
