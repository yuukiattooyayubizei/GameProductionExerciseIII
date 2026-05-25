#include"Data.h"

CData* CData::m_Instance = NULL;

CData::CData() {

}

CData::~CData() {

}

void CData::Init() {

}

CData* CData::GetInstance() {
	//まだ生成されてないなら
	if (m_Instance == NULL)
	{
		m_Instance = new CData();
	}

	return m_Instance;
}

void CData::DeleteInstance() {
	//まだ生成されてないなら
	if (m_Instance)
	{
		delete m_Instance;
		m_Instance = NULL;
	}

}

void CData::Load() {
	//CSVファイルからデータを読み込み
	FILE* fp;

	//ファイルを開く　失敗したら即終了
	if (fopen_s(&fp, "../Data/Data/Trap1Data.csv", "r") != 0)return;

	// --- BOMチェック ---
	unsigned char bom[3];
	if (fread(bom, 1, 3, fp) == 3)
	{
		if (!(bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF))
		{
			// BOMじゃなければ先頭に戻す
			fseek(fp, 0, SEEK_SET);
		}
	}

	char line[256];
	float f;
	int i;




	//開いたファイルを閉じる
	fclose(fp);
}

