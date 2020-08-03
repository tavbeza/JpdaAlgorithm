#ifndef KALMAN_TESTS_H_
#define KALMAN_TESTS_H_

#define M_PI 3.14159265358979323846  /* pi */
#define NUMBER_DWELL 20
#define DT 93.0/1000.0;

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

#include "TestMethod.h"
#include "DataFileGenerator.h"
#include "EquationOfMotion.h"
#include "TwoDimensionalLine.h"
#include "TwoDimensionalSquigglyLine.h"

/// <summary>
/// Class that execute methods of test cases
/// </summary>
class KalmanTests : public TestMethod
{
public:
	virtual void Execute();

private:
	/// <summary>
	/// Create "dataPlots.csv" that contains NUMBER_DWELL dwells and 1 plot of each dwell
	/// </summary>
	static void CreateCSVfileOfPlotsAndDwells();

	/// <sumary>
	/// Test Extended Kalman 2D
	/// </summary>
	static void TestExtendedKalmanFilter2D();

	static void TestExtendedKalmanFilter3DWithOnePlot();

	void SingleTargetTrackTest();

	void DoTrackTest();
};

#endif // KALMAN_TESTS_H_

