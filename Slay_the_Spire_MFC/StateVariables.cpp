#include "StdAfx.h"
#include "StateVariables.h"
#include <time.h>
#define random(x) (rand()%x)

StateVariables::StateVariables(void)
{
	Block = 0;
	Dexterity = 0;
	Energy = 3;
	EnergyMax = 3;
	Gold = 99;
	HP = 70;
	HPMax = 70;
	Deck[0] = GameDeck[1];
	Deck[1] = GameDeck[1];
	Deck[2] = GameDeck[1];
	Deck[3] = GameDeck[1];
	Deck[4] = GameDeck[1];
	Deck[5] = GameDeck[3];
	Deck[6] = GameDeck[3];
	Deck[7] = GameDeck[3];
	Deck[8] = GameDeck[3];
	Deck[9] = GameDeck[2];//生成只有十张初始牌的牌库
	int i;
	for (i = 10; i <= 99; i++)
	{
		Deck[i] = 0;//剩下的空位用0表示
	}
	DrawNum = 5;//每回合抽5张牌
	DeckPoint = 10;
	DeckPtr = &DeckPoint;
	DrawPoint = 0;
	DrawPtr = &DrawPoint;
	DiscardPoint = 0;
	DiscardPtr = &DiscardPoint;
	HandPoint = 0;
	HandPtr = &HandPoint;
	State_Vulnerable = 0;
	State_Weak = 0;
	Strength = 0;
	StrengthUp = 0;
	StrengthUpTemp = 0;
	Dexterity = 0;
	DexterityUpTemp = 0;
	CantAttack = 0;
	CanDraw = 1;
	Frail = 0;
	FlameBarrier = 0;
	Berserk = 0;
	Juggernaut = 0;//Juggernaut效果
	Combust = 0;//Combust效果
	Metallicize = 0;//Metallicize效果
	RampageTime = 0;//Rampage使用次数
	Barricade = 0;//Barricade效果
	Brutality = 0;//Brutality效果
	Evolve = 0;//进化效果
	EnemyNum = 0;
}

StateVariables::~StateVariables(void)
{
}


void StateVariables::draw(int drawnum)
{
	for (int i = 0; i < drawnum; i++)
	{
		Hand[*HandPtr + i + 1] = DrawPile[*DrawPtr - i];
		DrawPile[*DrawPtr - i] = 0;
	}
	*HandPtr = *HandPtr + drawnum;
	*DrawPtr = *DrawPtr - drawnum;
};

void StateVariables::addTo(int cardnum, Cards* PileExample[], int* pilePoint)
{
	srand((unsigned int)(time(NULL)));
	int i = random(*pilePoint);
	int j;
	for (j = *pilePoint - 1; j > i; j--)
	{
		PileExample[j + 1] = PileExample[j];
	}
	PileExample[i] = GameDeck[cardnum];
	*pilePoint += 1;
}

void StateVariables::usecard(int cardnum, Enemy* target = NULL, int n = 0)
{
	addTo(cardnum, DiscardPile, DiscardPtr);
	switch (cardnum)
	{
	    case 1: //痛击
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(8, this, target, 0);
				target->State_Vulnerable += 2;
				break;
			}
			else
				return;
		}break;

	    case 201: //痛击+
	    {
		    if (GameDeck[cardnum]->EnergyConsume(this))
		    {
			    GameDeck[cardnum]->Damage(11, this, target, 0);
			    target->State_Vulnerable += 10;
			    break;
		    }
		    else
			    return;
	    }

	    case 2: //防御
	    {
		    if (GameDeck[cardnum]->EnergyConsume(this))
		    {
		    	GameDeck[cardnum]->Defence(5, this, NULL);
				break;
		    }
		    else
			    return;
	    }

		case 202: //防御+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Defence(8, this, NULL);
				break;
			}
			else
				return;
		}

		case 3: //打击
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(6, this, target, 0);
				break;
			}
			else
				return;
		}

		case 203: //打击+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(9, this, target, 0);
				break;
			}
			else
				return;
		}

		case 4: //愤怒
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(6, this, target, 0);
				addTo(4, DiscardPile, DiscardPtr);
				break;
			}
			else
				return;
		}

		case 204: // 愤怒+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(9, this, target, 0);
				addTo(204, DiscardPile, DiscardPtr);
				break;
			}
			else
				return;
		}
		//case 5:

		case 6: //全身撞击
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(Block, this, target, 0);
				break;
			}
			else
				return;
		}

		case 206: //全身撞击+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(Block, this, target, 0);
				break;
			}
			else
				return;
		}
		//case 7

		case 8: //
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(8, this, target, EnemyNum);
				break;
			}
			else
				return;
		}

		case 208: //
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(11, this, target, EnemyNum);
				break;
			}
			else
				return;
		}

		case 9: //金刚臂
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(12, this, target, 0);
				target->State_Weak += 2;
				break;
			}
			else
				return;
		}

		case 209: //金刚臂+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(14, this, target, 0);
				target->State_Weak += 3;
				break;
			}
			else
				return;
		}

		case 10: //活动肌肉
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				Strength += 2;
				StrengthUpTemp += 2;
				break;
			}
			else
				return;
		}

		case 210: //活动肌肉+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				Strength += 4;
				StrengthUpTemp += 4;
				break;
			}
			else
				return;
		}

		case 11: //破灭
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				usecard(DrawPile[DrawPoint - 1], target, n); //消耗功能没实现
				DrawPile[DrawPoint - 1] = 0;
				*DrawPtr -= 1;
				break;
			}
			else
				return;
		}

		case 211: //破灭+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				usecard(DrawPile[DrawPoint - 1], target, n); //消耗功能没实现
				DrawPile[DrawPoint - 1] = 0;
				*DrawPtr -= 1;
				break;
			}
			else
				return;
		}

		//case 12

		case 13: //重刃
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(9 + Strength * 2, this, target, 0);
				break;
			}
			else
				return;
		}

		case 213: //重刃+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(12 + Strength * 4, this, target, 0);
				break;
			}
			else
				return;
		}

		case 14: //铁斩波
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Defence(5, this, NULL);
				GameDeck[cardnum]->Damage(5, this, target, 0);
				break;
			}
			else
				return;
		}
		case 214: //铁斩波+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Defence(7, this, NULL);
				GameDeck[cardnum]->Damage(7, this, target, 0);
				break;
			}
			else
				return;
		}

		/*case 15 //完美打击
		{
		    if (GameDeck[cardnum]->EnergyConsume(this))
			{
				int strikeSum;
				for (int i = 0; i < DeckPoint; i++)
				{
					if (ifstrike(Deck[i]))  //ifstrike判断牌带不带“打击”，未实现
					{
						sumStrike += 1;
					}
				}
				GameDeck[cardnum]->Damage(6 + sumStrike * 2, this, target, 0);
				break;
			}
			else
				return;
		}
		*/

		case 16: //剑柄打击
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(9, this, target, 0);
				draw(1);
				break;
			}
			else
				return;
		}
		case 216: //剑柄打击+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(10, this, target, 0);
				draw(2);
				break;
			}
			else
				return;
		}

		case 17: //耸肩无视
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Defence(8, this, NULL);
				draw(1);
				break;
			}
			else
				return;
		}

		case 217: //耸肩无视+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Defence(11, this, NULL);
				draw(1);
				break;
			}
			else
				return;
		}

		case 18: //飞剑回旋镖
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				randomDamage(3, NULL, 3);
				break;
			}
			else
				return;
		}

		case 218: //飞剑回旋镖+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				randomDamage(3, NULL, 4);
				break;
			}
			else
				return;
		}

		/*case 19: //
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(4, this, NULL, EnemyNum);
				int iter;
				for (iter = 0; iter < EnemyNum; iter++)
				{

				}
				break;
			}
			else
				return;
		} */

		//case 20

		case 21: //双重打击
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(5, this, target, 0);
				GameDeck[cardnum]->Damage(5, this, target, 0);
				break;
			}
			else
				return;
		}

		case 221: //双重打击+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(7, this, target, 0);
				GameDeck[cardnum]->Damage(7, this, target, 0);
				break;
			}
			else
				return;
		}

		//case 22: //战吼

		case 23: //狂野打击
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(11, this, target, 0);
				addTo(131, DrawPile, DrawPtr);
				break;
			}
			else
				return;
		}
		case 223: //狂野打击+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(14, this, target, 0);
				addTo(131, DrawPile, DrawPtr);
				break;
			}
			else
				return;
		}

		case 24: //战斗专注
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				draw(3);
				CanDraw = 0;
				break;
			}
			else
				return;
		}

		case 224: //战斗专注
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				draw(4);
				CanDraw = 0;
				break;
			}
			else
				return;
		}

		//case 25 以血还血，需要“DamageCount”变量

		case 26: //放血
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				HP -= 3;
				Energy += 1;
				break;
			}
			else
				return;
		}
	
		case 226: //放血+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				HP -= 3;
				Energy += 2;
				break;
			}
			else
				return;
		}

		//case 27

		case 28: //残杀， 虚无未实现
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(20, this, target, 0);
				break;
			}
			else
				return;
		}

		case 228: //残杀+， 虚无未实现
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(28, this, target, 0);
				break;
			}
			else
				return;
		}
     
		case 29: //自燃
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				Combust += 5;
				break;
			}
			else
				return;
		}

		case 229: //自燃+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				Combust += 7;
				break;
			}
			else
				return;
		}

		//case 30: //腐化

		case 31: //缴械
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				target->Strength -= 2;
				break;
			}
			else
				return;
		}

		case 231: //缴械+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				target->Strength -= 3;
				break;
			}
			else
				return;
		}

		case 32: //飞身踢
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(5, this, target, 0);
				if (target->State_Vulnerable)
				{
					Energy += 1;
					draw(1);
				}
				break;
			}
			else
				return;
		}

		case 232: //飞身踢+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(8, this, target, 0);
				if (target->State_Vulnerable)
				{
					Energy += 1;
					draw(1);
				}
				break;
			}
			else
				return;
		}

		//case 33: //双持

		case 34: //巩固
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				Block *= 2;
				break;
			}
			else
				return;
		}

		case 234: //巩固+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				Block *= 2;
				break;
			}
			else
				return;
		}

		case 35: //进化
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				Evolve += 1;
				break;
			}
			else
				return;
		}

		case 235: //进化+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				Evolve += 2;
				break;
			}
			else
				return;
		}

		//case 36

		//case 37

		case 38: //火焰屏障
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Defence(12, this, NULL);
				FlameBarrier += 4;
				break;
			}
			else
				return;
		}

		case 238: //火焰屏障+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Defence(16, this, NULL);
				FlameBarrier += 6;
				break;
			}
			else
				return;
		}

		case 39: //幽灵铠甲， 虚无未实现
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Defence(10, this, NULL);
				break;
			}
			else
				return;
		}

		case 239: //幽灵铠甲+， 虚无未实现
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Defence(13, this, NULL);
				break;
			}
			else
				return;
		}

		case 40: //御血术
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				HP -= 3;
				GameDeck[cardnum]->Damage(14, this, target, 0);
				break;
			}
			else
				return;
		}

		case 240: //御血术+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				HP -= 2;
				GameDeck[cardnum]->Damage(18, this, target, 0);
				break;
			}
			else
				return;
		}

		/*case 41: //地狱之刃
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				int i = random(400);
				while(!isAttack(i))
				{
				    i = random(400);
				}
				addTo(i, Hand, HandPtr);
				break;
			}
			else
				return;
		}*/

		case 42: //Inflame
		{

		}
	}
}

void StateVariables::Defend(int DefendVal, Enemy* target = NULL, int EnemyNum = 0)   //有些获得格挡的效果是与敏捷虚弱无关的，所以保留这个函数
{
	if (Frail)
	{
		Block += int((DefendVal + Dexterity) * 0.75);
	}
	else
	{
		Block += (DefendVal + Dexterity);
	}
	if (Juggernaut)
	{
		randomDamage(Juggernaut, target, EnemyNum);
	}
}

void StateVariables::randomDamage(int damage, Enemy * target, int n)
{
	srand((unsigned int)(time(NULL)));
	int t = random(n);
	if ((target + t)->EnemyBlock >= damage)
	{
		(target + t)->EnemyBlock -= damage;
	}
	else
	{
		(target + t)->EnemyHP -= (damage - (target->EnemyBlock));
		(target + t)->EnemyBlock = 0;
	}
}

void StateVariables::combust(Enemy * target, int EnemyNum)
{
	if (Combust)
	{
		int iter;
		for (iter = 0; iter < EnemyNum; iter++)
		{
			if ((target + iter)->EnemyBlock >= Combust)
			{
				(target + iter)->EnemyBlock -= Combust;
			}
			else
			{
				(target + iter)->EnemyHP -= (Combust - (target + iter)->EnemyBlock);
				(target + iter)->EnemyBlock = 0;
			}
		}
	}
}

void StateVariables::metallicize(Enemy * target = NULL, int EnemyNum = 0)
{
	if (Metallicize)
	{
		Defend(Metallicize, target, EnemyNum);
	}
}

void StateVariables::brutality(void)
{
	HP -= 1;
	draw(1);
}