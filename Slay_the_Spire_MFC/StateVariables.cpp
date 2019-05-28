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
	GameDeckIni();
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
	Frail = 0;
	Strength = 0;
	StrengthUp = 0;
	StrengthUpTemp = 0;
	Dexterity = 0;
	DexterityUpTemp = 0;
	CanDraw = 1;
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
	Rage = 0;
}

StateVariables::~StateVariables(void)
{
}


void StateVariables::shuffle(Cards* Pile[], int* pilePoint)
{
	int iter;
	int i, j;
	if (*pilePoint == 1)
		return;
	for (iter = 1; iter <= 3 * (*pilePoint); iter++)
	{
		int i = random(*pilePoint);
		int j = random(*pilePoint);
		while (j == i)
			j = random(*pilePoint);
		Cards* hold = Pile[i];
		Pile[i] = Pile[j];
		Pile[j] = hold;
	}
}

void StateVariables::draw(int drawnum)
{
	int i;
	if (*DrawPtr < drawnum)
	{
		if ((*DrawPtr + *DiscardPtr) < drawnum)
			draw(*DrawPtr + *DiscardPtr);
		else
		{
			drawnum -= *DrawPtr;
			draw(*DrawPtr);
			for (i = 0; i < *DiscardPtr; i++)
			{
				DrawPile[i] = DiscardPile[i];
				DiscardPile[i] = 0;
			}
			*DrawPtr = *DiscardPtr;
			shuffle(DrawPile, DrawPtr);
			*DiscardPtr = 0;
		}
	}
	for (i = 0; i < drawnum; i++)
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

void StateVariables::addToHand(int cardnum)
{
	if (*HandPtr <= 9)
	{
		Hand[*HandPtr] = GameDeck[cardnum];
		*HandPtr += 1;
	}
	else
	{
		addTo(cardnum, DiscardPile, DiscardPtr);
	}
}

void StateVariables::usecard(int cardnum, Enemy* target = NULL, int n = 0)
{
	switch (cardnum)
	{
	    case 1: //痛击
	    {
		    if (GameDeck[cardnum]->EnergyConsume(this))
		    {
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Damage(8, this, target, 0);
			    target->State_Vulnerable += 2;
			    break;
		    }
		    else
			    return;
	    }

	    case 201: //痛击+
	    {
		    if (GameDeck[cardnum]->EnergyConsume(this))
		    {
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Damage(9, this, target, 0);
				addTo(204, DiscardPile, DiscardPtr);
				break;
			}
			else
				return;
		}
		//case 5:

		case 205: //武装+ ， 需要Upgrade函数
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				int i = 0;
				for (; i < *HandPtr; i++)
				{
					//Upgrade[Hand[i]];
				}
				GameDeck[cardnum]->Defence(5, this, NULL);
				break;
			}
			else
				return;
		}

		case 6: //全身撞击
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Damage(this->Block, this, target, 0);
				break;
			}
			else
				return;
		}

		case 206: //全身撞击+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Damage(this->Block, this, target, 0);
				break;
			}
			else
				return;
		}
		case 7: //交锋
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				int i = 0;
				for (; i < *HandPtr; i++)
				{
					if (Hand[i]->Kind != 0)
						return;
				}
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Damage(14, this, target, 0);
				break;
			}
			else
				return;
		}

		case 207: //交锋+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				int i = 0;
				for (; i < *HandPtr; i++)
				{
					if (Hand[i]->Kind != 0)
						return;
				}
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Damage(18, this, target, 0);
				break;
			}
			else
				return;
		}

		case 8: //顺劈斩
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Damage(8, this, target, EnemyNum);
				break;
			}
			else
				return;
		}

		case 208: //顺劈斩+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
				this->Strength += 2;
				this->StrengthUpTemp += 2;
				break;
			}
			else
				return;
		}

		case 210: //活动肌肉+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				this->Strength += 4;
				this->StrengthUpTemp += 4;
				break;
			}
			else
				return;
		}

		case 11: //破灭
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				usecard(DrawPile[DrawPoint - 1]->CardsNum, target, n); //消耗功能没实现
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
				addTo(cardnum, DiscardPile, DiscardPtr);
				usecard(DrawPile[DrawPoint - 1]->CardsNum, target, n); //消耗功能没实现
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
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Damage(9 + this->Strength * 2, this, target, 0);
				break;
			}
			else
				return;
		}

		case 213: //重刃+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Damage(12 + this->Strength * 4, this, target, 0);
				break;
			}
			else
				return;
		}

		case 14: //铁斩波
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Defence(7, this, NULL);
				GameDeck[cardnum]->Damage(7, this, target, 0);
				break;
			}
			else
				return;
		}

/*		case 15: //完美打击
		{
		    if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				int strikeSum;
				for (int i = 0; i < DeckPoint; i++)
				{
					if (isStrike(Deck[i]->CardsNum))  
					{
						strikeSum += 1;
					}
				}
				GameDeck[cardnum]->Damage(6 + strikeSum * 2, this, target, 0);
				break;
			}
			else
				return;
		}

		case 215: //完美打击+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				int strikeSum = 0;
				for (int i = 0; i < DeckPoint; i++)
				{
					if (isStrike(Deck[i]->CardsNum))
					{
						strikeSum += 1;
					}
				}
				GameDeck[cardnum]->Damage(6 + strikeSum * 3, this, target, 0);
				break;
			}
			else
				return;
		}*/
		

		case 16: //剑柄打击
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
				randomDamage(3, NULL, 4);
				break;
			}
			else
				return;
		}

		case 19: // 闪电霹雳
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Damage(4, this, NULL, EnemyNum);
				int iter;
				for (iter = 0; iter < EnemyNum; iter++)
				{
					(target + iter)->State_Vulnerable += 1;
				}
				break;
			}
			else
				return;
		} 

		case 219: // 闪电霹雳+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Damage(7, this, NULL, EnemyNum);
				int iter;
				for (iter = 0; iter < EnemyNum; iter++)
				{
					(target + iter)->State_Vulnerable += 2;
				}
				break;
			}
			else
				return;
		}

		//case 20

		case 21: //双重打击
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
				draw(3);
				this->CanDraw = 0;
				break;
			}
			else
				return;
		}

		case 224: //战斗专注+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				draw(4);
				this->CanDraw = 0;
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
				addTo(cardnum, DiscardPile, DiscardPtr);
				this->HP -= 3;
				this->Energy += 1;
				break;
			}
			else
				return;
		}
	
		case 226: //放血+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				this->HP -= 3;
				this->Energy += 2;
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
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				this->Combust += 5;
				break;
			}
			else
				return;
		}

		case 229: //自燃+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Combust += 7;
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
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Damage(5, this, target, 0);
				if (target->State_Vulnerable)
				{
					this->Energy += 1;
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
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Damage(8, this, target, 0);
				if (target->State_Vulnerable)
				{
					this->Energy += 1;
					draw(1);
				}
				break;
			}
			else
				return;
		}

		case 34: //巩固
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				this->Block *= 2;
				break;
			}
			else
				return;
		}

		case 234: //巩固+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				this->Block *= 2;
				break;
			}
			else
				return;
		}

		case 35: //进化
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Evolve += 1;
				break;
			}
			else
				return;
		}

		case 235: //进化+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Evolve += 2;
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
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Defence(12, this, NULL);
				this->FlameBarrier += 4;
				break;
			}
			else
				return;
		}

		case 238: //火焰屏障+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Defence(16, this, NULL);
				this->FlameBarrier += 6;
				break;
			}
			else
				return;
		}

		case 39: //幽灵铠甲， 虚无未实现
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr);
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
				addTo(cardnum, DiscardPile, DiscardPtr); 
				this->HP -= 3;
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
				addTo(cardnum, DiscardPile, DiscardPtr);
				this->HP -= 2;
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

		case 42: // 燃烧
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Strength += 2;
				break;
			}
			else
				return;
		}

		case 242: // 燃烧+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Strength += 3;
				break;
			}
			else
				return;
		}

		case 43: //威吓
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				int iter = 0;
				for (; iter < EnemyNum; iter++)
				{
					(target + iter)->State_Weak += 1;
				}
				break;
			}
			else
				return;
		}

		case 243: //威吓+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				int iter = 0;
				for (; iter < EnemyNum; iter++)
				{
					(target + iter)->State_Weak += 2;
				}
				break;
			}
			else
				return;
		}

		case 44: //金属化
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Metallicize += 3;
				break;
			}
			else
				return;
		}

		case 244: //金属化+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Metallicize += 4;
				break;
			}
			else
				return;
		}

		case 45: //硬撑
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				addToHand(131);
				addToHand(131);
				GameDeck[cardnum]->Defence(15, this, NULL);
				break;
			}
			else
				return;
		}

		case 245: //硬撑+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				addToHand(131);
				addToHand(131);
				GameDeck[cardnum]->Defence(20, this, NULL);
				break;
			}
			else
				return;
		}

		case 46: //连续拳
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				int i;
				for (i = 1; i <= 4; i++)
				{
					GameDeck[cardnum]->Damage(2, this, target, 0);
				}
				break;
			}
			else
				return;
		}

		case 246: //连续拳+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				int i;
				for (i = 1; i <= 5; i++)
				{
					GameDeck[cardnum]->Damage(2, this, target, 0);
				}
				break;
			}
			else
				return;
		}

		case 47: //狂怒
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Rage += 4;
				break;
			}
			else
				return;
		}

		case 247: //狂怒+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Rage += 6;
				break;
			}
			else
				return;
		}

		case 48: //暴走
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Damage(8 + RampageTime * 5, this, target, 0);
				this->RampageTime += 1;
				break;
			}
			else
				return;
		}

		case 248: //暴走+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Damage(8 + RampageTime * 8, this, target, 0);
				this->RampageTime += 1;
				break;
			}
			else
				return;
		}

		case 49: //无谋冲锋
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);

				GameDeck[cardnum]->Damage(7, this, target, 0);
				addTo(130, DrawPile, DrawPtr);
				break;
			}
			else
				return;
		}

		case 249: //无谋冲锋+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Damage(10, this, target, 0);
				addTo(130, DrawPile, DrawPtr);
				break;
			}
			else
				return;
		}

		//case 50: //撕裂

		case 51: //灼热打击
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Damage(12, this, target, 0);
				break;
			}
			else
				return;
		}

		case 251: //灼热打击+1
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Damage(16, this, target, 0);
				break;
			}
			else
				return;
		}

		//灼热打击+n伤害为 n * (n+7) + 12，未实现

		//case 52: //重振精神

		case 53: //盛怒
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Energy += 2;
				break;
			}
			else
				return;
		}

		case 253: //盛怒+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Energy += 2;
				break;
			}
			else
				return;
		}

		case 54: //哨卫
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Defence(5, this, NULL);
				break;
			}
			else
				return;
		}

		case 354: //哨卫+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Defence(8, this, NULL);
				break;
			}
			else
				return;
		}

		//case 55

		case 56: //震荡波
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				int iter = 0;
				for (; iter < EnemyNum; iter++)
				{
					(target + iter)->State_Weak += 3;
				}
				break;
			}
			else
				return;
		}

		case 256: //震荡波+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				int iter = 0;
				for (; iter < EnemyNum; iter++)
				{
					(target + iter)->State_Weak += 5;
				}
				break;
			}
			else
				return;
		}

		//case 57: //

		case 58: //上勾拳
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Damage(13, this, target, 0);
				target->State_Vulnerable += 1;
				target->State_Weak += 1;
				break;
			}
			else
				return;
		}

		case 258: //上勾拳+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addTo(cardnum, DiscardPile, DiscardPtr);
				GameDeck[cardnum]->Damage(13, this, target, 0);
				target->State_Vulnerable += 2;
				target->State_Weak += 2;
				break;
			}
			else
				return;
		}

		case 59: //旋风斩
		{
			addTo(cardnum, DiscardPile, DiscardPtr);
			if (Energy == 0)
				return;
			else
			{ 
				int x = Energy;
				Energy -= 0;
				int i = 1;
				for (; i <= x; i++)
				{
					GameDeck[cardnum]->Damage(5, this, target, EnemyNum);
				}
				break;
			}
		}

		case 259: //旋风斩+
		{
			addTo(cardnum, DiscardPile, DiscardPtr);
			if (Energy == 0)
				return;
			else
			{
				int x = Energy;
				Energy -= 0;
				int i = 1;
				for (; i <= x; i++)
				{
					GameDeck[cardnum]->Damage(8, this, target, EnemyNum);
				}
				break;
			}
		}

		case 60: //壁垒
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Barricade = 1;
				break;
			}
			else
				return;
		}

		case 260: //壁垒+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Barricade = 1;
				break;
			}
			else
				return;
		}

		case 61: //
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->State_Vulnerable += 3;
				this->Berserk += 1;
				break;
			}
			else
				return;
		}

		case 261: //
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->State_Vulnerable += 2;
				this->Berserk += 1;
				break;
			}
			else
				return;
		}

		case 62: //重锤
		{
			addTo(cardnum, DiscardPile, DiscardPtr);
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(32, this, target, 0);
				break;
			}
			else
				return;
		}

		case 262: //重锤+
		{
			addTo(cardnum, DiscardPile, DiscardPtr);
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(42, this, target, 0);
				break;
			}
			else
				return;
		}


		case 63: //残暴
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Brutality += 1;
				break;
			}
			else
				return;
		}

		case 263: //残暴+ //固有不想做
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Brutality += 1;
				break;
			}
			else
				return;
		}
		//case 64: //黑暗之拥

		case 65: //恶魔形态
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->StrengthUp += 2;
				break;
			}
			else
				return;
		}

		case 265: //恶魔形态+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->StrengthUp += 3;
				break;
			}
			else
				return;
		}

		//case 66: //双发

		//case 67: //发掘 不做

		case 68: //狂宴 消耗未实现
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(10, this, target, 0);
				if (target->EnemyHP <= 0)
				{
					this->HPMax += 3;
					this->HP += 3;
				}
				break;
			}
			else
				return;
		}

		case 268: //狂宴 消耗未实现
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(12, this, target, 0);
				if (target->EnemyHP <= 0)
				{
					this->HPMax += 4;
					this->HP += 4;
				}
				break;
			}
			else
				return;
		}

		//case 69: //恶魔之焰

		case 70: //燔祭
		{
			addTo(cardnum, DiscardPile, DiscardPtr);
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(21, this, target, EnemyNum);
				addTo(129, DiscardPile, DiscardPtr);
				break;
			}
			else
				return;
		}

		case 270: //燔祭+
		{
			addTo(cardnum, DiscardPile, DiscardPtr);
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(28, this, target, EnemyNum);
				addTo(129, DiscardPile, DiscardPtr);
				break;
			}
			else
				return;
		}

		case 71: //岿然不动，消耗未实现
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Defence(30, this, NULL);
				break;
			}
			else
				return;
		}

		case 271: //岿然不动+，消耗未实现
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Defence(40, this, NULL);
				break;
			}
			else
				return;
		}

		case 72: //势不可挡
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Juggernaut += 5;
				break;
			}
			else
				return;
		}


		case 272: //势不可挡+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Juggernaut += 7;
				break;
			}
			else
				return;
		}

		case 73: //突破极限，消耗未实现
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Strength *= 2;
				break;
			}
			else
				return;
		}

		case 273: //突破极限+
		{
			addTo(cardnum, DiscardPile, DiscardPtr);
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Strength *= 2;
				break;
			}
			else
				return;
		}

		case 74: //祭品，消耗未实现
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->HP -= 6;
				this->Energy += 2;
				draw(3);
				break;
			}
			else
				return;
		}

		case 274: //祭品+，消耗未实现
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->HP -= 6;
				this->Energy += 2;
				draw(5);
				break;
			}
			else
				return;
		}

		//case 75，死亡收割
	
	}
}

void StateVariables::Defend(int DefendVal, Enemy* target = NULL, int EnemyNum = 0)   //有些获得格挡的效果是与敏捷虚弱无关的，所以保留这个函数
{
	Block += DefendVal;
	if (Juggernaut)
	{
		randomDamage(Juggernaut, target, EnemyNum);
	}
}

void StateVariables::randomDamage(int damage, Enemy* target, int n)
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

void StateVariables::combust(Enemy* target, int EnemyNum)
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

void StateVariables::metallicize(Enemy* target = NULL, int EnemyNum = 0)
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

/*int StateVariables::isStrike(int cardnum)
{
	if (cardnum == 3 || cardnum == 203 || cardnum == 15 || cardnum == 215
		|| cardnum == 16 || cardnum == 216 || cardnum == 21 || cardnum == 221
		|| cardnum == 23 || cardnum == 223 || cardnum == 99 || cardnum == 299)
	{
		return 1;
	}
	else
		return 0;
}*/

