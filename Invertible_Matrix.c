#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define Max_Row 1024 //nxn 행렬에서 n값을 설정
#define Columns_language_type 32// 자료형 크기 (8 = unsigned char, 32 = unsigned int)

#define Success 0
#define False 1

#if Columns_language_type == 8
#define word unsigned char

#elif Columns_language_type == 32
#define word unsigned int 
#endif

void Making_Random_Matrix(word Matrix_for_cpy[Max_Row][Max_Row / Columns_language_type]);// nxn랜덤 행렬 만들기
void Making_Identity_Matrix(word I[Max_Row][Max_Row / Columns_language_type]); // nxn항등행렬 만들기

void Xor_Operation(word Matrix_for_cpy[Max_Row][Max_Row / Columns_language_type], word I[Max_Row][Max_Row / Columns_language_type],int Rows_King, int Rows_other); // Rows_other 행에 Rows_King을 xor시켜준다.
void Swap(word Matrix_for_cpy[Max_Row][Max_Row / Columns_language_type], word I[Max_Row][Max_Row / Columns_language_type],int i, int j); // i행과 j행을 바꿔준다.
int Find_first_one(word Matrix_for_cpy[Max_Row][Max_Row / Columns_language_type], int n, int current_row, int columns); // columns값 부터 선도 1값을 찾는 함수

void CopyMatrix(word Matrix_for_cpy[Max_Row][Max_Row / Columns_language_type], word Matrix_Original[Max_Row][Max_Row / Columns_language_type] ); // 원래 행렬을 보존하기 위해 Matrix_Original에 복사 해준다. (나중에 가역행렬을 찾은 후 원래 행렬을 출력해주기 위해서 만듦)
void sage_print(word Matrix_Original[Max_Row][Max_Row / Columns_language_type], word I[Max_Row][Max_Row / Columns_language_type]); // 원래 행렬과 역행렬을 sage로 출력
void print_Matrix(word Matrix_Original[Max_Row][Max_Row / Columns_language_type], word I[Max_Row][Max_Row / Columns_language_type]); //행렬 출력 함수

void Gauss_Jordan_Elimination(word Matrix_Original[Max_Row][Max_Row / Columns_language_type], word Matrix_for_cpy[Max_Row][Max_Row / Columns_language_type], word I[Max_Row][Max_Row / Columns_language_type], int *check); // 가우스 조던 함수를 실행, check 변수는 역행렬의 존재성을 나타내주는 변수 ( check = 0이면 존재, check = 1이면 존재하지 않는다.)

int main()
{
	int check = False;
	word Matrix_Original[Max_Row][Max_Row / Columns_language_type];
	word Matrix_for_cpy[Max_Row][Max_Row / Columns_language_type];
	word I[Max_Row][Max_Row / Columns_language_type];
	do {
		Making_Identity_Matrix(I);
		Making_Random_Matrix(Matrix_for_cpy);
		CopyMatrix(Matrix_for_cpy,Matrix_Original);
		Gauss_Jordan_Elimination(Matrix_Original, Matrix_for_cpy, I, &check);
	} while (check != Success);
}

void Making_Identity_Matrix(word I[Max_Row][Max_Row / Columns_language_type]) {
	int i, q, r;
	for (i = 0; i < Max_Row; i++) {
		for (q = 0; q < Max_Row / Columns_language_type; q++) {
			I[i][q] = 0x00;
		}
	}//I행렬을 0으로 초기화 해준다.
	for (i = 0; i < Max_Row; i++)
	{
		q = i / Columns_language_type;
		r = Columns_language_type - 1 - i + (Columns_language_type * q);
		I[i][q] = 0x01 << r;
	}//항등행렬 만드는 과정
}
void Making_Random_Matrix(word Matrix_for_cpy[Max_Row][Max_Row / Columns_language_type]) {
	//랜덤 행렬 만드는 과정
	int i, j;
	srand((unsigned)time(NULL));
	if (Columns_language_type == 8) {
		for (i = 0; i < Max_Row; i++) {
			for (j = 0; j < Max_Row / Columns_language_type; j++) {
				Matrix_for_cpy[i][j] = rand();
			}
		}
	}
	else {
		for (i = 0; i < Max_Row; i++) {
			for (j = 0; j < Max_Row / Columns_language_type; j++) {
				Matrix_for_cpy[i][j] = (rand() << 24) ^ (rand() << 16) ^ (rand() << 8) ^ rand();
			}
		}
	}
}

void Xor_Operation(word Matrix_for_cpy[Max_Row][Max_Row / Columns_language_type], word I[Max_Row][Max_Row / Columns_language_type], int Rows_King, int Rows_other) {
	//Rows_other행에 Rows_King행을 Xor해준다.
	int i;
	for (i = 0; i < Max_Row / Columns_language_type; i++) {
		Matrix_for_cpy[Rows_other][i] = Matrix_for_cpy[Rows_King][i] ^ Matrix_for_cpy[Rows_other][i];
		I[Rows_other][i] = I[Rows_King][i] ^ I[Rows_other][i];
	}
}
void Swap(word Matrix_for_cpy[Max_Row][Max_Row / Columns_language_type], word I[Max_Row][Max_Row / Columns_language_type], int i, int j) {
	//i행과 j행을 바꿔준다.
	unsigned int temp;
	int k;
	for (k = 0; k < Max_Row / Columns_language_type; k++) {
		temp = Matrix_for_cpy[i][k];
		Matrix_for_cpy[i][k] = Matrix_for_cpy[j][k];
		Matrix_for_cpy[j][k] = temp;

		temp = I[i][k];
		I[i][k] = I[j][k];
		I[j][k] = temp;
	}

}
int Find_first_one(word Matrix_for_cpy[Max_Row][Max_Row / Columns_language_type], int n, int current_row, int columns) {
	// colums행부터 current_row열에 있는 원소에서 x^n 의 계수가 1인 행을 return 하는 함수이다. 만약 계수가 1인 행이 없으면 -1을 return 한다.
	int i;

	i = columns;

	while (((Matrix_for_cpy[i][current_row] >> n) && 0x01) != 0x01) {
		i++;
		if (i == Max_Row)
		{
			return -1;
		}
	}
	//printf("%d\n", i);
	return i;
}

void CopyMatrix(word Matrix_for_cpy[Max_Row][Max_Row / Columns_language_type], word Matrix_Original[Max_Row][Max_Row / Columns_language_type]) {
	//Matrix_for_cpy행렬을 Matrix_Original에 복사한다.
	int i, j;
	for (i = 0; i < Max_Row; i++) {
		for (j = 0; j < Max_Row / Columns_language_type; j++)
			Matrix_Original[i][j] = Matrix_for_cpy[i][j];
	}
}

void sage_print(word Matrix_Original[Max_Row][Max_Row / Columns_language_type], word I[Max_Row][Max_Row / Columns_language_type])
{
	//결과값을  sage파일로 만든다.
	FILE*fp;
	fopen_s(&fp, "testM.sage", "w");
	int i, j, k;

	fprintf(fp, "A = matrix(GF(2),[");
	for (i = 0; i < Max_Row; i++)
	{
		fprintf(fp, "[");
		for (j = 0; j < Max_Row / Columns_language_type; j++)
		{
			for (k = Columns_language_type - 1; k >= 0; k--)//이 부분은 8부터 0으로 변경 가능.
			{
				fprintf(fp, "%x ,", ((Matrix_Original[i][j] >> k) & 0x01));
			}
		}
		fprintf(fp, "],");
	}
	fprintf(fp, "])\n");

	fprintf(fp, "B = matrix(GF(2),[");
	for (i = 0; i < Max_Row; i++)
	{
		fprintf(fp, "[");
		for (j = 0; j < Max_Row / Columns_language_type; j++)
		{
			for (k = Columns_language_type - 1; k >= 0; k--)//이 부분은 8부터 0으로 변경 가능.
			{
				fprintf(fp, "%x ,", ((I[i][j] >> k) & 0x01));
			}
		}
		fprintf(fp, "],");
	}
	fprintf(fp, "])\n");

	fclose(fp);

}

void print_Matrix(word Matrix_Original[Max_Row][Max_Row / Columns_language_type], word I[Max_Row][Max_Row / Columns_language_type]) {
	//결과 값을 도스창에 출력해주는 함수
	printf("	원래 행렬						역행렬\n");
	for (int y = 0; y < Max_Row; y++) {
		for (int u = 0; u < Max_Row / Columns_language_type; u++) {
			if (Columns_language_type == 8)
				printf("%02x", Matrix_Original[y][u]);
			else if (Columns_language_type == 32)
				printf("%08x", Matrix_Original[y][u]);
		}
		printf("	|	");
		for (int u = 0; u < Max_Row / Columns_language_type; u++) {
			if (Columns_language_type == 8)
				printf("%02x", I[y][u]);
			else if (Columns_language_type == 32)
				printf("%08x", I[y][u]);
		}
		printf("\n");
	}
	printf("\n");

}

void Gauss_Jordan_Elimination(word Matrix_Original[Max_Row][Max_Row / Columns_language_type],word Matrix_for_cpy[Max_Row][Max_Row / Columns_language_type], word I[Max_Row][Max_Row / Columns_language_type], int *check) {
	int n = -1, i = Columns_language_type, current_row = 0;
	int position = 0;
	int l;

	while (current_row != Max_Row / Columns_language_type) {
		n++;
		i--;
		position = Find_first_one(Matrix_for_cpy, i, current_row, n);

		if (position == -1)
		{
			//printf("역행렬이 없습니다.\n");
			*check = False;
			current_row = Max_Row / Columns_language_type;
			break;
		}
		else {
			if (position != n)
				Swap(Matrix_for_cpy, I, position, n);


			for (l = 0; l < Max_Row; l++) {
				if (l != n) {
					if (((Matrix_for_cpy[l][current_row] >> i) & 0x01) == 0x01)
						Xor_Operation(Matrix_for_cpy, I, n, l);
				}
			}

			if (i == 0) {
				i = Columns_language_type;
				current_row++;
				*check = Success;
			}
		}
	}
	if (*check == Success) {
		print_Matrix(Matrix_Original, I);
		sage_print(Matrix_Original, I);
	}

}