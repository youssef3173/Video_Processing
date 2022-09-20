
#include <stdio.h>
#include <stdlib.h>

#include "atypes.h"
#include "me_struct.h"
#include "me.h"
#define MAX_ITER 16

static int index_min_SAD(int* SAD, int* border)
{
	int index = 0;
	int min = SAD[0];
	for (int i = 1; i < 5; i++)
		if (SAD[i] < min && border[i] == 1)
		{
			min = SAD[i];
			index = i;
		}
	return index;
}

Void dsEstimateMotion(Uint8* pbSrc, Uint8* pbRef, MotionVector* pMotionVector)
{
	int _nX, _nY, sad;
	int index, x_ref, y_ref;
	int k = 0;
	int flag;

	// Pour les tableaux (convention) : 0=centre, 1=gauche, 2=haut, 3=droite , 4=bas.
	int SAD[5];
	int border[5];
	int var_x_ref[5];
	int var_y_ref[5];
	int var_X[5] = { 0, -1, 0, 1, 0 };
	int var_Y[5] = { 0, 0, -1, 0, 1 };

	for (int y0 = 0; y0 < NUM_MB_Y; y0 += 1)
	{
		for (int x0 = 0; x0 < NUM_MB_X; x0 += 1)
		{
			//x0 = x0 * 16; 
			//y0 = y0 * 16;
			_nX = 0;
			_nY = 0;
			sad = sad16(&pbSrc[16*y0 * PIC_WIDTH + 16*x0], &pbRef[16*y0 * PIC_WIDTH + 16*x0]);
			flag = 0;
			x_ref = 16*x0;
			y_ref = 16*y0;
			for (int i = 0; i < MAX_ITER; i++)
			{
				for (int t = 0; t < 5; t++)
				{
					var_x_ref[t] = x_ref + var_X[t]; //Prochaines valeurs potentielles pour la référence
					var_y_ref[t] = y_ref + var_Y[t];
					if (var_x_ref[t] >= 0 && var_y_ref[t] >= 0 && var_x_ref[t] <= PIC_WIDTH - 16 && var_y_ref[t] <= PIC_HEIGHT - 16)
						border[t] = 1; //Pas de pb de bord
					else
						border[t] = 0;
					if (border[t])
						SAD[t] = sad16(&pbSrc[16*y0 * PIC_WIDTH + 16*x0], &pbRef[var_y_ref[t] * PIC_WIDTH + var_x_ref[t]]);
				}

				index = index_min_SAD(SAD, border); //Retourne l'indice du plus petit SAD du tableau

				if (SAD[index] < sad)
				{
					x_ref = var_x_ref[index];
					y_ref = var_y_ref[index];
					_nX += var_X[index];
					_nY += var_Y[index];
					sad = SAD[index];
				}
				else flag = 1;
				if (flag) break;
			}
			pMotionVector[k].nX = _nX;
			pMotionVector[k].nY = _nY;
			pMotionVector[k].nSad = sad;
			k++;
		}
	}
}

