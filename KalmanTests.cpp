#include "KalmanTests.h"

void KalmanTests::Execute()
{
	CreateCSVfileOfPlotsAndDwells();
	TestExtendedKalmanFilter2D();
}

/// <summary>
/// Create "dataPlots.csv" that contains NUMBER_DWELL dwells and 1 plot of each dwell
/// </summary>
void KalmanTests::CreateCSVfileOfPlotsAndDwells()
{
	// file pointer 
	fstream fout;

	// Create CSV file for Extended Kalman 3D test
	string fileName = "dataPlots.csv";

	// opens an existing csv file or creates a new file. 
	fout.open(fileName, ios::out | ios::app);

	DataPlot allDataPlots[NUMBER_DWELL];	 // NUMBER_OF_DATA_PLOTS = 5
	bool isInc = true;
	double dwel = 1, x = 1, seqNumber = 1, y = 2;
	float azimuth;
	double max, min, diff, range, rx, ry;
	// create and write array of DataPlot
	for (int i = 0; i < NUMBER_DWELL; i++)
	{
		if (isInc)
		{
			min = 1;
			max = 1.125;
			isInc = false;
		}
		else
		{
			min = 0.875;
			max = 1;
			isInc = true;
		}
		diff = max - min;
		rx = (((double)rand() / RAND_MAX) * diff) + min;
		ry = (((double)rand() / RAND_MAX) * diff) + min;
		//range = sqrt(pow(x, 2) + pow((2 * x), 2));
		x *= rx;
		y *= ry;
		azimuth = atan(y / x);
		fout << dwel << ",";
		dwel++;

		allDataPlots[i].m_range = sqrt(pow(x, 2) + pow((y), 2));
		fout << allDataPlots[i].m_range << ",";


		allDataPlots[i].m_azimuth = azimuth;
		fout << allDataPlots[i].m_azimuth << ",";

		allDataPlots[i].m_elevation = M_PI / 2; //3.14159265359 / 2;	// pi / 2
		fout << allDataPlots[i].m_elevation << ",";

		allDataPlots[i].m_velocity = 24.04374169;
		fout << allDataPlots[i].m_velocity << ",";

		Vector3d spherical;
		spherical.m_Data[0] = allDataPlots[i].GetRange();
		spherical.m_Data[1] = allDataPlots[i].GetAzimuthAngle();
		spherical.m_Data[2] = allDataPlots[i].GetElevationAngle();

		Vector3d errorCartesian;
		errorCartesian.m_Data[0] = rx;
		errorCartesian.m_Data[1] = ry;
		errorCartesian.m_Data[2] = 0;

		Vector3d errorSpherical;
		errorSpherical.ErrorCartToSpherical(spherical, errorCartesian);

		allDataPlots[i].m_rangeAccuracy = errorSpherical.m_Data[0];
		fout << allDataPlots[i].m_rangeAccuracy << ",";

		allDataPlots[i].m_azimuthAccuracy = errorSpherical.m_Data[1];
		fout << allDataPlots[i].m_azimuthAccuracy << ",";

		allDataPlots[i].m_elevationAccuracy = errorSpherical.m_Data[2];
		fout << allDataPlots[i].m_elevationAccuracy << ",";

		allDataPlots[i].m_velocityAccuracy = 0.1;
		fout << allDataPlots[i].m_velocityAccuracy << ",";

		allDataPlots[i].m_SNR = 0.1;
		fout << allDataPlots[i].m_SNR << ",";

		allDataPlots[i].m_magnitude = 0.1;
		fout << allDataPlots[i].m_magnitude << ",";

		allDataPlots[i].m_seqNumber = seqNumber++;
		fout << allDataPlots[i].m_seqNumber << ",";
		//fout << "\trx\t" << rx << "\try\t" << ry;
		x = i + 1;
		y = x * 2;

		if (i != NUMBER_DWELL - 1)
			fout << "\n";
	}
}

/// <sumary>
/// Test Extended Kalman 2D
/// </summary>
void KalmanTests::TestExtendedKalmanFilter2D()
{
	TrackerJpda trackerJpda;
	std::ofstream m_kalmanFile;

	// CSV file contains NUMBER_DWELL dwells and 1 plot of each dwell
	string writeFile = "KalmanParams.csv";
	m_kalmanFile.open(writeFile, ios::out | ios::app);

	// CSV file contains NUMBER_DWELL dwells and 1 plot of each dwell
	string readFile = "dataPlots.csv";
	DataPlotFileReader plotsReader(readFile);

	DataPlotList plotsList;

	for (int i = 1; i < NUMBER_DWELL + 1; i++)
	{
		plotsReader.ReadDataPlot(&plotsList, i);

		m_kalmanFile << "//X" << std::endl;
		m_kalmanFile << cos(plotsList[0]->GetAzimuthAngle())*plotsList[0]->GetRange() << ",";	// Write x coordinate of the plot to file
		m_kalmanFile << sin(plotsList[0]->GetAzimuthAngle())*plotsList[0]->GetRange() << ",";	// Write y coordinate of the plot to file
		m_kalmanFile << std::endl;

		trackerJpda.DoTrack(plotsList);

		for (int j = 0; j < plotsList.GetCount(); j++)
		{
			DataPlot* pPlot = plotsList[j];

			DataTrackList *dataTrackList = trackerJpda.GetTrack();

			m_kalmanFile << "//X_Predict" << std::endl;
			m_kalmanFile << dataTrackList[0][0]->m_pKalman->m_X.m_Data[0] << ",";		// Write x coordinate after update from kalman
			m_kalmanFile << dataTrackList[0][0]->m_pKalman->m_X.m_Data[3] << "," << std::endl;  // Write y coordinate after update from kalman

			// Write m_P matrix from kalman to file
			m_kalmanFile << "//m_P" << std::endl;
			dataTrackList[0][0]->m_pKalman->m_P.PrintToFile(m_kalmanFile);

			m_kalmanFile << std::endl;
		}
		plotsList.Clear();
	}
	m_kalmanFile.close();

}