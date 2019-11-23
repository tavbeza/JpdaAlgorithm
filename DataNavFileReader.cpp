#include "stdafx.h"
#include "DataNavFileReader.h"
//#include "ServiceLayer\SrvNavigation.h"

#include <string>

DataNavFileReader::DataNavFileReader(string fileName)
{
	m_pLastDataNav = 0;
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
DataNavFileReader::~DataNavFileReader()
{
	//Closes the file.
	m_fileStream.close();
}

/// <summary>
/// fills a given plotList with plots read from a file located the FILE_NAME
/// </summary>
void DataNavFileReader::ReadDataNav(int dwellToRead/*=0*/)
{
	if (m_pLastDataNav != 0)
	{
		if (dwellToRead != 0 && dwellToRead == m_lastDwell)
			SrvNavigation::AddNavData(m_pLastDataNav);
	}

	string curValue;

	//Reads the file and updates the array of plots.
	while (m_fileStream.good())
	{
		unsigned int dwell;
		m_pLastDataNav = new NavPlatStatusStruct();

		getline(m_fileStream, curValue, ',');
		dwell = (unsigned int)stoul(curValue);

		getline(m_fileStream, curValue, ',');
		m_pLastDataNav->lati = stod(curValue);
		getline(m_fileStream, curValue, ',');
		m_pLastDataNav->longi = stod(curValue);
		getline(m_fileStream, curValue, ',');
		m_pLastDataNav->alti = stod(curValue);

		getline(m_fileStream, curValue, ',');
		m_pLastDataNav->rollAngle = stod(curValue);
		getline(m_fileStream, curValue, ',');
		m_pLastDataNav->pitchAngle = stod(curValue);
		getline(m_fileStream, curValue, ',');
		m_pLastDataNav->yawAngle = stod(curValue);

		getline(m_fileStream, curValue, ',');
		m_pLastDataNav->horizontalPositionAccuracy = stod(curValue);
		getline(m_fileStream, curValue, ',');
		m_pLastDataNav->verticalPositionAccuracy = stod(curValue);

		getline(m_fileStream, curValue, ',');
		m_pLastDataNav->yawAngle = stod(curValue);

		getline(m_fileStream, curValue, ',');
		m_pLastDataNav->rollRate = stof(curValue);
		getline(m_fileStream, curValue, ',');
		m_pLastDataNav->pitchRate = stof(curValue);
		getline(m_fileStream, curValue, ',');
		m_pLastDataNav->yawRate = stof(curValue);

		getline(m_fileStream, curValue, ',');
		m_pLastDataNav->inertialVelocityNorth = stof(curValue);
		getline(m_fileStream, curValue, ',');
		m_pLastDataNav->inertialVelocityEast = stof(curValue);
		getline(m_fileStream, curValue, ',');
		m_pLastDataNav->inertialVelocityDown = stof(curValue);

		getline(m_fileStream, curValue, '\n');
		m_pLastDataNav->calibratedAirspeed = (unsigned int)stoul(curValue);


		if (dwellToRead != 0)
		{

			if (dwellToRead != dwell)
			{
				m_lastDwell = dwell;
				return;
			}
		}
		SrvNavigation::AddNavData(m_pLastDataNav);
	}

}