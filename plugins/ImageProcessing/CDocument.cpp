#include "CDocument.h"

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;
using namespace DocumentScanner;

struct ContourSort
{
    bool operator()(const Contour & a, const Contour & b) const
    {
        return contourArea(a) < contourArea(b);
    }
};

struct sumComparator
{
	bool operator()(const Point & a, const Point & b) {
		return (a.y + a.x) < (b.y + b.x);
	}
};

struct diffComparator
{
	bool operator()(const Point & a, const Point & b) {
		return (a.y - a.x) < (b.y - b.x);
	}
};


vector<Contour> findContours(Mat src)
{
	// Scale to height of 500
	float ratio = src.size().height / 500.0;
	int height = 500;
	int width = static_cast<int>(src.size().width / ratio);
	Size size(width, height);

	Mat grayImage(size, CV_8UC4);
	Mat cannedImage(size, CV_8UC4);
	Mat resizedImage(size, CV_8UC1);

	resize(src, resizedImage, size);
	cvtColor(resizedImage, grayImage, COLOR_RGBA2GRAY, 4);
	GaussianBlur(grayImage, grayImage, Size(5, 5), 0.0, 0.0);
	Canny(grayImage, cannedImage, 75, 200);

	vector<Contour> contours;
	Mat hierarchy;

	findContours(cannedImage, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);

	hierarchy.release();

	// TODO Test, if sorting is inverted!
	sort(contours.begin(), contours.end(), ContourSort());

	resizedImage.release();
	grayImage.release();
	cannedImage.release();

	cout << contours.size() << " contours found." << endl;

	return contours;
}

vector<Point> sortPoints(vector<Point> src)
{
	vector<Point> srcPoints = src;
	vector<Point> result(4);

	// top-left corner = minimal sum
	result[0] = *min_element(srcPoints.begin(), srcPoints.end(), sumComparator());

	// bottom-right corner = maximal sum
	result[2] = *max_element(srcPoints.begin(), srcPoints.end(), sumComparator());

	// top-right corner = minimal diference
	result[1] = *min_element(srcPoints.begin(), srcPoints.end(), diffComparator());

	// bottom-left corner = maximal diference
	result[3] = *max_element(srcPoints.begin(), srcPoints.end(), diffComparator());

	return result;
}

bool insideArea(vector<Point> rp, Size size)
{
	int width = size.width;
	int height = size.height;
	int baseMeasureHeight = height/4;
	int baseMeasureWidth = width/4;

	int bottomPos = height-baseMeasureHeight;
	int topPos = baseMeasureHeight;
	int leftPos = baseMeasureWidth;
	int rightPos = width-baseMeasureWidth;

//	int bottomPos = height-baseMeasure;
//	int topPos = baseMeasure;
//	int leftPos = width/2-baseMeasure;
//	int rightPos = width/2+baseMeasure;

	cout << "Grenzen: " << leftPos << "," << rightPos << "," << topPos << "," << bottomPos << endl;
	cout << "Punkte: " << rp[0] << rp[1] << rp[2] << rp[3] << endl;
	cout << "Größe: " << size << endl;
//	cout << "baseMeasure: " << baseMeasure << endl;

	cout << "Bed1: " << (rp[0].x <= leftPos  && rp[0].y <= topPos) << endl;
	cout << "Bed2: " << (rp[1].x >= rightPos && rp[1].y <= topPos) << endl;
	cout << "Bed3: " << (rp[2].x >= rightPos && rp[2].y >= bottomPos) << endl;
	cout << "Bed4: " << (rp[3].x <= leftPos  && rp[3].y >= bottomPos) << endl;

	Mat test(size, CV_8UC3);
	//			cout << "Sorted points:" << endl;
	Vec3b white(255,255,255);
	Vec3b red(255,0,0);
	Vec3b green(0,255,0);
	Vec3b blue(0,0,255);
	test.at<Vec3b>(rp[0]) = blue;
	test.at<Vec3b>(rp[1]) = blue;
	test.at<Vec3b>(rp[2]) = blue;
	test.at<Vec3b>(rp[3]) = blue;
	test.at<Vec3b>(topPos, leftPos) = red;
	test.at<Vec3b>(topPos, rightPos) = red;
	test.at<Vec3b>(bottomPos, rightPos) = red;
	test.at<Vec3b>(bottomPos, leftPos) = red;

//  namedWindow( "insideTest", CV_WINDOW_AUTOSIZE );
//  imshow( "insideTest", test);

	return rp[0].x <= leftPos  && rp[0].y <= topPos
		&& rp[1].x >= rightPos && rp[1].y <= topPos
		&& rp[2].x >= rightPos && rp[2].y >= bottomPos
		&& rp[3].x <= leftPos  && rp[3].y >= bottomPos;
}

CQuadrilateral findQuadrilateral(vector<Contour> contours, Size srcSize)
{
	// Scale to height of 500
	float ratio = srcSize.height / 500.0;
	int height = 500;
	int width = static_cast<int>(srcSize.width / ratio);
	Size size(width, height);

	for (size_t ic = 0; ic < contours.size(); ic++)
	{
		// Convert contour to float
		vector<Point2f> c2f;
		Mat(contours[ic]).copyTo(c2f);

		// Get contour length
		float peri = arcLength(c2f, true);
		vector<Point2f> approx;

		// Approx. polygone
		approxPolyDP(c2f, approx, 0.02 * peri, true);

		// Convert to int
		vector<Point> points;
		Mat(approx).copyTo(points);

		// select biggest 4 angles polygon
		if (points.size() == 4)
		{
			vector<Point> foundPoints = sortPoints(points);

			Mat test(srcSize, CV_8UC1);
//			cout << "Sorted points:" << endl;
			for (size_t i = 0; i < 4; i++)
			{
				test.at<unsigned char>(foundPoints[i].y, foundPoints[i].x) = 255;
//				cout << foundPoints[i] << endl;
			}

//			  namedWindow( "Eckpunkte", CV_WINDOW_AUTOSIZE );
//			  imshow( "Eckpunkte", test);

			cout << "InsideArea: " << insideArea(foundPoints, size) << endl;

			if (insideArea(foundPoints, size))
			{
				return CQuadrilateral(contours[ic], foundPoints);
			}
		}
	}

	return CQuadrilateral();
}

Mat fourPointTransform(Mat src, vector<Point> pts)
{
	assert(pts.size() >= 4);

	// Scale to height of 500
	float ratio = src.size().height / 500.0;

	cout << "Ratio: " << ratio << endl;

	Point tl = pts[0];
	Point tr = pts[1];
	Point br = pts[2];
	Point bl = pts[3];

	float widthA = sqrt(pow(br.x - bl.x, 2) + pow(br.y - bl.y, 2));
	float widthB = sqrt(pow(tr.x - tl.x, 2) + pow(tr.y - tl.y, 2));

	float dw = max(widthA, widthB)*ratio;
	int maxWidth = static_cast<int>(dw);

	float heightA = sqrt(pow(tr.x - br.x, 2) + pow(tr.y - br.y, 2));
	float heightB = sqrt(pow(tl.x - bl.x, 2) + pow(tl.y - bl.y, 2));

	float dh = max(heightA, heightB)*ratio;
	int maxHeight = static_cast<int>(dh);

	Mat doc(maxHeight, maxWidth, CV_8UC4);

	float src_data[] = {tl.x*ratio, tl.y*ratio, tr.x*ratio, tr.y*ratio, br.x*ratio, br.y*ratio, bl.x*ratio, bl.y*ratio};
	float dst_data[] = {0.0, 0.0, dw, 0.0, dw, dh, 0.0, dh};

	Mat src_mat(4, 1, CV_32FC2, &src_data);
	Mat dst_mat(4, 1, CV_32FC2, &dst_data);

	Mat m = getPerspectiveTransform(src_mat, dst_mat);

	warpPerspective(src, doc, m, doc.size());

	return doc;
}

/**
 * When a pixel have any of its three elements above the threshold
 * value and the average of the three values are less than 80% of the
 * higher one, brings all three values to the max possible keeping
 * the relation between them, any absolute white keeps the value, all
 * others go to absolute black.
 *
 * src must be a 3 channel image with 8 bits per channel
 *
 * @param src
 * @param threshold
 */
void colorThresh(Mat &src, const int & threshold)
{
	assert(src.type() == CV_8UC3);

	for(int i = 0; i < src.rows; i++)
	{
		for(int j = 0; j < src.cols; j++)
		{
			float maxC = max(max(src.at<Vec3b>(i, j)[0], src.at<Vec3b>(i, j)[1]), src.at<Vec3b>(i, j)[2]);
			float meanC = static_cast<float>(src.at<Vec3b>(i, j)[0] + src.at<Vec3b>(i, j)[1] + src.at<Vec3b>(i, j)[2]) / 3.0;

			if (src.at<Vec3b>(i, j) != Vec3b(255, 255, 255))
			{
				if (maxC > threshold && meanC < maxC * 0.8) {
					// Scale pixel so that the maximal color value becomes 255
					src.at<Vec3b>(i, j) *= 255.0 / maxC;
				} else {
					// Set pixel to black
					src.at<Vec3b>(i, j) = Vec3b(0,0,0);
				}
			}
		}
	}
}

void enhanceDocument(Mat src)
{
	//TODO originally class member
	bool colorMode = true;
	bool filterMode = true;
	int colorThr = 110;
    float colorGain = 1.5;       // contrast
	float colorBias = 0; // bright

	if (colorMode && filterMode)
	{
		src.convertTo(src,-1, colorGain, colorBias);
		Mat mask(src.size(), CV_8UC1);
		cvtColor(src, mask, COLOR_RGBA2GRAY);

		Mat copy(src.size(), CV_8UC3);
		src.copyTo(copy);

		adaptiveThreshold(mask, mask, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 15, 15);

		src.setTo(Scalar(255,255,255));
		copy.copyTo(src,mask);

		copy.release();
		mask.release();

		// special color threshold algorithm
		colorThresh(src, colorThr);
	}
	else if (!colorMode)
	{
		cvtColor(src, src, COLOR_RGBA2GRAY);
		if (filterMode)
		{
			adaptiveThreshold(src, src, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, 15);
		}
	}
}

void CDocument::detectDocument(const cv::Mat & inputRgba)
{
	// TODO These have been members of the handler class
//    vector<Point> mPreviewPoints;
//    Size mPreviewSize;

  m_original = inputRgba;

	vector<Contour> contours = findContours(inputRgba);

	CQuadrilateral quad = findQuadrilateral(contours, inputRgba.size());

	Mat doc;

	if (quad.initialized()) {
		cout << "Transforming image..." << endl;
		Contour c = quad.getContour();

		m_quadrilateral = quad;
		//TODO necessary?
//		m_previewPoints = mPreviewPoints;
//		m_previewSize = mPreviewSize;

		doc = fourPointTransform(inputRgba, quad.getPoints());

	} else {
		cout << "WARNING: No document detected." << endl;
		doc = Mat(inputRgba.size(), CV_8UC4);
		inputRgba.copyTo(doc);
	}

	enhanceDocument(doc);
  m_processed = doc;
}
