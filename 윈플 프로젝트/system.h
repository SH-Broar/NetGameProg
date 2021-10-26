#pragma once
#include<Windows.h>
#include<atlimage.h>
#include<string>
#include<random>
using namespace std;

#define Window_XSize 900
#define Window_YSize 900
#define Image_Size 130
#define Yellow_Sit_Lenght 101
#define Yellow_Sit_Raw 130
#define Block_IMG_Size 50
#define Thorn_Size 60
#define Block_Size_Lenght 90
#define Block_Size_Raw 30
#define CrawlNum 6
#define IdleNum 32
#define JumpDownNum 5
#define JumpUpNum 3
#define RunNum 8
#define SitNum 13
#define TumblingNum 12
#define Background_SIZE_Lenght 884
#define Background_SIZE_Raw 861
#define Background4_SIZE_Lenght 1440
#define Background4_SIZE_Raw 3440

constexpr int Pause_MaxCount = 45;
constexpr int PauseButton_Size = 50;
constexpr int RetryButton_Size_Lenght = 266;
constexpr int RetryButton_Size_Raw = 100;

constexpr int Stand_xMove = 8;
constexpr int Crawl_xMove = 4;
constexpr int Crush_Power_x = 8;
constexpr int Crush_Power_y = 6;
constexpr int Bounce_Power_x = 8;
constexpr int Bounce_Power_y = 10;
constexpr int Crush_Down_Power = 8;
constexpr int FallOut_Power = 30;

constexpr float Jump_Power = 2.5f;
constexpr float Speed_Power = 3.f;
constexpr float Gravity = 0.3f;

constexpr int Window_Clinet_Left = 0;
constexpr int Window_Clinet_Top = 0;
constexpr int Window_Clinet_Right = 884;
constexpr int Window_Clinet_Bottom = 861;

constexpr int BlockNum = 95;
constexpr int NormalBlockNum = 40;
constexpr int ColorBlockNum_Pupple = 12;
constexpr int ColorBlockNum_Yellow = 12;
constexpr int BreakBlockNum = 12;
constexpr int ThornBlockNum = 6;
constexpr int DaggerBlockNum = 12;
constexpr int WallBlockNum = 100;

constexpr int WallBlock_Size = 30;

constexpr int DaggerSpeed = 20;

constexpr int ScorePoint = 10;

constexpr int ShakeNum = 50;