#pragma once
#include "stdafx.h"

class Enemy;
class Cards;

class StateVariables
{
public:
	StateVariables(void);
	~StateVariables(void);
	int HP;//����ֵ
	int HPMax;//����ֵ����
	int Energy;//����ֵ
	int EnergyMax;//����ֵ����
	int Block;//����
	int Strength;//����
	int StrengthUpTemp;//���غ����ӵ�����
	int Dexterity;//����
	int DexterityUpTemp;//���غ����ӵ�����
	int Gold;//���
	Cards* Hand[10];//����
	Cards* GameDeck[300];//��Ϸ�ƿ�
	int GameDeckPoint;//��Ϸ�ƿ�����
	Cards* Deck[100];//����ƿ�
	int DeckPoint;//����ƿ�����
	int* DeckPtr;//����ƿ�����ָ��
	int DrawNum;
	Cards* DrawPile[100];//���ƶ�
	Cards* DiscardPile[100];
	int DrawPoint;//���ƶ�����
	int* DrawPtr;//���ƶ�����ָ��
	int DiscardPoint;//���ƶ�����
	int* DiscardPtr;//	���ƶ�����ָ��
	int HandPoint;//������
	int* HandPtr;//������ָ��
	int StrengthUp;//ÿ�غ���������
	int State_Vulnerable;//����
	int State_Weak;//����
	int Berserk;//�»غϿ�ʼ���Energy
	int Frail;//����
	int CantAttack;//���غ��ڲ��ܴ������
	int CanDraw;//�Ƿ�ɼ�������
	int FlameBarrier;//��������Ч��
	int Juggernaut;//�Ʋ��ɵ�Ч��
	int Combust;//CombustЧ��
	int Metallicize;//������Ч��
	int RampageTime;//Rampageʹ�ô���
	int Rage;//��ŭЧ��
	int Barricade;//BarricadeЧ��
	int Brutality;//BrutalityЧ��
	int Evolve;//����Ч��
	int EnemyNum;//��������

	void shuffle(Cards* Pile[], int* pilePoint);
	void draw(int drawnum);
	void usecard(int cardnum, Enemy* target, int n);
	void addTo(int cardnum, Cards* PileExample[], int* pilePoint);
	void addToHand(int cardnum);
	void randomDamage(int damage, Enemy* target, int n);
	void combust(Enemy* target, int EnemyNum);
	void metallicize(Enemy* target, int EnemyNum);
	void Defend(int DefendVal, Enemy* target, int EnemyNum);
	void brutality(void);
	void GameDeckIni();
};

class Enemy
{
public:
	Enemy(int hp, int damage1, int attackTimes1 = 1, int damage2 =0, int attackTimes2 = 0, int damage3 = 0, int attackTimes3 = 0);
	Enemy(Enemy &example);
	~Enemy(void);
	int EnemyHP;//����Ѫ��
	int EnemyBlock;//���ﻤ��
	int Damage1;//���﹥������1�˺�
	int AttackTimes1;//���﹥������1����
	int Damage2;//���﹥������2�˺�
	int AttackTimes2;//���﹥������2����
	int Damage3;//���﹥������3�˺�
	int AttackTimes3;//���﹥������3����

	int State_Weak;//����
	int State_Vulnerable;//����
	int State_Poison;//�ж�
	int State_Ritual;//��ʽ���غϽ����ӹ�
	int Strength;//���������������˺�
	int intend;//�´�ִ������
	int Dexterity;

	void debuffWeak(int n, StateVariables* player);//��n������
	void debuffCards(int cardnum, int n, StateVariables* player);//�����ƶ���
	void debuffVulnerable(int n, StateVariables* player);//��n������
	void buffRitual();//��ʽЧ��
	void buffStrengthUp(int n);//��n����
	void buffFrail(int n, StateVariables* player);//��n���Ƽ�
	void attack(int attackMethod, StateVariables* player);//��������
	void damage(int damage, StateVariables* target);//�˺�����
	void deffend(int deffendVal);//���ܺ���
	void net(int n, StateVariables* player);//��������

	void acidSlimeMiddle(StateVariables* player);
};

class Cards
{
public:
	Cards(int cardnum, int Cost, int kind, char* name);
	~Cards(void);
	unsigned int CardsNum;//���Ʊ��
	int EnergyCost;//����
	int Kind;//����
	CString suffix;
	CString bigSuffix;
	CString smallSuffix;
	CString BigPicPath;
	CString SmallPicPath;


	int EnergyConsume(StateVariables* player);
	void Damage(int damage, StateVariables* player, Enemy* target, int n);
	void Defence(int block, StateVariables* player, Enemy* target);
};