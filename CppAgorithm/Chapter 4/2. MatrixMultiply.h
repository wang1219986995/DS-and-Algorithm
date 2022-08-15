#include <vector>
using namespace std;

typedef vector< vector<int>> Matrix;

Matrix MatrixMultiply(Matrix& matrix1, Matrix& matrix2)
{
	if (matrix1.size() <= 0)	return Matrix();
	if (matrix1.size() != matrix2.size() || matrix1[0].size() != matrix2[0].size() || matrix1.size() != matrix1[0].size())
		return Matrix();
	
	int n = matrix1.size();
	Matrix result(n, vector<int>(n, 0));
	
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			for (int k = 0; k < n; k++)
			{
				result[i][j] += matrix1[i][k] * matrix2[k][j];
			}
		}
	}
	return result;
}


Matrix MatrixMultiply_Recursive(Matrix& matrix1, Matrix& matrix2)
{
	if (matrix1.size() <= 0)	return Matrix();
	if (matrix1.size() != matrix2.size() || matrix1[0].size() != matrix2[0].size() || matrix1.size() != matrix1[0].size())
		return Matrix();
	int size = matrix1.size();
	while (size > 1)
	{
		if (size % 2 != 0)	return Matrix();
		size = size / 2;
	}

	size = matrix1.size();
	Matrix result = MatrixMultiply_Recursive_Aux(matrix1, 0, 0, matrix2, 0, 0, size);

}

Matrix MatrixMultiply_Recursive_Aux(Matrix& matrix1, int startRow1, int startCol1, Matrix& matrix2, int startRow2, int startCol2, int size)
{
	if (size == 1)	return { {matrix1[startRow1][startCol1] * matrix2[startRow2][startCol2]} };
	else
	{
		size = size / 2;
		Matrix res11 = MatrixAdd(MatrixMultiply_Recursive_Aux(matrix1, startRow1, startCol1, matrix2, startRow2, startCol2, size), 
			MatrixMultiply_Recursive_Aux(matrix1, startRow1, startCol1 + size, matrix2, startRow1 + size, startCol2, size));

		Matrix res12 = MatrixAdd(MatrixMultiply_Recursive_Aux(matrix1, startRow1, startCol1, matrix2, startRow2, startCol2 + size, size),
			MatrixMultiply_Recursive_Aux(matrix1, startRow1, startCol1 + size, matrix2, startRow1 + size, startCol2 + size, size));

		Matrix res21 = MatrixAdd(MatrixMultiply_Recursive_Aux(matrix1, startRow1 + size, startCol1, matrix2, startRow2, startCol2, size),
			MatrixMultiply_Recursive_Aux(matrix1, startRow1 + size, startCol1 + size, matrix2, startRow1 + size, startCol2, size));

		Matrix res22 = MatrixAdd(MatrixMultiply_Recursive_Aux(matrix1, startRow1 + size, startCol1, matrix2, startRow2, startCol2 + size, size),
			MatrixMultiply_Recursive_Aux(matrix1, startRow1 + size, startCol1 + size, matrix2, startRow1 + size, startCol2 + size, size));
		
		return MatrixMerge(res11, res12, res 21, res22);
	}
}

Matrix MatrixAdd(const Matrix& matrix1, const Matrix& matrix2)
{
	if (matrix1.size() <= 0)	return Matrix();
	if (matrix1.size() != matrix2.size() || matrix1[0].size() != matrix2[0].size() || matrix1.size() != matrix1[0].size())
		return Matrix();

	Matrix res(matrix1.size(), matrix1[0].size());
	for (int i = 0; i < matrix1.size(); i++)
	{
		for (int j = 0; j < matrix1[0].size(); j++)
		{
			res[i][j] = matrix1[i][j] + matrix2[i][j];
		}
	}
	return res;
}

Matrix MatrixSub(const Matrix& matrix1, const Matrix& matrix2)
{
	if (matrix1.size() <= 0)	return Matrix();
	if (matrix1.size() != matrix2.size() || matrix1[0].size() != matrix2[0].size() || matrix1.size() != matrix1[0].size())
		return Matrix();

	Matrix res(matrix1.size(), matrix1[0].size());
	for (int i = 0; i < matrix1.size(); i++)
	{
		for (int j = 0; j < matrix1[0].size(); j++)
		{
			res[i][j] = matrix1[i][j] - matrix2[i][j];
		}
	}
	return res;
}

Matrix MatrixAdd(const Matrix& matrix1, startRow1, startCol1, const Matrix& matrix2, startRow2, startCol2, size)
{
	Matrix res(size, vector<int>(size, 0));
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			res[i][j] = matrix1[i + startRow1][j + startCol1] + matrix2[i + startRow2][j + startCol2];
		}
	}
	return res;
}

Matrix MatrixSub(const Matrix& matrix1, startRow1, startCol1, const Matrix& matrix2, startRow2, startCol2, size)
{
	Matrix res(size, vector<int>(size, 0));
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			res[i][j] = matrix1[i + startRow1][j + startCol1] - matrix2[i + startRow2][j + startCol2];
		}
	}
	return res;
}


Matrix MatrixMerge(const Matrix& matrix11, const Matrix& matrix12, const Matrix& matrix21, const Matrix& matrix22)
{
	int size = matrix11.size();
	int n = size * 2;
	Matrix res(n, vector<int>(n, 0));
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			if ((i >= 0 && i < size) && (j >= 0 && j < size))						res[i][j] = matrix11[i][j];
			else if ((i >= 0 && i < size) && (j >= size && j < 2 * size))			res[i][j] = matrix12[i][j - size];
			else if ((i >= size && i < 2 * size) && (j >= 0 && j < size))			res[i][j] = matrix21[i - size][j];
			else if ((i >= size && i < 2 * size) && (j >= size && j < 2 * size))	res[i][j] = matrix22[i - size][j - size];
		}
	}
	return res;
}




Matrix MatrixMultiply_Strassen(Matrix& matrix1, Matrix& matrix2)
{

}


Matrix MatrixMultiply_Strassen_Aux(Matrix& matrix1, Matrix& matrix2)
{
	int size = matrix1.size();
	if (size == 1)	return { {matrix1[startRow1][startCol1] * matrix2[startRow2][startCol2]} };
	else
	{
		int n = size / 2;
		Matrix S1 = MatrixSub(matrix2, 0, n, matrix2, n, n, n);
		Matrix S2 = MatrixAdd(matrix1, 0, 0, matrix1, 0, n, n);
		Matrix S3 = MatrixAdd(matrix1, n, 0, matrix1, n, n, n);
		Matrix S4 = MatrixSub(matrix2, n, 0, matrix2, 0, 0, n);
		Matrix S5 = MatrixAdd(matrix1, 0, 0, matrix1, n, n, n);
		Matrix S6 = MatrixAdd(matrix2, 0, 0, matrix2, n, n, n);
		Matrix S7 = MatrixSub(matrix1, 0, n, matrix1, n, n, n);
		Matrix S8 = MatrixAdd(matrix1, n, 0, matrix1, n, n, n);
		Matrix S9 = MatrixSub(matrix1, 0, 0, matrix1, n, 0, n);
		Matrix S10 = MatrixAdd(matrix2, 0, 0, matrix2, 0, n, n);

		Matrix P1 = 

	}
}


