#include "ONSExtractor.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <vector>
#include <cmath>
#include <QDebug>

using namespace cv;
using namespace std;
using namespace DocumentScanner;

typedef vector<Point> Border;
/** A Quad is meant to be a border consisting of exactly 4 points */
typedef Border Quad;

struct BorderSort
{
    bool operator()(const Border & a, const Border & b) const
    {
        return contourArea(a) < contourArea(b);
    }
};

struct sumComparator
{
    bool operator()(const Point & a, const Point & b)
    {
        return (a.y + a.x) < (b.y + b.x);
    }
};

struct diffComparator
{
    bool operator()(const Point & a, const Point & b)
    {
        return (a.y - a.x) < (b.y - b.x);
    }
};

inline
vector<Border> findCandidates(const Mat &src)
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

    vector<Border> candidates;
    Mat hierarchy;

    findContours(cannedImage, candidates, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);

    hierarchy.release();

    // TODO Test, if sorting is inverted!
    sort(candidates.begin(), candidates.end(), BorderSort());

    resizedImage.release();
    grayImage.release();
    cannedImage.release();

    //~ qDebug() << candidates.size() << " contours found.";

    return candidates;
}

inline
Quad sortPoints(const Quad &src)
{
    Quad result(4);

    // top-left corner = minimal sum
    result[0] = *min_element(src.begin(), src.end(), sumComparator());

    // bottom-right corner = maximal sum
    result[2] = *max_element(src.begin(), src.end(), sumComparator());

    // top-right corner = minimal diference
    result[1] = *min_element(src.begin(), src.end(), diffComparator());

    // bottom-left corner = maximal diference
    result[3] = *max_element(src.begin(), src.end(), diffComparator());

    return result;
}

inline
bool insideArea(const Quad &rp, const Size &size)
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

    //~ qDebug() << "Grenzen: " << leftPos << "," << rightPos << "," << topPos << "," << bottomPos;
    //~ qDebug() << "Punkte: " << rp[0] << rp[1] << rp[2] << rp[3];
    //~ qDebug() << "Größe: " << size;
//	qDebug() << "baseMeasure: " << baseMeasure;

    //~ qDebug() << "Bed1: " << (rp[0].x <= leftPos  && rp[0].y <= topPos);
    //~ qDebug() << "Bed2: " << (rp[1].x >= rightPos && rp[1].y <= topPos);
    //~ qDebug() << "Bed3: " << (rp[2].x >= rightPos && rp[2].y >= bottomPos);
    //~ qDebug() << "Bed4: " << (rp[3].x <= leftPos  && rp[3].y >= bottomPos);

    Mat test(size, CV_8UC3);
    //			qDebug() << "Sorted points:";
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

inline
bool findQuad(const vector<Border> &candidates,
              const Size &srcSize,
              Quad &quad)
{
    // Scale to height of 500
    float ratio = srcSize.height / 500.0;
    int height = 500;
    int width = static_cast<int>(srcSize.width / ratio);
    Size size(width, height);

    for (size_t ic = 0; ic < candidates.size(); ic++)
    {
        // Convert contour to float
        vector<Point2f> c2f;
        Mat(candidates[ic]).copyTo(c2f);

        // Get contour length
        float peri = arcLength(c2f, true);
        vector<Point2f> approx;

        // Approx. polygone
        approxPolyDP(c2f, approx, 0.02 * peri, true);

        // Convert to int
        vector<Point> points;
        Mat(approx).copyTo(points);

        //~ qDebug() << "Candidate " << ic+1 << " has " << points.size();

        // select biggest 4 angles polygon
        if (points.size() == 4)
        {
            Quad foundPoints = sortPoints(points);

            Mat test(srcSize, CV_8UC1);
//			qDebug() << "Sorted points:";
            for (size_t i = 0; i < 4; i++)
            {
                test.at<unsigned char>(foundPoints[i].y, foundPoints[i].x) = 255;
//				qDebug() << foundPoints[i];
            }

//			  namedWindow( "Eckpunkte", CV_WINDOW_AUTOSIZE );
//			  imshow( "Eckpunkte", test);

            //~ qDebug() << "InsideArea: " << insideArea(foundPoints, size);

            if (insideArea(foundPoints, size))
            {
                quad = foundPoints;
                return true;
            }
        }
    }

    return false;
}

inline
void rectify(const Quad &pts, const Mat &src, Mat &dst)
{
    assert(pts.size() == 4);

    // Scale to height of 500
    float ratio = src.size().height / 500.0;

    //~ qDebug() << "Ratio: " << ratio;

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

    dst = Mat(maxHeight, maxWidth, CV_8UC4);

    float src_data[] = {tl.x*ratio, tl.y*ratio, tr.x*ratio, tr.y*ratio, br.x*ratio, br.y*ratio, bl.x*ratio, bl.y*ratio};
    float dst_data[] = {0.0, 0.0, dw, 0.0, dw, dh, 0.0, dh};

    Mat src_mat(4, 1, CV_32FC2, &src_data);
    Mat dst_mat(4, 1, CV_32FC2, &dst_data);

    Mat m = getPerspectiveTransform(src_mat, dst_mat);

    warpPerspective(src, dst, m, dst.size());
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
inline
void colorThresh(const int &threshold, Mat &img)
{
    assert(img.type() == CV_8UC3);

    for(int i = 0; i < img.rows; i++)
    {
        for(int j = 0; j < img.cols; j++)
        {
            float maxC = max(max(img.at<Vec3b>(i, j)[0], img.at<Vec3b>(i, j)[1]), img.at<Vec3b>(i, j)[2]);
            float meanC = static_cast<float>(img.at<Vec3b>(i, j)[0] + img.at<Vec3b>(i, j)[1] + img.at<Vec3b>(i, j)[2]) / 3.0;

            if (img.at<Vec3b>(i, j) != Vec3b(255, 255, 255))
            {
                if (maxC > threshold && meanC < maxC * 0.8)
                {
                    // Scale pixel so that the maximal color value becomes 255
                    img.at<Vec3b>(i, j) *= 255.0 / maxC;
                }
                else
                {
                    // Set pixel to black
                    img.at<Vec3b>(i, j) = Vec3b(0,0,0);
                }
            }
        }
    }
}

inline
void enhanceDocument(Mat &img)
{
    //TODO make params configurable
    bool colorMode = true;
    bool filterMode = true;
    int colorThr = 110;
    float colorGain = 1.5; // contrast
    float colorBias = 0;   // bright

    if (colorMode && filterMode)
    {
        img.convertTo(img,-1, colorGain, colorBias);
        Mat mask(img.size(), CV_8UC1);
        cvtColor(img, mask, COLOR_RGBA2GRAY);

        Mat copy(img.size(), CV_8UC3);
        img.copyTo(copy);

        adaptiveThreshold(mask, mask, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 15, 15);

        img.setTo(Scalar(255,255,255));
        copy.copyTo(img,mask);

        copy.release();
        mask.release();

        // special color threshold algorithm
        colorThresh(colorThr, img);
    }
    else if (!colorMode)
    {
        cvtColor(img, img, COLOR_RGBA2GRAY);
        if (filterMode)
        {
            adaptiveThreshold(img, img, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, 15);
        }
    }
}

bool ONSExtractor::extractDocument(const Mat &rawImg,
                                   Mat & docImg) const
{
    vector<Border> candidates;
    Quad quad;

    qDebug() << "Starting document extraction...";

    // Find candidates
    candidates = findCandidates(rawImg);
    //~ qDebug() << " " << candidates.size() << " candidates found.";

    // Find quad
    if (!findQuad(candidates, rawImg.size(), quad))
    {
        qDebug() << " Didn't find a proper quad between the " << candidates.size() << " candidates.";
        return false;
    }

    qDebug() << " Document detected. Processing image...";

    // Process image
    rectify(quad, rawImg, docImg);
    enhanceDocument(docImg);

    qDebug() << " Document extraction finished.";
    return true;
}
