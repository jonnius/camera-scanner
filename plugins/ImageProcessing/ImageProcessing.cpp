#include <QDebug>
#include <QtGui/QImage>
#include <QStandardPaths>
#include <QString>

#include "ImageProcessing.h"

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "highgui.h"

#include <iostream>
#include <vector>
#include <cmath>

using namespace cv;

ImageProcessing::ImageProcessing() {
  /* empty */
}

QString ImageProcessing::processImage(const QString & imageURL)
{
  //Let's get the name of the picture
  QString fileName = imageURL.right(imageURL.size() - (imageURL.lastIndexOf('/') + 1));
  //~ //Set the destination path
  QString destfile = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/" + fileName;
  //~ qDebug() << "image file path: " << destfile;
  QString imagePath = imageURL;
  if (imagePath.startsWith("file://"))
  {
	  imagePath = imagePath.right(imagePath.size()-7);
  }
  qDebug() << "image file name: " << imagePath;
  
  qDebug() << "starting to process image...";
  Mat img = imread(imagePath.toStdString());
  m_document.detectDocument(img);

  qDebug() << "finished image processing...";
  Mat result = m_document.getResult();
  imwrite(destfile.toStdString(), result);
  //~ QImage processedImage = QImage((uchar*) result.data, result.cols, result.rows, result.step, QImage::Format_RGB888);
  //~ processedImage.save(QString::fromStdString(destfile));
  qDebug() << "Stored at " << destfile;

  return destfile;
}

//std::vector<cv::Point> ImageProcessing::getContour()
//{
//  return m_document.getContour();
//}

//std::vector<cv::Point> ImageProcessing::getQuadrilateral()
//{
//  return m_document.getQuadrilateral();
//}

//cv::Mat ImageProcessing::getResult()
//{
//  return m_document.getResult();
//}
