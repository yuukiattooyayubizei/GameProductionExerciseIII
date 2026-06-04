#include "item.h"

CFieldItem::CFieldItem() {
	Init();
}

void CFieldItem::Init() {
	m_FieldItem.item.type = ITEM_NON;
	m_FieldItem.pos.x = -1;
	m_FieldItem.pos.y = -1;
	m_Active = true;
}

void CFieldItem::Load() {

}

void CFieldItem::Step() {

}

void CFieldItem::Draw() {

}

void CFieldItem::Exit() {

}
