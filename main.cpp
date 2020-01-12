#include <iostream>
#include <fstream>
#include <math.h>
#include <sstream>

#include "Kalman.h"
#include "ExtendedKalman.h"
#include "DataPlot.h"
#include "DataPlotList.h"
#include "DataPlotFileReader.h"
#include "TrackerJpda.h"


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
	//createCSVfile(); 


	// Test Extended Kalman Filter 3D.
	TrackerJpda trackerJpda;
	ExtendedKalman extendedKalman;
	std::ofstream m_kalmanFile;
	std::stringstream path;
	path << "C:\\Users\\tavbe\\Desktop\\KalmanParams.txt";
	m_kalmanFile.open(path.str());
	
	// for kalman constructor
	float dt = 93.0/1000.0;
	DataPlotList plotsList;
	DataPlotFileReader plotsReader(fileName);
	
	for (int i = 1; i < NUMBER_DWELL + 1; i++)
	{
		plotsReader.ReadDataPlot(&plotsList, i);
		trackerJpda.DoTrack(plotsList);
		
		for (int j = 0; j < plotsList.GetCount(); j++)
		{
			DataPlot* pPlot = plotsList[j];
			
			//extendedKalman = ExtendedKalman(*pPlot);
			//extendedKalman.Predict(dt);
			//extendedKalman.Update(pPlot);

			DataTrackList *dataTrackList = trackerJpda.GetTrack();
			
			m_kalmanFile << "//m_X" << std::endl;
			for (int x = 0; x < 9; x++)
			{			
				m_kalmanFile << dataTrackList[0][0]->m_pKalman->m_X.m_Data[x] << std::endl;
			}
			
			m_kalmanFile << "//m_P" << std::endl;
			dataTrackList[0][0]->m_pKalman->m_P.PrintToFile(m_kalmanFile);
			//extendedKalman.m_P.PrintToFile(m_kalmanFile);
			m_kalmanFile << std::endl << std::endl;
		}
		plotsList.Clear();
	}
	m_kalmanFile.close();
}


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
