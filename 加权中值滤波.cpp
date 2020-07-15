/*
*┌────────────────────────────────────────────────┐
*│　描    述：根据原理编写程序，实现加权中值滤波												   │
*│　作    者：牟峙桦|BestBlade																	   │
*│　版    本：1.0																			   	   │
*│　创建时间：2020.07.15																		   │
*└────────────────────────────────────────────────┘
*/

#include <iostream>
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp>  
#include"opencv2/imgproc/imgproc.hpp"
#include <stdio.h>

using namespace std;
using namespace cv;

void weight_median_fliter(Mat& origin_pic, Mat& result_pic, Size ksize, int weight)
{
	if (origin_pic.empty())
	{
		cerr << "ERROR:输入图片为空" << endl;
	}

	if ((weight <= 0) || (weight % 2) == 0)
	{
		cerr << "ERROR:请输入正确权值（正奇数）" << endl;
	}

	result_pic = origin_pic.clone();
	Mat during_pic = result_pic.clone();

	int row = result_pic.rows;
	int col = result_pic.cols;

	int krow = ksize.width;
	int kcol = ksize.height;

	if ((krow % 2 == 0) || (kcol % 2 == 0))
	{
		cerr << "ERROR:滑窗口尺寸错误" << endl;												/*	平滑窗口尺寸错误	*/
	}
	else if (weight >= krow * kcol)
	{
		result_pic = origin_pic;
	}
	else
	{
		int median = (krow * kcol + weight - 1) / 2;

		int row_start = krow / 2;
		int col_start = kcol / 2;
		if (origin_pic.channels() == 1)
		{
			for (int i = row_start; i < row - row_start; i++)
			{
				for (int j = col_start; j < col - col_start; j++)
				{
					int n = { 0 };
					vector<int> channel(krow * kcol + weight - 1);

					for (int p = i - row_start; p <= i + row_start; p++)
					{
						for (int q = j - col_start; q <= j + col_start;)
						{
							if ((n > krow * kcol / 2) && n <= (krow * kcol / 2 - 1 + weight))
							{
								channel[n++] = channel[krow * kcol / 2];
							}
							else
								channel[n++] = during_pic.at<uchar>(p, q), q++;						/*	记录像素(i,j)周围模板的每一个像素值	*/
						}
					}

					std::sort(channel.begin(), channel.end());								/*	排序	*/		/*	std库和cv库都有sort函数，因此要注明	*/

					result_pic.at<uchar>(i, j) = channel[median];							/*	将中值赋给结果图第(i,j)个像素，完成中值滤波	*/
				}
			}
		}
		else if (origin_pic.channels() == 3)
		{
			for (int i = row_start; i < row - row_start; i++)
			{
				for (int j = col_start; j < col - col_start; j++)
				{
					int n = { 0 };
					vector<int> channel_r(krow * kcol + weight - 1);
					vector<int> channel_g(krow * kcol + weight - 1);
					vector<int> channel_b(krow * kcol + weight - 1);

					for (int p = i - row_start; p <= i + row_start; p++)
					{
						for (int q = j - col_start; q <= j + col_start;)
						{
							if ((n > krow * kcol / 2) && n <= (krow * kcol / 2 - 1 + weight))
							{
								channel_r[n] = channel_r[krow * kcol / 2];
								channel_g[n] = channel_g[krow * kcol / 2];
								channel_b[n] = channel_b[krow * kcol / 2];
								n++;
							}
							else
							{
								channel_r[n] = during_pic.at<Vec3b>(p, q)[2];					/*	记录像素(i,j)周围模板的每一个像素值	*/
								channel_g[n] = during_pic.at<Vec3b>(p, q)[1];
								channel_b[n] = during_pic.at<Vec3b>(p, q)[0];
								n++;
								q++;
							}
						}
					}

					std::sort(channel_r.begin(), channel_r.end());								/*	排序	*/		/*	std库和cv库都有sort函数，因此要注明	*/
					std::sort(channel_g.begin(), channel_g.end());
					std::sort(channel_b.begin(), channel_b.end());

					result_pic.at<Vec3b>(i, j)[2] = channel_r[median];							/*	将中值赋给结果图第(i,j)个像素，完成中值滤波	*/
					result_pic.at<Vec3b>(i, j)[1] = channel_g[median];
					result_pic.at<Vec3b>(i, j)[0] = channel_b[median];
				}
			}
		}
	}
}

int main()
{
	Mat origin_pic = imread("C:/Users/Chrysanthemum/Desktop/2.JPG", 1);							/*	三通道测试图	*/
	//Mat origin_pic = imread("C:/Users/Chrysanthemum/Desktop/1.jpg", -1);						/*	单通道测试图	*/

	Mat result_pic;
	Size ksize(3, 3);
	int weight = 3;

	cout << "图片通道数为" << origin_pic.channels() << endl;

	weight_median_fliter(origin_pic, result_pic, ksize, weight);

	imshow("原图", origin_pic);
	imshow("5x5平滑结果图", result_pic);

	waitKey(0);

	return 0;
}