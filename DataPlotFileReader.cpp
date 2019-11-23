#include "stdafx.h"
#include "DataPlotFileReader.h"
#include <string>

using namespace std;
DataPlotFileReader::DataPlotFileReader(std::string fileName)
{
	m_pLastDataPlot = 0;
	m_lastDwell = 0;
	m_fileName = fileName;
	
	//Opens file.
	m_fileStream.open(fileName);
	
	//Checks if file was opened correctly.
	if (!m_fileStream.is_open())
	{
		printf("File was not opened");
	}
}

DataPlotFileReader::~DataPlotFileReader()
{
	//Closes the file.
	m_fileStream.close();
}

/// <summary>
/// fills a given plotList with plots read from a file located the FILE_NAME
/// </summary>
void DataPlotFileReader::ReadDataPlot(DataPlotList* plotList, int dwellToRead/*=0*/)
{
	if (m_pLastDataPlot != 0)
	{
		if (dwellToRead != 0 && dwellToRead == m_lastDwell)
			plotList->AddPlot(m_pLastDataPlot);
	}

	string curValue;

	//Reads the file and updates the array of plots.
	while (m_fileStream.good())
	{
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
		unsigned int seqNumber;

		getline(m_fileStream, curValue, ',');
		dwell = (unsigned int)stoul(curValue);

		getline(m_fileStream, curValue, ',');
		range = stof(curValue);
		getline(m_fileStream, curValue, ',');
		azimuthAngle = stof(curValue);
		getline(m_fileStream, curValue, ',');
		elevationAngle = stof(curValue);
		getline(m_fileStream, curValue, ',');
		velocity = stof(curValue);
		getline(m_fileStream, curValue, ',');

		rangeAccuracy = stof(curValue);
		getline(m_fileStream, curValue, ',');
		azimuthAccuracy = stof(curValue);
		getline(m_fileStream, curValue, ',');
		elevationAccuracy = stof(curValue);
		getline(m_fileStream, curValue, ',');
		velocityAccuracy = stof(curValue);
		getline(m_fileStream, curValue, ',');
		snr = stof(curValue);
		getline(m_fileStream, curValue, ',');
		magnitudeAbsSq = stof(curValue);
		getline(m_fileStream, curValue, '\n');
		seqNumber = (unsigned int)stoul(curValue);

		m_pLastDataPlot = new DataPlot(range,
			rangeAccuracy,
			velocity,
			velocityAccuracy,
			azimuthAngle,
			azimuthAccuracy,
			elevationAngle,
			elevationAccuracy,
			snr,
			magnitudeAbsSq,
			seqNumber);

		if (dwellToRead != 0)
		{

			if (dwellToRead != dwell)
			{
				m_lastDwell = dwell;
				return;
			}
		}

		plotList->AddPlot(m_pLastDataPlot);
	}

}