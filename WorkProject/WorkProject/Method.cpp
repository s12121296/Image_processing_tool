#define _USE_MATH_DEFINES
#include "Method.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <time.h>

using namespace std;

Method::Method()
{

}

Method::~Method()
{
	
}

void Method::OpenImage(void)
{
	//new�@�ӷs��openFileDialog1�}���ɮ�
	OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;
	//�]�wFilter�A�ΥH���w�ϥΪ̶}�Ҫ��ɮ�
	openFileDialog1->Filter = "�I�}�� (*.bmp)| *.bmp| All files (*.*)| *.*";
	//�w�]�ɮצW�٬��ŭ�
	openFileDialog1->FileName = "";
	//�]�w���X��ܵ��������D�W��
	openFileDialog1->Title = "���J�v��";
	//�]�wFilter��ܼҦ�(�̷�Filter�ơA�p���ҿ��1��ܰ_�l�X�{�����I�}�ϡA���2���All filts)
	openFileDialog1->FilterIndex = 2;


	//���X�ɮ׿�ܵ���(ShowDialog)�A�åB�p�G�ϥΪ��I���ɮסA�åB�ɮצW�ٶW�L0�Ӧr���A�h�P�_�O��True�A�i�J�B�z�{��
	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK && openFileDialog1->FileName->Length > 0)
	{
		//�N������ɮ�Ū���åB�s��Image1
		image1 = safe_cast<Bitmap^>(Image::FromFile(openFileDialog1->FileName));
		//�]�w�Ϲ��j�r
		rect = Rectangle(0, 0, image1->Width, image1->Height);
		//���o�Ϲ��榡
		System::Drawing::Imaging::PixelFormat format = image1->PixelFormat;
		//�ƻs����x�s
		original_Image = image1->Clone(rect, format);
	}
}

void Method::copytoImage1(IntPtr aPtr, IntPtr bPtr)
{
	p = (Byte*)((Void*)image_Ptr); //�]�w����
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

	// �s���Ҧ������I�ƻs��image1
	for (int y = 0; y < image1->Height; y++)
	{
		for (int x = 0; x < image1->Width; x++)
		{
			for (int z = 0; z < 3; z++)
			{
				int temp = q[z];
				p[z] = q[z];
			}
			p += 3;
			q += 3;
		}
	}
}

void Method::Reduction(void)
{
	//���o�Ϲ��榡
	System::Drawing::Imaging::PixelFormat format = original_Image->PixelFormat;
	//�ƻs����x�s
	image1 = original_Image->Clone(rect, format);
}

void Method::RGBtoGray(void)
{	
	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����
	
	////��ϪŶ�
	image1_Data= image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����
	

	//�s���Ҧ������I�A�N�������T��Ƕ�
	for (int y = 0; y < image1->Height; y++)
	{
		for (int x = 0; x < image1->Width; x++)
		{	
			int pixel = (p[0] + p[1] + p[2]) / 3;

			q[0] = pixel;
			q[1] = pixel;
			q[2] = pixel;

			q += 3;
			p += 3;		
		}
	}
	
	//�N�B�z�����Ͻƻs��image1
	copytoImage1(image_Ptr, temp_Ptr);

	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
	
}

void Method::ZoomImage(float scale)
{
	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

	////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����
	//�s���Ҧ������I�A���ι����A�B�z�����ϰ�
	for (int y = 0; y < image1->Height; y++) {
		for (int x = 0; x < image1->Width; x++) {

			//�N�v���e�פΰ��ש�jscale��  +0.5 �����
			int xx = x / scale + 0.5;
			int yy = y / scale + 0.5;

			//��X�ثe���лP�ؼй������t
			int a = (xx - x) + ((yy - y)*image1->Width);

			for (int z = 0; z < 2; z++)
			{
				//���V�ؼй���
				r = p + a * 3;

				//���X�ؼй�����J�ثe����
				q[0] = r[0];
				q[1] = r[1];
				q[2] = r[2];
			}
			p += 3;
			q += 3;
		}
	}

	//�N�B�z�����Ͻƻs��image1
	copytoImage1(image_Ptr, temp_Ptr);

	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
}

void Method::ZoomOutImage(float scale)
{
	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

	////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����

	//�s���Ҧ������I�A���ι����A�B�z�����ϰ�
	for (int y = 0; y < image1->Height; y++) {
		for (int x = 0; x < image1->Width; x++) {

			//�N�Ϲ���j2�� +0.5�����
			int xx = x / scale + 0.5;
			int yy = y / scale + 0.5;

			//��X�ثe���лP�ؼЦ�}���t
			int a = (xx - x) + ((yy - y)*image1->Width);

			//���V�ؼЦ�}
			r = q + a * 3;

			//�N�ثe������J�ؼЦ�}
			r[0] = p[0];
			r[1] = p[1];
			r[2] = p[2];
	
			p += 3;
			q += 3;
		}
	}

	//�N�B�z�����Ͻƻs��image1
	copytoImage1(image_Ptr, temp_Ptr);

	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
}

void Method::TwoZoomImage(double scale)
{
	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

	////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����

	//�s���Ҧ������I�A���ι����A�B�z�����ϰ�
	for (int y = 0; y < image1->Height; y++)
	{
		for (int x = 0; x < image1->Width; x++)
		{
			//�Y����
			double dstx = ((double)x) / scale;
			double dsty = ((double)y) / scale;

			//���o�|�I�y��
			int x1 = (int)dstx;
			int y1 = (int)dsty;
			int x2 = x1 + 1;
			int y2 = y1 + 1;

			double u = dstx - x1;
			double v = dsty - y1;

			double s1 = (1.0 - u)*(1.0 - v);
			double s2 = u * (1.0 - v);
			double s3 = (1.0 - u)*v;
			double s4 = u * v;

			//���W�I�������
			int a = (x - x1) + ((y - y1)*image1->Width);

			r = p - a * 3;

			int m, n, o = 0;
			m += r[0] * s1;
			n += r[1] * s1;
			o += r[2] * s1;

			//�k�W�I�������
			a = (x - x2) + ((y - y1)*image1->Width);

			r = p - a * 3;
			m += r[0] * s2;
			n += r[1] * s2;
			o += r[2] * s2;

			//���U�I�������
			a = (x - x1) + ((y - y2)*image1->Width);

			r = p - a * 3;
			m += r[0] * s3;
			n += r[1] * s3;
			o += r[2] * s3;

			//�k�U�I�������
			a = (x - x2) + ((y - y2)*image1->Width);

			r = p - a * 3;
			m += r[0] * s4;
			n += r[1] * s4;
			o += r[2] * s4;

			q[0] = m;
			q[1] = n;
			q[2] = o;

			m = 0;
			n = 0;
			o = 0;

			q += 3;
			p += 3;
		}
	}

	//�N�B�z�����Ͻƻs��image1
	copytoImage1(image_Ptr, temp_Ptr);

	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
}

void Method::TwoZoomOutImage(double scale)
{
	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

								  ////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����

								   //�s���Ҧ������I�A���ι����A�B�z�����ϰ�
	for (int y = 0; y < image1->Height; y++)
	{
		for (int x = 0; x < image1->Width; x++)
		{
			//�Y����
			double dstx = ((double)x) / scale;
			double dsty = ((double)y) / scale;

			//���o�|�I�y��
			int x1 = (int)dstx;
			int y1 = (int)dsty;
			int x2 = x1 + 1;
			int y2 = y1 + 1;

			double u = dstx - x1;
			double v = dsty - y1;

			double s1 = (1.0 - u)*(1.0 - v);
			double s2 = u * (1.0 - v);
			double s3 = (1.0 - u)*v;
			double s4 = u * v;

			//���W�I�������
			int a = (x - x1) + ((y - y1)*image1->Width);

			r = q - a * 3;

			int m, n, o = 0;
			m += p[0] * s1;
			n += p[1] * s1;
			o += p[2] * s1;

			//�k�W�I�������
			a = (x - x2) + ((y - y1)*image1->Width);

			r = q - a * 3;
			m += p[0] * s2;
			n += p[1] * s2;
			o += p[2] * s2;

			//���U�I�������
			a = (x - x1) + ((y - y2)*image1->Width);

			r = q - a * 3;
			m += p[0] * s3;
			n += p[1] * s3;
			o += p[2] * s3;

			//�k�U�I�������
			a = (x - x2) + ((y - y2)*image1->Width);

			r = q - a * 3;
			m += p[0] * s4;
			n += p[1] * s4;
			o += p[2] * s4;

			r[0] = m;
			r[1] = n;
			r[2] = o;

			m = 0;
			n = 0;
			o = 0;

			q += 3;
			p += 3;
		}
	}

	//�N�B�z�����Ͻƻs��image1
	copytoImage1(image_Ptr, temp_Ptr);

	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
}

void Method::SaltPepperNoise(int number)
{
	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

	////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����

	double mean = 0;//�����
	double std = 1;//�зǮt
	double u, v;//���ä����ƭ�
	double X;//�`�A���G�ƭ�

	long int total = 0;
	//�s���Ҧ������I�A���ι����A�B�z�����ϰ�
	for (int y = 0; y < image1->Height; y++) {
		for (int x = 0; x < image1->Width; x++) {

			u = rand() / (double)RAND_MAX;//RAND_MAX=32767
			v = rand() / (double)RAND_MAX;

			X = sqrt(-2.0 * log(u)) * cos(2.0 * M_PI * v) * std + mean;//M_PI=3.14159

			if (X > 2 || X < -2)
			{
				if (X > 2)
				{
					q[0] = 255;
					q[1] = 255;
					q[2] = 255;
					total++;
				}
				else if (X < -2)
				{
					q[0] = 0;
					q[1] = 0;
					q[2] = 0;
					total++;
				}
			}
			else
			{
				//������Ϲ���
				q[0] = p[0];
				q[1] = p[1];
				q[2] = p[2];
			}

			q += 3;
			p += 3;
		}
	}

	double len, Percentage;
	len = image1->Height * image1->Width;
	Percentage = total / len *100;
	cout << "Noise_total:" << total << " " << "Pixel_total: " << len << "  Percentage: " << Percentage << endl;

	//�N�B�z�����Ͻƻs��image1
	copytoImage1(image_Ptr, temp_Ptr);
	
	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
	
}

void Method::Gaussian_Noise(double mean, double std)
{
	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

	////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����

	int temp;
	double u, v;//uniform distribution
	double m = 0;//normal distribution

	//�ŧi�@�ӶüƲ��;�
	Random^ Rand = gcnew Random();
	const double max = 32767;

	//�s���Ҧ������I�A���ι����A�B�z�����ϰ�
	for (int y = 0; y < image1->Height; y++) {
		for (int x = 0; x < image1->Width; x++) {

			u = Rand->Next(0, max) / max; //RAND_MAX=32767
			v = Rand->Next(0, max) / max;
			m = (sqrt(-2 * log(u)) * cos(2 * M_PI * v) * std) + mean;//M_PI=3.14159
															
			for (int i = 0; i < 3; i++)
			{
				temp = p[i] + (int)m;
				if (temp > 255)
					temp = 255;
				if (temp < 0)
					temp = 0;
				q[i] = temp;
			}

			q += 3;
			p += 3;
		}
	}

	//�N�B�z�����Ͻƻs��image1
	copytoImage1(image_Ptr, temp_Ptr);

	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
}

void Method::Mean_Filter(void)
{
	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

	////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����

	//�s���Ҧ������I�A���ι����A�B�z�����ϰ�
	for (int y = 0; y < image1->Height; y++) {
		for (int x = 0; x < image1->Width; x++) {
			int sum[3] = {0};
			if (y > 0 && x > 0 && y < image1->Height-1 && x < image1->Width-1)
			{
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -3; j <= 3; j += 3)
					{
						r = (Byte*)(Void*)p + i * image1->Width * 3 + j;
						sum[0] += r[0];
						sum[1] += r[1];
						sum[2] += r[2];
					}
				}

				for (int k = 0; k < 3; k++)
				{
					sum[k] = sum[k] / 9;
					q[k] = (Byte)sum[k];
				}
			}
			else
			{
				q[0] = p[0];
				q[1] = p[1];
				q[2] = p[2];
			}
			q += 3;
			p += 3;
		}
	}

	//�N�B�z�����Ͻƻs��image1
	copytoImage1(image_Ptr, temp_Ptr);

	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
}

void Method::Median_Blur(void)
{
	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

	////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����

	//�ŧi�}�C�s��3X3�x�}R,G,B�ƭ�
	int Array[3][9] = {0};
	int temp[3];
	//�s���Ҧ������I�A���ι����A�B�z�����ϰ�
	for (int y = 0; y < image1->Height; y++) {
		for (int x = 0; x < image1->Width; x++) {

			if (y > 0 && x > 0 && y < image1->Height-1 && x < image1->Width-1)
			{
				int Masksize = 0;
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -3; j <= 3; j += 3)
					{
						r = (Byte*)(Void*)p + i * image1->Width * 3 + j;
						Array[0][Masksize] = r[0];
						Array[1][Masksize] = r[1];
						Array[2][Masksize] = r[2];
						Masksize++;
					}
				}

				int min[3] = { 0 };
				for (int m = 0; m < 3; m++)
				{
					for (int n = 0; n < 5; ++n)
					{
						min[m] = n;
						for (int o = n+1; o < 9; ++o)
						{
							if (Array[m][o] < Array[m][min[m]])
								min[m] = o;
						}
						temp[m] = Array[m][n];
						Array[m][n] = Array[m][min[m]];
						Array[m][min[m]] = temp[m];

						q[m] = (Byte)Array[m][4];
					}
				}
			}
			else
			{
				q[0] = p[0];
				q[1] = p[1];
				q[2] = p[2];
			}
			q += 3;
			p += 3;
		}
	}

	//�N�B�z�����Ͻƻs��image1
	copytoImage1(image_Ptr, temp_Ptr);

	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
}

void Method::Gaussian_Blur(void)
{
	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

	////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����

	//�B�n
	int gaussian[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
	int sum[3];

	//�s���Ҧ������I�A���ι����A�B�z�����ϰ�
	for (int y = 0; y < image1->Height; y++) {
		for (int x = 0; x < image1->Width; x++) {
			if (y > 0 && x > 0 && y < image1->Height-1 && x < image1->Width-1)
			{
				int Masksize = 0;
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -3; j <= 3; j += 3)
					{
						r = (Byte*)(Void*)p + i * image1->Width * 3 + j;
						sum[0] += r[0] * gaussian[Masksize];
						sum[1] += r[1] * gaussian[Masksize];
						sum[2] += r[2] * gaussian[Masksize];
						Masksize++;
					}
				}

				for (int k = 0; k < 3; k++)
				{
					sum[k] = sum[k] / 16;
					if (sum[k] > 255)
						sum[k] = 255;
					else if (sum[k] < 0)
						sum[k] = 0;
				}
				q[0] = sum[0];
				q[1] = sum[1];
				q[2] = sum[2];
			}
			else
			{
				q[0] = p[0];
				q[1] = p[1];
				q[2] = p[2];
			}
			q += 3;
			p += 3;
		}
	}

	//�N�B�z�����Ͻƻs��image1
	copytoImage1(image_Ptr, temp_Ptr);

	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
}

void Method::Histogram_Shrink(int minvalue, int maxvalue)
{
	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

	////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����

	//��X�����̤j�P�̤p��
	double max = 0;
	double min = 255;

	for (int y = 0; y < image1->Height; y++)
	{
		for (int x = 0; x < image1->Width; x++)
		{
			if (p[0] > max)
				max = p[0];
			if (p[0] < min)
				min = p[0];
			p += 3;
		}
	}

	//������Y���
	p = (Byte*)((Void*)image_Ptr);

	for (int y = 0; y < image1->Height; y++)
	{
		for (int x = 0; x < image1->Width; x++)
		{
			int pixel = (p[0] * (maxvalue - minvalue) / (max - min)) + minvalue;

			pixel > 255 ? (pixel = 255) : (pixel < 0) ? (pixel = 0) : (pixel = pixel);
			/*
			if (pixel > 255)
				pixel = 255;
			if (pixel < 0)
				pixel = 0;
				*/
			q[0] = pixel;
			q[1] = pixel;
			q[2] = pixel;
			p += 3;
			q += 3;
		}
	}

	//�N�B�z�����Ͻƻs��image1
	copytoImage1(image_Ptr, temp_Ptr);

	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
}

void Method::Histogram_Stretch(int minvalue, int maxvalue)
{
	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

	////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����

	double Max = 0;
	double Min = 255;

	for (int y = 0; y < image1->Height; y++)
	{
		for (int x = 0; x < image1->Width; x++)
		{
			if (p[0] > Max)
				Max = p[0];
			if (p[0] < Min)
				Min = p[0];
			p += 3;
		}
	}

	//������X�i����
	p = (Byte*)((Void*)image_Ptr);
	for (int y = 0; y < image1->Height; y++)
	{
		for (int x = 0; x < image1->Width; x++)
		{
			double pixel = ((p[0] - Min) / (Max - Min)) * (maxvalue - minvalue);

			if (pixel > 255)
				pixel = 255;
			if (pixel < 0)
				pixel = 0;

			q[0] = (Byte)pixel;
			q[1] = (Byte)pixel;
			q[2] = (Byte)pixel;
			p += 3;
			q += 3;
		}
	}

	//�N�B�z�����Ͻƻs��image1
	copytoImage1(image_Ptr, temp_Ptr);

	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
}

void Method::Histogram_Equalization(void)
{
	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

	////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����

	int Graycount[256] = { 0 };
	double fpGraycount[256] = { 0 };
	double totaltemp[256] = { 0 };
	double eqtotaltemp[256] = { 0 };

	//�έp�����
	for (int y = 0; y < image1->Height; y++)
	{
		for (int x = 0; x < image1->Width; x++)
		{
			int value = p[0];
			Graycount[value]++;
			p += 3;
		}
	}

	for (int i = 0; i < 256; i++)
	{
		//�p�⹳���b��i�Ϫ����G�K��
		fpGraycount[i] = (float)Graycount[i] / (float)(image1->Height * (float)image1->Width);

		//�}�C��l��
		totaltemp[i] = 0;

		//�p��ֿn����Ϥ��G				
		totaltemp[i] = totaltemp[i - 1] + fpGraycount[i];

		//�ֿn����Ϥ��G����� �ñN���v�M�g��Ƕ�����0~255
		eqtotaltemp[i] = (int)(255 * totaltemp[i] + 0.5);
	}

	//p���Vimage1���I
	p = (Byte*)((Void*)image_Ptr);

	//����ϧ��Ť�
	for (int y = 0; y < image1->Height; y++)
	{
		for (int x = 0; x < image1->Width; x++)
		{
			int value = p[0];
			if (value > 255) value = 255;
			if (value < 0) value = 0;
			q[0] = (unsigned char)eqtotaltemp[value];
			q[1] = (unsigned char)eqtotaltemp[value];
			q[2] = (unsigned char)eqtotaltemp[value];
			p += 3;
			q += 3;
		}
	}

	//�N�B�z�����Ͻƻs��image1
	copytoImage1(image_Ptr, temp_Ptr);

	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
}

void Method::Erode(void)
{
	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

	////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����

	int Array[9];
	int ArraySize = 9;
	//�s���Ҧ������I�A���ι����A�B�z�����ϰ�
	for (int y = 0; y < image1->Height; y++) {
		for (int x = 0; x < image1->Width; x++) {

			if (y > 0 && x > 0 && y < image1->Height-1 && x < image1->Width-1)
			{
				int Masksize = 0;
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -3; j <= 3; j += 3)
					{
						r = (Byte*)(Void*)p + i * image1->Width * 3 + j;
						Array[Masksize] = r[0] + r[1] + r[2];
						Masksize++;
					}
				}

				//���y3X3�x�}
				for (int k = 0; k < ArraySize; k++)
				{
					//�Y3X3�x�}�������I �h�N�ثe�����]�����I,�Y�L�h�]�����I
					if (Array[k] == 0)
					{
						q[0] = 0;
						q[1] = 0;
						q[2] = 0;
						break;
					}
					else if (k == ArraySize - 1)
					{
						q[0] = 255;
						q[1] = 255;
						q[2] = 255;
					}
				}
			}
			else
			{
				q[0] = p[0];
				q[1] = p[1];
				q[2] = p[2];
			}
			q += 3;
			p += 3;
		}
	}

	//�N�B�z�����Ͻƻs��image1
	copytoImage1(image_Ptr, temp_Ptr);

	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
}

void Method::Dilate(void)
{
	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

	////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����

	int Array[9];
	int ArraySize = 9;
	//�s���Ҧ������I�A���ι����A�B�z�����ϰ�
	for (int y = 0; y < image1->Height; y++) {
		for (int x = 0; x < image1->Width; x++) {

			if (y > 0 && x > 0 && y < image1->Height-1 && x < image1->Width-1)
			{
				int Masksize = 0;
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -3; j <= 3; j += 3)
					{
						r = (Byte*)(Void*)p + i * image1->Width * 3 + j;
						Array[Masksize] = r[0] + r[1] + r[2];
						Masksize++;
					}
				}

				//���y5X5�x�}
				for (int k = 0; k < ArraySize; k++)
				{
					//�Y5X5�x�}�������I �h�ثe�����]�����I,�Y�L�h�]�����I
					if (Array[k] != 0)
					{
						q[0] = 255;
						q[1] = 255;
						q[2] = 255;
						break;
					}
					else if (k == ArraySize - 1)
					{
						q[0] = 0;
						q[1] = 0;
						q[2] = 0;
					}
				}
			}
			else
			{
				q[0] = p[0];
				q[1] = p[1];
				q[2] = p[2];
			}
			q += 3;
			p += 3;
		}
	}

	//�N�B�z�����Ͻƻs��image1
	copytoImage1(image_Ptr, temp_Ptr);

	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
}

void Method::Sobel(void)
{
	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

	////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����

	//�����P�����B�n
	int Gx[9] = { 1, 0, -1,
		2, 0, -2,
		1, 0, -1 };
	int Gy[9] = { -1, -2, -1,
		0, 0, 0,
		1, 2, 1 };

	int m, n, G;

	//�s���Ҧ������I�A���ι����A�B�z�����ϰ�
	for (int y = 0; y < image1->Height; y++) {
		for (int x = 0; x < image1->Width; x++) {
			m = 0, n = 0;
			if (y > 0 && x > 0 && y < image1->Height-1 && x < image1->Width-1)
			{
				int Masksize = 0;
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -3; j <= 3; j += 3)
					{
						r = (Byte*)(Void*)p + i * image1->Width * 3 + j;
						m += ((r[0] + r[1] + r[2])/3) * Gx[Masksize];
						n += ((r[0] + r[1] + r[2])/3) * Gy[Masksize];
						Masksize++;
					}
				}

				G = abs(m) + abs(n);
				//G = sqrt(pow(m,2)+ pow(n,2));
				q[0] = G;
				q[1] = G;
				q[2] = G;
			}
			q += 3;
			p += 3;
		}
	}

	//�N�B�z�����Ͻƻs��image1
	copytoImage1(image_Ptr, temp_Ptr);

	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
}

void Method::Haar_Wavelet(int pass)
{
	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);

	////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);

	//�s��ثe�����ΤU�@�ӹ���
	int nowp, nextp, pp;
	int passWidth = image1->Width;
	int passHeight = image1->Height;
	float temp;

	for (int i = 0; i < pass; i++)
	{
		//���Ы��V�Ϲ��_�l��m
		temp_Ptr = temp_Image_Data->Scan0;  
		q = (Byte*)((Void*)temp_Ptr); 
		image_Ptr = image1_Data->Scan0; 
		p = (Byte*)((Void*)image_Ptr); 

		//�]�w�Ϥ������e��
		passWidth = passWidth / 2;

		//���V�������W�ϰ�
		r = q + passWidth * 3;

		//�s���Ҧ������I�������p�i�ഫ
		for (int y = 0; y < passHeight; y++) {
			for (int x = 0; x < passWidth; x++) {
				//���o�ثe����
				nowp = p[0];

				//����U�@�ӹ���
				p += 3;

				//���o�U�@�ӹ���
				nextp = p[0];

				//�p��C�W����
				temp = nowp + nextp;
				pp = temp / 2 + 0.5;

				//��m�C�W����
				q[0] = pp;
				q[1] = pp;
				q[2] = pp;

				//�p�Ⱚ�W����
				pp = (nowp - nextp) / 2;

				if (pp < 0)
					pp = 0;

				pp += 128;

				if (pp > 255)
					pp = 255;

				//��m���W�V��
				r[0] = pp;
				r[1] = pp;
				r[2] = pp;

				//���U�@�C
				q += 3;
				p += 3;
				r += 3;
			}

			//���U�@��_�l�I
			q += (image1->Width * 3) - (passWidth * 3);
			r += (image1->Width * 3) - (passWidth * 3);

			if (i >= 1)
			{
				p += (image1->Width * 3) - (passWidth * 2 * 3);
			}
		}

		//���Ы��V�Ϲ��_�l��m
		temp_Ptr = image1_Data->Scan0;  
		q = (Byte*)((Void*)temp_Ptr); 
		image_Ptr = temp_Image_Data->Scan0; 
		p = (Byte*)((Void*)image_Ptr); 

		//�]�w�Ϥ���������
		passHeight = passHeight / 2;

		//���V�����p�i���W�ϰ�
		r = q + (passHeight *image1->Width) * 3;

		//�s���Ҧ������I�������p�i�ഫ
		for (int y = 0; y < passWidth * 2; y++) {
			for (int x = 0; x < passHeight; x++) {

				//���o�ثe����
				nowp = p[0];

				//���U�@��
				p += image1->Width * 3;

				//���o�U�@�ӹ���
				nextp = p[0];

				//�p��C�W����
				temp = nowp + nextp;
				pp = temp / 2 + 0.5;

				//��m�C�W����
				q[0] = pp;
				q[1] = pp;
				q[2] = pp;

				//�p�Ⱚ�W����
				pp = (nowp - nextp) / 2;
				if (pp < 0)
					pp = 0;

				pp += 128;

				if (pp > 255)
					pp = 255;

				//��m���W����
				r[0] = pp;
				r[1] = pp;
				r[2] = pp;

				//���U�@�C
				q += image1->Width * 3;
				p += image1->Width * 3;
				r += image1->Width * 3;
			}
			//���W�ܰ_�l�I
			q = q - (passHeight * image1->Width) * 3 + 3;
			r = r - (passHeight * image1->Width) * 3 + 3;
			p = p - ((passHeight * 2) * image1->Width) * 3 + 3;
		}

		//���Ы��V�Ϲ��_�l��m
		temp_Ptr = image1_Data->Scan0;  
		image_Ptr = temp_Image_Data->Scan0; 

		//�N�B�z�����Ͻƻs��temp
		copytoImage1(temp_Ptr, image_Ptr);
	}

	//�N�B�z�����Ͻƻs��image1
	copytoImage1(image_Ptr, temp_Ptr);

	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
}

void Method::Rgb_Hsi_convert(void)
{
	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

	////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����

	//�ŧi�ʺA�}�C
	double  *Ha = new double[image1->Height * image1->Width];
	double  *Sa = new double[image1->Height * image1->Width];
	double  *Ia = new double[image1->Height * image1->Width];

	long int count = 0;
	double  H, S, I;

	//�s���Ҧ������I�A���ι����A�B�z�����ϰ�
	for (int y = 0; y < image1->Height; y++) {
		for (int x = 0; x < image1->Width; x++) {

			double rr = p[2];
			double gg = p[1];
			double bb = p[0];


			float min, max; /* minimum and maximum RGB values */
			float angle; /* temp variable used to compute Hue */

			if ((rr <= gg) && (rr <= bb))
				min = rr;
			else if ((gg <= rr) && (gg <= bb))
				min = gg;
			else
				min = bb;

			/* compute intensity */
			I = (rr + gg + bb) / 3.0;

			/* compute hue and saturation */
			if ((rr == gg) && (gg == bb)) /* gray-scale */
			{
				S = 0.0;
				H = 0.0;
			}
			else
			{
				S = 1.0 - (3.0 / (rr + gg + bb)) * min;
				angle = (rr - 0.5 * gg - 0.5 * bb) /
					sqrt((rr - gg) * (rr - gg) + (rr - bb) * (gg - bb));
				H = acos(angle);
				H *= 57.29577951; /* convert to degrees */
			}
			if (bb>gg)
				H = 360.0 - H;

			
			Ha[count] = H;
			Sa[count] = S;
			Ia[count] = I;

			count++;
			p += 3;
		}
	}

	//����ϧ���
	int Graycount[256] = { 0 };
	double fpGraycount[256] = { 0 };
	double totaltemp[256] = { 0 };
	double eqtotaltemp[256] = { 0 };


	count = 0;
	//�έp�����
	for (int y = 0; y < image1->Height; y++)
	{
		for (int x = 0; x < image1->Width; x++)
		{
			int value = Ia[count];
			Graycount[value]++;
			count++;
		}
	}

	for (int i = 0; i < 256; i++)
	{
		//�p�⹳���b��i�Ϫ����G�K��
		fpGraycount[i] = (float)Graycount[i] / (float)(image1->Height * (float)image1->Width);

		//�}�C��l��
		totaltemp[i] = 0;

		//�p��ֿn����Ϥ��G				
		totaltemp[i] = totaltemp[i - 1] + fpGraycount[i];

		//�ֿn����Ϥ��G����� �ñN���v�M�g��Ƕ�����0~255
		eqtotaltemp[i] = (int)(255 * totaltemp[i] + 0.5);
	}

	count = 0;
	//����ϧ��Ť�
	for (int y = 0; y < image1->Height; y++)
	{
		for (int x = 0; x < image1->Width; x++)
		{
			int value = Ia[count];
			if (value > 255) value = 255;
			if (value < 0) value = 0;
			Ia[count] = (double)eqtotaltemp[value];
			count++;
		}
	}

	////�Ȧs�ϪŶ�
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

	////��ϪŶ�
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����

	double r, b, g;
	count = 0;
	//�s���Ҧ������I�A���ι����A�B�z�����ϰ�
	for (int y = 0; y < image1->Height; y++) {
		for (int x = 0; x < image1->Width; x++) {

			H = Ha[count];
			S = Sa[count];
			I = Ia[count];

			float angle1, angle2, scale, temp, denom; /* temp variables */

			if (I == 0.0) /* BLACK */
			{
				r = 0.0;
				g = 0.0;
				b = 0.0;
				//return;
			}
			if (S == 0.0) /* gray-scale H is undefined*/
			{
				r = I;
				g = I;
				b = I;
				//return;
			}
			if (H<0.0)
				H += 360.0;

			scale = 3.0 * I;

			if (H <= 120.0)
			{
				angle1 = H * 0.017453293; /* convert to radians - mul by pi/180 */
				angle2 = (60.0 - H)*0.017453293;

				b = (1.0 - S) / 3.0;
				r = (1.0 + (S*cos(angle1) / cos(angle2))) / 3.0;
				g = 1.0 - r - b;
				b *= scale;
				r *= scale;
				g *= scale;
			}
			else if ((H>120.0) && (H <= 240.0))
			{
				H -= 120.0;
				angle1 = H * 0.017453293; /* convert to radians - mul by pi/180 */
				angle2 = (60.0 - H)*0.017453293;

				r = (1.0 - S) / 3.0;
				g = (1.0 + (S*cos(angle1) / cos(angle2))) / 3.0;
				b = 1.0 - r - g;
				r *= scale;
				g *= scale;
				b *= scale;
			}
			else
			{
				H -= 240.0;
				angle1 = H * 0.017453293; /* convert to radians - mul by pi/180 */
				angle2 = (60.0 - H)*0.017453293;

				g = (1.0 - S) / 3.0;
				b = (1.0 + (S*cos(angle1) / cos(angle2))) / 3.0;
				r = 1.0 - g - b;
				r *= scale;
				g *= scale;
				b *= scale;
			}

			if (b > 255) b = 255;
			if (g > 255) g = 255;
			if (r > 255) r = 255;
			if (b < 0) b = 0;
			if (g < 0) g = 0;
			if (r < 0) r = 0;
			q[0] = (Byte)b;
			q[1] = (Byte)g;
			q[2] = (Byte)r;
			count++;
			q += 3;
		}
	}
	
	//�N�B�z�����Ͻƻs��image1
	copytoImage1(image_Ptr, temp_Ptr);

	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
	
	delete []Ha;
	delete []Sa;
	delete []Ia;
}

void Method::Cale_Histogram(int * color,int channel)
{
	////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����


	for (int y = 0; y < image1->Height; y++) {
		for (int x = 0; x < image1->Width; x++) {
			int value = p[channel];	
			color[value]++;
			p += 3;
		}
	}
	image1->UnlockBits(image1_Data);
}

void Method::Hough_Transform(void)
{
	double START, END;
	START = clock();

	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

	////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����

								   //�x�sSin Cos����
	float sintab[180], costab[180];

	float theta;
	//�]��cos��X�Ӫ��O���� �ҥH���N�����ഫ������
	for (int angle = 0; angle < 180; angle++)
	{
		theta = angle * M_PI / 180.0;
		sintab[angle] = sin(theta);
		costab[angle] = cos(theta);
	}

	float rmax = sqrt(image1->Height * image1->Height + image1->Width * image1->Width);
	//rmax *= 2;
	//�ŧi�ʺA�@���}�C
	float **hough_Array;
	hough_Array = new float*[(int)rmax * 2];
	cout << rmax << endl;

	//�ŧi�ʺA�G���}�C
	for (int k = 0; k < rmax * 2; k++)
	{
		hough_Array[k] = new float[180];
	}

	//�M�Ű}�C
	for (int k = 0; k < rmax * 2; k++)
	{
		for (int n = 0; n < 180; n++)
		{
			hough_Array[k][n] = 0;
		}
	}

	float r;
	int nr, angle;
	//�N�y���ഫ���N�ҪŶ�
	for (int y = 0; y < image1->Height; y++) {
		for (int x = 0; x < image1->Width; x++) {
			//�p�G�I�O�զ�(��t)
			if (p[0] > 191)
			{
				//����0~180
				for (int angle = 0; angle < 180; angle++)
				{
					r = y * sintab[angle] + x * costab[angle];
					nr = (int)(r + rmax);
					hough_Array[nr][angle]++;
				}
			}
			p += 3;
		}
	}

	//�p�⦳�h�֭��I���|�Ƥj��]�w��
	int point_Number = 0;
	int hough_value = 108;
	for (int k = 0; k < rmax * 2; k++)
	{
		for (int n = 0; n < 180; n++)
		{
			if (hough_Array[k][n] > hough_value)
			{
				point_Number++;
			}
		}
	}
	cout << point_Number << endl;

	//�������|�Ƥj��]�w�Ȫ�r�Pangle
	int *r_Array = new int[point_Number];
	int *angle_Array = new int[point_Number];
	int count = 0;
	for (int k = 0; k < rmax * 2; k++)
	{
		for (int n = 0; n < 180; n++)
		{
			if (hough_Array[k][n] > hough_value)
			{
				r_Array[count] = k;
				angle_Array[count] = n;
				count++;
			}
		}
	}

	/////////////////////�N�ҪŶ�
	//�s�W�@�ӪŶ���m�����o�i�v��
	Bitmap^ hough_TransformImage;
	Imaging::BitmapData^ hough_TransformImageData;
	hough_TransformImage = gcnew Bitmap(180, rmax * 2);
	Rectangle hough_Rect;
	hough_Rect = Rectangle(0, 0, 180, rmax * 2);
	hough_TransformImageData = hough_TransformImage->LockBits(hough_Rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);

	//���Ы��V�Ϲ��_�l��m
	IntPtr ResultPtr;
	Byte * R;
	ResultPtr = hough_TransformImageData->Scan0;
	R = (Byte*)((Void*)ResultPtr);

	//�e�X�N�ҪŶ�
	for (int k = 0; k < rmax * 2; k++)
	{
		for (int n = 0; n < 180; n++)
		{
			if (hough_Array[k][n] > 0)
			{
				int point = hough_Array[k][n];
				if (point > 255) point = 255;
				if (point < 0) point = 0;
				R[0] = (Byte)point;
				R[1] = (Byte)point;
				R[2] = (Byte)point;
			}
			R += 3;
		}
	}

	//�s�W�@�ӪŶ���m�����o�i�v��
	Bitmap^ lineImage;
	Imaging::BitmapData^ lineImageData;
	lineImage = gcnew Bitmap(image1->Width, image1->Height);
	lineImageData = lineImage->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);

	//���Ы��V�Ϲ��_�l��m
	ResultPtr = lineImageData->Scan0;
	R = (Byte*)((Void*)ResultPtr);

	IntPtr ptr;
	ptr = image1_Data->Scan0;
	p = (Byte*)((Void*)ptr);
	//���X���u
	for (int y = 0; y < image1->Height; y++) {
		for (int x = 0; x < image1->Width; x++) {
			for (int i = 0; i < point_Number; i++)
			{
				if (x == (int)round(((r_Array[i] - rmax) - y * sintab[(int)angle_Array[i]]) / (costab[(int)angle_Array[i]])))
				{
					R[0] = 0;
					R[1] = 0;
					R[2] = 255;
				}
			}
			p += 3;
			R += 3;
		}
	}

	ptr = image1_Data->Scan0;
	p = (Byte*)((Void*)ptr);
	ResultPtr = lineImageData->Scan0;
	R = (Byte*)((Void*)ResultPtr);

	//�N���u�л\����
	for (int y = 0; y < image1->Height; y++)
	{
		for (int x = 0; x < image1->Width; x++)
		{
			if (R[2] == 255)
			{
				p[0] = 0;
				p[1] = 0;
				p[2] = 255;
			}
			p += 3;
			R += 3;
		}
	}

	lineImage->UnlockBits(lineImageData);
	hough_TransformImage->UnlockBits(hough_TransformImageData);

	//�N�B�z�����Ͻƻs��image1
//	copytoImage1(image_Ptr, temp_Ptr);

	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
	END = clock();
	cout << endl << "�{������Ҫ�O�G" << (double)clock() / CLOCKS_PER_SEC << " S";
	cout << endl << "�i��B��Ҫ�O���ɶ��G" << (END - START) / CLOCKS_PER_SEC << " S" << endl;
}

void Method::Prewitt(void)
{
	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

								  ////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����

								   //�����P�����B�n
	int Gx[9] = { 1, 1, 1,
		0, 0, 0,
		-1, -1, -1 };
	int Gy[9] = { 1, 0, -1,
		1, 0, -1,
		1, 0, -1 };

	int m, n, G;

	//�s���Ҧ������I�A���ι����A�B�z�����ϰ�
	for (int y = 0; y < image1->Height; y++) {
		for (int x = 0; x < image1->Width; x++) {
			m = 0, n = 0;
			if (y > 0 && x > 0 && y < image1->Height-1 && x < image1->Width-1)
			{
				int Masksize = 0;
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -3; j <= 3; j += 3)
					{
						r = (Byte*)(Void*)p + i * image1->Width * 3 + j;
						m += ((r[0] + r[1] + r[2]) / 3) * Gx[Masksize];
						n += ((r[0] + r[1] + r[2]) / 3) * Gy[Masksize];
						Masksize++;
					}
				}
				G = abs(m) + abs(n);
				//G = sqrt(pow(m,2) + pow(n,2));

				q[0] = G;
				q[1] = G;
				q[2] = G;
			}
			q += 3;
			p += 3;
		}
	}

	//�N�B�z�����Ͻƻs��image1
	copytoImage1(image_Ptr, temp_Ptr);

	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
}

void Method::Laplacian(void)
{
	////�Ȧs�ϪŶ�
	temp_Image = gcnew Bitmap(image1->Width, image1->Height); //�ͦ��@�i���e��image�@�˪�Bitmap
	temp_Image_Data = temp_Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	temp_Ptr = temp_Image_Data->Scan0;  //�Nint���Ы��VImage������ƪ��̫e����m
	q = (Byte*)((Void*)temp_Ptr); //�]�w����

								  ////��ϪŶ�
	image1_Data = image1->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, image1->PixelFormat);
	image_Ptr = image1_Data->Scan0; //�Nint���Ы��VImage������ƪ��̫e����m
	p = (Byte*)((Void*)image_Ptr); //�]�w����

								   //�����P�����B�n
	int Gx[9] = { 0, 1, 0,
		1, -4, 1,
		0, 1, 0};
	int Gy[9] = { 0, 1, 0,
		1, -4, 1,
		0, 1, 0 };

	int m, n, G;

	//�s���Ҧ������I�A���ι����A�B�z�����ϰ�
	for (int y = 0; y < image1->Height; y++) {
		for (int x = 0; x < image1->Width; x++) {
			m = 0, n = 0;
			if (y > 0 && x > 0 && y < image1->Height-1 && x < image1->Width-1)
			{
				int Masksize = 0;
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -3; j <= 3; j += 3)
					{
						r = (Byte*)(Void*)p + i * image1->Width * 3 + j;
						m += ((r[0] + r[1] + r[2]) / 3) * Gx[Masksize];
						n += ((r[0] + r[1] + r[2]) / 3) * Gy[Masksize];
						Masksize++;
					}
				}
				G = abs(m) + abs(n);
				//G = sqrt(pow(m, 2) + pow(n, 2));

				q[0] = G;
				q[1] = G;
				q[2] = G;
			}
			q += 3;
			p += 3;
		}
	}

	//�N�B�z�����Ͻƻs��image1
	copytoImage1(image_Ptr, temp_Ptr);

	//����Ϲ��O����Ŷ�
	image1->UnlockBits(image1_Data);
	temp_Image->UnlockBits(temp_Image_Data);
}