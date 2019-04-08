#pragma once
#include <cmath>    //asin()

constexpr double PI = 3.14159265;

double radsToDeg(double angle) { return 180 * (angle / PI); }

//returns a pair, first is the magnitude of the line from the start point to the end point, the second is the closest eDirection.  If the start point is the same as the end point it 
std::pair<double, eDirection> calculateDirection(std::pair<int, int> startLocation, std::pair<int, int> endLocation) 
{
	std::pair<double,double> distance{ endLocation.first - startLocation.first, endLocation.second - startLocation.second }; // Calculates the distance vector of the line between the start and end point.
	// Calculating the magnitude of the line between the start and end point.
	double magnitude{ 0 };
	if(distance.first || distance.second)
	{ 
		magnitude = sqrt(distance.first * distance.first + distance.second * distance.second);
	}		           
	std::pair<double, eDirection> returnVariable{ magnitude, eNorth };

	if (magnitude)
	{
		if (distance.first == 0)
		{
			if (distance.second > 0)
			{
				returnVariable.second = eSouthEast;
				return returnVariable;
			}
			else returnVariable.second = eNorthWest;
			return returnVariable;
		}
		double angle;
		if (distance.first > 0)	                                                                               // Calculating the angle between the verticle and the distance vector.
		{
			double Bitch{ radsToDeg(atan(distance.second / distance.first))};
			angle = 90 + radsToDeg(atan(distance.second / distance.first)) ;
		}
		else
		{
			angle = 270 + radsToDeg(atan(distance.second / distance.first)) ;
		}																									   
		int tmp{ (int)(angle+30)/60};																		   // This calculation is just to manipulate the angle into a number that makes the switch case more convenient.

		switch (tmp)
		{
		case 0: returnVariable.second = eNorth;
			break;
		case 1: returnVariable.second = eNorthEast;
			break;
		case 2: returnVariable.second = eSouthEast;
			break;
		case 3: returnVariable.second = eSouth;
			break;
		case 4: returnVariable.second = eSouthWest;
			break;
		case 5: returnVariable.second = eNorthWest;
			break;
		}
	}
	return returnVariable;
}

//returns a pair, first is the magnitude of the line from the start point to the end point, the second is the closest eDirection.
std::pair<double, eDirection> calculateDirection(Tile* startLocation, Tile* endLocation)
{
	// Calculating the distance vector of the line between the start and end point
	std::pair<int, int> distance{ endLocation->m_tileCoordinate.first - startLocation->m_tileCoordinate.first, endLocation->m_tileCoordinate.second - startLocation->m_tileCoordinate.second }; 
	// Calculating the magnitude of the line between the start and end point.
	double magnitude{ sqrt(distance.first * distance.first + distance.second * distance.second) };			           
	std::pair<double, eDirection> returnVariable{ magnitude, eNorth };

	if (magnitude)
	{
		if (distance.first == 0)
		{
			if (distance.second > 0)
			{
				returnVariable.second = eSouthEast;
				return returnVariable;
			}
			else returnVariable.second = eNorthWest;
			return returnVariable;
		}
		double angle;
		if (distance.first > 0)	                                        // Calculating the angle between the verticle and the distance vector.
		{
			angle = 90 + radsToDeg(atan(distance.second / distance.first));
		}
		else
		{
			angle = 270 + radsToDeg(atan(distance.second / distance.first));
		}
		int tmp{ (int)(angle + 30) / 60 };								// This calculation is just to manipulate the angle into a number that makes the switch case more convenient.

		switch (tmp)
		{
		case 0: returnVariable.second = eNorth;
		case 1: returnVariable.second = eNorthEast;
		case 2: returnVariable.second = eSouthEast;
		case 3: returnVariable.second = eSouth;
		case 4: returnVariable.second = eSouthWest;
		case 5: returnVariable.second = eNorthWest;
		}

	}
}