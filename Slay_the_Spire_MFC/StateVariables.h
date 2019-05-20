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
	unsigned int Hand[10];//����
	Cards* GameDeck[300];//��Ϸ�ƿ�
	unsigned int GameDeckPoint;//��Ϸ�ƿ�ָ��
	Cards* Deck[100];//����ƿ�
	unsigned int DeckPoint;//����ƿ�ָ��
	int DrawNum;
	int DrawPile[100];//���ƶ�����
	int DiscardPile[100];
	int DrawPoint;//���ƶ�ָ��
	int DiscardPoint;//���ƶ�ָ��
	int HandPoint;//����ָ��
	int StrengthUp;//ÿ�غ���������
	int Vulnerable;//����
	int Weak;//����
	int Berserk;//�»غϿ�ʼ���Energy
	int Frail;//�Ƽ�
	float WeakCoefficient;//����ϵ��
	int CanDraw;//�Ƿ�ɼ�������
	int FlameBarrier;//FBЧ��
	int Juggernaut;//JuggernautЧ��
	int Combust;//CombustЧ��
	int Evolve;//EvolveЧ��
	int Metallicize;//MetallicizeЧ��
	int Rage;//RageЧ��
	int RampageTime;//Rampageʹ�ô���
	int Rupture;//RuptureЧ��
	int Barricade;//BarricadeЧ��
	int Brutality;//BrutalityЧ��

	void draw(int drawnum);
	void usecard(int cardnum, Enemy* target, int n);
	void addToDrawPile(int cardnum);

};

class Enemy
{
public:
	Enemy(int hp, unsigned int damage1, unsigned int attackTimes1 = 1, unsigned int damage2 = 0, unsigned int attackTimes2 = 0, unsigned int damage3 = 0, unsigned int attackTimes3 = 0);
	~Enemy(void);
	int EnemyHP;//����Ѫ��
	unsigned int EnemyBlock;//���ﻤ��
	unsigned int Damage1;//���﹥������1�˺�
	unsigned int AttackTimes1;//���﹥������1����
	unsigned int Damage2;//���﹥������2�˺�
	unsigned int AttackTimes2;//���﹥������2����
	unsigned int Damage3;//���﹥������3�˺�
	unsigned int AttackTimes3;//���﹥������3����

	int State_Weak;//����
	int State_Vulnerable;//����
	int State_Poison;//�ж�
	int State_Ritual;//��ʽ���غϽ����ӹ�
	unsigned int strength;//���������������˺�
	float Vulnerable;
	int intend;//�´�ִ������

	void debuffWeak(int n, StateVariables* player);//��n������
	void debuffMucus(int n, StateVariables* player);//��ճҺ��
	void debuffVulnerable(int n, StateVariables* player);//��n������
	void buffRitual();//��ʽЧ��
	void buffStrengthUp(unsigned int n);//��n����
	void buffFrail(int n, StateVariables* player);//��n���Ƽ�
};

class Cards
{
public:
	Cards(void);
	~Cards(void);
	unsigned int CardsNum;//���Ʊ��
	unsigned int EnergyCost;//����
	unsigned int EnergyCost_Up;//���������
	int GainBlock;//��û���ֵ
	int GainBlock_Up;//�������û���ֵ
	int Damage;//����˺�ֵ
	int Damage_Up;//����������˺�ֵ
	int LevelUp;//�Ƿ���������1��ʾ������0��ʾδ����

	int EnergyConsume(int x, StateVariables* player);
};