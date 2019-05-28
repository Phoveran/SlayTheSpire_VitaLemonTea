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
	Deck[9] = GameDeck[2];//����ֻ��ʮ�ų�ʼ�Ƶ��ƿ�
	int i;
	for (i = 10; i <= 99; i++)
	{
		Deck[i] = 0;//ʣ�µĿ�λ��0��ʾ
	}
	DrawNum = 5;//ÿ�غϳ�5����
	DeckPoint = 10;
	DrawPoint = 0;
	DiscardPoint = 0;
	HandPoint = 0;
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
	Juggernaut = 0;//JuggernautЧ��
	Combust = 0;//CombustЧ��
	Metallicize = 0;//MetallicizeЧ��
	RampageTime = 0;//Rampageʹ�ô���
	Barricade = 0;//BarricadeЧ��
	Brutality = 0;//BrutalityЧ��
	Evolve = 0;//����Ч��
	EnemyNum = 0;
	Rage = 0;
}

StateVariables::~StateVariables(void)
{
}


void StateVariables::shuffle(Cards* Pile[], int pilePoint)
{
	int iter;
	int i, j;
	if (pilePoint == 1)
		return;
	for (iter = 1; iter <= 3 * (pilePoint); iter++)
	{
		int i = random(pilePoint);
		int j = random(pilePoint);
		while (j == i)
			j = random(pilePoint);
		Cards* hold = Pile[i];
		Pile[i] = Pile[j];
		Pile[j] = hold;
	}
}

void StateVariables::draw(int drawnum)
{
	int i;
	if (DrawPoint < drawnum)
	{
		if ((DrawPoint + DiscardPoint) < drawnum)
			draw(DrawPoint + DiscardPoint);
		else
		{
			int drawnum_1 = drawnum - DrawPoint;
			draw(DrawPoint);
			for (i = 0; i < DiscardPoint; i++)
			{
				DrawPile[i] = DiscardPile[i];
				DiscardPile[i] = 0;
			}
			DrawPoint = DiscardPoint;
			shuffle(DrawPile, DrawPoint);
			DiscardPoint = 0;
			draw(drawnum_1);
		}
	}
	for (i = 0; i < drawnum; i++)
	{
		Hand[HandPoint + i + 1] = DrawPile[DrawPoint - i];
		DrawPile[DrawPoint - i] = 0;
	}
	HandPoint = HandPoint + drawnum;
	DrawPoint = DrawPoint - drawnum;
};

void StateVariables::addToDiscard(int cardnum)
{
	srand((unsigned int)(time(NULL)));
	int i = random(DiscardPoint);
	int j;
	for (j = DiscardPoint - 1; j > i; j--)
	{
		DiscardPile[j + 1] = DiscardPile[j];
	}
	DiscardPile[i] = GameDeck[cardnum];
	DiscardPoint += 1;
}

void StateVariables::addToHand(int cardnum)
{
	if (HandPoint <= 9)
	{
		Hand[HandPoint] = GameDeck[cardnum];
		HandPoint += 1;
	}
	else
	{
		addToDiscard(cardnum);
	}
}

void StateVariables::addToDraw(int cardnum)
{
	srand((unsigned int)(time(NULL)));
	int i = random(DrawPoint);
	int j;
	for (j = DrawPoint - 1; j > i; j--)
	{
		DrawPile[j + 1] = DrawPile[j];
	}
	DrawPile[i] = GameDeck[cardnum];
	DrawPoint += 1;
}

void StateVariables::usecard(int cardnum, Enemy* target = NULL, int n = 0)
{
	switch (cardnum)
	{
	    case 1: //ʹ��
	    {
		    if (GameDeck[cardnum]->EnergyConsume(this))
		    {
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(8, this, target, 0);
			    target->State_Vulnerable += 2;
			    break;
		    }
		    else
			    return;
	    }

	    case 201: //ʹ��+
	    {
		    if (GameDeck[cardnum]->EnergyConsume(this))
		    {
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(11, this, target, 0);
			    target->State_Vulnerable += 10;
			    break;
		    }
		    else
			    return;
	    }

	    case 2: //����
	    {
		    if (GameDeck[cardnum]->EnergyConsume(this))
		    {
				addToDiscard(cardnum);
				GameDeck[cardnum]->Defence(5, this, NULL);
				break;
		    }
		    else
			    return;
	    }

		case 202: //����+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Defence(8, this, NULL);
				break;
			}
			else
				return;
		}

		case 3: //���
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(6, this, target, 0);
				break;
			}
			else
				return;
		}

		case 203: //���+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(9, this, target, 0);
				break;
			}
			else
				return;
		}

		case 4: //��ŭ
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(6, this, target, 0);
				addToDiscard(4);
				break;
			}
			else
				return;
		}

		case 204: // ��ŭ+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(9, this, target, 0);
				addToDiscard(204);
				break;
			}
			else
				return;
		}
		//case 5:

		case 205: //��װ+ �� ��ҪUpgrade����
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				int i = 0;
				for (; i < HandPoint; i++)
				{
					//Upgrade[Hand[i]];
				}
				GameDeck[cardnum]->Defence(5, this, NULL);
				break;
			}
			else
				return;
		}

		case 6: //ȫ��ײ��
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(this->Block, this, target, 0);
				break;
			}
			else
				return;
		}

		case 206: //ȫ��ײ��+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(this->Block, this, target, 0);
				break;
			}
			else
				return;
		}
		case 7: //����
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				int i = 0;
				for (; i < HandPoint; i++)
				{
					if (Hand[i]->Kind != 0)
						return;
				}
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(14, this, target, 0);
				break;
			}
			else
				return;
		}

		case 207: //����+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				int i = 0;
				for (; i < HandPoint; i++)
				{
					if (Hand[i]->Kind != 0)
						return;
				}
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(18, this, target, 0);
				break;
			}
			else
				return;
		}

		case 8: //˳��ն
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(8, this, target, EnemyNum);
				break;
			}
			else
				return;
		}

		case 208: //˳��ն+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(11, this, target, EnemyNum);
				break;
			}
			else
				return;
		}

		case 9: //��ձ�
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(12, this, target, 0);
				target->State_Weak += 2;
				break;
			}
			else
				return;
		}

		case 209: //��ձ�+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(14, this, target, 0);
				target->State_Weak += 3;
				break;
			}
			else
				return;
		}

		case 10: //�����
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				this->Strength += 2;
				this->StrengthUpTemp += 2;
				break;
			}
			else
				return;
		}

		case 210: //�����+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				this->Strength += 4;
				this->StrengthUpTemp += 4;
				break;
			}
			else
				return;
		}

		case 11: //����
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				usecard(DrawPile[DrawPoint - 1]->CardsNum, target, n); //���Ĺ���ûʵ��
				DrawPile[DrawPoint - 1] = 0;
				DrawPoint -= 1;
				break;
			}
			else
				return;
		}

		case 211: //����+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				usecard(DrawPile[DrawPoint - 1]->CardsNum, target, n); //���Ĺ���ûʵ��
				DrawPile[DrawPoint - 1] = 0;
				DrawPoint -= 1;
				break;
			}
			else
				return;
		}

		//case 12

		case 13: //����
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(9 + this->Strength * 2, this, target, 0);
				break;
			}
			else
				return;
		}

		case 213: //����+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(12 + this->Strength * 4, this, target, 0);
				break;
			}
			else
				return;
		}

		case 14: //��ն��
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Defence(5, this, NULL);
				GameDeck[cardnum]->Damage(5, this, target, 0);
				break;
			}
			else
				return;
		}
		case 214: //��ն��+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Defence(7, this, NULL);
				GameDeck[cardnum]->Damage(7, this, target, 0);
				break;
			}
			else
				return;
		}

/*		case 15: //�������
		{
		    if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
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

		case 215: //�������+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
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
		

		case 16: //�������
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(9, this, target, 0);
				draw(1);
				break;
			}
			else
				return;
		}
		case 216: //�������+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(10, this, target, 0);
				draw(2);
				break;
			}
			else
				return;
		}

		case 17: //�ʼ�����
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Defence(8, this, NULL);
				draw(1);
				break;
			}
			else
				return;
		}

		case 217: //�ʼ�����+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Defence(11, this, NULL);
				draw(1);
				break;
			}
			else
				return;
		}

		case 18: //�ɽ�������
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				randomDamage(3, NULL, 3);
				break;
			}
			else
				return;
		}

		case 218: //�ɽ�������+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				randomDamage(3, NULL, 4);
				break;
			}
			else
				return;
		}

		case 19: // ��������
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
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

		case 219: // ��������+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
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

		case 21: //˫�ش��
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(5, this, target, 0);
				GameDeck[cardnum]->Damage(5, this, target, 0);
				break;
			}
			else
				return;
		}

		case 221: //˫�ش��+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(7, this, target, 0);
				GameDeck[cardnum]->Damage(7, this, target, 0);
				break;
			}
			else
				return;
		}

		//case 22: //ս��

		case 23: //��Ұ���
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(11, this, target, 0);
				addToDraw(131);
				break;
			}
			else
				return;
		}
		case 223: //��Ұ���+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(14, this, target, 0);
				addToDraw(131);
				break;
			}
			else
				return;
		}

		case 24: //ս��רע
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				draw(3);
				this->CanDraw = 0;
				break;
			}
			else
				return;
		}

		case 224: //ս��רע+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				draw(4);
				this->CanDraw = 0;
				break;
			}
			else
				return;
		}

		//case 25 ��Ѫ��Ѫ����Ҫ��DamageCount������

		case 26: //��Ѫ
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				this->HP -= 3;
				this->Energy += 1;
				break;
			}
			else
				return;
		}
	
		case 226: //��Ѫ+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				this->HP -= 3;
				this->Energy += 2;
				break;
			}
			else
				return;
		}

		//case 27

		case 28: //��ɱ�� ����δʵ��
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(20, this, target, 0);
				break;
			}
			else
				return;
		}

		case 228: //��ɱ+�� ����δʵ��
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(28, this, target, 0);
				break;
			}
			else
				return;
		}
     
		case 29: //��ȼ
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Combust += 5;
				break;
			}
			else
				return;
		}

		case 229: //��ȼ+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Combust += 7;
				break;
			}
			else
				return;
		}

		//case 30: //����

		case 31: //��е
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				target->Strength -= 2;
				break;
			}
			else
				return;
		}

		case 231: //��е+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				target->Strength -= 3;
				break;
			}
			else
				return;
		}

		case 32: //������
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
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

		case 232: //������+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
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

		case 34: //����
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				this->Block *= 2;
				break;
			}
			else
				return;
		}

		case 234: //����+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				this->Block *= 2;
				break;
			}
			else
				return;
		}

		case 35: //����
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Evolve += 1;
				break;
			}
			else
				return;
		}

		case 235: //����+
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

		case 38: //��������
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Defence(12, this, NULL);
				this->FlameBarrier += 4;
				break;
			}
			else
				return;
		}

		case 238: //��������+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Defence(16, this, NULL);
				this->FlameBarrier += 6;
				break;
			}
			else
				return;
		}

		case 39: //�������ף� ����δʵ��
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Defence(10, this, NULL);
				break;
			}
			else
				return;
		}

		case 239: //��������+�� ����δʵ��
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Defence(13, this, NULL);
				break;
			}
			else
				return;
		}

		case 40: //��Ѫ��
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum); 
				this->HP -= 3;
				GameDeck[cardnum]->Damage(14, this, target, 0);
				break;
			}
			else
				return;
		}

		case 240: //��Ѫ��+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				this->HP -= 2;
				GameDeck[cardnum]->Damage(18, this, target, 0);
				break;
			}
			else
				return;
		}

		/*case 41: //����֮��
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

		case 42: // ȼ��
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Strength += 2;
				break;
			}
			else
				return;
		}

		case 242: // ȼ��+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Strength += 3;
				break;
			}
			else
				return;
		}

		case 43: //����
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

		case 243: //����+
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

		case 44: //������
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Metallicize += 3;
				break;
			}
			else
				return;
		}

		case 244: //������+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Metallicize += 4;
				break;
			}
			else
				return;
		}

		case 45: //Ӳ��
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				addToHand(131);
				addToHand(131);
				GameDeck[cardnum]->Defence(15, this, NULL);
				break;
			}
			else
				return;
		}

		case 245: //Ӳ��+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				addToHand(131);
				addToHand(131);
				GameDeck[cardnum]->Defence(20, this, NULL);
				break;
			}
			else
				return;
		}

		case 46: //����ȭ
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

		case 246: //����ȭ+
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

		case 47: //��ŭ
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Rage += 4;
				break;
			}
			else
				return;
		}

		case 247: //��ŭ+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Rage += 6;
				break;
			}
			else
				return;
		}

		case 48: //����
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(8 + RampageTime * 5, this, target, 0);
				this->RampageTime += 1;
				break;
			}
			else
				return;
		}

		case 248: //����+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(8 + RampageTime * 8, this, target, 0);
				this->RampageTime += 1;
				break;
			}
			else
				return;
		}

		case 49: //��ı���
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);

				GameDeck[cardnum]->Damage(7, this, target, 0);
				addToDraw(130);
				break;
			}
			else
				return;
		}

		case 249: //��ı���+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(10, this, target, 0);
				addToDraw(130);
				break;
			}
			else
				return;
		}

		//case 50: //˺��

		case 51: //���ȴ��
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(12, this, target, 0);
				break;
			}
			else
				return;
		}

		case 251: //���ȴ��+1
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(16, this, target, 0);
				break;
			}
			else
				return;
		}

		//���ȴ��+n�˺�Ϊ n * (n+7) + 12��δʵ��

		//case 52: //������

		case 53: //ʢŭ
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Energy += 2;
				break;
			}
			else
				return;
		}

		case 253: //ʢŭ+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Energy += 2;
				break;
			}
			else
				return;
		}

		case 54: //����
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Defence(5, this, NULL);
				break;
			}
			else
				return;
		}

		case 354: //����+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Defence(8, this, NULL);
				break;
			}
			else
				return;
		}

		//case 55

		case 56: //�𵴲�
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

		case 256: //�𵴲�+
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

		case 58: //�Ϲ�ȭ
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(13, this, target, 0);
				target->State_Vulnerable += 1;
				target->State_Weak += 1;
				break;
			}
			else
				return;
		}

		case 258: //�Ϲ�ȭ+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				addToDiscard(cardnum);
				GameDeck[cardnum]->Damage(13, this, target, 0);
				target->State_Vulnerable += 2;
				target->State_Weak += 2;
				break;
			}
			else
				return;
		}

		case 59: //����ն
		{
			addToDiscard(cardnum);
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

		case 259: //����ն+
		{
			addToDiscard(cardnum);
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

		case 60: //����
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Barricade = 1;
				break;
			}
			else
				return;
		}

		case 260: //����+
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

		case 62: //�ش�
		{
			addToDiscard(cardnum);
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(32, this, target, 0);
				break;
			}
			else
				return;
		}

		case 262: //�ش�+
		{
			addToDiscard(cardnum);
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(42, this, target, 0);
				break;
			}
			else
				return;
		}


		case 63: //�б�
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Brutality += 1;
				break;
			}
			else
				return;
		}

		case 263: //�б�+ //���в�����
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Brutality += 1;
				break;
			}
			else
				return;
		}
		//case 64: //�ڰ�֮ӵ

		case 65: //��ħ��̬
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->StrengthUp += 2;
				break;
			}
			else
				return;
		}

		case 265: //��ħ��̬+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->StrengthUp += 3;
				break;
			}
			else
				return;
		}

		//case 66: //˫��

		//case 67: //���� ����

		case 68: //���� ����δʵ��
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

		case 268: //���� ����δʵ��
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

		//case 69: //��ħ֮��

		case 70: //�ܼ�
		{
			addToDiscard(cardnum);
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(21, this, target, EnemyNum);
				addToDiscard(129);
				break;
			}
			else
				return;
		}

		case 270: //�ܼ�+
		{
			addToDiscard(cardnum);
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Damage(28, this, target, EnemyNum);
				addToDiscard(129);
				break;
			}
			else
				return;
		}

		case 71: //��Ȼ����������δʵ��
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Defence(30, this, NULL);
				break;
			}
			else
				return;
		}

		case 271: //��Ȼ����+������δʵ��
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				GameDeck[cardnum]->Defence(40, this, NULL);
				break;
			}
			else
				return;
		}

		case 72: //�Ʋ��ɵ�
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Juggernaut += 5;
				break;
			}
			else
				return;
		}


		case 272: //�Ʋ��ɵ�+
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Juggernaut += 7;
				break;
			}
			else
				return;
		}

		case 73: //ͻ�Ƽ��ޣ�����δʵ��
		{
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Strength *= 2;
				break;
			}
			else
				return;
		}

		case 273: //ͻ�Ƽ���+
		{
			addToDiscard(cardnum);
			if (GameDeck[cardnum]->EnergyConsume(this))
			{
				this->Strength *= 2;
				break;
			}
			else
				return;
		}

		case 74: //��Ʒ������δʵ��
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

		case 274: //��Ʒ+������δʵ��
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

		//case 75�������ո�
	
	}
}

void StateVariables::Defend(int DefendVal, Enemy* target = NULL, int EnemyNum = 0)   //��Щ��ø񵲵�Ч���������������޹صģ����Ա����������
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

