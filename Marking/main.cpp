#include <string>  
#include <vector>
#include <algorithm>
#include <iostream>
#include <cv.hpp>
#include <highgui.hpp>
#include <fstream>
#include <direct.h>  
#include <io.h>
#include <stack>
#include <queue>

using namespace std;
using namespace cv;

Mat original, image, temp;
ofstream out;

void on_mouse(int event, int x, int y, int flag, void *ustc)
{
	static Point begin(-1, -1), end(-1, -1);
	static Point prePoint(-1, -1), curPoint(-1, -1);
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		original.copyTo(image);
		begin = Point(x, y);
		prePoint = Point(x, y);
		imshow("image", image);
	}
	else if (event == CV_EVENT_MOUSEMOVE && !(flag & CV_EVENT_LBUTTONDOWN)) {
		original.copyTo(image);
		curPoint = Point(x, y);
		imshow("image", image);
	}
	else if (event == CV_EVENT_MOUSEMOVE && (flag & CV_EVENT_LBUTTONDOWN)) {
		image.copyTo(temp);
		curPoint = Point(x, y);
		rectangle(temp, prePoint, curPoint, Scalar(0, 0, 255, 0), 1, 8, 0);
		imshow("image", temp);
	}
	else if (event == CV_EVENT_LBUTTONUP)
	{
		original.copyTo(image);
		end = Point(x, y);
		curPoint = Point(x, y);
		rectangle(image, begin, end, Scalar(0, 0, 255, 0), 1, 8, 0);
		stringstream ss;
		ss << begin.x << " " << begin.y << " "  << begin.x << " " << end.y << " " << end.x << " " << begin.y << " "<< end.x << " " << end.y << "\n";
		string Stemp = ss.str();
		out.write(Stemp.c_str(), Stemp.size());
		image.copyTo(original);
		imshow("image", image);
	}
}

void getFiles(string path, vector<string>& files)
{
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				continue;
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void SplitC(vector<string> &result, const string &str, const char &pattern)
{
	queue<char> s;
	int i = 0, strSize = str.size();
	for (; i < strSize; i++)
	{
		if (str[i] != pattern)
		{
			s.push(str[i]);
		}
		else
		{
			string temp = "";
			while (!s.empty())
			{
				temp += s.front();
				s.pop();
			}
			result.push_back(temp);
		}
	}

	while (!s.empty())
	{
		string temp = "";
		while (!s.empty())
		{
			temp += s.front();
			s.pop();
		}
		result.push_back(temp);
	}
}

int main(void)
{
	char *filePath = "D:\\发票\\票据扫描\\zengzhishui\\data";
	string imagePath = "D:\\发票\\票据扫描\\zengzhishui\\data\\";
	string labelPath = "D:\\发票\\票据扫描\\zengzhishui\\label\\";
	vector<string> files;

	////获取该路径下的所有文件  
	getFiles(filePath, files);

	char str[30];
	int size = files.size();
	for (int i = 0; i < size; i++)
	{
		string file = files[i];
		vector<string> Name;
		SplitC(Name, file, '\\');
		string imageName = (*(Name.end() - 1));
		imageName.erase(imageName.end() - 4, imageName.end());
		out.open(labelPath + imageName + ".txt");
		original = imread(imagePath + imageName + ".bmp");
		namedWindow("image");
		imshow("image", original);
		setMouseCallback("image", on_mouse, 0);
		char c = waitKey(0);
		out.close();
		if (c == ' ')
			break;
	}

	return 0;
}
