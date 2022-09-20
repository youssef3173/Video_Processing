#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "atypes.h"
#include "me_struct.h"
#include "me.h"


Void fsEstimateMotion(Uint8* pbSrc, Uint8* pbRef, MotionVector* pMotionVector)
{
    int x_min, y_min;
    Int sad, s;
    Uint8* src, * ref; 
    for (int y = 0; y < NUM_MB_Y; y++) {
        for (int x = 0; x < NUM_MB_X; x++) {

            s = 16 * 16 * 255;
            for (int j = -16; j < 16; j++) {
                for (int i = -16; i < 16; i++) {

                    if ((((x * 16 + i) >= 0) && ((x * 16 + i + 16 - 1) < PIC_WIDTH)) && (((y * 16 + j) >= 0) && ((y * 16 + j + 16 - 1) < PIC_HEIGHT))) {

                        src = pbSrc + (y * 16    ) * PIC_WIDTH + 16 * x;
                        ref = pbRef + (y * 16 + j) * PIC_WIDTH + 16 * x + i;

                        sad = sad16(src, ref);

                        if (sad < s) {
                            s = sad;
                            x_min = i;
                            y_min = j;
                        }
                    }

                }
            }
            (pMotionVector + y * NUM_MB_X + x)->nX = x_min;
            (pMotionVector + y * NUM_MB_X + x)->nY = y_min;
            (pMotionVector + y * NUM_MB_X + x)->nSad = s;
        }
    }
}













