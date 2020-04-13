#ifndef KALMAN_TESTS_H_
#define KALMAN_TESTS_H_

#include "TestMethod.h"
#include "ExtendedKalman.h"

#define M_PI 3.14159265358979323846  /* pi */
#define NUMBER_DWELL 3
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
};

#endif // KALMAN_TESTS_H_

