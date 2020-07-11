#ifndef _DATA_FILE_GENERATOR_H_
#define _DATA_FILE_GENERATOR_H_

#include <iostream>
#include <fstream>  
#include <string>
#include "Vector.h"
#include "SrvDspMath.h"
#include <math.h>
//#include <time.h>
#include "EquationOfMotion.h"
#include <vector>

using namespace std;


static const string FOLDER_NAME("Resources/");
static const string DEFAULT_FILE_NAME("dataTest.csv");


class DataFileGenerator
{

public:
	DataFileGenerator(EquationOfMotion& movmentEquation, string fileName = "");
	DataFileGenerator(vector<EquationOfMotion*>& movmentEquations, string fileName = "");
	void GenerateDataFile();
	const string& GetFileName() const;


private:
	//static const int LINE_FUNC_ID = 1;
	//static const int SQUIGGLY_LINE_FUNC_ID = 2;

	void WriteDwell(ofstream& outfile, int dwellCount, int maxDwell, EquationOfMotion& movmentEquation, unsigned int seqNumber);
	//void function2DimLine(int dwellCount, Vector3f& locVec, Vector3f& velVec);
	//void function2DimSquigglyLine(int dwellCount, Vector3f& locVec, Vector3f& velVec);

	string m_fileName;
	vector<EquationOfMotion*> m_movments;

	void SetFileName(string& fileName);
};




#endif //_DATA_FILE_GENERATOR_H_