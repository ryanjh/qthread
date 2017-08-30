#pragma once

extern otIcmp6Handler mIcmpHandler;
void s_HandleIcmpReceive(void *aContext, otMessage *aMessage, const otMessageInfo *aMessageInfo, const otIcmp6Header *aIcmpHeader);
