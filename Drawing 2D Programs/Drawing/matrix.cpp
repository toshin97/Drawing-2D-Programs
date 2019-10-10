#include <cfloat>
#include <cmath>
#include <cstdlib>
#include "matrix.h"

using namespace std;

Vector::Vector()
{
	data = new double[3];
	data[0] = 0;
	data[1] = 0;
	data[2] = 1;
}

Vector::Vector(const Vector& oldVector) 
{
	data = new double[3];
	data[0] = oldVector[0];
	data[1] = oldVector[1];
	data[2] = 1;
}

Vector::Vector(const double x, const double y)
{
	data = new double[3];
	data[0] = x;
	data[1] = y;
	data[2] = 1;
}


Vector::~Vector()
{
	delete [] data;
}

double& Vector::operator[](int index) const
{
	return data[index];

}

Matrix::Matrix() 
{
	matData = new double *[3];
	for (int i = 0; i < 3; i++) 
		matData[i] = new double[3];

	for (int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
		
		if (i == j)
			matData[i][j] = 1;
		else
			matData[i][j] = 0;
		}
	}

}

Matrix::Matrix(const Matrix& oldMatrix) 
{
	matData = new double *[3];
	for (int i = 0; i < 3; i++)
		matData[i] = new double[3];  

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			matData[i][j] = oldMatrix[i][j];
		}
	}
}

Matrix::~Matrix()
{
	delete [] matData;
}

Matrix* Matrix::multiply(const Matrix* otherMatrix) const
{
	 Matrix* multiplied = new Matrix();

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {

			double v = 0.0;
			for (int k = 0; k < 3; k++) {
				double f = matData[i][k];
				double g = (*otherMatrix)[k][j];
				v += f * g;
			}
			(*multiplied)[i][j] = v;
		}
	}

	return multiplied;
}

Vector* Matrix::multiply(const Vector* theVector) const
{
	 Vector* newVect = new Vector();
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			(*newVect)[i] += matData[i][j] * (*theVector)[j];
		}
	}
	return newVect;
}

double* Matrix::operator[](int index) const
{
	return matData[index];
}

 Matrix* Matrix::translation(double deltaX, double deltaY)
{
	Matrix* transMat = new Matrix();
	(*transMat)[0][2] = deltaX;
	(*transMat)[1][2] = deltaY;

	return transMat;
}

Matrix* Matrix::rotation(double theta)
{
	Matrix* rotMat = new Matrix();
	(*rotMat)[0][0] = cos(theta);
	(*rotMat)[0][1] = -sin(theta);
	(*rotMat)[1][0] = sin(theta);
	(*rotMat)[1][1] = cos(theta);

	return rotMat;

}
Matrix* Matrix::shearing(double shearXY, double shearYX)
{
	Matrix* shearMat = new Matrix();
	(*shearMat)[0][1] = shearXY;
	(*shearMat)[1][0] = shearYX;

	return shearMat;
}

Matrix* Matrix::scaling(double scaleX, double scaleY)
{ 
	Matrix* scaleMat = new Matrix();
	(*scaleMat)[0][0] = scaleX;
	(*scaleMat)[1][1] = scaleY;

	return scaleMat;
}


Matrix* Matrix::getInverse() const
{
	Matrix* answer = new Matrix();
	double det = getDeterminant();

	answer->matData[0][0] = -matData[1][2] * matData[2][1] + matData[1][1] * matData[2][2];
	answer->matData[0][1] = matData[0][2] * matData[2][1] - matData[0][1] * matData[2][2];
	answer->matData[0][2] = -matData[0][2] * matData[1][1] + matData[0][1] * matData[1][2];
	answer->matData[1][0] = matData[1][2] * matData[2][0] - matData[1][0] * matData[2][2];
	answer->matData[1][1] = -matData[0][2] * matData[2][0] + matData[0][0] * matData[2][2];
	answer->matData[1][2] = matData[0][2] * matData[1][0] - matData[0][0] * matData[1][2];
	answer->matData[2][0] = -matData[1][1] * matData[2][0] + matData[1][0] * matData[2][1];
	answer->matData[2][1] = matData[0][1] * matData[2][0] - matData[0][0] * matData[2][1];
	answer->matData[2][2] = -matData[0][1] * matData[1][0] + matData[0][0] * matData[1][1];

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			answer->matData[i][j] /= det;

	return answer;
}

double Matrix::getDeterminant() const
{
	return  -matData[0][2] * matData[1][1] * matData[2][0] + matData[0][1] * matData[1][2] * matData[2][0] +
		matData[0][2] * matData[1][0] * matData[2][1] - matData[0][0] * matData[1][2] * matData[2][1] -
		matData[0][1] * matData[1][0] * matData[2][2] + matData[0][0] * matData[1][1] * matData[2][2];
}
