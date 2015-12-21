#include "string.h"
#include "fstream"
#include "iostream"
#include "stdio.h"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(void)
{
	//Declare Input & Output directory
	string labels_dir = "./input/train/labels/";
	string volume_dir = "./input/train/volume/";
	string save_dir =  "./output/";

	int n=0;//number of image patches generated
	char savename[50];
	
	ofstream fout("./train.txt");
	if(fout)
	{
		printf("train.txt is created");
	}


	Directory dir;

	vector<string> volume_fnames = dir.GetListFiles(volume_dir,"*",false);
	vector<string> label_fnames = dir.GetListFiles(labels_dir,"*",false);

	if (volume_fnames.size()!=label_fnames.size())
	{
		printf("No matching data found.\n");
		return -1;
	}

	for (int i=0; i<volume_fnames.size(); i++)
	{
		//cout<<"volume_name:"<<volume_fnames[i]<<endl;
		//cout<<"label_name:"<<label_fnames[i]<<endl;
		
		Mat volume_src_img;
		Mat volume_dst_img; // Image Patches as output
		Mat label_img;

		volume_src_img = imread(volume_dir+volume_fnames[i],0);
		//0 for greyscale, 1 for BGR
		label_img = imread(labels_dir+label_fnames[i],0);
		
			
		if ((!volume_src_img.data)||(!label_img.data))
		{
			printf("No Image Found\n");
			return -1;
		}
		
		//Parameters for Sliding Window
		int window_h =32;
		int window_w =32;
		int stepsize =100;

		for (int row=0; row<=volume_src_img.rows-window_w;row+=stepsize)
		{
			for (int col=0;col<=volume_src_img.cols-window_h;col+=stepsize)
			{	
				Rect windows(col,row,window_w,window_h);
				volume_dst_img = volume_src_img(windows);
				++n;
				sprintf(savename,"p_%d.jpg",n);//patch name
				imwrite(save_dir+savename,volume_dst_img);


				/*	
				Extract central point intensity from label_img
				 to generate labels for windows.Save in train.txt
				 */
				int labels=label_img.at<uchar>(row+window_h/2,col+window_w/2);//label_img.at<Vec3b> for BGR images.

				switch(labels/100)//binary label
				{
				case 2:	
				case 1:
				fout<<volume_dir+volume_fnames[i]<<" 1"<<endl;
				break;
				case 0:
				fout<<volume_dir+volume_fnames[i]<<" 0"<<endl;
				break;
				Default:
			 	printf("Not Belong to any class");
				return -1;
				}
			}
		}
		cout<<"Complete!"<<endl;

	}

	cout<<"Generated "<<n<<" image patches."<<endl;
	return 0;
}

/*Bat for val dataset: 
Write all val image names into val.txt:
Val_Dir b/s/p/w *.jpg > val.txt
*/
