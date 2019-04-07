#pragma once
#include <cmath>    //asin()

//returns a pair, first is the magnitude of the line from the start point to the end point, the second is the closest eDirection.
std::pair<double, eDirection> calculateDirection(std::pair<int, int> startLocation, std::pair<int, int> endLocation) 
{
	std::pair<int,int> distance{ endLocation.first - startLocation.first, endLocation.second - startLocation.second }; // Calculates the distance vector of the line between the start and end point.
	double magnitude{ sqrt(distance.first * distance.first + distance.second * distance.second) };			           // Calculates the magnitude of the line between the start and end point.
	double angle{ asin(distance.first / distance.second) };														   	   // Calculates the angle between the verticale and the distance vector.
	int tmp{ (int)( (angle + 30) / 60 )};																			   // This calculation is just to manipulate the angle into a number that makes the switch case more convenient.
	std::pair<double, eDirection> returnVariable{ magnitude, eNorth };
	switch (tmp)
	{
	case 0: returnVariable.second = eNorth;																			   // Checking what eDirection the calculated angle most closely reprisents.
			return returnVariable;
	case 1: returnVariable.second = eNorthEast;
			return returnVariable;
	case 2: returnVariable.second = eSouthEast;
			return returnVariable;
	case 3: returnVariable.second = eSouth;
			return returnVariable;
	case 4: returnVariable.second = eSouthWest;
			return returnVariable;
	case 5: returnVariable.second = eNorthWest;
			return returnVariable;
	}
}

//returns a pair, first is the magnitude of the line from the start point to the end point, the second is the closest eDirection.
std::pair<double, eDirection> calculateDirection(Tile* startLocation, Tile* endLocation)
{
	std::pair<int, int> distance{ endLocation->m_tileCoordinate.first - startLocation->m_tileCoordinate.first, endLocation->m_tileCoordinate.second - startLocation->m_tileCoordinate.second }; // Calculates the distance vector of the line between the start and end point
	double magnitude{ sqrt(distance.first * distance.first + distance.second * distance.second) };		                                                                      	                // Calculates the magnitude of the line between the start and end point
	double angle{ asin(distance.first / distance.second) };														   	                                                                            // Calculates the angle between the verticale and the distance vector
	int tmp{ (int)((angle + 30) / 60) };																																						// This calculation is just to manipulate the angle into a number that makes the switch case more convenient.
	std::pair<double, eDirection> returnVariable{ magnitude, eNorth };
	switch (tmp)
	{
	case 0: returnVariable.second = eNorth;																			   // Checking what eDirection the calculated angle most closely reprisents.
		return returnVariable;
	case 1: returnVariable.second = eNorthEast;
		return returnVariable;
	case 2: returnVariable.second = eSouthEast;
		return returnVariable;
	case 3: returnVariable.second = eSouth;
		return returnVariable;
	case 4: returnVariable.second = eSouthWest;
		return returnVariable;
	case 5: returnVariable.second = eNorthWest;
		return returnVariable;
	}
}