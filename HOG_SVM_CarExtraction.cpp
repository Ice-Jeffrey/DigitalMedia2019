#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml.hpp>
#include <string>
using namespace std;
using namespace cv;

const int PosSamNO = 550;    //正样本个数  
const int NegSamNO = 500;    //负样本个数 
const bool TRAIN = true;

int main() {
    //HOG检测器，用来计算HOG描述子的
    //检测窗口(48,48),块尺寸(16,16),块步长(8,8),cell尺寸(8,8),直方图bin个数9 
    cv::HOGDescriptor hog(cv::Size(48, 48), cv::Size(16, 16), cv::Size(8, 8), cv::Size(8, 8), 9);
    int DescriptorDim;//HOG描述子的维数，由图片大小、检测窗口大小、块大小、细胞单元中直方图bin个数决定  

    //所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数 
    cv::Mat sampleFeatureMat;
    //训练样本的类别向量，行数等于所有样本的个数，列数等于1；1表示有目标，-1表示无目标 
    cv::Mat sampleLabelMat;

    //读入文件时需要用到的文件名
    char adpos[128], adneg[128];

    //设置SVM参数
    cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
    svm->setType(cv::ml::SVM::Types::C_SVC);
    svm->setKernel(cv::ml::SVM::KernelTypes::LINEAR);
    svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));

    if (TRAIN) {
        //依次读取正样本图片，生成HOG描述子  
        for (int num = 0; num < PosSamNO; num++)
        {
            sprintf_s(adpos, "F:\\Codes\\DigitalMedia\\HOG_SVM_CarExtraction\\CarData\\TrainImages\\pos-%d.pgm", num);
            cv::Mat image = cv::imread(adpos);
            image.resize(128, 64);

            //HOG描述子向量 
            std::vector<float> descriptors;
            //计算HOG描述子，检测窗口移动步长(8,8)
            hog.compute(image, descriptors, cv::Size(8, 8));

            //处理第一个样本时初始化特征向量矩阵和类别矩阵，因为只有知道了特征向量的维数才能初始化特征向量矩阵  
            if (0 == num)
            {
                //HOG描述子的维数 
                DescriptorDim = descriptors.size();
                //初始化所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数sampleFeatureMat  
                sampleFeatureMat = cv::Mat::zeros(PosSamNO + NegSamNO, DescriptorDim, CV_32FC1);
                //初始化训练样本的类别向量，行数等于所有样本的个数，列数等于1 
                sampleLabelMat = cv::Mat::zeros(PosSamNO + NegSamNO, 1, CV_32SC1);
            }

            //将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat  
            for (int i = 0; i < DescriptorDim; i++)
            {
                //第num个样本的特征向量中的第i个元素 
                sampleFeatureMat.at<float>(num, i) = descriptors[i];
            }
            //正样本类别为1，有目标	 
            sampleLabelMat.at<signed>(num, 0) = 1;
        }

        //依次读取负样本图片，生成HOG描述子  
        for (int num = 0; num < NegSamNO; num++)
        {
            sprintf_s(adneg, "F:\\Codes\\DigitalMedia\\HOG_SVM_CarExtraction\\CarData\\TrainImages\\neg-%d.pgm", num);
            cv::Mat src = cv::imread(adneg);
            src.resize(128, 64);
            //HOG描述子向量
            std::vector<float> descriptors;
            //计算HOG描述子，检测窗口移动步长(8,8) 
            hog.compute(src, descriptors, cv::Size(8, 8));
            //std::cout << "descriptor dimention：" << descriptors.size() << std::endl;
            //将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat  
            for (int i = 0; i < DescriptorDim; i++)
            {
                //第PosSamNO+num个样本的特征向量中的第i个元素
                sampleFeatureMat.at<float>(num + PosSamNO, i) = descriptors[i];
            }

            //负样本类别为-1，无目标
            sampleLabelMat.at<signed>(num + PosSamNO, 0) = -1;
        }

        //训练SVM分类器  
        //迭代终止条件，当迭代满1000次或误差小于FLT_EPSILON时停止迭代  
        std::cout << "开始训练SVM分类器" << std::endl;
        cv::Ptr<cv::ml::TrainData> td = cv::ml::TrainData::create(sampleFeatureMat, cv::ml::SampleTypes::ROW_SAMPLE, sampleLabelMat);
        //训练分类器  
        svm->train(td);
        std::cout << "训练完成" << std::endl;
        //将训练好的SVM模型保存为xml文件
        svm->save("F:\\Codes\\DigitalMedia\\HOG_SVM_CarExtraction\\xml\\SVM_HOG.xml");
    }

    else {
        svm->load("F:\\Codes\\DigitalMedia\\HOG_SVM_CarExtraction\\xml\\SVM_HOG.xml");//加载模型文件
    }
    return 0;
}