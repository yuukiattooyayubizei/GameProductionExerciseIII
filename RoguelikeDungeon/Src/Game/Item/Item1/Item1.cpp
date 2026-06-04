#include "Item1.h"
#include "../../Object/Player/Player.h"
#include "../../Object/Object.h"
#include <iostream>

CItem1::CItem1() {

}

void CItem1::Init(){
    m_FieldItem.item.type = ITEM_1;
	m_FieldItem.item.type = ITEM_NON;
	m_FieldItem.pos.x = -1;
	m_FieldItem.pos.y = -1;
	m_Active = true;
}

void CItem1::Load() {
}

void CItem1::Step() {
}

void CItem1::Draw() {
}

void CItem1::Exit() {
}

bool CItem1::Use(Int2* plPos,  std::vector<CObject*>& targets) {
	
    return true;
}