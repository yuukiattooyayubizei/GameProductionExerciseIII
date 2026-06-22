#pragma once
#include <string>

class CLog {
private:
    static CLog* m_Instance;
public:
    static CLog* GetInstance();
    static void DeleteInstance();
private:
    static constexpr int LOG_LINE_NUM = 5;
    static constexpr int DRAW_X = 16;
    static constexpr int DRAW_Y = 16;
    static constexpr int DRAW_DISTANCE_Y = 28;
    std::string m_Log[LOG_LINE_NUM];

    CLog() = default;
public:
    CLog(const CLog&) = delete;

    void Init();
    void Draw();
    void AddLog(const std::string& log);
};