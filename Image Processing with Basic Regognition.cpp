
\


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


// LPR.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "highgui/highgui.hpp"
#include "core/core.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include "highgui\highgui.hpp"
#include "core\core.hpp"
#include <vector>
#include "imgproc.hpp"
#include <string>
#include <vector>
#include <chrono>
#include <ctime> 
/*#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>*/

using namespace std;
using namespace cv;
int n = 6;
int x = 65;
int y = 65;
int z = 65;

Mat converttogrey(Mat RGB)
{
	Mat Grey = Mat::zeros(RGB.size(), CV_8UC1);
	for (int i = 0; i < RGB.rows; i++)
	{
		for (int j = 0; j < RGB.cols * 3; j = j + 3)
		{
			Grey.at<uchar>(i, j / 3) = (RGB.at<uchar>(i, j) + RGB.at<uchar>(i, j + 1) + RGB.at<uchar>(i, j + 2)) / 3;
		}
	}
	return Grey;
}

Mat converttobinary(Mat Grey, int threshold)
{
	Mat Binary = Mat::zeros(Grey.size(), CV_8UC1);
	for (int i = 0; i < Grey.rows; i++)
		for (int j = 0; j < Grey.cols; j++)
			if (Grey.at<uchar>(i, j) >= threshold)
				Binary.at<uchar>(i, j) = 255;
	return Binary;
}

Mat StepFun(Mat Grey, int th1, int th2)
{
	Mat Step = Mat::zeros(Grey.size(), CV_8UC1);
	for (int i = 0; i < Grey.rows; i++)
		for (int j = 0; j < Grey.cols; j++)
			if (Grey.at<uchar>(i, j) >= th1 && Grey.at<uchar>(i, j) <= th2)
				Step.at<uchar>(i, j) = 255;
	return Step;
}

Mat Inversion(Mat Grey)
{
	Mat Invert = Mat::zeros(Grey.size(), CV_8UC1);
	for (int i = 0; i < Grey.rows; i++)
		for (int j = 0; j < Grey.cols; j++)
			Invert.at<uchar>(i, j) = 255 - Grey.at<uchar>(i, j);
	return Invert;
}

Mat Func2(Mat Grey, int Th)
{
	Mat Second = Mat::zeros(Grey.size(), CV_8UC1);
	for (int i = 0; i < Grey.rows; i++)
		for (int j = 0; j < Grey.cols; j++)
			if (Grey.at<uchar>(i, j) >= Th)
				Second.at<uchar>(i, j) = Th;
			else
				Second.at<uchar>(i, j) = Grey.at<uchar>(i, j);
	return Second;
}

Mat Func3(Mat Grey, int th1, int th2)
{
	Mat Third = Mat::zeros(Grey.size(), CV_8UC1);
	for (int i = 0; i < Grey.rows; i++)
		for (int j = 0; j < Grey.cols; j++)
			if (Grey.at<uchar>(i, j) < th1)
				Third.at<uchar>(i, j) = th1;
			else if (Grey.at<uchar>(i, j) >= th1 && Grey.at<uchar>(i, j) <= th2)
				Third.at<uchar>(i, j) = th2;
			else
				Third.at<uchar>(i, j) = 255;
	return Third;
}

Mat EHistogram(Mat Grey)
{
	int count[256] = { 0 };
	float prob[256] = { 0.0 };
	float accProb[256] = { 0.0 };
	int newpixel[256] = { 0 };

	for (int i = 0; i < Grey.rows; i++)
		for (int j = 0; j < Grey.cols; j++)
			count[Grey.at<uchar>(i, j)]++;

	for (int i = 0; i < 256; i++)
		prob[i] = (float)count[i] / (float)(Grey.rows * Grey.cols);

	accProb[0] = prob[0];
	for (int j = 1; j < 256; j++)
		accProb[j] = accProb[j - 1] + prob[j];

	for (int i = 0; i < 256; i++)
		newpixel[i] = 255 * accProb[i];

	Mat Equalized = Mat::zeros(Grey.size(), CV_8UC1);
	for (int i = 0; i < Grey.rows; i++)
		for (int j = 0; j < Grey.cols; j++)
			Equalized.at<uchar>(i, j) = newpixel[Grey.at<uchar>(i, j)];
	return Equalized;
}

Mat AvgMask(Mat Equalized, int AVG)
{
	//float AVG = 0;
	//Mat MaskImg = Mat::zeros(Equalized.size(), CV_8UC1);
	//for (int i = 1; i < Equalized.rows - 1; i++)
	//	for (int j = 1; j < Equalized.cols - 1; j++)
	//		for (int ii = -1; ii <= 1; ii++)
	//			for (int jj = -1; jj <= 1; jj++)
	//				AVG += Equalized.at<uchar>(i + ii, j + jj);
	//					MaskImg.at<uchar>(i, j) = AVG / 9;


	/*Mat MaskImg = Mat::zeros(Equalized.size(), CV_8UC1);
	for (int i = 1; i < Equalized.rows - 1; i++)
		for (int j = 1; j < Equalized.cols - 1; j++)
			MaskImg.at<uchar>(i, j) = ((Equalized.at<uchar>(i - 1, j - 1)) + (Equalized.at<uchar>(i - 1, j)) + (Equalized.at<uchar>(i - 1, j + 1)) + (Equalized.at<uchar>(i, j - 1)) + (Equalized.at<uchar>(i, j)) + (Equalized.at<uchar>(i, j + 1)) + (Equalized.at<uchar>(i + 1, j - 1)) + (Equalized.at<uchar>(i + 1, j)) + (Equalized.at<uchar>(i + 1, j + 1))) / 9;
	return MaskImg;*/

	//int AVG = 3;
	bool AVGEVEN;
	if (AVG % 2 == 0) {
		AVGEVEN = true;
	}
	else {
		AVGEVEN = false;
	}
	int start = -(AVG / 2);
	int end;
	if (AVGEVEN == true) {
		end = AVG / 2;
	}
	else {
		end = (AVG / 2);
	}
	int AVGPOW = AVG * AVG;
	float Temp = 0;
	Mat MaskImg = Mat::zeros(Equalized.size(), CV_8UC1);
	for (int i = AVG / 2; i < Equalized.rows - (AVG / 2); i++) {
		for (int j = AVG / 2; j < Equalized.cols - (AVG / 2); j++) {
			for (int ii = start; ii <= end; ii++) {

				for (int jj = start; jj <= end; jj++) {
					Temp += Equalized.at<uchar>(i + ii, j + jj);
				}


			}
			MaskImg.at<uchar>(i, j) = Temp / AVGPOW;
			Temp = 0;
		}
	}

	return MaskImg;
}

Mat EdgeDetection(Mat MaskImg)
{
	Mat EdgeImg = Mat::zeros(MaskImg.size(), CV_8UC1);
	for (int i = 10; i < MaskImg.rows - 1; i++)
		for (int j = 10; j < MaskImg.cols - 1; j++)
		{
			float AVGL = ((MaskImg.at<uchar>(i - 1, j - 1)) + (MaskImg.at<uchar>(i, j - 1)) + (MaskImg.at<uchar>(i + 1, j - 1))) / 3;
			float AVGR = ((MaskImg.at<uchar>(i - 1, j + 1)) + (MaskImg.at<uchar>(i, j + 1)) + (MaskImg.at<uchar>(i + 1, j + 1))) / 3;
			if (abs(AVGL - AVGR) > x)
				EdgeImg.at<uchar>(i, j) = 255;
		}
	return EdgeImg;
}

Mat Dilation(Mat EdgeImg, int neighbour)
{
	Mat DilImg = Mat::zeros(EdgeImg.size(), CV_8UC1);
	for (int i = neighbour; i < EdgeImg.rows - neighbour; i++)
		for (int j = neighbour; j < EdgeImg.cols - neighbour; j++)
		{
			for (int ii = -neighbour; ii <= neighbour; ii++)
				for (int jj = -neighbour; jj <= neighbour; jj++)
					if (EdgeImg.at<uchar>(i + ii, j + jj) == 255)
						DilImg.at<uchar>(i, j) = 255;
		}
	return DilImg;
}

int Otsu(Mat plate)
{
	int count[256] = { 0 };
	float prob[256] = { 0.0 };
	float accProb[256] = { 0.0 };
	float meu[256] = { 0.0 };
	float sigma[256] = { 0.0 };
	for (int i = 0; i < plate.rows; i++)
		for (int j = 0; j < plate.cols; j++)
			count[plate.at<uchar>(i, j)]++;

	for (int i = 0; i < 256; i++)
		prob[i] = (float)count[i] / (float)(plate.rows * plate.cols);

	accProb[0] = prob[0];
	for (int j = 1; j < 256; j++)
		accProb[j] = accProb[j - 1] + prob[j];

	meu[0] = 0;
	for (int j = 1; j < 256; j++)
		meu[j] = meu[j - 1] + j * prob[j];
	sigma[0] = 0;
	for (int i = 1; i < 256; i++)
		sigma[i] = pow(meu[255] * accProb[i] - meu[i], 2) / (accProb[i] * (1 - accProb[i]));
	float init = -1;
	int otsuInd = -1;
	for (int i = 1; i < 256; i++)
	{
		if (sigma[i] > init)
		{
			init = sigma[i];
			otsuInd = i;
		}
	}
	return otsuInd + 30;
}

string OCR(Mat img, int PSMOpt, tesseract::TessBaseAPI *ocr)
{
	string outText;

	// Set Page segmentation mode to PSM_AUTO (3)
	if (PSMOpt == 1) {
		ocr->SetPageSegMode(tesseract::PSM_AUTO);
	}
	else if (PSMOpt == 2) {
		ocr->SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
	}
	else if (PSMOpt == 3) {
		ocr->SetPageSegMode(tesseract::PSM_SINGLE_LINE);
	}
	else if (PSMOpt == 4) {
		ocr->SetPageSegMode(tesseract::PSM_SINGLE_WORD);
	}
	else if (PSMOpt == 5) {
		ocr->SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
	}

	// Set image data
	ocr->SetImage((uchar*)img.data, img.size().width, img.size().height, img.channels(), img.step1());

	// Run Tesseract OCR on image
	outText = string(ocr->GetUTF8Text());
	return outText;
}

bool isPlateChar(char c) {
	bool flag = true;
	//ABCDEFGHJKLMNPQRTUVWXYZ
	//abcdefghjklmnpqrtuvwxyz
	if ((c >= '0' && c <= '9') ||
		(c >= 'A' && c <= 'Z') ||
		c == 'b' ||
		c == 'a' ||
		c == 'c' ||
		c == 'g' ||
		c == 'k' ||
		c == 'p' ||
		c == 's' ||
		c == 'v' ||
		c == 'w' ||
		c == 'x' ||
		c == 'z')
	{
		flag = false;
	}
	if (c == 'I' || c == 'O') {
		flag = true;
	}
	return flag;
}

int main()
{
	tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
	ocr->Init("C:\\Users\\Aphrodite\\Downloads\\vcpkg-master\\buildtrees\\tesseract\\src\\4.0.0-030345c012\\tessdata", "eng", tesseract::OEM_LSTM_ONLY);
	ocr->SetVariable("debug_file", "tesseract.log");

	for (int i = 0; i < 20; i++)
	{
		Mat img;
		string arr[20] = { "C:\\Users\\Aphrodite\\Desktop\\1.jpg", "C:\\Users\\Aphrodite\\Desktop\\2.jpg", "C:\\Users\\Aphrodite\\Desktop\\3.jpg", "C:\\Users\\Aphrodite\\Desktop\\4.jpg", "C:\\Users\\Aphrodite\\Desktop\\5.jpg", "C:\\Users\\Aphrodite\\Desktop\\6.jpg", "C:\\Users\\Aphrodite\\Desktop\\7.jpg", "C:\\Users\\Aphrodite\\Desktop\\8.jpg", "C:\\Users\\Aphrodite\\Desktop\\9.jpg", "C:\\Users\\Aphrodite\\Desktop\\10.jpg", "C:\\Users\\Aphrodite\\Desktop\\11.jpg", "C:\\Users\\Aphrodite\\Desktop\\12.jpg", "C:\\Users\\Aphrodite\\Desktop\\13.jpg", "C:\\Users\\Aphrodite\\Desktop\\14.jpg", "C:\\Users\\Aphrodite\\Desktop\\15.jpg", "C:\\Users\\Aphrodite\\Desktop\\16.jpg", "C:\\Users\\Aphrodite\\Desktop\\17.jpg", "C:\\Users\\Aphrodite\\Desktop\\18.jpg", "C:\\Users\\Aphrodite\\Desktop\\19.jpg", "C:\\Users\\Aphrodite\\Desktop\\20.jpg" };
		img = imread(arr[i]);
		//Mat img;
		//img = imread("C:\\Users\\Aphrodite\\Desktop\\babyj.jpg");
		//string arr[20] = { "C:\\Users\\Aphrodite\\Desktop\\1.jpg, C:\\Users\\Aphrodite\\Desktop\\2.jpg, C:\\Users\\Aphrodite\\Desktop\\3.jpg, C:\\Users\\Aphrodite\\Desktop\\4.jpg, C:\\Users\\Aphrodite\\Desktop\\5.jpg, C:\\Users\\Aphrodite\\Desktop\\6.jpg, C:\\Users\\Aphrodite\\Desktop\\7.jpg, C:\\Users\\Aphrodite\\Desktop\\8.jpg, C:\\Users\\Aphrodite\\Desktop\\9.jpg, C:\\Users\\Aphrodite\\Desktop\\10.jpg, C:\\Users\\Aphrodite\\Desktop\\11.jpg, C:\\Users\\Aphrodite\\Desktop\\12.jpg, C:\\Users\\Aphrodite\\Desktop\\13.jpg, C:\\Users\\Aphrodite\\Desktop\\14.jpg, C:\\Users\\Aphrodite\\Desktop\\15.jpg, C:\\Users\\Aphrodite\\Desktop\\16.jpg, C:\\Users\\Aphrodite\\Desktop\\17.jpg, C:\\Users\\Aphrodite\\Desktop\\18.jpg, C:\\Users\\Aphrodite\\Desktop\\19.jpg, C:\\Users\\Aphrodite\\Desktop\\20.jpg" };
		//Mat img = imread(arr[i]);
		imshow("Test1", img);
		waitKey();
		Mat grey = converttogrey(img);
		//imshow("Test2", grey);
		//waitKey();
		//Mat binary = converttobinary(grey, 60);
		//imshow("Test3", binary);
		//waitKey();
		//Mat step = StepFun(grey, 60, 100);
		//imshow("Test4", step);
		//waitKey();
		//Mat inversion = Inversion(grey);
		//imshow("Test5", inversion);
		//waitKey();
		//Mat second = Func2(grey, 70);
		//imshow("Test6", second);
		//waitKey();
		//Mat third = Func3(grey, 80, 160);
		//imshow("Test7", third);
		//waitKey();
		Mat equalized = EHistogram(grey);
		//imshow("Test8", equalized);
		//waitKey();
		Mat avgmask = AvgMask(equalized, 3);
		//imshow("Test9", avgmask);
		//waitKey();
		Mat edgedetection = EdgeDetection(avgmask);
		//imshow("Test10", edgedetection);
		//waitKey();
		Mat dilation = Dilation(edgedetection, n);
		//imshow("Test11", dilation);
		//waitKey();
		Mat Blob;
		Blob = dilation.clone();

		vector<vector<Point> > contours1;
		vector<Vec4i> hierarchy1;
		findContours(dilation, contours1, hierarchy1, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));

		Mat dst = Mat::zeros(grey.size(), CV_8UC3);
		{
			if (!contours1.empty())
			{
				for (int i = 0; i < contours1.size(); i++)
				{
					Scalar colour((rand() & 255), (rand() & 255), (rand() & 255));
					drawContours(dst, contours1, i, colour, CV_FILLED, 8, hierarchy1);
				}
			}

			//imshow("dst", dst);
			//waitKey();
		}

		Rect rect_first;
		Scalar black = CV_RGB(0, 0, 0);
		Mat plate;
		for (int i = 0; i < contours1.size(); i++)
		{
			rect_first = boundingRect(contours1[i]);
			if (rect_first.width > Blob.cols / 2 || rect_first.height > Blob.cols / 2 || (double)rect_first.width / rect_first.height > 6.75 || (double)rect_first.width / rect_first.height < 1.35 || rect_first.width < y || rect_first.height > z)
			{
				drawContours(Blob, contours1, i, black, CV_FILLED, 8, hierarchy1);
			}
			else
			{
				plate = grey(rect_first);
				imshow("Blob", plate);
				//imshow("Nominated plate", plate);
				//waitKey();
				Mat plateFN;
				plateFN = plate.clone(); // read in the test numbers image

				Mat plate1 = Mat::zeros(plateFN.size(), plateFN.type());
				Mat sub_mat = Mat::ones(plateFN.size(), plateFN.type()) * 255;
				subtract(sub_mat, plateFN, plate1);



				int threshold = Otsu(plate1);
				Mat converttoBin = converttobinary(plate1, threshold);
				/*imshow("Segmented", plate1);
				waitKey();*/
				
				Mat plate2;
				plate1.convertTo(plate2, -1, 1 , 10); //brightness
				Mat plate3;
				plate2.convertTo(plate3, -1, 1.8, 0);
				/*imshow("Con&B", plate3);
				waitKey();*/
				
				/*Mat EqPlate = EHistogram(plate1);
				imshow("EqPlate", EqPlate);
				waitKey();*/

				imshow("BIN", plate3);  ///ocr feed
				std::string detectedText = OCR(plate3, 5, ocr);
				detectedText.erase(std::remove_if(detectedText.begin(), detectedText.end(), isPlateChar), detectedText.end());
				printf("Detected plate: %s\n", detectedText);

				/*Mat Blob2 = converttoBin.clone();
				vector<vector<Point> > contours2;
				vector<Vec4i> hierarchy2;
				findContours(converttoBin, contours2, hierarchy2, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));
				Mat dst2 = Mat::zeros(converttoBin.size(), CV_8UC3);
				{
					if (!contours2.empty())
					{
						for (int i = 0; i < contours2.size(); i++)
						{
							Scalar colour((rand() & 255), (rand() & 255), (rand() & 255));
							drawContours(dst2, contours2, i, colour, CV_FILLED, 8, hierarchy2);
						}
					}

					imshow("dst2", dst2);
					waitKey();
				}*/

				/*Rect rect_2nd;
				Mat Character;
				for (int i = 0; i < contours2.size(); i++)
				{
					rect_2nd = boundingRect(contours2[i]);
					if (rect_2nd.height < 10)
					{
						drawContours(Blob2, contours2, i, black, CV_FILLED, 8, hierarchy2);
					}
					else
					{
						Character = converttoBin(rect_2nd);
						imshow("Character", Character);
						waitKey();
					}
				}*/

			}
			//imshow("Blob", Blob);
			//waitKey();

		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


/*Mat gray, binary;
vector<vector<Point> > contours;
vector<Vec4i> hierarchy;
cvtColor(image, gray, CV_BGR2GRAY);
adaptiveThreshold(gray, binary, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 55, 5);
Mat or_binary = binary.clone();
//Mat element = getStructuringElement(MORPH_RECT, Size(5,5), Point(2,2));
//erode(binary, binary, element);
findContours(binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
if (contours.size() <= 0) return false;
for (size_t i = 0; i < contours.size(); ++i)
{
	Rect r = boundingRect(contours.at(i));
	if (r.width > image.cols / 2 || r.height > image.cols / 2 || r.width < 120 || r.height < 20
		|| (double)r.width / r.height > 4.5 || (double)r.width / r.height < 3.5)
		continue;
	Mat sub_binary = or_binary(r);
	Mat _plate = sub_binary.clone();
	vector<vector<Point> > sub_contours;
	vector<Vec4i> sub_hierarchy;
	findContours(sub_binary, sub_contours, sub_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	if (sub_contours.size() < 8) continue;
	int count = 0;
	vector<Mat> c;
	Mat sub_image = image(r);
	vector<Rect> r_characters;
	for (size_t j = 0; j < sub_contours.size(); ++j)
	{
		Rect sub_r = boundingRect(sub_contours.at(j));
		if (sub_r.height > r.height / 2 && sub_r.width < r.width / 8 && sub_r.width > 5 && r.width > 15 && sub_r.x > 5)
		{
			Mat cj = _plate(sub_r);
			double ratio = (double)count_pixel(cj) / (cj.cols*cj.rows);
			if (ratio > 0.2 && ratio < 0.7)
			{
				r_characters.push_back(sub_r);
				rectangle(sub_image, sub_r, Scalar(0, 0, 255), 2, 8, 0);
			}
		}
	}
	if (r_characters.size() >= 7)
	{
		// sap xep
		for (int i = 0; i < r_characters.size() - 1; ++i)
		{
			for (int j = i + 1; j < r_characters.size(); ++j)
			{
				Rect temp;
				if (r_characters.at(j).x < r_characters.at(i).x)
				{
					temp = r_characters.at(j);
					r_characters.at(j) = r_characters.at(i);
					r_characters.at(i) = temp;
				}
			}
		}
		for (int i = 0; i < r_characters.size(); ++i)
		{
			Mat cj = _plate(r_characters.at(i));
			c.push_back(cj);
		}
		characters.push_back(c);
		sub_binary = or_binary(r);
		plates.push_back(_plate);
		draw_character.push_back(sub_image);
	}
} */