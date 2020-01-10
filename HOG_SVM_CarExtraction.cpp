#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml.hpp>
#include <string>
using namespace std;
using namespace cv;

const int PosSamNO = 550;    //����������  
const int NegSamNO = 500;    //���������� 
const bool TRAIN = true;

int main() {
    //HOG���������������HOG�����ӵ�
    //��ⴰ��(48,48),��ߴ�(16,16),�鲽��(8,8),cell�ߴ�(8,8),ֱ��ͼbin����9 
    cv::HOGDescriptor hog(cv::Size(48, 48), cv::Size(16, 16), cv::Size(8, 8), cv::Size(8, 8), 9);
    int DescriptorDim;//HOG�����ӵ�ά������ͼƬ��С����ⴰ�ڴ�С�����С��ϸ����Ԫ��ֱ��ͼbin��������  

    //����ѵ������������������ɵľ��������������������ĸ�������������HOG������ά�� 
    cv::Mat sampleFeatureMat;
    //ѵ����������������������������������ĸ�������������1��1��ʾ��Ŀ�꣬-1��ʾ��Ŀ�� 
    cv::Mat sampleLabelMat;

    //�����ļ�ʱ��Ҫ�õ����ļ���
    char adpos[128], adneg[128];

    //����SVM����
    cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
    svm->setType(cv::ml::SVM::Types::C_SVC);
    svm->setKernel(cv::ml::SVM::KernelTypes::LINEAR);
    svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));

    if (TRAIN) {
        //���ζ�ȡ������ͼƬ������HOG������  
        for (int num = 0; num < PosSamNO; num++)
        {
            sprintf_s(adpos, "F:\\Codes\\DigitalMedia\\HOG_SVM_CarExtraction\\CarData\\TrainImages\\pos-%d.pgm", num);
            cv::Mat image = cv::imread(adpos);
            image.resize(128, 64);

            //HOG���������� 
            std::vector<float> descriptors;
            //����HOG�����ӣ���ⴰ���ƶ�����(8,8)
            hog.compute(image, descriptors, cv::Size(8, 8));

            //�����һ������ʱ��ʼ�����������������������Ϊֻ��֪��������������ά�����ܳ�ʼ��������������  
            if (0 == num)
            {
                //HOG�����ӵ�ά�� 
                DescriptorDim = descriptors.size();
                //��ʼ������ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��sampleFeatureMat  
                sampleFeatureMat = cv::Mat::zeros(PosSamNO + NegSamNO, DescriptorDim, CV_32FC1);
                //��ʼ��ѵ����������������������������������ĸ�������������1 
                sampleLabelMat = cv::Mat::zeros(PosSamNO + NegSamNO, 1, CV_32SC1);
            }

            //������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat  
            for (int i = 0; i < DescriptorDim; i++)
            {
                //��num�����������������еĵ�i��Ԫ�� 
                sampleFeatureMat.at<float>(num, i) = descriptors[i];
            }
            //���������Ϊ1����Ŀ��	 
            sampleLabelMat.at<signed>(num, 0) = 1;
        }

        //���ζ�ȡ������ͼƬ������HOG������  
        for (int num = 0; num < NegSamNO; num++)
        {
            sprintf_s(adneg, "F:\\Codes\\DigitalMedia\\HOG_SVM_CarExtraction\\CarData\\TrainImages\\neg-%d.pgm", num);
            cv::Mat src = cv::imread(adneg);
            src.resize(128, 64);
            //HOG����������
            std::vector<float> descriptors;
            //����HOG�����ӣ���ⴰ���ƶ�����(8,8) 
            hog.compute(src, descriptors, cv::Size(8, 8));
            //std::cout << "descriptor dimention��" << descriptors.size() << std::endl;
            //������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat  
            for (int i = 0; i < DescriptorDim; i++)
            {
                //��PosSamNO+num�����������������еĵ�i��Ԫ��
                sampleFeatureMat.at<float>(num + PosSamNO, i) = descriptors[i];
            }

            //���������Ϊ-1����Ŀ��
            sampleLabelMat.at<signed>(num + PosSamNO, 0) = -1;
        }

        //ѵ��SVM������  
        //������ֹ��������������1000�λ����С��FLT_EPSILONʱֹͣ����  
        std::cout << "��ʼѵ��SVM������" << std::endl;
        cv::Ptr<cv::ml::TrainData> td = cv::ml::TrainData::create(sampleFeatureMat, cv::ml::SampleTypes::ROW_SAMPLE, sampleLabelMat);
        //ѵ��������  
        svm->train(td);
        std::cout << "ѵ�����" << std::endl;
        //��ѵ���õ�SVMģ�ͱ���Ϊxml�ļ�
        svm->save("F:\\Codes\\DigitalMedia\\HOG_SVM_CarExtraction\\xml\\SVM_HOG.xml");
    }

    else {
        svm->load("F:\\Codes\\DigitalMedia\\HOG_SVM_CarExtraction\\xml\\SVM_HOG.xml");//����ģ���ļ�
    }
    return 0;
}