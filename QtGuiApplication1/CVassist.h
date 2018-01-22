#pragma once
float MyMax(float a, float b, float c) {
	if (a > b) {
		if (a > c) return a;
		else return c;
	}
	else if (b > c) return b;
	else return c;
}
float MyMax(float a, float b) {
	if (a > b)return a;
	else return b;
}
float MyMin(float a, float b, float c) {
	if (a < b) {
		if (a < c) return a;
		else return c;
	}
	else if (b < c) return b;
	else return c;
}
float MyMin(float a, float b) {
	if (a > b)return b;
	else return a;
}

//它的原理是经过icvHoughSortDescent32s函数后，以sort_buf中元素作为adata数组下标，adata中的元素降序排列，即adata[sort_buf[0]]是adata所有元素中最大的，adata[sort_buf[center_count-1]]是所有元素中最小的    
void MyHoughSortDescent32s(int* buf, int length, int* data) {
	int *res = buf;
	int *src = data;
	int *flag = new int[length];
	memset(flag, 0, length * sizeof(int));

	for (int j = 0; j < length; j++) {
		int max = 0;
		int tmp = 0;
		for (int i = 0; i < length; i++) {
			if (flag[i] == 1) continue;
			if (src[i] > max) {
				max = src[i];
				tmp = i;
			}
		}
		//cout << j << endl;
		res[j] = tmp;
		flag[tmp] = 1;
	}
}

void inc_if_inside(double *** H, int x, int y, int height, int width, int r)
{
	if (x>0 && x<width && y> 0 && y<height)
		H[y][x][r]++;
}

void MatMinus(Mat src, Mat mask, Mat dst) {
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			int a;
			if (src.at<Vec4b>(i, j)[0] == 0) a = 0;
			else if (src.at<Vec4b>(i, j)[0] == 255) {
				a = src.at<Vec4b>(i, j)[0] - mask.at<Vec4b>(i, j)[0];
			}

			dst.at<Vec4b>(i, j)[0] = a;
			dst.at<Vec4b>(i, j)[2] = a;
			dst.at<Vec4b>(i, j)[1] = a;
		}
	}
}

void MatOr(Mat src, Mat mask, Mat dst) {
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			int a;
			if (src.at<Vec4b>(i, j)[0] == 255 || mask.at<Vec4b>(i, j)[0] == 255) a = 255;
			else  a = 0;

			dst.at<Vec4b>(i, j)[0] = a;
			dst.at<Vec4b>(i, j)[1] = a;
			dst.at<Vec4b>(i, j)[2] = a;
		}
	}
}

void MatAnd(Mat src, Mat mask, Mat dst) {
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			int a;
			if (src.at<Vec4b>(i, j)[0] == 255 && mask.at<Vec4b>(i, j)[0] == 255) a = 255;
			else  a = 0;

			dst.at<Vec4b>(i, j)[0] = a;
			dst.at<Vec4b>(i, j)[1] = a;
			dst.at<Vec4b>(i, j)[2] = a;
		}
	}
}

void MatComplement(Mat src, Mat dst) {
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			int a;
			if (src.at<Vec4b>(i, j)[0] == 255) a = 0;
			else  a = 255;

			dst.at<Vec4b>(i, j)[0] = a;
			dst.at<Vec4b>(i, j)[1] = a;
			dst.at<Vec4b>(i, j)[2] = a;
		}
	}
}

bool MatEqual(Mat src, Mat mask) {
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			if (src.at<Vec4b>(i, j)[0] != mask.at<Vec4b>(i, j)[0]) return false;
		}
	}
	return true;
}