#include "stdafx.h"
#include "StateVariables.h"

Cards::Cards(int cardnum, int Cost)
{
	CardsNum = cardnum;
	EnergyCost = Cost;
}

Cards::~Cards(void)
{
}

int Cards::EnergyConsume(int x, StateVariables* player)
{
	if (player->Energy < x)
		return 0;
	else
	{
		player->Energy -= x;
		return 1;
	}
}

void Cards::Damage(int damage, StateVariables* player, Enemy* target, int n = 0)
{
	double WeakCoefficient = (player->Weak > 0) ? 0.75 : 1;
	double Vulnerable = (target->State_Vulnerable > 0) ? 1.5 : 1;
	int pre_damage = int((damage + player->Strength) * WeakCoefficient * Vulnerable);
	if (pre_damage < 0)
		pre_damage = 0;
	if (n)
	{
		int iter;
		for (iter = 0; iter < n; iter++)
		{
			if ((target + iter)->EnemyBlock >= pre_damage)
			{
				(target + iter)->EnemyBlock -= pre_damage;
			}
			else
			{
				(target + iter)->EnemyHP -= (pre_damage - (target + iter)->EnemyBlock);
				(target + iter)->EnemyBlock = 0;
			}
		}
	}
	else
	{
		if (target->EnemyBlock >= pre_damage)
		{
			target->EnemyBlock -= pre_damage;
		}
		else
		{
			target->EnemyHP -= (pre_damage - (target->EnemyBlock));
			target->EnemyBlock = 0;
		}
	}
}

void Cards::Defence(int block, StateVariables* player)
{
	double FrailCoefficient = (player->Frail > 0) ? 0.75 : 1;
	int pre_block = int((block + player->Dexterity) * FrailCoefficient);
	if (pre_block >= 0)
	{
		player->Block += pre_block;
	}
}
