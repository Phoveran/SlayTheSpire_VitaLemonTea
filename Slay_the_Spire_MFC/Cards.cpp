#include<string.h>
#include "stdafx.h"
#include "StateVariables.h"

Cards::Cards(int cardnum, int Cost, int kind, char* name)
{
	CardsNum = cardnum;
	EnergyCost = Cost;
	Kind = kind;
	suffix = CString(".bmp");
	bigSuffix = CString("_b");
	smallSuffix = CString("_s");
	BigPicPath = CString(name)+bigSuffix+suffix;
	SmallPicPath = CString(name)+smallSuffix+suffix;
}

Cards::~Cards(void)
{
}

int Cards::EnergyConsume(StateVariables* player)
{
	if (player->Energy < EnergyCost)
		return 0;
	else
	{
		player->Energy -= EnergyCost;
		return 1;
	}
}

void Cards::Damage(int damage, StateVariables* player, Enemy* target, int n = 0)
{
	double WeakCoefficient = (player->State_Weak > 0) ? 0.75 : 1;
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

void Cards::Defence(int block, StateVariables* player, Enemy* target = NULL)
{
	double FrailCoefficient = (player->Frail > 0) ? 0.75 : 1;
	int pre_block = int((block + player->Dexterity) * FrailCoefficient);
	if (pre_block >= 0)
	{
		if (player->Juggernaut)
		{
			player->randomDamage(player->Juggernaut, target, player->EnemyNum);
		}
		player->Block += pre_block;
	}
}

void StateVariables::GameDeckIni()
{
	*GameDeck[1] = Cards(1, 2, 0, "Bash");
	*GameDeck[201] = Cards(201, 2, 0, "Bash");
	*GameDeck[2] = Cards(2, 1, 1, "DefendR");
	*GameDeck[3] = Cards(3, 1, 0, "StrikeR");
	*GameDeck[4] = Cards(4, 0, 0, "Anger");
	*GameDeck[6] = Cards(6, 1, 0, "BodySlam");
}