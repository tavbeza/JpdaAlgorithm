#include <iostream>
#include <fstream>
#include <math.h>

#include "Kalman.h"
#include "ExtendedKalman.h"
#include "DataPlot.h"
#include "DataPlotList.h"
#include "DataPlotFileReader.h"


#define NUMBER_DWELL 5

using namespace std;

void createCSVfile();

void main()
{
	// Test Kalman 2D
	//Kalman kalman;
	//kalman.kalmanTest();

	// Create CSV file for Extended Kalman 3D test
	string fileName = "dataPlots.csv";
	
	// already created 
	createCSVfile(); 


	// Test Extended Kalman Filter 3D.
	ExtendedKalman extendedKalman;
	
	// for InitXP()
	NavPlatStatusStruct nav;
	Vector9d xInit;
	Matrix9d pInit;
	
	// for kalman constructor
	float dt = 93.0/1000.0;
	Matrix4d R;

	DataPlotList plotsList;
	DataPlotFileReader plotsReader(fileName);
	
	for (int i = 1; i < NUMBER_DWELL + 1; i++)
	{
		plotsReader.ReadDataPlot(&plotsList, i);
		for (list<DataPlot*>::iterator &it = plotsList.getDataPlotList()->begin(); it != plotsList.getDataPlotList()->end(); ++it)
		{
			DataPlot* pPlot = *it;
			extendedKalman.InitXP(*pPlot, nav, xInit, pInit);
			extendedKalman = ExtendedKalman(dt,															// dt
												cos(pPlot->GetAzimuthAngle())*pPlot->GetRange(),		// x
												sin(pPlot->GetAzimuthAngle())*pPlot->GetRange(),		// y
												0,														// z
												cos(pPlot->GetAzimuthAngle())*pPlot->GetVelocity(),		// vx
												sin(pPlot->GetAzimuthAngle())*pPlot->GetVelocity(),		// vy
												0,														// vz
												R);														// R
		
			extendedKalman.Predict(dt);
			extendedKalman.Update(pPlot);
		}

	}
}
/*
ExtendedKalman::ExtendedKalman(
	const float& dt,
	const float& x,
	const float& y,
	const float& z,
	const float& vx,
	const float& vy,
	const float& vz,
	const Matrix4d& R)
*/


void createCSVfile()
{
	// file pointer 
	fstream fout;

	// opens an existing csv file or creates a new file. 
	fout.open("dataPlots.csv", ios::out | ios::app);

	DataPlot allDataPlots[NUMBER_DWELL];	 // NUMBER_OF_DATA_PLOTS = 5
	int dwel = 1, x = 1, seqNumber = 1;
	float azimuth = atan(2);
	
	// create and write array of DataPlot
	for (int i = 0; i < NUMBER_DWELL; i++)
	{
		fout << dwel << ",";
		dwel++;

		allDataPlots[i].m_range = sqrt(pow(x, 2) + pow((2*x), 2));
		fout << allDataPlots[i].m_range << ",";
		x++;
		
		allDataPlots[i].m_azimuth = azimuth;
		fout << allDataPlots[i].m_azimuth << ",";
		
		allDataPlots[i].m_elevation = 0;
		fout << allDataPlots[i].m_elevation << ",";
		
		allDataPlots[i].m_velocity = 2;
		fout << allDataPlots[i].m_velocity << ",";
		
		allDataPlots[i].m_rangeAccuracy = 0.1;
		fout << allDataPlots[i].m_rangeAccuracy << ",";
		
		allDataPlots[i].m_azimuthAccuracy = 0.1;
		fout << allDataPlots[i].m_azimuthAccuracy << ",";
		
		allDataPlots[i].m_elevationAccuracy = 0.1;
		fout << allDataPlots[i].m_elevationAccuracy << ",";
		
		allDataPlots[i].m_velocityAccuracy = 0.1;
		fout << allDataPlots[i].m_velocityAccuracy << ",";
		
		allDataPlots[i].m_SNR = 0.1;
		fout << allDataPlots[i].m_SNR << ",";
		
		allDataPlots[i].m_magnitude = 0.1;
		fout << allDataPlots[i].m_magnitude << ",";
		
		allDataPlots[i].m_seqNumber = seqNumber++;
		fout << allDataPlots[i].m_seqNumber << ",";

		if( i != NUMBER_DWELL - 1)
			fout << "\n";
	}
}
