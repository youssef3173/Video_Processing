#include <stdio.h>
#include <stdlib.h>

#include "atypes.h"
#include "me_struct.h"
#include "me.h"

Void UcbdsEstimateMotion(Uint8* pbSrc, Uint8* pbRef, MotionVector* pMotionVector)
{
	int Mb_C, Mb_G, Mb_H, Mb_D, SAD_down, Mb_GB, Mb_DB, Mb_GH, Mb_DH, SAD_min, loc;

	for (int y = 0; y < PIC_HEIGHT - 15; y = y + 16) {

		for (int x = 0; x < PIC_WIDTH - 15; x = x + 16) {

			int i = 0, j = 0, max_i = 0;

			while (max_i < 16) { // Comparing the sum of absolute differences SAD of the center with the peripheries
				// Initializing Flags
				int F_G = 0, F_H = 0, F_D = 0, F_B = 0, F_DB = 0, F_DH = 0, F_GB = 0, F_GH = 0;
				Mb_C = sad16(pbSrc + (x + PIC_WIDTH * y), pbRef + (x + i) + PIC_WIDTH * (y + j));
				SAD_min = Mb_C;
				// Controlling/detecting the edges
				if (x + i > 0) {

					Mb_G = sad16(pbSrc + (x + PIC_WIDTH * y), pbRef + (x + i - 1) + PIC_WIDTH * (y + j));

					if (Mb_G < SAD_min) {

						SAD_min = Mb_G;
						F_G = 1;
					}
				}

				if (x - i > 0) {

					Mb_GB = sad16(pbSrc + (x + PIC_WIDTH * y), pbRef + (x + i - 1) + PIC_WIDTH * (y + j));

					if (Mb_GB < SAD_min) {

						SAD_min = Mb_GB;
						F_G = 0, F_GB = 1;
					}
				}

				if (y + j > 0) {

					Mb_H = sad16(pbSrc + (x + PIC_WIDTH * y), pbRef + (x + i) + PIC_WIDTH * (y + j - 1));

					if (Mb_H < SAD_min) {

						SAD_min = Mb_H;
						F_G = 0, F_GB = 0, F_H = 1;
					}
				}
				if (y - j > 0) {

					Mb_GH = sad16(pbSrc + (x + PIC_WIDTH * y), pbRef + (x + i) + PIC_WIDTH * (y + j - 1));

					if (Mb_GH < SAD_min) {

						SAD_min = Mb_GH;
						F_G = 0, F_GB = 0, F_H = 0, F_GH = 1;
					}
				}
				if (x + i < PIC_WIDTH - 16) {

					Mb_D = sad16(pbSrc + (x + PIC_WIDTH * y), pbRef + (x + i + 1) + PIC_WIDTH * (y + j));
					if (Mb_D < SAD_min) {

						SAD_min = Mb_D;
						F_G = 0, F_H = 0, F_GB = 0, F_GH = 0, F_D = 1;
					}
				}
				if (x - i < PIC_WIDTH - 16) {

					Mb_DB = sad16(pbSrc + (x + PIC_WIDTH * y), pbRef + (x + i + 1) + PIC_WIDTH * (y + j));
					if (Mb_D < SAD_min) {

						SAD_min = Mb_DB;
						F_G = 0, F_H = 0, F_GB = 0, F_GH = 0, F_D = 0, F_DB = 1;
					}
				}
				if (y + j < PIC_HEIGHT - 16) {

					SAD_down = sad16(pbSrc + (x + PIC_WIDTH * y), pbRef + (x + i) + PIC_WIDTH * (y + j + 1));

					if (SAD_down < SAD_min) {

						SAD_min = SAD_down;
						F_G = 0, F_H = 0,  F_D = 0, F_GB = 0, F_GH = 0, F_DB = 0, F_B = 1;
					}

				}
				if (y - j < PIC_HEIGHT - 16) {

					Mb_DB = sad16(pbSrc + (x + PIC_WIDTH * y), pbRef + (x + i) + PIC_WIDTH * (y + j + 1));

					if (Mb_DB < SAD_min) {

						SAD_min = Mb_DB;
						F_G = 0, F_H = 0, F_D = 0, F_GB = 0, F_GH = 0, F_DB = 0, F_B = 0, F_DH = 1;
					}

				}
				//Choosing the new center of the diamond depending on the minimizer position of the SAD

				if (F_G == 1) i = i - 2;
				else if (F_H == 1) j = j - 2;
				else if (F_D == 1) i = i + 2;
				else if (F_B == 1) j = j + 2;

				else if (F_DH == 1) j = j + 1;
				else if (F_DB == 1) j = j - 1;
				else if (F_GB == 1) j = j - 1;
				else if (F_GH == 1) j = j + 1;

				else max_i = 15;
				max_i++;
			}

			loc = (x / 16) + NUM_MB_X * (y / 16);
			pMotionVector[loc].nSad = SAD_min;
			pMotionVector[loc].nX = i;
			pMotionVector[loc].nY = j;
		}
	}
}
