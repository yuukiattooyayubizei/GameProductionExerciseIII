#pragma once

enum WEAPON_TYPE
{
	WEAPON_NON = -1,	//•Ší‚ª‚È‚¢ó‘Ô

	WEAPON_1,


	WEAPON_NUM,		//•Ší‚Ìí—Ş”
};

class CWeapon {
private:
	int m_Atk;
	WEAPON_TYPE m_WeaponType;

private:
	CWeapon();
	~CWeapon();

	void SetAtk(int atk) { m_Atk = atk; }
	void AddAtk(int atk) { m_Atk += atk; }
	int GetAtk() { return m_Atk; }

	void SetWeaponType(WEAPON_TYPE type) { m_WeaponType = type; }
	WEAPON_TYPE GetWeaponType() { return m_WeaponType; }
};