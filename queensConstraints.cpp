#include "queensConstraints.hpp"
#include <cmath>

queensConstraints::queensConstraints(){}

bool queensConstraints::verifyConstraints(vector< pair <int,int> >  indval)
{
	bool res = true; 

	for (unsigned int i=0; i<indval.size(); ++i)
	{
		for (unsigned int j=i+1; j<indval.size(); ++j)
		{
			if (indval.at(i).second == indval.at(j).second)
			{
				res = false;
			}

			double resval = abs(indval.at(i).second - indval.at(j).second);
			int resind = indval.at(j).first - indval.at(i).first;

			if (resval == resind)
			{
				res = false;
			}
		}
	}

	return res;
}
