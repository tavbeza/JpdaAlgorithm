#include "DataTrack.h"
#include "ExtendedKalman.h"

/// <summary>
/// Track constractor.
/// </summary>
DataTrack::DataTrack()
{

}

void DataTrack::InitTrack(DataPlot* pDataPlot)
{
	m_pKalman = new ExtendedKalman(pDataPlot);
}

DataTrack::~DataTrack()
{
}
