#include "DataTrack.h"
#include "ExtendedKalman.h"

/// <summary>
/// Track constractor.
/// </summary>
DataTrack::DataTrack(int id)
{
	m_Id = id;
}

void DataTrack::InitTrack(const DataPlot &pDataPlot)
{
	m_pKalman = new ExtendedKalman(pDataPlot);
	//m_pKalman->Predict(93.0 / 1000);
}

DataTrack::~DataTrack()
{
}
