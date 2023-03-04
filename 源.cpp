#include <stdio.h>
#include<Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include <opencv2/opencv.hpp>

using namespace cv;

int main(void) {
	// 1.打开视频文件
	// 定义变量  处理视频文件
	VideoCapture video;

	bool ret = video.open("siu.mp4");
	if (ret == false) {
		printf("视频文件打开失败！！");
		return 1;
	}
	// 获取这个视频文件  有多少帧
	int frameCount = video.get(CAP_PROP_FRAME_COUNT);
	int rows = video.get(CAP_PROP_FRAME_HEIGHT);
	int cols = video.get(CAP_PROP_FRAME_WIDTH);
	int fps = video.get(CAP_PROP_FPS); //每秒播放几帧

	// 从视频中获取多张图片
	Mat frameImg, grayImg;

	int hSize = 2;
	int wSize = 1;
	// 分配内存
	int frameSize = (rows / hSize) * (cols / wSize);
	char* data = (char*)malloc(frameSize * frameCount);

	for (int n = 0; n < frameCount; n++)
	{
		//把视频帧  转换为字符串

	
	//获得到第几张视频帧图片
	video.read(frameImg);  // 从视频文件中读取一帧图片

	// 把彩色图片转为黑白
	cvtColor(frameImg, grayImg, COLOR_BGR2GRAY);
	
	char* p = data + frameSize * n;
	int k = 0;

	if (data == NULL) {
		printf("内存不足");
		return 1;
	}
	// 把黑白图片  变为字符串
	int hSize = 2; // 每个小块的高度
	int wSize = 1; // 
	for (int row = 0; row < rows; row += hSize)
	{
		for (int col = 0; col < cols; col += wSize)
		{
			//读取抽样点
			int value = grayImg.at<uchar>(row, col);
			char charImgs[] = ".,-'`:!1&@#$";
			p[k++] = charImgs[value / 20];
		}
		p[k++] = '\n';
	}
	p[k] = '\0';
	printf("正在转换第%d帧（一共%d帧）\n",n+1,frameCount);
	}
	char cmd[256];
	sprintf_s(cmd, sizeof(cmd), "mode con cols=%d lines=%d",cols/wSize, rows/hSize+1);
	system(cmd);  //设置控制台的大小


	//背景音乐
	/*mciSendString("play xx.mp3 repeat", 0, 0, 0);*/

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { 0,0 };
	while (1) {
		for (int i = 0; i < frameCount; i++)
		{
			char* p = data + i * frameSize;

			// 把光标移动到终端的左上角
			SetConsoleCursorPosition(h, pos);
			printf("%s", p);

			// 帧等待  控制速度
			Sleep(1000 / fps);
		}
	}
	return 0;
}