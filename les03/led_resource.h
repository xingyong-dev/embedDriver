#ifndef LED_RESOURCE_H
#define LED_RESOURCE_H

/*GPIO10_0*/
#define GROUP(X) (X >> 16)
#define PIN(X) (X&0XFFFF)
#define GROUP_PIN(g,p) ((g<<16) | (p))


#endif // !LED_RESOURCE_H#


