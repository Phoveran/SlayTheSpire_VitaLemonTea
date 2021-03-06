#include "stdafx.h"
#include "StateVariables.h"

Enemy::Enemy(int hp, int damage1, int attackTimes1, int damage2, int attackTimes2, int damage3, int attackTimes3)
{
	EnemyHP = hp;
	EnemyBlock = 0;
	Damage1 = damage1;
	AttackTimes1 = attackTimes1;
	AttackTimes2 = attackTimes2;
	AttackTimes3 = attackTimes3;
	Damage2 = damage2;
	Damage3 = damage3;
	State_Vulnerable = 0;
	State_Poison = 0;
	State_Ritual = 0;
	State_Weak = 0;
	Strength = 0;
	intend = 0;//0无动作,1、2、3分别对应攻击
	Dexterity = 0;
}

Enemy::Enemy(Enemy &example)
{
	EnemyHP = example.EnemyHP;
	EnemyBlock = example.EnemyBlock;
	Damage1 = example.Damage1;
	AttackTimes1 = example.AttackTimes1;
	AttackTimes2 = example.AttackTimes2;
	AttackTimes3 = example.AttackTimes3;
	Damage2 = example.Damage2;
	Damage3 = example.Damage3;
	State_Vulnerable = example.State_Vulnerable;
	State_Poison = example.State_Poison;
	State_Ritual = example.State_Ritual;
	State_Weak = example.State_Weak;
	Strength = example.Strength;
	intend = example.intend;//0无动作,1、2、3分别对应攻击
	Dexterity = example.Dexterity;
}

Enemy::~Enemy(void)
{
}

void Enemy::debuffWeak(int n, StateVariables* player)
{
	player->State_Weak += n;
}

void Enemy::debuffCards(int cardnum, int n, StateVariables* player)//往弃牌堆扔牌
{
	int i;
	for (i = 0; i < n; i++)
	{
		player->DiscardPile[player->DiscardPoint+i] = player->GameDeck[cardnum];
	}
	player->DiscardPoint += n;
}

void Enemy::debuffVulnerable(int n, StateVariables* player)
{
	player->State_Vulnerable += n;
}

void Enemy::buffRitual()
{
	State_Ritual = 1;
}

void Enemy::buffStrengthUp(int n)
{
	Strength += n;
}

void Enemy::buffFrail(int n, StateVariables* player)
{
	player->Frail += n;
}

void Enemy::attack(int attackMethod, StateVariables* player)
{
	int iter;
	switch (attackMethod)
	{
	case 1:
	{
		for (iter = 0; iter < AttackTimes1; iter++)
		{
			damage(Damage1, player);
		}
	}
	case 2:
	{
		for (iter = 0; iter < AttackTimes2; iter++)
		{
			damage(Damage2, player);
		}
	}
	case 3:
	{
		for (iter = 0; iter < AttackTimes3; iter++)
		{
			damage(Damage3, player);
		}
	}
	}
}

void Enemy::damage(int damage, StateVariables* target)
{
	double WeakCoefficient = (State_Weak > 0) ? 0.75 : 1;
	double VulnerableCoefficient = (target->State_Vulnerable > 0) ? 1.5 : 1;
	int pre_damage = int((damage + Strength) * WeakCoefficient * VulnerableCoefficient);
	if (target->Block >= pre_damage)
	{
		target->Block -= pre_damage;
	}
	else
	{
		target->HP -= (pre_damage - (target->Block));
		target->Block = 0;
	}
}

void Enemy::deffend(int deffendVal)
{
	EnemyBlock += int(deffendVal + Dexterity);
}

void Enemy::net(int n, StateVariables* player)
{
	player->CantAttack += n;
}

void Enemy::acidSlimeMiddle(StateVariables* player)
{
	switch (intend)
	{
	case 1:
	{
		attack(1, player);
	}break;
	case 2:
	{
		attack(2, player);
		debuffCards(132, 1, player);
	}break;
	case 3:
	{
		debuffWeak(1, player);
	}break;
	default:
		break;
	}
}
