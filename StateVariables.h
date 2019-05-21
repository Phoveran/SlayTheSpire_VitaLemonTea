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
	int Frail;//���Ƽ�
	int CanDraw;//�Ƿ�ɼ�������
	int FlameBarrier;//FBЧ��
	int Juggernaut;//JuggernautЧ��
	int Combust;//CombustЧ��
	int Metallicize;//MetallicizeЧ��
	int RampageTime;//Rampageʹ�ô���
	int Barricade;//BarricadeЧ��
	int Brutality;//BrutalityЧ��

	void draw(int drawnum);
	void usecard(int cardnum, Enemy* target, int n, Cards* GameDeck[], StateVariables* player);
	void addTo(int cardnum, int Pile[], int pilePoint);
	void randomDamage(int damage, Enemy* target, int n);
	void combust(Enemy* target, int EnemyNum);
	void metallicize(Enemy* target, int EnemyNum);
	void Dexup(int DexupVal, Enemy* target, int EnemyNum);
	void brutality(void);
};

class Enemy
{
public:
	Enemy(int hp, unsigned int damage1, unsigned int attackTimes1 = 1, unsigned int damage2 = 0, unsigned int attackTimes2 = 0, unsigned int damage3 = 0, unsigned int attackTimes3 = 0);
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
	int strength;//���������������˺�
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
	Cards(int cardnum, int EnergyCost);
	~Cards(void);
	unsigned int CardsNum;//���Ʊ��
	unsigned int EnergyCost;//����
	int EnergyConsume(int x, StateVariables* player);
	void Damage(int damage, StateVariables* player, Enemy* target, int n);
	void Defence(int block, StateVariables* player);
};