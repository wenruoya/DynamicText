#include <stdio.h>
#include<Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include <opencv2/opencv.hpp>

using namespace cv;

int main(void) {
	// 1.����Ƶ�ļ�
	// �������  ������Ƶ�ļ�
	VideoCapture video;

	bool ret = video.open("siu.mp4");
	if (ret == false) {
		printf("��Ƶ�ļ���ʧ�ܣ���");
		return 1;
	}
	// ��ȡ�����Ƶ�ļ�  �ж���֡
	int frameCount = video.get(CAP_PROP_FRAME_COUNT);
	int rows = video.get(CAP_PROP_FRAME_HEIGHT);
	int cols = video.get(CAP_PROP_FRAME_WIDTH);
	int fps = video.get(CAP_PROP_FPS); //ÿ�벥�ż�֡

	// ����Ƶ�л�ȡ����ͼƬ
	Mat frameImg, grayImg;

	int hSize = 2;
	int wSize = 1;
	// �����ڴ�
	int frameSize = (rows / hSize) * (cols / wSize);
	char* data = (char*)malloc(frameSize * frameCount);

	for (int n = 0; n < frameCount; n++)
	{
		//����Ƶ֡  ת��Ϊ�ַ���

	
	//��õ��ڼ�����Ƶ֡ͼƬ
	video.read(frameImg);  // ����Ƶ�ļ��ж�ȡһ֡ͼƬ

	// �Ѳ�ɫͼƬתΪ�ڰ�
	cvtColor(frameImg, grayImg, COLOR_BGR2GRAY);
	
	char* p = data + frameSize * n;
	int k = 0;

	if (data == NULL) {
		printf("�ڴ治��");
		return 1;
	}
	// �Ѻڰ�ͼƬ  ��Ϊ�ַ���
	int hSize = 2; // ÿ��С��ĸ߶�
	int wSize = 1; // 
	for (int row = 0; row < rows; row += hSize)
	{
		for (int col = 0; col < cols; col += wSize)
		{
			//��ȡ������
			int value = grayImg.at<uchar>(row, col);
			char charImgs[] = ".,-'`:!1&@#$";
			p[k++] = charImgs[value / 20];
		}
		p[k++] = '\n';
	}
	p[k] = '\0';
	printf("����ת����%d֡��һ��%d֡��\n",n+1,frameCount);
	}
	char cmd[256];
	sprintf_s(cmd, sizeof(cmd), "mode con cols=%d lines=%d",cols/wSize, rows/hSize+1);
	system(cmd);  //���ÿ���̨�Ĵ�С


	//��������
	/*mciSendString("play xx.mp3 repeat", 0, 0, 0);*/

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { 0,0 };
	while (1) {
		for (int i = 0; i < frameCount; i++)
		{
			char* p = data + i * frameSize;

			// �ѹ���ƶ����ն˵����Ͻ�
			SetConsoleCursorPosition(h, pos);
			printf("%s", p);

			// ֡�ȴ�  �����ٶ�
			Sleep(1000 / fps);
		}
	}
	return 0;
}