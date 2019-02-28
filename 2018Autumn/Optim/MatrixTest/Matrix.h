//#define _CRT_SECURE_NO_WARNINGS
#ifndef MY_MATRIX_H
#define MY_MATRIX_H
#include <math.h>
#include <stdio.h>
//#include <Windows.h>
#include <iostream>
#include "mkl.h"
using namespace std;

#define EPS 1e-6
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0

typedef int Status;
typedef double ElemType;

/*������Ķ���*/
class matrix 
{
public:
	/*�������Ԫ��*/
	int Col;	//����
	int Row;	//����
	long int Numel;
	ElemType *Elem;
public:
	/*�����������*/
	matrix(void);													//Ĭ�Ϲ��캯��
	matrix(int Rows, int Cols, double val);				//�����й������,Ĭ��Ϊ0
	matrix(int Cols, double val);								//���й���Խ���Ĭ��Ϊ��λ��
	matrix(int Rows,int Cols, ElemType *Array);					//ʹ��һά���鹹�����
	matrix(int Rows,int Cols, ElemType **Array);					//����ά���鹹�����
	matrix(const matrix& matrix);

	void show();													//��ӡ����
	ElemType GetElem(int L, int R);									//�鿴����Ԫ
	void SetElem(int L, int R, ElemType val);									//�޸ľ���Ԫ

	matrix GetCols(int Col_index);										//��ȡָ������
	matrix GetCols(int start, int end, int step);							//��ȡָ����,stepĬ��Ϊ1
	matrix GetRows(int Row_index);										//��ȡָ����
	matrix GetRows(int start, int end, int step);						//��ȡָ����,stepĬ��Ϊ1
	matrix slice(int start_row, int end_row, int start_col, int end_col, int row_step, int col_step);//��Ƭ����
	matrix transpose();									//����ת��
	ElemType& operator()(int L, int R);
	matrix& operator()(matrix Row_index, matrix Col_index);
	matrix& operator=(const matrix& matrix);            //����ĸ�ֵ����������
	matrix& operator+=(matrix& matrix2);          //����ļӷ�����������
	matrix& operator+=(ElemType val);                //���������ļӷ�����������
	matrix& operator-=(matrix& matrix2);          //����ļ�������������
	matrix& operator-=(ElemType val);                //���������ļ�������������
	matrix& operator*=(matrix& matrix2);          //����ĳ˷�����������
	matrix& operator*=(ElemType val);                //���������ĳ˷�����������
	
public:
	/*�����������*/
	friend matrix operator+(matrix &matrix1, matrix &matrix2);
	friend matrix operator-(matrix &matrix1, matrix &matrix2);
	friend matrix operator*(matrix &matrix1, matrix &matrix2);			
	friend matrix operator+(double val, matrix& matrix1);      //���������ӷ������������أ�2��
	friend matrix operator-(matrix& matrix1, double val);      //�����������������������أ�1��
	friend matrix operator-(double val, matrix& matrix1);      //�����������������������أ�2��
	friend matrix operator-(matrix& M);									   //M�ĸ�����
	friend matrix operator*(matrix& matrix1, double val);      //���������˷������������أ�1��
	friend matrix operator*(double val, matrix& matrix1);      //���������˷������������أ�2)
	friend std::ostream& operator<<(std::ostream &os,matrix &matrix);	//���
public:
	/*�����ܺ���*/
	Status reshape(int Rows, int Cols);								//�������,Ϊ-1���Զ�����
	~matrix();														//��������
};



/*Ĭ�Ϲ������*/
matrix::matrix() {
	Row = 0;
	Col = 0;
	Numel = 0;
	Elem = NULL;
}
/*����0����*/
matrix::matrix(int Rows, int Cols, double val = 0) {
	Col = Cols;
	Row = Rows;
	Numel = Col * Row;
	Elem = (ElemType *)mkl_malloc(Col*Row * sizeof(ElemType), 64);
	for (int i = 0; i < Numel; i++)
		Elem[i] = val;
}
/*���쵥λ��*/
matrix::matrix(int Cols, double val = 1) {
	Col = Row = Cols;
	Numel = Col * Row;
	Elem = (ElemType *)mkl_malloc(Col*Row * sizeof(ElemType), 64);
	for (int i = 0; i < Numel; i++)
		Elem[i] = 0;				//�ȳ�ʼ����0
	for (int i = 0; i < Col; i++)
		Elem[i*Col + i] = val;		//ָ��Ԫ��ʼ����val
}
/*ʹ����֪���鹹�����*/
matrix::matrix(int Rows,int Cols, ElemType *Array) {
	Col = Cols;
	Row = Rows;
	Numel = Col * Row;
	Elem = (ElemType *)mkl_malloc(Col*Row * sizeof(ElemType), 64);
	for (int i = 0; i < Numel; i++)
		Elem[i] = Array[i];
}
/*ʹ����֪���鹹�����*/
matrix::matrix(int Rows,int Cols, ElemType **Array) {
	Col = Cols;
	Row = Rows;
	Numel = Col * Row;
	Elem = (ElemType *)mkl_malloc(Col*Row * sizeof(ElemType), 64);
	for (int i = 0; i < Col; i++) {
		for (int j = 0; j < Row; j++) {
			Elem[i*Row + j] = Array[i][j];
		}
	}
}
/*ʹ����֪���������*/
matrix::matrix(const matrix &M) {
	Col = M.Col;
	Row = M.Row;
	Numel = M.Numel;
	Elem = (ElemType *)mkl_malloc(Col*Row * sizeof(ElemType), 64);
	for (int i = 0; i < Numel; i++)
		Elem[i] = M.Elem[i];
}
/*����*/
matrix::~matrix(void) {
	mkl_free(Elem);		//�ͷ��ڴ�
}

/*��ӡ����*/
void matrix::show() {
	for (int i = 0; i < Numel; i++) {
		if (i%Col == 0)
			cout << "\n";
		cout << Elem[i] << " ";
	}
}

/*��ȡָ����*/
matrix matrix::GetCols(int Col_index) {
	if (Col_index > Col) {
		printf("������������ά��,M:%d*%d,Col_index:%d\n", Row, Col, Col_index);
		matrix M;
		return M;
	}
	else {
		matrix M(Row, 1);
		for (int i = 0; i < Row; i++)
			M.Elem[i] = Elem[i*Row + Col_index];
		return M;
	}
}
/*��ȡָ����,ʹ����˫ѭ��,���Ľ�*/
matrix matrix::GetCols(int start, int end, int step = 1) {
	/*��start��end(������end)������Ϊstep*/
	if (start <0 || end > Col) {
		printf("������������ά��,M:%d*%d,start:%d,end:%d\n", Row, Col, start, end);
		matrix M;
		return M;
	}
	int num = (end - start - 1) / step + 1;
	matrix M(Row, num);
	for (int i = 0; i < Row; i++)
		for (int j = 0; j < num; j++)
			M.Elem[i*Row + j] = Elem[i*Row + start + j * step];
	return M;

}
/*��ȡָ����*/
matrix matrix::GetRows(int Row_index) {
	if (Row_index > Row) {
		printf("������������ά��,M:%d*%d,Row_index:%d\n", Row, Col, Row_index);
		matrix M;
		return M;
	}
	else {
		matrix M(1, Col);
		for (int i = 0; i < Col; i++)
			M.Elem[i] = Elem[Row_index*Row + i];
		return M;
	}
}
/*��ȡָ����,�õ���˫ѭ��,���Ľ�*/
matrix matrix::GetRows(int start, int end, int step = 1) {
	if (start < 0 || end > Row) {
		printf("������������ά��,M:%d*%d,start:%d,end:%d\n", Row, Col, start, end);
		matrix M;
		return M;
	}
	int num = (end - start - 1) / step + 1;
	matrix M(num, Col);
	for (int i = 0; i < num; i++)
		for (int j = 0; j < Col; j++)
			M.Elem[i*Col + j] = Elem[(start + i * step)*Col + j];
	return M;
}

/*��Ƭ����,ֻ֧�־��Ȳ�������,����start��end(������end),����Ϊ1����Lapack�����*/
matrix matrix::slice(int start_row, int end_row, int start_col, int end_col, int row_step = 1, int col_step = 1) {
	if (start_row < 0 || start_col < 0 || end_row > Row || end_col > Col || row_step <= 0 || col_step <= 0) {
		printf("����ʧ��,��������ά�Ȼ�����������Ҫ��,M:%d*%d\n", Row, Col);
		matrix M;
		return M;
	}
	/*
	else if (row_step == 1 && col_step == 1) {
		int num_row = end_row - start_row;
		int num_col = end_col - start_col;
		matrix M(num_row, num_col);
		//int *row_index = (int *)malloc(num_row * sizeof(int));
		//int *col_index = (int *)malloc(num_col * sizeof(int));
		int *index_in = (int *)malloc(num_row*num_col * sizeof(int));
		int *index_out = (int *)malloc(num_row*num_col * sizeof(int));
		int k = 0;
		for (int i = 0; i < num_row; i++) {
			for (int j = 0; j < num_col; j++, k++) {
				index_in[k] = start_col + j + (start_row + i)*Col;
				index_out[k] = k;
			}
		}
			
		/*for (int i = 0; i < num_row; i++)
			row_index[i] = start_row + i;
		for (int i = 0; i < num_col; i++)
			col_index[i] = start_col + i;	 
		pdlacpy("A", &num_row, &num_col, Elem, &start_row, &start_col, index_in, M.Elem, 0, 0, index_out);
		return M;
	}*/
	else {
		int num_row = (end_row - start_row - 1) / row_step + 1;
		int num_col = (end_col - start_col - 1) / col_step + 1;
		matrix M(num_row, num_col);
		for (int i = 0; i < num_row; i++)
			for (int j = 0; j < num_col; j++)
				M.Elem[i*num_col + j] = Elem[(start_row + i * row_step)*Col + j * col_step + start_col];
		return M;
	}	
}

/*�鿴,�޸ľ�������*/
ElemType& matrix::operator()(int L, int R) {
	return Elem[L*Row + R];
}
/*��������,ʹ����˫ѭ��,���Ľ�,֧�����ⲽ������,����ö�������(���ڲ���ǿ��ת����int��)*/
matrix& matrix::operator()(matrix Row_index, matrix Col_index) {
	int Row1 = Row_index.Row;
	int Col1 = Row_index.Col;
	int Row2 = Col_index.Row;
	int Col2 = Col_index.Col;
	if ((Row1 == 1 && Row2 == 1) || (Row1 == 1 && Col2 == 1) || (Col1 == 1 && Row2 == 1) || (Col1 == 1 && Col2 == 1)) {
		int New_Row = max(Row1, Col1);
		int New_Col = max(Row2, Col2);
		matrix New(New_Row, New_Col);
		for (int i = 0; i < New.Row; i++)
			for (int j = 0; j < New.Col; j++)
				*(New.Elem + i * New_Row + j) = *(Elem + int(*(Row_index.Elem + i))*Row + int(*(Col_index.Elem + j)));
		return New;
	}
	else {
		printf("��������\n");
		matrix M;
		return M;
	}
}

ElemType matrix::GetElem(int L, int R) {
	return Elem[(L - 1)*Row + R - 1];
}

void matrix::SetElem(int L, int R, ElemType val) {
	Elem[(L - 1)*Row + R - 1] = val;
}


/*��ֵ���������*/
matrix& matrix::operator=(const matrix &M) {
	if (this == &M)
		return *this;
	Col = M.Col;
	Row = M.Row;
	Numel = M.Numel;
	Elem = (ElemType *)mkl_malloc(Col*Row * sizeof(ElemType), 64);
	for (int i = 0; i < Numel; i++)
		Elem[i] = M.Elem[i];
	return *this;
}
matrix& matrix::operator+=(matrix &M) {
	if (Col == M.Col&&Row == M.Row) {
		for (int i = 0; i < Numel; i++)
			Elem[i] += M.Elem[i];
		return *this;
	}
	else {
		cout << "����ά�Ȳ�ͬ,�������\n";
		return *this;
	}
}
matrix& matrix::operator-=(matrix &M) {
	if (Col == M.Col&&Row == M.Row) {
		for (int i = 0; i < Numel; i++)
			Elem[i] -= M.Elem[i];
		return *this;
	}
	else {
		cout << "����ά�Ȳ�ͬ,�������\n";
		return *this;
	}
}

/*����������*/
matrix& matrix::operator+=(ElemType val) {
	for (int i = 0; i < Numel; i++)
		Elem[i] += val;
	return *this;
}
matrix& matrix::operator-=(ElemType val) {
	for (int i = 0; i < Numel; i++)
		Elem[i] -= val;
	return *this;
}

/*��������������*/
matrix& matrix::operator*=(matrix&M) {
	/*��ʱ������ͨ����˷�,֮�����취�Ż����㷨,����ʹ�ò��м���*/
	/*��������� A = A*B  */
	ElemType temp = 0;
	ElemType *NewElem = (ElemType *)mkl_malloc(Col*Row * sizeof(ElemType), 64);
	for (int i = 0; i < Col*M.Row; i++)
		NewElem[i] = 0;
	if (Col == M.Row) {
		//CPU���м���
		int max_threads = mkl_get_max_threads();
		mkl_set_num_threads(max_threads);
		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, Row, M.Col, Col, 1, Elem, Col, M.Elem, M.Col, 0, NewElem, M.Col);
		Elem = NewElem;
		return *this;
	}
	else {
		printf("����ά�Ȳ�һ��,�޷����,A:%d*%d����,B:%d*%d����\n", Row, Col, M.Col, M.Row);
		return *this;
	}
}
matrix& matrix::operator*=(ElemType val) {
	for (int i = 0; i < Numel; i++)
		Elem[i] *= val;
	return *this;
}


/*���������˷�*/
matrix operator*(matrix& M1, matrix& M2)
{
	ElemType temp = 0;
	matrix Temp(M1.Row, M2.Col);
	if (M1.Col == M2.Row) {
		//CPU���м���
		//int max_threads = mkl_get_max_threads();
		//mkl_set_num_threads(max_threads);
		//mkl_set_num_threads(8);
		mkl_set_num_threads(1);
		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, M1.Row, M2.Col, M1.Col, 1, M1.Elem, M1.Col, M2.Elem, M2.Col, 0, Temp.Elem, M2.Col);
		return Temp; 
	}
	else {
		printf("����ά�Ȳ�һ��,�޷����,A:%d*%d����,B:%d*%d����\n", M1.Row, M1.Col, M2.Row, M2.Col);
		return M1;
	}
}

/*���������˷�*/
matrix operator*(double val, matrix& M) {
	for (int i = 0; i < M.Numel; i++)
		M.Elem[i] *= val;
	return M;
}
matrix operator*(matrix& M, double val) {
	for (int i = 0; i < M.Numel; i++)
		M.Elem[i] *= val;
	return M;
}


/*���������ӷ�*/
matrix operator+(matrix& M, double val) {
	for (int i = 0; i < M.Numel; i++)
		M.Elem[i] += val;
	return M;
}
matrix operator+(double val, matrix& M) {
	for (int i = 0; i < M.Numel; i++)
		M.Elem[i] += val;
	return M;
}

/*���������ӷ�*/
matrix operator+(matrix& M1, matrix& M2) {
	matrix M3(M1);
	if (M1.Col == M2.Col&&M1.Row == M2.Row) {
		for (int i = 0; i < M1.Numel; i++)
			M3.Elem[i] += M2.Elem[i];
		return M3;
	}
	else {
		printf("����ά�Ȳ�һ��,�޷����,A:%d*%d����,B:%d*%d����\n", M1.Row, M1.Col, M2.Row, M2.Col);
		return M1;
	}
}


/*������������*/
matrix operator-(matrix& M, double val) {
	for (int i = 0; i < M.Numel; i++)
		M.Elem[i] -= val;
	return M;
}
matrix operator-(double val, matrix& M) {
	for (int i = 0; i < M.Numel; i++)
		M.Elem[i] = val - M.Elem[i];
	return M;
}
matrix operator-(matrix& M) {
	for (int i = 0; i < M.Numel; i++)
		M.Elem[i] = -M.Elem[i];
	return M;
}


/*������������*/
matrix operator-(matrix& M1, matrix& M2) {
	matrix M3(M1);
	if (M1.Col == M2.Col&&M1.Row == M2.Row) {
		for (int i = 0; i < M1.Numel; i++)
			M3.Elem[i] -= M2.Elem[i];
		return M3;
	}
	else {
		printf("����ά�Ȳ�һ��,�޷����,A:%d*%d����,B:%d*%d����\n", M1.Row, M1.Col, M2.Row, M2.Col);
		return M1;
	}
}

/*�������*/
std::ostream& operator<<(std::ostream &os,matrix& M) {
	for (int i = 0; i < M.Numel; i++) {
		if (i%M.Col == 0)
			cout << "\n";
		os << M.Elem[i] << " ";
	}
	return os;
}


/*reshape����*/
Status matrix::reshape(int Rows, int Cols){
	if (Rows == -1 && Numel%Cols == 0) {
		Col = Cols;
		Row = Numel / Col;
		return OK;

	}
	else if (Cols == -1 && Numel%Rows == 0) {
		Row = Rows;
		Col = Numel / Row;
		return OK;
	}
	else if (Rows*Cols == Numel) {
		Row = Rows;
		Col = Cols;
		return OK;
	}
	else {
		cout << "����Reshapeά�ȴ���:" << Row << "*" << Col << "\n";
		return ERROR;
	}
}


/*����ת�ú���*/
matrix matrix::transpose() {
	/*����MKL BLAs���е�ת�ò���*/
	for (int i = 0; i < Numel; i++)
		cout << Elem[i] << " ";
	cout << "\n";
	mkl_dimatcopy('r', 't', Row, Col, 1, Elem, Row, Col);
	for (int i = 0; i < Numel; i++)
		cout << Elem[i] << " ";
	int temp = Col;
	Col = Row;
	Row = temp;
	return *this;
}
#endif
