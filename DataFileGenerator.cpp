#include "pch.h"
#include "DataFileGenerator.h"



DataFileGenerator::DataFileGenerator(EquationOfMotion& movmentEquation , string fileName) 
{

	SetFileName(fileName);

	m_movments.push_back(&movmentEquation);

}


DataFileGenerator::DataFileGenerator(vector<EquationOfMotion*>& movmentEquations, string fileName)
{

	SetFileName(fileName);

	for(EquationOfMotion* eom : movmentEquations)
		m_movments.push_back(eom);

}


void DataFileGenerator::SetFileName(string& fileName)
{
	if (fileName.empty())
		m_fileName.append(FOLDER_NAME).append(DEFAULT_FILE_NAME);
	else
		m_fileName.append(FOLDER_NAME).append(fileName);
}


const string& DataFileGenerator::GetFileName() const
{
	return m_fileName;
}

void DataFileGenerator::GenerateDataFile()
{

//	int seed = time(NULL);
//	srand(seed);

	ofstream outfile(m_fileName);

	int maxDwell = 20;
	static unsigned int seqNumber;

	for (int dwellCount = 1; dwellCount <= maxDwell; dwellCount++)
	{
		seqNumber = 0;
		for (EquationOfMotion* eom : m_movments)
		{
			WriteDwell(outfile, dwellCount, maxDwell, *eom, seqNumber);
			seqNumber++;
		}
	}

	//outfile << endl;

	outfile.close();
}


void DataFileGenerator::WriteDwell(ofstream& outfile, int dwellCount, int maxDwell, EquationOfMotion& movmentEquation, unsigned int seqNumber)
{
	Vector3f locVecCart, velVecCart , locVecSpherical, velVecSpherical;

	//function2DimSquigglyLine(dwellCount, locVecCart, velVecCart);
	//function2DimLine(dwellCount, locVecCart, velVecCart);
	movmentEquation.CalculateMotion(dwellCount, locVecCart, velVecCart);

	locVecSpherical.CartToSpherical(locVecCart);
	//velVecSpherical.CartToSpherical(locVecCart);

	velVecSpherical.CartToSphericalVelocity(locVecCart, velVecCart);


	//locVecSpherical.m_Data[1] = M_PI/2.0;
	//velVecSpherical.m_Data[1] = M_PI/2.0;

	float ACC = 0.1;

	unsigned int dwell;
	float range;
	float rangeAccuracy;
	float velocity;
	float velocityAccuracy;
	float azimuthAngle;
	float azimuthAccuracy;
	float elevationAngle;
	float elevationAccuracy;
	float snr;
	float magnitudeAbsSq;

	dwell = dwellCount;
	range = locVecSpherical.m_Data[0];
	if (dwellCount != 0)
	{
		azimuthAngle = locVecSpherical.m_Data[1];
		elevationAngle = locVecSpherical.m_Data[2];
	}
	else
	{
		azimuthAngle = 0;
		elevationAngle = 0;
	}
	velocity = velVecSpherical.m_Data[0];
	rangeAccuracy = ACC;
	azimuthAccuracy = ACC;
	elevationAccuracy = ACC;
	velocityAccuracy = ACC;
	snr = 0;
	magnitudeAbsSq = 0;


	outfile << dwell << ",";
	outfile << range << ",";
	outfile << azimuthAngle << ",";
	outfile << elevationAngle << ",";
	outfile << velocity << ",";
	outfile << rangeAccuracy << ",";
	outfile << azimuthAccuracy << ",";
	outfile << elevationAccuracy << ",";
	outfile << velocityAccuracy << ",";
	outfile << snr << ",";
	outfile << magnitudeAbsSq << ",";
	if (dwellCount != maxDwell)
		outfile << seqNumber << endl;
	else
		outfile << seqNumber << endl;
}



/*
void DataFileGenerator::function2DimLine(int dwellCount, Vector3f& locVec, Vector3f& velVec)
{
	float x , y , z, dt, vx, vy, vz;

	z = vz = 0;

	dt = 93.0 / 1000;
	vx = vy = 5;
	x = y = vx * dt * dwellCount;

	locVec.m_Data[0] = x;
	locVec.m_Data[1] = y;
	locVec.m_Data[2] = z;

	//locVec.ToSpherical(locVec);

	velVec.m_Data[0] = vx;
	velVec.m_Data[1] = vy;
	velVec.m_Data[2] = vz;

	//velVec.ToSpherical(velVec);
}


void DataFileGenerator::function2DimSquigglyLine(int dwellCount, Vector3f& locVec, Vector3f& velVec)
{
	float x, y, z, dt, vx, vy, vz;

	z = vz = 0;

	dt = 93.0 / 1000;
	vx = vy = 5;
	x = y = vx * dt * dwellCount;


	float random = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*10 - 5;

	locVec.m_Data[0] = x + (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 10 - 5;
	locVec.m_Data[1] = y + (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 10 - 5;
	locVec.m_Data[2] = z;

	//locVec.ToSpherical(locVec);

	velVec.m_Data[0] = vx;
	velVec.m_Data[1] = vy;
	velVec.m_Data[2] = vz;

	//velVec.ToSpherical(velVec);
}

*/