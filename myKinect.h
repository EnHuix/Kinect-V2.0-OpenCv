#pragma once
#include <Kinect.h>
#include <opencv2\opencv.hpp>
#include<vector>

//������Ƶ
#include<dsound.h>
#include <chrono>
#pragma comment(lib, "WINMM.LIB")

#define threshold 130	//�Զ����ֵ����������ֵ
#define threshold_area 5000	//���ڸ�ֵ���жϸ���ͨ����Ч�������ж�Ϊ���
#define threshold_Distance 50//��λΪ�ֱ���
#define frame_Update 5	//ÿframe_Update,�����¼����Ƿ����ʱ���ٶ�,���йر�������Ϊ0
#define threshold_Speed 0.6	//����ʱ�ٶȵ���ֵ
#define threshold_Height 0.2 //����ʱ�߶ȵ���ֵ
#define threshold_Width	100	//���Կ�Խʱ�Ŀ����ֵ



typedef struct _Feather
{
	int label;					// ��ͨ���labelֵ
	int area;					// ��ͨ������
	cv::Rect boundingbox;       // ��ͨ�����Ӿ��ο�
	bool isBody;			//�ж���ͨ������࣬������ϰ�����Ϊfalse�����Ϊ���壬��Ϊtrue
	int min_Z;
	int max_Z;				//������ͷ����Ϊz�ᣬ�ûҶ�ֵ��ʾ��Ծ���
} Feather;
// Safe release for interfaces
template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

class CBodyBasics
{
	//kinect 2.0 ����ȿռ�ĸ�*���� 424 * 512���ڹ�������˵��
	static const int        cDepthWidth = 512;
	static const int        cDepthHeight = 424;

public:
	CBodyBasics();
	~CBodyBasics();
	void                    run();//��ùǼܡ�������ֵͼ�������Ϣ
	void                    get_Barrier(IBodyIndexFrame*);//ʶ�𲢰����ϰ���
	void					measure_Distance();//���������ϰ���ľ��룬�������Сʱ����������
	bool                    get_IsFall();	//�õ��Ƿ��������Ϣ
	HRESULT                 InitializeDefaultSensor();//���ڳ�ʼ��kinect
	static void             f(CBodyBasics &a) { a.run(); }
	static void             f2(CBodyBasics &a) { a.measure_Distance(); }
	
   //��ʾͼ���Mat
	cv::Mat skeletonImg;	//����ͼ��
	cv::Mat depthImg;		//���ͼ��
	cv::Mat barrierImg;		//�ϰ���ͼ��
	cv::Mat colorImg;		//��ɫͼ��
	//�洢�˵�һ�㣬�����ж���ͨ������࣬���˻��ϰ���
	cv::Point body;
	// �����ͨ������
	vector<Feather> featherList;

private:
	IKinectSensor*          m_pKinectSensor;//kinectԴ
	ICoordinateMapper*      m_pCoordinateMapper;//��������任
	IBodyFrameReader*       m_pBodyFrameReader;//���ڹǼ����ݶ�ȡ
	IDepthFrameReader*      m_pDepthFrameReader;//����������ݶ�ȡ
	IColorFrameReader*      m_pColorFrameReader;//���ڲ�ɫ���ݶ�ȡ
	IBodyIndexFrameReader*  m_pBodyIndexFrameReader;//���ڱ�����ֵͼ��ȡ
	IBodyIndexFrameReader*  m_pBodyIndexFrameReader2;//���ڱ�����ֵͼ��ȡ
	static int frameTimer;							//���ڶ�λ֡��
	static int timeIn;								//���ڼ����ٶȵ���ʼʱ��
	static int timeOut;								//���ڼ����ٶȵ�ĩβʱ��
	static float SpineHeightin;
	static float SpineHeightout;						//�ֱ�洢��ʼ��ĩβ�ĸ߶�
	bool isFall=false;

	//ͨ����õ�����Ϣ���ѹǼܺͱ�����ֵͼ������
	void ProcessBody(int nBodyCount, IBody** ppBodies);
	//���Ǽܺ���
	void DrawBone(const Joint* pJoints, const DepthSpacePoint* depthSpacePosition, JointType joint0, JointType joint1);
	//���ֵ�״̬����
	void DrawHandState(const DepthSpacePoint depthSpacePosition, HandState handState);
	//Ѱ����ͨ��
	int bwLabel(cv::Mat & src,cv:: Mat & dst, vector<Feather> & featherList);
	//Ѱ����ͨ�������
	void find_ConnectedDomain(cv::Mat &src, vector<Feather> &featherList);
	//�ҵ�ÿ����ͨ����������ľ��룬�����������Զ�ĻҶ�ֵ�����Ա�ʾ����
	void get_DepthOfConnectedDomain(vector<Feather> & featherList);
	//����Ƿ�ˤ��
	void detection_isFall(IBody** ppBodies,IBodyFrame *pBodyFrame);
	//��ˤ����Ϊtrue
	void set_IsFall(bool res);
	//�õ��˵Ĺ�����
	Joint *get_Joints(IBody** ppBodies);		
	//bool relieve_Fall(IBody** ppBodies, IBodyFrame *pBodyFrame);
	//����ʾ�ϰ����mat���Ƴ���
	//void removeBodyFromBarrierImg(IBodyIndexFrame*);
};
